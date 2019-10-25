// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "BitsAnalyser.h"
//#include "BitsAnalyserView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//USB 相关变量
usb_dev_handle *dev ; /* the device handle */
BYTE _send[Command_Head_Length];
BYTE __Bits[Max_Size];
int  ConnectState ;
int  threadState  ;
UINT uiBitsLen = 0;
UINT uiRecvLen = 0;
UINT uiPrinLen = 0;

#if _Command_Mode


#define _USB_Dev_Error -1

#define _USB_Fail            0
#define _USB_Send_Fail       -0x10
#define _USB_Recieve_Fail    -0x11

#define _USB_Success         1
#define _USB_Send_Success    1
#define _USB_Recieve_Success 1

#define _Command_Class       00
#define _Command_INS         01
#define _Command_P1          02
#define _Command_P2          03
#define _Command_P3          04


#define _Command_Class_Data      0xF0
#define _Command_INS_GetBitsSize 0xA0
#define _Command_INS_GetBitsData 0xB0
#define _Command_INS_InitBits    0xF0

int _GetBitsSize(BYTE* _Bits_Size,int _Size)
{
	int iRet;
	if(dev == NULL)
		return _USB_Dev_Error;

	_send[_Command_Class] = _Command_Class_Data;
	_send[_Command_INS]   = _Command_INS_GetBitsSize;
	_send[_Command_P1]    = 00;
	_send[_Command_P2]    = 00;
	_send[_Command_P3]    = 0x8;

	iRet = usb_bulk_write(dev,EP_OUT,(char *)_send,Command_Head_Length,Command_TimeOut);

	if (iRet<=0)
		return _USB_Send_Fail;

	iRet   = usb_bulk_read(dev, EP_IN, (char*)_Bits_Size,8,Command_TimeOut);
	if (iRet < 0)
		return _USB_Recieve_Fail;

	return iRet;
}


bool GetBitsSize(UINT* iBitsSize,UINT* iSended)
{
	int iRet;
	BYTE _Reci[8];
	if(dev)
	{
		iRet = _GetBitsSize(_Reci,8);
		if (iRet > 0)
		{
			* iBitsSize = _Reci[0]*0x1000000+ 
				_Reci[1]*0x10000+
				_Reci[2]*0x100+
				_Reci[3];
			* iSended   = _Reci[4]*0x1000000+ 
				_Reci[5]*0x10000+
				_Reci[6]*0x100+
				_Reci[7];

			return true;
		}
	}

	return false;


}

int _GetBits(BYTE* _Bits,int _Size)
{
	int iRet,iRead;
	if(dev == NULL)
		return _USB_Dev_Error;
	if (_Size > 0x100)
		_Size = 0x100;
	_send[_Command_Class] = _Command_Class_Data;
	_send[_Command_INS]   = _Command_INS_GetBitsData;
	_send[_Command_P1]    = 00;
	_send[_Command_P2]    = 00;
	_send[_Command_P3]    = _Size&0xFF;



	iRet = usb_bulk_write(dev,EP_OUT,(char *)_send,Command_Head_Length,Command_TimeOut);

	if (iRet<0)
		return _USB_Send_Fail;
	iRead = 0;
	while(iRead < _Size)
	{
		iRet   = usb_bulk_read(dev, EP_IN, (char*)_Bits+iRead,512,Command_TimeOut);
		if (iRet < 0)
			return _USB_Recieve_Fail;
		iRead += iRet;
	}
	return iRead;
}


int _InitBits()
{
	int iRet;
	if(dev == NULL)
		return _USB_Dev_Error;

	_send[_Command_Class] = _Command_Class_Data;
	_send[_Command_INS]   = _Command_INS_InitBits;
	_send[_Command_P1]    = 00;
	_send[_Command_P2]    = 00;
	_send[_Command_P3]    = 00;

	iRet = usb_bulk_write(dev,EP_OUT,(char *)_send,Command_Head_Length,Command_TimeOut);

	if (iRet<=0)
		return _USB_Send_Fail;

	return _USB_Success;
}

#endif



UINT GetBits(LPVOID pParam) 
{ 
#if _Command_Mode
	int iRet;
	UINT uiBitsSize;
	UINT uiRecvSize;
	//BYTE _Reci[8];

	while(threadState)
	{
		if(dev)
		{
			//iRet = _GetBitsSize(_Reci,8);
			//if (iRet > 0)
			//{
			//	uiBitsSize = _Reci[0]*0x1000000+ 
			//		_Reci[1]*0x10000+
			//		_Reci[2]*0x100+
			//		_Reci[3];

			//	uiRecvLen  = _Reci[4]*0x1000000+ 
			//		_Reci[5]*0x10000+
			//		_Reci[6]*0x100+
			//		_Reci[7];
			//}

			if (! GetBitsSize(&uiBitsSize,&uiRecvSize))
				goto __wait_;
			if (uiBitsSize <= uiRecvLen )
				goto __wait_;
			else uiBitsLen = uiBitsSize;

			iRet = _GetBits( (BYTE*)__Bits + uiRecvLen,uiBitsSize - uiRecvLen);

			uiRecvLen += iRet;
			continue;
		}
__wait_:Sleep(100);
	}


#else
	int ret;
	while(threadState)
	{
		if(dev)
		{


			ret = usb_bulk_read(dev, EP_IN, __Bits+uiRecvLen,BUF_SIZE, 5000);
			if (ret>0)
				uiRecvLen += ret;
			//Sleep(5);

		}
	}
#endif

	return  0 ; 
} 

UINT ViewBits(LPVOID pParam) 
{ 

//	int ret;


	UINT uiTEmp;
	
	CMainFrame* pFrame       =  (CMainFrame*)   AfxGetApp()->GetMainWnd();
	CBitsAnalyserView* pView =  (CBitsAnalyserView*)(pFrame->GetActiveView());
	
	while(threadState)
	{

		
		if (uiPrinLen < uiRecvLen)
		{



			uiTEmp = uiRecvLen-uiPrinLen;
			if (uiTEmp>256) uiTEmp = 256;

		//	Sleep(1);
			//pView->PrintEdit(__Bits+uiPrinLen,uiTEmp );
			pView->ViewAPDU ((BYTE*)__Bits+uiPrinLen,uiTEmp );
			uiPrinLen = (uiPrinLen+uiTEmp);
			pFrame->SendMessage(ID_MESSAGE_UPDATE_PROGRESS,0,0 );
			pFrame->SendMessage(ID_MESSAGE_UPDATE_EVENT, 0, 0);
	
		
		}
		//else if (uiPrinLen > uiRecvLen)
		//{
		//	 Sleep(10000);
		//}else
		// Sleep(1000);
		/*if(dev)
		{
		ret = usb_bulk_read(dev, EP_IN, _recv+uiRecvLen,BUF_SIZE, 5000);
		if (ret>0)
		{
		uiRecvLen += ret;
		}

		}*/
	}

	return 0;
}

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_Connect_BUTTON, &CMainFrame::OnConnectButton)
	ON_COMMAND(ID_Disconnect_BUTTON, &CMainFrame::OnDisconnectButton)
	ON_UPDATE_COMMAND_UI_RANGE(ID_Connect_BUTTON, ID_Disconnect_BUTTON, &CMainFrame::OnUpdateConnected)
	
	ON_COMMAND_RANGE(ID_VIEW_CHECK_WAVEFORM,ID_VIEW_CHECK_EVENTLIST, &CMainFrame::OnDockablePane)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_CHECK_WAVEFORM, ID_VIEW_CHECK_EVENTLIST, &CMainFrame::OnUpdateDockablePane)

	ON_MESSAGE(ID_MESSAGE_UPDATE_PROGRESS, &CMainFrame::OnUpdateProgress)
	ON_MESSAGE(ID_MESSAGE_UPDATE_EVENT, &CMainFrame::OnUpdateEvent)
	
	ON_UPDATE_COMMAND_UI(ID_COMBO_PRESCALE, &CMainFrame::OnUpdateComboPrescale)
	ON_COMMAND(ID_COMBO_PRESCALE, &CMainFrame::OnComboPrescale)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);


	CreateRibbon();

	CreateStatueBar();



	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);

	m_wndEventList.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndEventList);

	m_wndWaveView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndWaveView);
	//自定义参数进行初始化
	__Init();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


BOOL CMainFrame::CreateRibbon(void)
{
	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	return TRUE;
}



BOOL CMainFrame::Initialize_Ribbon(void)
{

	CMFCRibbonComboBox* pFontComboBox = DYNAMIC_DOWNCAST(
		CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_PRESCALE));

	if (pFontComboBox == NULL)
	{
		return FALSE;
	}

	pFontComboBox->AddItem(_T("10"));
	pFontComboBox->AddItem(_T("16"));
	pFontComboBox->AddItem(_T("31"));
	pFontComboBox->AddItem(_T("32"));
	pFontComboBox->AddItem(_T("64"));
	pFontComboBox->AddItem(_T("100"));
	pFontComboBox->AddItem(_T("372"));
	pFontComboBox->AddItem(_T("1000"));
	pFontComboBox->AddItem(_T("10000"));
	pFontComboBox->AddItem(_T("100000"));
	pFontComboBox->AddItem(_T("1000000"));

	pFontComboBox->SelectItem(DEF_PRESCALE_372);

	return TRUE;
}

void CMainFrame::OnUpdateComboPrescale(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//CString csPrescale;
	//CMFCRibbonComboBox* pFontComboBox = DYNAMIC_DOWNCAST(
	//	CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_PRESCALE));
	//csPrescale = pFontComboBox->GetText();

	//int iPrescale = _CString2IntDecimal(csPrescale);
	//if (iPrescale <= 0)
	//	return;


	//m_wndWaveView.InputPrescale(iPrescale);

	////通知更新绘制 WaveView
	//OnUpdateEvent(NULL, NULL);


}


BOOL CMainFrame::CreateStatueBar()
{

	BOOL bNameValid;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);



	//m_wndStatusBar.AddElement        (new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
	//m_Progress.Create(this);

	m_Progress = new CMFCRibbonProgressBar(ID_STATUSBAR_PANE1, 300, 22);
	m_Progress->SetRange(0, 300);
	m_Progress->SetPos(0);
	m_wndStatusBar.AddElement(m_Progress, _T("进度条"));

	strTitlePane2 = _T("0/0             \1");

	m_ProgressData = new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE);
	m_wndStatusBar.AddElement(m_ProgressData, _T("接受到/采集到"));

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// 创建输出窗口
	CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	//if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100),
	//	TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("未能创建输出窗口\n");
	//	return FALSE; // 未能创建
	//}

	//SetDockingWindowIcons(theApp.m_bHiColorIcons);


	//创建 EVENTLIST 停靠控件

	strOutputWnd = "EVENTLIST";
	if (!m_wndEventList.Create(strOutputWnd, this, CRect(0, 0, 100, 100), 
		TRUE, ID_VIEW_EVENTLIST, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	

	strOutputWnd = "WaveForm";
	if (!m_wndWaveView.Create(strOutputWnd, this, CRect(0, 0, 100, 100),
		TRUE, ID_VIEW_WAVEFORM, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);

	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	//
	m_wndEventList.SetIcon(hOutputBarIcon, FALSE);

	m_wndWaveView.SetIcon(hOutputBarIcon, FALSE);

}



// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();

	m_wndEventList.UpdateFonts();
}




void CMainFrame::__Init(void)
{
	GetBitThread = NULL;
	ViewBitThread= NULL;
	dev = NULL; /* the device handle */
	ConnectState = 0;
	threadState  = 1;
	uiRecvLen    = 0;
	Initialize_Ribbon();

	//此行为测试使用
	//*************************
	//CString csTemp;
	//csTemp = _T("010121016103320ED372750132E702 727401 325C04 72E802 72FFFF 33CABD02 72E802 327401 727401 32E802 72E802327401727901326C83725C0432740172740132740172E80232348772A00B323487727501324307725C0432C48572730132E80272D00532E802727901324C8472A00B323887727301327501725B0432E80272E80232C28572B808321D8A72730132E90272E702325C0472740132C08572E70232F78F72730132740172D00532750172600432528472730132E90272730132E80272D50532508472D00532108D727301325D0472430732058672740132E80272740132740172D00532B185722B0A327401727401226E1B010121016104320FD372750132E702727401325C0472E80272FFFF33D3BD0272E80232740172740132E80272E802327401727C01326883725C0432740172740132740172E802322B8772A00B322B87727401324407725C0432B98572740132E80272D00532E802727C01323F8472A00B323187727401327401725C0432E80272E80232BB8572B80832168A72740132E80272E802325C0472740132B68572E80232E98F72740132740172D00532740172640432458472740132E80272740132E80272D80532478472D00532088D727401325C0472440732038672740132E80272740132740172D00532AE85722C0A32740172740132503572750132A40B72740132E902724707327401725E04325D04725E0432740172750132740172E90232D20572750132E90272740132D20572E902322A0972740132A00B727E0132DE02724407327401725C04325C04725C0432740172740132740172E80232D00572740132E80272740132D00572F20272FFFF33E8FE02728002318071C03140718031407140320001728002318071C0314072400131C07180314072800172FFFF33BB95087280013180718A326127724001318071CA319E7280013180718A72FFFF333CCB06728002318072C0013180714031807280023180728002318072800272FFFF");
	//_CString2UcHex(csTemp, __Bits);
	//m_wndWaveView.m_pWaveForm.InputBitsDatas(__Bits, csTemp.GetLength() / 2);

	//m_wndEventList.AddEvent("0101");
	//m_wndEventList.AddEvent("2101");
	//m_wndEventList.AddEvent("6103");
	//m_wndEventList.AddEvent("320ED37");
	//m_wndEventList.AddEvent("727501");
	//m_wndEventList.AddEvent("32E702");
	//m_wndEventList.AddEvent("727401");
	//m_wndEventList.AddEvent("325C04");
	//m_wndEventList.AddEvent("72E802");
	//m_wndEventList.AddEvent("72FFFF");
	//m_wndEventList.AddEvent("33CABD02");
	//m_wndEventList.AddEvent("72E802");
	//m_wndEventList.AddEvent("327401");
	//m_wndEventList.AddEvent("727401");
	//m_wndEventList.AddEvent("32E802");
	//m_wndEventList.AddEvent("72E802");

	m_wndWaveView.InputEventWnd(&m_wndEventList);
	m_wndWaveView.InputPrescale(372);
	//*************************

}
void CMainFrame::OnConnectButton()
{
	UINT PreBits, PreSend;
	if(ConnectState == 0)
	{
		usb_init(); /* initialize the library */
		usb_find_busses(); /* find all busses */
		usb_find_devices(); /* find all connected devices */

		if (!(dev = Open_Dev()))
		{
			AfxMessageBox("connect fail!");
			return;
		}

		if (usb_set_configuration(dev, MY_CONFIG) < 0)
		{
			usb_close(dev);
			AfxMessageBox("set config fail!");
			return;
		}

		if (usb_claim_interface(dev, 0) < 0)
		{
			usb_close(dev);
			AfxMessageBox("set interface fail!");
			return;
		}

		ConnectState = 1;

		_InitBits();


		if (GetBitsSize(&PreBits,&PreSend))
		{
			uiRecvLen = PreSend;
			uiPrinLen = PreSend;
		}


		if (GetBitThread == NULL)
			GetBitThread = AfxBeginThread(GetBits, _T("GetBits")); 	
		if (ViewBitThread == NULL)
			ViewBitThread = AfxBeginThread(ViewBits, _T("ViewBits")); 	
		//GetDlgItem(IDC_ComConnect_Button)->SetWindowText("Disconnect");
	}
	else
	{
		if (dev)
		{
			usb_release_interface(dev, 0);
			usb_close(dev);
			dev = NULL;
		}

		ConnectState = 0;
	}
}




// //打开USB设备
usb_dev_handle * CMainFrame::Open_Dev(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;

	for (bus = usb_get_busses(); bus; bus = bus->next)
	{
		for (dev = bus->devices; dev; dev = dev->next)
		{
			if (dev->descriptor.idVendor == MY_VID
				&& dev->descriptor.idProduct == MY_PID)
			{
				return usb_open(dev);
			}
		}
	}
	return NULL;
}

void CMainFrame::OnDisconnectButton()
{

	if (ConnectState == 1)
	{	if (dev)
		{
			usb_release_interface(dev, 0);
			usb_close(dev);
			dev = NULL;
		}
		ConnectState = 0;
	}
}

 void CMainFrame::OnUpdateConnected(CCmdUI* pCmdUI)
{
	switch(pCmdUI->m_nID)
	{
	case ID_Connect_BUTTON:     pCmdUI->Enable(!ConnectState);break;
	case ID_Disconnect_BUTTON:   pCmdUI->Enable(ConnectState);break;
	default:break;
	}
}
 void CMainFrame::OnDockablePane(UINT iID)
 {
	 
	 switch (iID)
	 {
	 case ID_VIEW_CHECK_EVENTLIST:
		 if (m_wndEventList.IsVisible())
			 m_wndEventList.ShowPane(FALSE, FALSE, TRUE);
		 else
		 { 
			 m_wndEventList.SetAutoHideMode(FALSE, CBRS_ALIGN_ANY);
			 m_wndEventList.ShowPane(TRUE, FALSE, TRUE);
		 }
		 break;
	 case ID_VIEW_CHECK_WAVEFORM: 
		 if (m_wndWaveView.IsVisible())
			 m_wndWaveView.ShowPane(FALSE, FALSE, TRUE);
		 else
		 { 
			 m_wndWaveView.SetAutoHideMode(FALSE, CBRS_ALIGN_ANY);
			 m_wndWaveView.ShowPane(TRUE, FALSE, TRUE);
		 }
		 break;
	 }
 }

 void CMainFrame::OnUpdateDockablePane(CCmdUI* pCmdUI)
 {

	 BOOL bShow;

	 switch (pCmdUI->m_nID)
	 {
	 case ID_VIEW_CHECK_EVENTLIST:bShow = m_wndEventList.IsVisible(); break;
	 case ID_VIEW_CHECK_WAVEFORM:bShow  = m_wndWaveView.IsVisible(); break;
	 default:break;
	 }

	 pCmdUI->SetCheck(bShow);


 }

 LRESULT CMainFrame::OnUpdateProgress(WPARAM /* wParam*/,LPARAM /* LParam*/)
 {
	// UINT iPos = uiPrinLen*100/uiRecvLen;
	 CString csText;
	 if (uiBitsLen!= 0)
	 {
		 UINT iPos = uiRecvLen*300/uiBitsLen;
		 m_Progress->SetPos(iPos);


	 }

	 csText.Format("%d/%d",uiRecvLen,uiBitsLen);
	 m_ProgressData->SetText(csText);

	 m_wndStatusBar.RedrawWindow();


	 return 1;
 }

 LRESULT CMainFrame::OnUpdateEvent(WPARAM /* wParam*/, LPARAM /* LParam*/)
 {

	int iCount = m_wndEventList.GetEventCount();

	if (iCount < 1)
		return 0;
		 

	m_wndWaveView.m_pScrollBar.SetScrollRange(1, iCount);

	m_wndWaveView.m_pWaveForm.OnPaint();

	m_wndWaveView.m_pScrollBar.SetScrollPos(m_wndWaveView.m_pScrollBar.GetScrollPos());

	return 1;
  }




 void CMainFrame::OnComboPrescale()
 {
	 // TODO: 在此添加命令处理程序代码

	 CString csPrescale;
	 CMFCRibbonComboBox* pFontComboBox = DYNAMIC_DOWNCAST(
		 CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_PRESCALE));
	 csPrescale = pFontComboBox->GetEditText();

	 

	 int iPrescale = _CString2IntDecimal(csPrescale);
	 if (iPrescale <= 0)
		 return;


	 m_wndWaveView.InputPrescale(iPrescale);

	 //通知更新绘制 WaveView
	 OnUpdateEvent(NULL, NULL);
 }
