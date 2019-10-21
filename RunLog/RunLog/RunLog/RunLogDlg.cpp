// RunLogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RunLog.h"
#include "RunLogDlg.h"
#include "ExpAPDU/ExpAPDU.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int iThreadEnd;
HWND  h_SafeHand;
stmHandle NewHand;
HANDLE CCR5Hand;


CString UcHex2CString(BYTE* ucInput,int iInputLen)
{

	CString __CS;
	_UcHex2CString(ucInput,iInputLen,__CS);
	return __CS;
}

CString GetCStringData(CString csData,int iNum)
{
	CString __CS;
	_GetCStringData(csData,iNum,__CS);
	return  __CS;
}


CString GetUserDataInTPDU(CString csTPDU)
{

	int iMTI =   _CString2Int(csTPDU.Mid(0,2)) ;
	int iVPF;
	int iDALen;
	iVPF =  _Getbit(csTPDU,5)*2 +_Getbit(csTPDU,4);

	iDALen = _CString2Int(csTPDU.Mid(4,2))+4;
	if (iDALen%2)
		iDALen= iDALen+1;

	int iVPLen = 0;
	if (iVPF != 0)
	{
		if (iVPF == 3)
			iVPLen = 7;
		else
			iVPLen = 1;
	}
	return csTPDU.Mid(iDALen+10+iVPLen*2);
}

CString GetUserDataInCDMATPDU(CString csTPDU)
{

	CString csBearData,csUD,csTEmp;
	csBearData = GetTLVData(csTPDU.Mid(2),0x08);
	if (csBearData.IsEmpty())
		return "";
	csTEmp = GetTLVData(csBearData,01);
	if(csTEmp.IsEmpty())
		return "";

 _RightShift(csTEmp,3,csUD);
  csUD = GetTLVData(csUD,0x00);
	return csUD;

}

bool GetChannelData(CString csFetch,CString& csChannel)
{
	CString csProactiveCommand,csDetails,csTPDU;
	csProactiveCommand = GetTLVData(csFetch,0xD0);
	if (csProactiveCommand.IsEmpty())
		return false;

	csDetails = GetTLVData(csProactiveCommand,0x81);
	if (csDetails.IsEmpty())
			csDetails = GetTLVData(csProactiveCommand,01);

	if ((csDetails.IsEmpty())||
		(csDetails.Mid(2,2) != _T("43")))
		return false;

	csChannel = GetTLVData(csProactiveCommand,0x36);
	if (csChannel.IsEmpty())
		csChannel = GetTLVData(csProactiveCommand,0xB6);

 if (csChannel.IsEmpty())
 {
	 return false;
 }

 return true;
}


bool GetUserDataInSendMessage(CString csFetch,CString& csUD)
{

	CString csProactiveCommand,csDetails,csTPDU;
	csProactiveCommand = GetTLVData(csFetch,0xD0);
	if (csProactiveCommand.IsEmpty())
		return false;

	csDetails = GetTLVData(csProactiveCommand,0x81);
	if (csDetails.IsEmpty())
		csDetails = GetTLVData(csProactiveCommand,01);
	if ((csDetails.IsEmpty())||
		(csDetails.Mid(2,2) != _T("13")))
		return false;

	csTPDU = GetTLVData(csProactiveCommand,0x8B);

	if (csTPDU.IsEmpty())
		csTPDU = GetTLVData(csProactiveCommand,0x0B);
	if (!csTPDU.IsEmpty())
	{
		csUD = GetUserDataInTPDU(csTPDU);

		if (csUD.IsEmpty())
			return false;
		else
		return true;
	}

	csTPDU = GetTLVData(csProactiveCommand,0x48);

	if (csTPDU.IsEmpty())
		csTPDU = GetTLVData(csProactiveCommand,0xC8);
	if (!csTPDU.IsEmpty())
	{
		csUD = GetUserDataInCDMATPDU(csTPDU);

		if (csUD.IsEmpty())
			return false;
		else
			return true;
	}
			


	return false;


}


			// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
			public:
	     			CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



// CRunLogDlg 对话框




CRunLogDlg::CRunLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRunLogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRunLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CTLOG_Edit, m_CTLOG_Edit);
	DDX_Control(pDX, IDC_ReaderList_Combo, m_PCSCReader_Combo);
	DDX_Control(pDX, IDC_TREE1, m_FileTree);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_PROGRESS2, m_ProgreeGlob);
}

BEGIN_MESSAGE_MAP(CRunLogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_INITMENUPOPUP()
	ON_WM_SIZE()


	//}}AFX_MSG_MAP
	ON_COMMAND(ID_OpenCTLOG, &CRunLogDlg::OnOpenctlog)
	ON_COMMAND(ID_PCSC_Reader, &CRunLogDlg::OnPcscReader)
	ON_BN_CLICKED(IDC_Connect_Button, &CRunLogDlg::OnBnClickedConnectButton)
	ON_EN_SETFOCUS(IDC_CTLOG_Edit, &CRunLogDlg::OnEnSetfocusCtlogEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CRunLogDlg::OnNMDblclkTree1)
	ON_COMMAND(ID_OpenDIR, &CRunLogDlg::OnOpendir)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CRunLogDlg::OnNMClickTree1)
	ON_COMMAND(ID_All_List, &CRunLogDlg::OnAllList)
	ON_BN_CLICKED(IDC_Stop, &CRunLogDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_Pause, &CRunLogDlg::OnBnClickedPause)
	ON_COMMAND(ID_1_Script, &CRunLogDlg::On1Script)
	ON_COMMAND(ID_MenuStop, &CRunLogDlg::OnMenustop)
	ON_COMMAND(ID_MenuParse, &CRunLogDlg::OnMenuparse)
	ON_COMMAND(ID_CCR5_Reader, &CRunLogDlg::OnCcr5Reader)
	ON_COMMAND(ID_Other_Reader, &CRunLogDlg::OnOtherReader)
	ON_UPDATE_COMMAND_UI(ID_PCSC_Reader, &CRunLogDlg::OnUpdatePcscReader)
	ON_UPDATE_COMMAND_UI(ID_CCR5_Reader, &CRunLogDlg::OnUpdateCcr5Reader)
	ON_UPDATE_COMMAND_UI(ID_Other_Reader, &CRunLogDlg::OnUpdateOtherReader)
	ON_COMMAND(ID_WriteLoG, &CRunLogDlg::OnWritelog)
	ON_COMMAND(ID_WrongPause, &CRunLogDlg::OnWrongpause)
	ON_UPDATE_COMMAND_UI(ID_WriteLoG, &CRunLogDlg::OnUpdateWritelog)
	ON_UPDATE_COMMAND_UI(ID_WrongPause, &CRunLogDlg::OnUpdateWrongpause)
	ON_COMMAND(ID_ADJ_0x82, &CRunLogDlg::OnAdj0x82)
	ON_UPDATE_COMMAND_UI(ID_Para_0x82, &CRunLogDlg::OnUpdatePara0x82)
	ON_UPDATE_COMMAND_UI(ID_ADJ_0x82, &CRunLogDlg::OnUpdateAdj0x82)
	ON_COMMAND(ID_Para_0x82, &CRunLogDlg::OnPara0x82)
	ON_UPDATE_COMMAND_UI(ID_MenuParse, &CRunLogDlg::OnUpdateMenuparse)
	ON_UPDATE_COMMAND_UI(ID_MenuStop, &CRunLogDlg::OnUpdateMenustop)
	ON_UPDATE_COMMAND_UI(ID_1_Script, &CRunLogDlg::OnUpdate1Script)
	ON_UPDATE_COMMAND_UI(ID_All_List, &CRunLogDlg::OnUpdateAllList)
	ON_COMMAND(ID_MenuRestore, &CRunLogDlg::OnMenurestore)
	ON_UPDATE_COMMAND_UI(ID_MenuRestore, &CRunLogDlg::OnUpdateMenurestore)
	ON_COMMAND(ID_ADJ_0x12, &CRunLogDlg::OnAdj0x12)
	ON_COMMAND(ID_Para_0x12, &CRunLogDlg::OnPara0x12)
	ON_UPDATE_COMMAND_UI(ID_ADJ_0x12, &CRunLogDlg::OnUpdateAdj0x12)
	ON_UPDATE_COMMAND_UI(ID_Para_0x12, &CRunLogDlg::OnUpdatePara0x12)
	ON_COMMAND(ID_UIM_UPDATESSD, &CRunLogDlg::OnUimUpdatessd)
	ON_COMMAND(ID_CSIM_MANAGESSD, &CRunLogDlg::OnCsimManagessd)
	ON_COMMAND(ID_CatchLog, &CRunLogDlg::OnCatchlog)

	ON_BN_CLICKED(IDC_Exit, &CRunLogDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_Restore, &CRunLogDlg::OnBnClickedRestore)
	ON_COMMAND(ID_ADJ_0xF2, &CRunLogDlg::OnAdj0xf2)
	ON_UPDATE_COMMAND_UI(ID_ADJ_0xF2, &CRunLogDlg::OnUpdateAdj0xf2)
	ON_COMMAND(ID_ADJ_0xC0, &CRunLogDlg::OnAdj0xc0)
	ON_UPDATE_COMMAND_UI(ID_ADJ_0xC0, &CRunLogDlg::OnUpdateAdj0xc0)
	ON_COMMAND(ID_Leve1, &CRunLogDlg::OnLeve1)
	ON_UPDATE_COMMAND_UI(ID_Leve1, &CRunLogDlg::OnUpdateLeve1)
	ON_COMMAND(ID_Leve2, &CRunLogDlg::OnLeve2)
	ON_UPDATE_COMMAND_UI(ID_Leve2, &CRunLogDlg::OnUpdateLeve2)
	ON_UPDATE_COMMAND_UI(ID_Leve3, &CRunLogDlg::OnUpdateLeve3)
	ON_COMMAND(ID_Leve3, &CRunLogDlg::OnLeve3)
	ON_COMMAND(ID_Leve4, &CRunLogDlg::OnLeve4)
	ON_UPDATE_COMMAND_UI(ID_Leve4, &CRunLogDlg::OnUpdateLeve4)
	ON_COMMAND(ID_Leve5, &CRunLogDlg::OnLeve5)
	ON_UPDATE_COMMAND_UI(ID_Leve5, &CRunLogDlg::OnUpdateLeve5)
	ON_UPDATE_COMMAND_UI(ID_OpenCTLOG, &CRunLogDlg::OnUpdateOpenctlog)
	ON_UPDATE_COMMAND_UI(ID_OpenDIR, &CRunLogDlg::OnUpdateOpendir)
	ON_UPDATE_COMMAND_UI(ID_CatchLog, &CRunLogDlg::OnUpdateCatchlog)
	ON_BN_CLICKED(IDC_OneStep, &CRunLogDlg::OnBnClickedOnestep)
	ON_COMMAND(ID_ADJ_Res_0x84, &CRunLogDlg::OnAdjRes0x84)
	ON_UPDATE_COMMAND_UI(ID_ADJ_Res_0x84, &CRunLogDlg::OnUpdateAdjRes0x84)
	ON_COMMAND(ID_ADJ_Res_0x8A, &CRunLogDlg::OnAdjRes0x8a)
	ON_UPDATE_COMMAND_UI(ID_ADJ_Res_0x8A, &CRunLogDlg::OnUpdateAdjRes0x8a)
	ON_COMMAND(ID_WriteRunTime, &CRunLogDlg::OnWriteruntime)
	ON_UPDATE_COMMAND_UI(ID_WriteRunTime, &CRunLogDlg::OnUpdateWriteruntime)
	ON_COMMAND(ID_ADJ_Res_0xA4, &CRunLogDlg::OnAdjRes0xa4)
	ON_UPDATE_COMMAND_UI(ID_ADJ_Res_0xA4, &CRunLogDlg::OnUpdateAdjRes0xa4)
	ON_COMMAND(ID_ADJ_Res_0x32, &CRunLogDlg::OnAdjRes0x32)
	ON_UPDATE_COMMAND_UI(ID_ADJ_Res_0x32, &CRunLogDlg::OnUpdateAdjRes0x32)




	ON_COMMAND(ID_OpenCMCCLog, &CRunLogDlg::OnOpencmcclog)
	ON_COMMAND(ID_OpenCMCCList, &CRunLogDlg::OnOpencmcclist)
	ON_UPDATE_COMMAND_UI(ID_OpenCMCCLog, &CRunLogDlg::OnUpdateOpencmcclog)
	ON_UPDATE_COMMAND_UI(ID_OpenCMCCList, &CRunLogDlg::OnUpdateOpencmcclist)


	ON_COMMAND(ID_About, &CRunLogDlg::OnAbout)
	ON_COMMAND(ID_ADJ_0x10, &CRunLogDlg::OnAdj0x10)
	ON_UPDATE_COMMAND_UI(ID_ADJ_0x10, &CRunLogDlg::OnUpdateAdj0x10)
	ON_COMMAND(ID_STK_StartUp, &CRunLogDlg::OnStkStartup)
	ON_UPDATE_COMMAND_UI(ID_STK_StartUp, &CRunLogDlg::OnUpdateStkStartup)
	ON_COMMAND(ID_ADJ_0x14, &CRunLogDlg::OnAdj0x14)
	ON_UPDATE_COMMAND_UI(ID_ADJ_0x14, &CRunLogDlg::OnUpdateAdj0x14)
	ON_COMMAND(ID_ConvertionLog, &CRunLogDlg::OnConvertionlog)
	ON_STN_CLICKED(IDC_CurrentFile, &CRunLogDlg::OnStnClickedCurrentfile)
	ON_BN_CLICKED(IDC_StartCase, &CRunLogDlg::OnBnClickedStartcase)
	ON_COMMAND(ID_DefaultSetting, &CRunLogDlg::OnDefaultsetting)
	ON_UPDATE_COMMAND_UI(ID_ConvertionLog, &CRunLogDlg::OnUpdateConvertionlog)
	ON_COMMAND(ID_BJ_LOG, &CRunLogDlg::OnBjLog)
		ON_COMMAND(ID_Path_Setting_Button, &CRunLogDlg::OnPathSettingButton)
		END_MESSAGE_MAP()


// CRunLogDlg 消息处理程序

BOOL CRunLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	GetClientRect(&m_RectSize);
	//	ShowWindow(SW_MINIMIZE);

	Init_Control();

	//LOG解析等级
	SetExplainLevel(0xF);
	// TODO: 在此添加额外的初始化代码

	//初始化存储LOG的位置
//	csTestLogPath.Empty();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRunLogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void CRunLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CRect crControl,m_NRectSize;
	CWnd *p_Wnd;
	int iCyc;

	int iADJ2Control[29] = {IDC_CardATR,IDC_CardStatuse,IDC_Times,
		IDC_STATIC_TIME,IDC_CurrentRow,IDC_CurrentFile,IDC_PROGRESS1,IDC_PROGRESS2,
		IDC_TREE1,IDC_STATIC_Line,
		IDC_CTLOG_Edit,
		IDC_OneStep,IDC_Restore,IDC_Pause,IDC_Stop,IDC_StartCase,IDC_Exit,
		IDC_CurrentSend_Edit,IDC_CurrentResponse_Edit,IDC_ExpectReponse_Edit,IDC_CurrentSW_Edit,IDC_ExpectSW_Edit,
		IDC_STATIC4,IDC_STATIC5,IDC_STATIC6,IDC_STATIC7,IDC_ExpectLine,IDC_RecieveLine,
		IDC_STATIC8};
	GetClientRect(&m_NRectSize);

	int iSum;

	//当为最小化的时候不进行更变大小
	if ((m_NRectSize.bottom != 0)||(m_NRectSize.right!=0))
	{

		iSum =sizeof(iADJ2Control)/4;

		for (iCyc = 0 ; iCyc < iSum ; iCyc++)
		{


			p_Wnd = GetDlgItem(iADJ2Control[iCyc]);

			if (p_Wnd == NULL)
				return;

			p_Wnd->GetWindowRect(&crControl);

			ScreenToClient(&crControl);

			//顶端位置
			if ((iCyc < 8)||(iCyc>28))
				crControl.top = crControl.top +  m_NRectSize.bottom - m_RectSize.bottom ;

			//底端位置
			if ((iCyc < 11)||(iCyc>28))
				crControl.bottom = crControl.bottom + m_NRectSize.bottom-m_RectSize.bottom  ;


			//右边位置
			if ((iCyc > 8)||(iADJ2Control[iCyc] == IDC_PROGRESS1)||(iADJ2Control[iCyc] ==IDC_PROGRESS2))
				crControl.right =  crControl.right + m_NRectSize.right - m_RectSize.right  ;

			//左边位置
			if (iCyc > 10)
				crControl.left =  crControl.left + m_NRectSize.right - m_RectSize.right  ;

			p_Wnd->MoveWindow(crControl);

		}
		m_RectSize = m_NRectSize;
	}
}


void CRunLogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CRunLogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRunLogDlg::OnOpendir()
{

	CString csDIR;


	OnBnClickedStop();

	csDIR  = GetDIR();

	if (csDIR.IsEmpty())
		return;


	GetLogFileList(csDIR);
	iLogType = _CT_LOG;

}

void CRunLogDlg::Put1File2Edit(CString csFileName,CString csFilePath)
{
	HTREEITEM hCurTreeItem ;

	m_FileTree.DeleteAllItems();

	hCurTreeItem = NULL;

	if (!csFileName.IsEmpty())
	{
		hCurTreeItem = m_FileTree.InsertItem(csFileName,Def_Image_File,Def_Image_File);
		m_FileTree.InsertItem(csFilePath,Def_Image_Path,Def_Image_Path,hCurTreeItem);
		m_FileTree.SetCheck(hCurTreeItem);
		m_FileTree.SelectItem(hCurTreeItem);
		SetItemFileData2Edit(hCurTreeItem);
	}


}

void CRunLogDlg::OnOpenctlog()
{
	CString csFileName,csFilePath;

	//	CString csData;

	//	HTREEITEM hCurTreeItem ;

	OnBnClickedStop();

	ContrleBusy(TRUE);

	CFileDialog CFDataFile(TRUE,NULL,_T("*.LOG"),4|1,_T("Data File文件(*.LOG)"));

	if ( CFDataFile.DoModal() == IDOK)
	{

		csFileName = CFDataFile.GetFileTitle();

		csFilePath = CFDataFile.GetPathName();

		iLogType = _CT_LOG;

		Put1File2Edit(csFileName,csFilePath);


	}
	ContrleBusy(FALSE);


	return ;
}

void CRunLogDlg::OnCatchlog()
{
	CString csFileName,csFilePath;

	OnBnClickedStop();

	ContrleBusy(TRUE);

	CFileDialog CFDataFile(TRUE,NULL,_T("*.*"),4|1,_T("Data File文件(*.*)"));
	if ( CFDataFile.DoModal() == IDOK)
	{

		csFileName = CFDataFile.GetFileTitle();

		csFilePath = CFDataFile.GetPathName();

		iLogType = _Catch_LOG;
		Put1File2Edit(csFileName,csFilePath);

	}
	ContrleBusy(FALSE);


	return ;
}

void CRunLogDlg::OnOpencmcclog()
{

	CString csFileName,csFilePath;


	OnBnClickedStop();

	ContrleBusy(TRUE);

	CFileDialog CFDataFile(TRUE,NULL,_T("*.HTML"),4|1,_T("HTML File文件(*.HTML)"));
	if ( CFDataFile.DoModal() == IDOK)
	{


		csFileName = CFDataFile.GetFileTitle();

		csFilePath = CFDataFile.GetPathName();

		iLogType = _CMCC_LOG;

		Put1File2Edit(csFileName,csFilePath);


	}
	ContrleBusy(FALSE);


	return ;
}

void CRunLogDlg::OnConvertionlog()
{
	CString csFileName,csFilePath;

	OnBnClickedStop();

	ContrleBusy(TRUE);

	CFileDialog CFDataFile(TRUE,NULL,_T("*.txt"),4|1,_T("Data File文件(*.txt)"));
	if ( CFDataFile.DoModal() == IDOK)
	{

		csFileName = CFDataFile.GetFileTitle();

		csFilePath = CFDataFile.GetPathName();

		iLogType = _Convertion_LOG;

		Put1File2Edit(csFileName,csFilePath);


	}
	ContrleBusy(FALSE);


	return ;
}

//************************************
// Method:    OnBjLog
// FullName:  打开北京大的抓包器数据
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CRunLogDlg::OnBjLog()
{
	CString csFileName,csFilePath;

	OnBnClickedStop();

	ContrleBusy(TRUE);

	CFileDialog CFDataFile(TRUE,NULL,_T("*.*"),4|1,_T("Data File文件(*.*)"));
	if ( CFDataFile.DoModal() == IDOK)
	{

		csFileName = CFDataFile.GetFileTitle();

		csFilePath = CFDataFile.GetPathName();

		iLogType = _BJ_LOG;
		Put1File2Edit(csFileName,csFilePath);

	}
	ContrleBusy(FALSE);


	return ;
}

void CRunLogDlg::OnBnClickedConnectButton()
{

	CString csCardATR;
	int iRet;



	switch(iCurReader)
	{
	case Def_PCSC:
		iRet = Connect_PSCS(&csCardATR);
		break;


	case Def_CCR5:

		iRet = Connect_CCR5(&csCardATR);

		break;

	case  Def_XXXX:

		iRet = Connect_Stm(&csCardATR);

		break;

	default:
		break;
	}
	m_PCSCReader_Combo.EnableWindow(TRUE);

	CStringArray csTemp;
	ExplainATR(csCardATR,csTemp);
	
	SetStatus(csCardATR);

}

void CRunLogDlg::OnCcr5Reader()
{
	iCurReader = Def_CCR5;
	Init_CCR5_Combo();
}

void CRunLogDlg::OnOtherReader()
{
	iCurReader = Def_XXXX;
	Init_Stm_Combo();
}

void CRunLogDlg::OnPcscReader()
{
	iCurReader = Def_PCSC;
	Init_PCSCReader_Combo();
}

void CRunLogDlg::OnUpdatePcscReader(CCmdUI *pCmdUI)
{
	int iFlag;
	if (iCurReader == Def_PCSC)
	{
		iFlag = 1;
	}
	else
		iFlag = 0;
	pCmdUI->SetCheck(iFlag);
	if (iConSta == 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);

}

void CRunLogDlg::OnUpdateCcr5Reader(CCmdUI *pCmdUI)
{
	int iFlag;
	if (iCurReader == Def_CCR5)
	{
		iFlag = 1;
	}
	else
		iFlag = 0;
	pCmdUI->SetCheck(iFlag);
	if (iConSta == 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CRunLogDlg::OnUpdateOtherReader(CCmdUI *pCmdUI)
{
	int iFlag;
	if (iCurReader == Def_XXXX)
	{
		iFlag = 1;
	}
	else
		iFlag = 0;
	pCmdUI->SetCheck(iFlag);
	if (iConSta == 0)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CRunLogDlg::OnWritelog()
{
	bWriteLog = !bWriteLog;
}

void CRunLogDlg::OnWrongpause()
{
	bWrongPause = !bWrongPause;
}

void CRunLogDlg::OnUpdateWritelog(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(bWriteLog);
}

void CRunLogDlg::OnWriteruntime()
{
	bWriteTime = !bWriteTime;
}

void CRunLogDlg::OnUpdateWriteruntime(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bWriteTime);
}

void CRunLogDlg::OnUpdateWrongpause(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(bWrongPause);
}

void CRunLogDlg::OnMenustop()
{

	OnBnClickedStop();
}

void CRunLogDlg::OnMenuparse()
{
	OnBnClickedPause();
}


void CRunLogDlg::OnBnClickedPause()
{

	DWORD code;
	GetExitCodeThread(hThread,&code);

	if (code != STILL_ACTIVE)
	{
		return;
	}

	if (iThreadEnd >0)
	{
		iThreadEnd = 0;	
		ContrleBusy(FALSE);
		SuspendThread(hThread);
	}

}

void CRunLogDlg::OnBnClickedStop()
{

	DWORD code;



	GetExitCodeThread(hThread,&code);

	if (code != STILL_ACTIVE)
		return;


	if (iThreadEnd == 0)
	{
		iThreadEnd = -1;
		ResumeThread(hThread);
	}else
		iThreadEnd = -1;


	ContrleBusy(FALSE);

}

void CRunLogDlg::On1Script()
{
	DWORD code;


	b1Step = false;

	if (m_CTLOG_Edit.GetLineCount()<2)
	{
		MessageBox("未发现脚本内容！");
		return;
	}


	if (CurrentStatus()!=0)
		return;

	GetExitCodeThread(hThread,&code);      


	if (code != STILL_ACTIVE)
	{

		m_CTLOG_Edit.SetSel(0,0);
		m_CTLOG_Edit.SetFocus();
		hThread  = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) ThreadRun,NULL, 0, NULL);

	}
	else
		ResumeThread(hThread);


	iThreadEnd = 1;	
	ContrleBusy(TRUE);

	return ;

}

void CRunLogDlg::OnAllList()
{
	DWORD code;

	b1Step = false;

	if (m_FileTree.GetRootItem() == NULL)
	{
		MessageBox("未发现脚本列表！");
		return;
	}

	if (CurrentStatus()!=0)
		return;


	GetExitCodeThread(hThread,&code);      




	if (code != STILL_ACTIVE)
	{


		m_CTLOG_Edit.SetSel(0,0);
		m_CTLOG_Edit.SetFocus();

		hThread  = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) ThreadAllRun,NULL, 0, NULL);


	}
	else
		ResumeThread(hThread);

	iThreadEnd = 1;

	ContrleBusy(TRUE);
	return ;


}
//
//void CRunLogDlg::OnBnClickedRunButton()
//{
//
//
//
//	DWORD code;
//
//	if (CurrentStatus()!=0)
//	{
//		return;
//	}
//	GetExitCodeThread(hThread,&code);		
//
//
//
//	if (code != STILL_ACTIVE)
//	{
//		m_CTLOG_Edit.SetSel(0,0);
//		m_CTLOG_Edit.SetFocus();
//		hThread  = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)ThreadRun,NULL, 0, NULL);
//	}
//	else
//	{
//		ResumeThread(hThread);
//	}
//
//	iThreadEnd = 1;	
//	ContrleBusy(TRUE);
//
//	return ;
//
//}



void CRunLogDlg::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items. 

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL); 

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	} 

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it. 

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		} 

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
} 

BOOL CRunLogDlg::CreateLogFile(CString csFile)
{

	CStdioFile CSFFile;
	BOOL bRet;


	bRet = CSFFile.Open(csFile,CFile::modeCreate);
	if (!bRet)
		MessageBox("创建Log文件失败");
	CSFFile.Close();

	return bRet;


}


BOOL CRunLogDlg::CreateErrFile(CString csFile)
{

	CStdioFile CSFFile;
	BOOL bRet;

	bRet = CSFFile.Open(csFile,CFile::modeRead);

	if (!bRet)
		bRet = CSFFile.Open(csFile,CFile::modeCreate);

	if (!bRet)
		MessageBox("创建Log文件失败");
	CSFFile.Close();

	return bRet;


}


void CRunLogDlg::TRSPCommand(CString csData)
{

	if (csData.Mid(0,2) .Compare("\\$") != 0)
		return;

	if (csData.Mid(2).Compare(_T("STOP")) == 0)
	{
		OnBnClickedPause();	
		return;
	}

	MessageBox(csData.Mid(2),_T("TEXT.."),MB_OK);
	/*if (csData.Mid(2).Compare(_T("\\$PAUSE")) == 0)
	OnBnClickedPause();	*/

}

void CRunLogDlg::RunLOGofEdit(HTREEITEM hCurrentItem)
{


	int iCyc,iRet;
	int iLine,iLen,iLineIndex;
	bool bResult;

	CString csData;
	CString csSend,csRes;
	CString csATR;
	CString csRows;



	iLine = m_CTLOG_Edit.GetLineCount();

	m_CTLOG_Edit.SetFocus();

	bResult = true;

	csData.Empty();

	//RESET
	Run1LineScript("0012000000");

	//BJLOG 特用
	//此处为了ＢＪＬＯＧ　使用特别存在，如果不是则取消

	uiPreRet= _BJ_ABORT;
	if (iLogType == _BJ_LOG)
		PcscReader.SetAutoGetResponse(1);
	else
		PcscReader.SetAutoGetResponse(0);

	Init_Progress(iLine);

	for (iCyc=0;iCyc<iLine;iCyc++)	
	{
		if ((CheckPrograssStatus(hThread))&&((iThreadEnd<0)))
			ExitThread(0); 

		SetProgress(iCyc);

		iLineIndex = m_CTLOG_Edit.LineIndex(iCyc);

		iLen       = m_CTLOG_Edit.LineLength(iLineIndex);

		csRows.Format("%d",iCyc);

		SetDlgItemText(IDC_CurrentRow,_T("行：")+csRows);

		m_CTLOG_Edit.SetSel(iLineIndex,iLineIndex+iLen);



		if (iLen!=0)
		{

			csData =m_CTLOG_Edit.GetSelText();


			TRSPCommand(csData);

			iRet = TransationData(csData);

			m_CTLOG_Edit.SetFocus();

			//当iRet == 0 则判断为已经收到完整的发送数据
			if (iRet == _LOG_Complete)
			{

				iRet = GetDlgItemText(IDC_CurrentSend_Edit,csSend);

				if (iRet != 0)
				{
					csRes.Empty();

					if (PretreatCurrentCommand())
						GetDlgItemText(IDC_CurrentSend_Edit,csSend);


					if (csSend.Compare("compsta") == 0)
					{
						break;
					}

					csRes = Run1LineScript(csSend);




					if ((csSend == "0012000000")||(csSend =="$0")||(csSend =="Reset")||(csSend =="RESET")||(csSend =="reset"))

					{
						csATR = csRes;
						csRes =csRes +"9000";
					}

					SetStatus(csATR);


				}
				else
					break;

				iRet = CompareResponse(csRes);


				PostCurrentCommand();

				PriteCurrentTime();


				if (bWriteLog)
				{
					PrintCurrentTransation();

					if (iRet==false)
					{
						PrintLog2File("//Error!");
						PrintLog2File("//Error in row "+csRows+"!");
						PrintErr2File("Error in " + csCurLogFile+"!");
					}

				}

				if (iRet == false)
				{

					if (hCurrentItem==NULL)
						hCurrentItem = m_FileTree.GetSelectedItem();
					m_FileTree.SetItemImage(hCurrentItem,Def_Image_Wrong,Def_Image_Wrong);

					if (bWrongPause)
						OnBnClickedPause();	

					bResult = false;
				}
				else if (b1Step)
					OnBnClickedPause();	

			}
		}
	}

	if (bResult)
	{
		if (hCurrentItem==NULL)
			hCurrentItem = m_FileTree.GetSelectedItem();
		m_FileTree.SetItemImage(hCurrentItem,Def_Image_Right,Def_Image_Right);
	}


	PriteCurrentTime();
}



int CRunLogDlg::Print2Edit(CString csData)
{

	m_CTLOG_Edit.SetSel(-1,-1);;
	m_CTLOG_Edit.ReplaceSel(csData+_T("\r\n"));
	m_CTLOG_Edit.SetFocus();
	return true;
}



int CRunLogDlg::Init_Control(void)
{

	Init_PCSCReader_Combo();
	Init_File_Tree();
	Init_Progress();

	_CString2UcHex("7B70B748F0765AB7",ucAKey);

	_CString2UcHex("11223344556677881122334455667788",ucKIc);

	_CString2UcHex("11223344556677881122334455667788",ucKID);

	ZeroMemory(ucRand,4);
	ZeroMemory(ucUIMID,8);
	ZeroMemory(ucRandSSD,7);
	ZeroMemory(ucIMSI_M,0xA);


	iPreIns         = 0;
	iFetchLen       = 0;
	iStatusLen      = 0;
	iGetResponseLen = 0;
	iCurReader      = Def_PCSC;
	iConSta         = -1;
	iLogType        =  0;



	bWrongPause=  true;
	bWriteLog  =  true;
	bWriteTime =  true;


	bAdj       =  true;
	bADJ0x10   =  true;
	bADJ0x12   =  true;
	bADJ0x14   =  true;
	bADJ0x82   =  true;
	bADJ0xC0   =  true;
	bADJ0xF2   =  true;



	bADJRes0x8A = true;
	bADJRes0x84 = true;
	bADJRes0xA4 = true;
	bADJRes0x32 = true;


	bADJSTKStartup = true;


	bInSTK = false;
	bInSTKStartup = false;




	iCheckLevel=  1;
	iThreadEnd = -1;
	h_SafeHand = GetSafeHwnd();




	GetModuleFileName(NULL,csCurMDir.GetBuffer(255),255);
	csCurMDir.ReleaseBuffer();
	csCurMDir = csCurMDir.Left ((int) csCurMDir.ReverseFind( '\\'));

	GetSetting(csCurMDir+_T("\\Setting.ini"));

	csTestLogPath = csCurMDir;

	ContrleBusy(FALSE);
	return 0;
}

int CRunLogDlg::Init_CCR5_Combo(void)
{

	CString csTemp;
	unsigned char ucProt[10];
	unsigned int uiPortMax;
	BOOL bRet;
	uiPortMax = 10;
	m_PCSCReader_Combo.ResetContent();
	//bRet = CCR5_GetList(ucProt,&uiPortMax);

	bRet = TRUE;

	uiPortMax = 5;
	ucProt[0] = 1;
	ucProt[1] = 2;
	ucProt[2] = 3;
	ucProt[3] = 4;
	if (bRet)
	{
		for (unsigned int i = 1 ; i<= uiPortMax;i++)
		{
			csTemp.Format("%d",ucProt[i-1]);
			m_PCSCReader_Combo.AddString("COM"+csTemp);
		}
		m_PCSCReader_Combo.SetCurSel(0);
	}

	return bRet;



}

int CRunLogDlg::Init_PCSCReader_Combo(void)
{

	int iSum;
	LPTSTR    szReaderName[10];

	m_PCSCReader_Combo.ResetContent();

	iSum  =  PcscReader.GetContext();
	iSum  =  PcscReader.GetPcsclist(szReaderName);
	for (int i = 0 ;i < iSum;i++)
		m_PCSCReader_Combo.AddString(szReaderName[i]);

	m_PCSCReader_Combo.SetCurSel(0);
	return iSum;
}


int CRunLogDlg::Init_Stm_Combo(void)
{

	unsigned int uiListLen;
	char  *cReadList;
	CString csReadList;
	unsigned int iCyc;


	STM_GetTermList(STM_LIST_ALL,1000,&uiListLen,NULL);
	cReadList = new char[uiListLen];
	STM_GetTermList(STM_LIST_ALL,uiListLen,&uiListLen,cReadList);
	m_PCSCReader_Combo.ResetContent();

	m_PCSCReader_Combo.AddString(cReadList);
	for (iCyc = 0 ; iCyc< uiListLen-2;iCyc++)
	{
		if (cReadList[iCyc] == 0x0)
			m_PCSCReader_Combo.AddString(cReadList+iCyc+1);
	}



	delete cReadList;

	m_PCSCReader_Combo.SetCurSel(0);

	return uiListLen;
}



int CRunLogDlg::Init_File_Tree(void)
{
	HICON hFileImage[6];

	hFileImage[0] = AfxGetApp()->LoadIcon(IDI_DIRIco);
	hFileImage[1] = AfxGetApp()->LoadIcon(IDI_FileIco);
	hFileImage[2] = AfxGetApp()->LoadIcon(IDI_PathIco);
	hFileImage[3] = AfxGetApp()->LoadIcon(IDI_RightIco);
	hFileImage[4] = AfxGetApp()->LoadIcon(IDI_WrongIco);
	hFileImage[5] = AfxGetApp()->LoadIcon(IDI_ArrowIco);

	CImageList* m_FileImage = new CImageList;
	m_FileImage->Create(16,16,ILC_COLOR32,7,7);

	m_FileImage->SetBkColor(RGB(255,255,255));

	for (int i = 0 ;i<6;i++)
	{
		m_FileImage->Add(hFileImage[i]);
	}


	m_FileTree.SetImageList(m_FileImage,TVSIL_NORMAL);

	return 5;
}


int CRunLogDlg::Init_Progress(int iRange)
{

	m_Progress.SetRange32(0,iRange);
	return 0;

}

int CRunLogDlg::Init_ProgressGlob(int iRange)
{

	m_ProgreeGlob.SetRange32(0,iRange);
	m_ProgreeGlob.SetPos(0);
	return 0;

}


int CRunLogDlg::CurrentStatus(void)
{
	return iConSta;
}



int CRunLogDlg::TransationData(CString csData)
{

	int iRet;

	switch(iLogType)
	{
	case _CT_LOG:
		iRet = TransationLogFileData(csData);
		break;
	case _Catch_LOG:
		iRet = TransationCatchLogFileData(csData);
		break;
	case  _CMCC_LOG:
		iRet = TransationCMCCLogFileData(csData);

		break;
	case _Convertion_LOG:

		iRet = TransationConvertionLogFileData(csData);

		break;
	case _BJ_LOG:
		uiPreRet = TransationBJLogFileData(csData,uiPreRet); 
		if ((uiPreRet == _BJ_APDU_SW)||
			(uiPreRet == _BJ_RESET))
			iRet = _LOG_Complete;
		else
			iRet = 1;

		
		break;
	default:
		iRet = TransationLogFileData(csData);


		break;


	}

	return iRet;
}

int CRunLogDlg::TransationCatchLogFileData(CString csData)
{

	CString csTag;
	CString csTemp;
	int iLen,iRet,iApduIns;
	iRet   = 0x10;
	csData = DeleteEnterSpace(csData);
	if (csData.IsEmpty())
		return iRet;



	iLen = csData.Find("//");
	if (iLen>=0)
		return iRet;

	iLen = csData.Find(":");

	if (iLen>0)
	{
		csTag  = csData.Left(iLen);

		iLen   = csData.GetLength()-iLen-1;
		csData = csData.Right(iLen); 

	}

	else
	{

		GetDlgItemText(IDC_CurrentSend_Edit,csTemp);

		if (csTemp=="$0")
			return _CTLOG_CONT;

		iApduIns = _CString2Int(csTemp.Mid(2,2));

		switch(iApduIns)
		{
		case 0xA4:
		case 0xD6:
		case 0xDC:
		case 0xA2:
		case 0x32:
		case 0x20:
		case 0x24:
		case 0x26:
		case 0x28:
		case 0x2c:
		case 0x04:
		case 0x44:
		case 0x88:
		case 0x10:
		case 0xC2:
		case 0x14:
		case 0x8E:
		default:
			csData = csTemp+csData;

			SetDlgItemText(IDC_CurrentSend_Edit,csData);
			iRet = _CTLOG_AH;

			break;

		case 0x70:
		case 0xF2:
		case 0xB2:
		case 0xB0:
		case 0xC0:
		case 0x12:
			GetDlgItemText(IDC_ExpectReponse_Edit,csTemp);

			csData = csTemp+csData;
			SetDlgItemText(IDC_ExpectReponse_Edit,csData);
			break;
		}

	}

	if (csTag == "APDU")
	{
		SetDlgItemText(IDC_CurrentSend_Edit,csData);
		SetDlgItemText(IDC_CurrentResponse_Edit,"");
		SetDlgItemText(IDC_CurrentSW_Edit,"");
		SetDlgItemText(IDC_ExpectReponse_Edit,"");
		SetDlgItemText(IDC_ExpectSW_Edit,"");

		iRet = _CTLOG_AH;

	}

	if (csTag == "DATA")
	{

		GetDlgItemText(IDC_CurrentSend_Edit,csTemp);

		iApduIns = _CString2Int(csTemp.Mid(2,2));

		switch(iApduIns)
		{
		case 0xA4:
		case 0xD6:
		case 0xDC:
		case 0xA2:
		case 0x32:
		case 0x20:
		case 0x24:
		case 0x26:
		case 0x28:
		case 0x2c:
		case 0x04:
		case 0x44:
		case 0x88:
		case 0x10:
		case 0xC2:
		case 0x14:
		case 0x8E:
		default:

			csData = csTemp+csData;

			SetDlgItemText(IDC_CurrentSend_Edit,csData);
			iRet = _CTLOG_AH;

			break;

		case 0x70:
		case 0xF2:
		case 0xB2:
		case 0xB0:
		case 0xC0:
		case 0x12:
			SetDlgItemText(IDC_ExpectReponse_Edit,csData);
			break;


		}



	}

	if (csTag == "SW")
	{
		SetDlgItemText(IDC_ExpectSW_Edit,csData);
		iRet = _LOG_Complete;

	}

	if (csTag == "ATR")
	{			
		SetDlgItemText(IDC_CurrentSend_Edit,csData);
		SetDlgItemText(IDC_CurrentResponse_Edit,"");
		SetDlgItemText(IDC_CurrentSW_Edit,"");
		SetDlgItemText(IDC_ExpectReponse_Edit,"");
		SetDlgItemText(IDC_ExpectSW_Edit,"");

		SetDlgItemText(IDC_CurrentSend_Edit,"$0");
		iRet = _LOG_Complete;

	}



	return iRet;

}

int CRunLogDlg::TransationLogFileData(CString csData)
{
	CString csTag,csExpData;
	CString csUD;
	int iLen,iRet;

	iRet   = _CTLOG_CONT;
	csData = DeleteEnterSpace(csData);

	iLen = csData.GetLength();

	if (iLen< 3)
		return -1;

	csTag = csData.Left(03);
	csData  = csData.Right(iLen-3);



	if (csTag == "AH:")
	{

		SetDlgItemText(IDC_CurrentSend_Edit,csData);
		SetDlgItemText(IDC_CurrentResponse_Edit,"");
		SetDlgItemText(IDC_CurrentSW_Edit,"");
		SetDlgItemText(IDC_ExpectReponse_Edit,"");
		SetDlgItemText(IDC_ExpectSW_Edit,"");

		iRet = _CTLOG_AH;
	}

	if (csTag == "ED:")
	{
		SetDlgItemText(IDC_ExpectReponse_Edit,csData);
		iRet = _CTLOG_ED;
	}

	if (csTag == "AD:")
	{
		GetDlgItemText(IDC_ExpectReponse_Edit,csExpData);
		if (csExpData.IsEmpty())
		{
			switch( iCheckLevel  )
			{
			case 02:	SetDlgItemText(IDC_ExpectReponse_Edit,csData);break;
			case 03:	
				if (GetUserDataInSendMessage(csData,csUD))
						SetDlgItemText(IDC_ExpectReponse_Edit,_T("C_UD:")+csUD);
				else if(GetChannelData(csData,csUD))
						SetDlgItemText(IDC_ExpectReponse_Edit,_T("C_CD:")+csUD);

				break;
			default:break;
			}

		}
		//if (iCheckLevel > 1)
		//{
		//	GetDlgItemText(IDC_ExpectReponse_Edit,csExpData);
		//	
		//	if(csExpData.IsEmpty())
		//		SetDlgItemText(IDC_ExpectReponse_Edit,csData);

		//}
		iRet = _CTLOG_CONT+_CTLOG_ED;

	}

	if (csTag == "ES:")
	{
		SetDlgItemText(IDC_ExpectSW_Edit,csData);
		iRet = _CTLOG_ES;
	}

	if (csTag == "AS:")
	{
		if (iCheckLevel > 1)
		{
			GetDlgItemText(IDC_ExpectSW_Edit,csExpData);

			if(csExpData.IsEmpty())
			{
				SetDlgItemText(IDC_ExpectSW_Edit,csData);
			}

		}
		iRet = _CTLOG_AS;
	}

	if (csTag == "AJ:")
	{

		if (csData == "0xC0_ON")
			bADJ0xC0 = true;

		if (csData == "0xC0_OFF")
			bADJ0xC0 = false;

	}
	return iRet;
}

int CRunLogDlg::TransationCMCCLogFileData(CString csData)
{
	CString csTag,csInputeData,csExpData;
	int iOffset;
	int iRet;

	iOffset      = csData.Find(":");
	csTag        = csData.Left(iOffset);
	csInputeData = csData.Right(csData.GetLength() - iOffset -1);
	csInputeData = DeleteEnterSpace(csInputeData);

	if(csTag == "APDU")
	{
		SetDlgItemText(IDC_CurrentSend_Edit,csInputeData);
		SetDlgItemText(IDC_CurrentResponse_Edit,"");
		SetDlgItemText(IDC_CurrentSW_Edit,"");
		SetDlgItemText(IDC_ExpectReponse_Edit,"");
		SetDlgItemText(IDC_ExpectSW_Edit,"");
		iRet = _CTLOG_AH;

	}
	else if((csTag == "期望状态值")||(csTag == "期望值"))
	{
		if ((csInputeData == "无")||(csInputeData == "文件头检查"))
		{
			csInputeData.Empty();
		}
		SetDlgItemText(IDC_ExpectSW_Edit,csInputeData);
		iRet = _CTLOG_ED;
	}
	else if((csTag == "实际状态值")||(csTag == "实际值"))
	{
		if (iCheckLevel > 1)
		{

			if(csInputeData == "无")
			{
				csInputeData.Empty();
			}
			SetDlgItemText(IDC_ExpectSW_Edit,csInputeData);
		}
		iRet = _CTLOG_CONT+_CTLOG_ED;
	}
	else if(csTag == "期望响应值")
	{
		if((csInputeData == "无")||(csInputeData == "成功"))
			csInputeData.Empty();

		//if (csInputeData.Right(04) == _T("9000"))
		//{
		//	csInputeData = csInputeData.Left(csInputeData.GetLength()-4);
		//}

		SetDlgItemText(IDC_ExpectReponse_Edit,csInputeData);
		iRet = _CTLOG_ES;
	}
	else if(csTag == "实际响应值")
	{


		if (iCheckLevel > 1)
		{
			if((csInputeData == "无")||(csInputeData == "成功"))
			{
				csInputeData.Empty();
			}
			GetDlgItemText(IDC_ExpectReponse_Edit,csExpData);

			if(csExpData.IsEmpty())
				SetDlgItemText(IDC_ExpectReponse_Edit,csInputeData);

		}
		iRet = _CTLOG_AS;

	}

	return iRet;

}




int CRunLogDlg::TransationConvertionLogFileData(CString csData)
{

	CString csTag;
	CString csTemp;
	int iLen,iRet,iApduIns;
	iRet   = 0x10;
	csData = DeleteEnterSpace(csData);
	if (csData.IsEmpty())
		return iRet;



	iLen = csData.GetLength();

	SetDlgItemText(IDC_CurrentSend_Edit,"");
	SetDlgItemText(IDC_CurrentResponse_Edit,"");
	SetDlgItemText(IDC_CurrentSW_Edit,"");
	SetDlgItemText(IDC_ExpectReponse_Edit,"");
	SetDlgItemText(IDC_ExpectSW_Edit,"");


	iRet = _CTLOG_AS;
	if (csData.Mid(00,02) == _T("3B"))
	{
		SetDlgItemText(IDC_CurrentSend_Edit,"RESET");
	}
	else if (csData.Mid(00,02) == _T("FF"))
	{
		iRet = _CTLOG_CONT;
	}
	else
	{


		iApduIns = _CString2Int(csData.Mid(2,2));

		switch(iApduIns)
		{
		case 0xA4:
		case 0xD6:
		case 0xDC:
		case 0xA2:
		case 0x32:
		case 0x20:
		case 0x24:
		case 0x26:
		case 0x28:
		case 0x2c:
		case 0x04:
		case 0x44:
		case 0x88:
		case 0x10:
		case 0xC2:
		case 0x14:
		case 0x8E:
		default:

			SetDlgItemText(IDC_CurrentSend_Edit,csData.Mid(0,iLen - 4));

			SetDlgItemText(IDC_ExpectSW_Edit,csData.Right(4));

			break;

		case 0x70:
		case 0xF2:
		case 0xB2:
		case 0xB0:
		case 0xC0:
		case 0x12:


			SetDlgItemText(IDC_CurrentSend_Edit,csData.Mid(0,10));

			SetDlgItemText(IDC_ExpectReponse_Edit,csData.Mid(10,iLen - 14));

			SetDlgItemText(IDC_ExpectSW_Edit,csData.Right(4));

			break;
		}

	}

	return iRet;

}


void CRunLogDlg::_RemoveAllEdit()
{

	SetDlgItemText(IDC_CurrentSend_Edit,"");
	SetDlgItemText(IDC_CurrentResponse_Edit,"");
	SetDlgItemText(IDC_CurrentSW_Edit,"");
	SetDlgItemText(IDC_ExpectReponse_Edit,"");
	SetDlgItemText(IDC_ExpectSW_Edit,"");
}
//************************************
// Method:    _GetDataFromBJLog
// FullName:  _GetDataFromBJLog
// Access:    public 
// Returns:   int     0 Success . other faild
// Qualifier:
// Parameter: CString csData
// Parameter: CString & csSequence
// Parameter: CString & csOpration
// Parameter: CString & csDescription
// Parameter: CString & csValue
// Parameter: CString & csTimeText
// Parameter: CString & csMeaning
//************************************
int _GetDataFromBJLog(CString csData,
	                      CString &csSequence   ,CString& csOpration,
	                      CString &csDescription,CString& csValue,
						  CString &csTimeText   ,CString & csMeaning)
{
	int iOffset,iStart;
	iStart     = 0;
	CString csTemp;

	for (int i = 1 ; i < 7 ; i ++ )
	{
		//查找分隔符 ASC09
		iOffset    = csData.Find(_T("	"),iStart);
		//如果出现失败，则表明数据不完整或者失败
		if (iOffset<0)
			return i;
	
		csTemp = csData.Mid(iStart,iOffset-iStart);
		iStart     = iOffset+1;
		
		switch(i)
		{
		case 1:csSequence    = csTemp;break;
		case 2:csOpration    = csTemp;break;
		case 4:csDescription = csTemp;break;
		case 5:csValue       = csTemp;break;
		case 6:csTimeText    = csTemp;break;
		//此处其他数据忽略
		default:break;
		}

	}
	csMeaning = csData.Mid(iStart);
	return 0;
}


 
UINT _TransateDataType(CString csOpration,CString csDescription,UINT iPreType)
{
	UINT uiRet = _BJ_ABORT;
	 _DeleteEnterSpace(csOpration);
	 _DeleteEnterSpace(csDescription);

	 if (csOpration.Compare(_T("ATR")) == 0)
		 return _BJ_RESET;
	 if (csOpration.Compare(_T("PPSrequest")) == 0)
		 return _BJ_PPS_SEND;
	 if (csOpration.Compare(_T("PPSresponse")) == 0)
		 return _BJ_PPS_RECIEVE;

	 if ((csOpration.Compare(_T("APDU")) == 0)&&
		 (csDescription.Compare(_T("Header[CLAINSP1P2]"))  == 0))
		  return _BJ_APDU_HEAD;

	 if (csOpration.IsEmpty())
	 {
	
		 switch(iPreType&0xF)
		 {
		 case _BJ_APDU_HEAD:
		 case _BJ_APDU_SEND_DATA:
		 case _BJ_APDU_RECIEVE_DATA:
		 case _BJ_APDU_SW:
			 if(csDescription.Compare(_T("Incomingdata"))  == 0)
				 uiRet =  _BJ_APDU_SEND_DATA;	
			 else if(csDescription.Compare(_T("Outgoingdata"))  == 0)
				 uiRet =  _BJ_APDU_RECIEVE_DATA;	
			 else if(csDescription.Compare(_T("Returncode[SW1SW2]"))  == 0)
				 uiRet =  _BJ_APDU_SW;
			 else if(csDescription.IsEmpty())
				 uiRet = _BJ_CONTINUE_DATA|iPreType;	
			 break;
		 default: uiRet = _BJ_CONTINUE_DATA|iPreType; break;
		 }	 
	 }
	 return uiRet;
}

//************************************
// Method:    TransationBJLogFileData
// FullName:  把数据解析出并且放入相对应EDIT
// Access:    protected 
// Returns:   int
// Qualifier:
// Parameter: CString csData
//************************************
int CRunLogDlg::TransationBJLogFileData(CString csData, UINT uiPreType)
{
	UINT uiRet = _BJ_ABORT;
	CString csSequence,csOpration,csDescription,csValue,csTimeText,csMeaning;
	CString csTemp,csSend,csRespone,csP3;

	if (_GetDataFromBJLog (csData,csSequence,csOpration,
		csDescription,csValue,csTimeText,csMeaning) != 0)
		return FALSE;

	uiRet = _TransateDataType(csOpration,csDescription,uiPreType);

	if (uiRet != _BJ_ABORT)
		_DeleteEnterSpace(csValue);
	else 
		return _BJ_ABORT;

	
	switch(uiRet&0xF)
	{
	case _BJ_APDU_HEAD: 
		_RemoveAllEdit();
		SetDlgItemText(IDC_CurrentSend_Edit,csValue);break;
	case _BJ_APDU_RECIEVE_DATA:
		csTemp.Empty();
		GetDlgItemText(IDC_ExpectReponse_Edit,csTemp);
		SetDlgItemText(IDC_ExpectReponse_Edit,csTemp+csValue);break;
	case _BJ_APDU_SEND_DATA:
		csTemp.Empty();
		GetDlgItemText(IDC_CurrentSend_Edit,csTemp);
		SetDlgItemText(IDC_CurrentSend_Edit,csTemp+csValue);
		break;
	case _BJ_APDU_SW:
		//由于没有p3 需要手动Ｐ3
		csTemp.Empty();
		GetDlgItemText(IDC_CurrentSend_Edit,csSend);
		if (csSend.GetLength() <= 8)
		{
			GetDlgItemText(IDC_ExpectReponse_Edit,csRespone);
			_Int2CString(csRespone.GetLength()/2,csP3,2);
		}
		else
		{
			_Int2CString(csSend.GetLength()/2 -4,csP3,2);
		}

		
		csSend = csSend.Mid(0,8)+csP3 + csSend.Mid(8);
		SetDlgItemText(IDC_CurrentSend_Edit,csSend);
		SetDlgItemText(IDC_ExpectSW_Edit,csValue);break;
	case _BJ_RESET:

		SetDlgItemText(IDC_CurrentSend_Edit,_T("RESET"));break;
	}
	return uiRet;

}


CString CRunLogDlg::Run1LineScript(CString csData)
{


	//***************************************
	LARGE_INTEGER   litmp;
	LONGLONG   QPstart,QPend;
	double   dfMinus,dfFreq;
	CString csTime;
	//***************************************



	int iSend,iResLen;
	int iRet;

//	unsigned int uiResLen;
	unsigned char ucSend[300];
	unsigned char ucResp[300];

	csData = DeleteEnterSpace(csData);
	iSend  = csData.GetLength();

	ZeroMemory(ucSend,300);
	ZeroMemory(ucResp,300);

	if (iConSta != 0)
	{
		csData.Empty();
		return csData;
	}


	//***********************************************
	//记录初始时间
	dfTim = 0;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	QPstart = litmp.QuadPart;
	//***********************************************

	switch(iCurReader)
	{
	case Def_PCSC:

		if ((csData == "0012000000")||(csData == "$0")||(csData == "Reset")||(csData == "reset")||(csData == "RESET"))
		{
			iRet = PcscReader.ResetCard(ColdReset);
			if (iRet == 0)
			{
				iResLen = PcscReader.GetCardATR(ucResp);
				iConSta = 0;

			}
			else
			{
				iResLen = 0;
				iConSta = -0x10;
			}


		}
		else
		{

			iSend = _CString2UcHex(csData,ucSend);

			iRet = PcscReader.SendCommand(ucSend,iSend,ucResp,&iResLen);

			if (iResLen < 2)
				iConSta = -2;
			else
				iConSta = 0;

		}

		break;

	case Def_CCR5:
		//if (csData == "0012000000")
		//{
		//	iRet = CCR5_PowerOffCard(CCR5Hand);
		//	Sleep(100);
		//	iRet = CCR5_PowerOnCard(CCR5Hand,NULL,ucResp,&uiResLen);
		//	if (iRet == CT_OK)
		//	{

		//	//	STM_GetATR(NewHand,300,&uiResLen,ucResp);

		//		iResLen = uiResLen;

		//		iConSta = 0;

		//	}
		//	else
		//	{
		//		iResLen = 0;
		//		iConSta = -0x10;
		//	}

		//}
		//else
		//{
		//	iSend = _CString2UcHex(csData,ucSend);

		//	iRet = Send_CCR5(ucSend,iSend,ucResp,&iResLen);

		//	if (iResLen < 2)
		//		iConSta = -2;
		//	else
		//		iConSta = 0;

		//}


		if ((csData == "0012000000")||(csData == "$0"))
		{

			//iRet  = STM_ICCReset(NewHand);
			//if (iRet == 0)
			//{

			//	STM_GetATR(NewHand,300,&uiResLen,ucResp);

			//	iResLen = uiResLen;

			//	iConSta = 0;

			//}
			//else
			//{
			//	iResLen = 0;
			//	iConSta = -0x10;
			//}

		}
		else
		{
			iSend = _CString2UcHex(csData,ucSend);

			iRet = Send_CCR5(ucSend,iSend,ucResp,&iResLen);

			if (iResLen < 2)
				iConSta = -2;
			else
				iConSta = 0;

		}


		break;

	case Def_XXXX:

		if ((csData == "0012000000")||(csData == "$0"))
		{
			iRet = STM_ICCReset(NewHand);
			if (iRet == 0)
			{
				unsigned int uiResLen;

				STM_GetATR(NewHand,300,&uiResLen,ucResp);

				iResLen = uiResLen;

				iConSta = 0;

			}
			else
			{
				iResLen = 0;
				iConSta = -0x10;
			}


		}
		else
		{

			iSend = _CString2UcHex(csData,ucSend);

			iRet = Send_Stm(ucSend,iSend,ucResp,&iResLen);

			if (iResLen < 2)
				iConSta = -2;
			else
				iConSta = 0;

		}



		break;

	default:


		break;
	}


	//***********************************************
	//记录结束时间
	QueryPerformanceCounter(&litmp);
	QPend = litmp.QuadPart;
	dfMinus = (double)(QPend - QPstart); 
	dfTim = dfMinus/dfFreq*1000;
	//***********************************************

	CString csResult;
	_UcHex2CString(ucResp,iResLen,csResult);

	return csResult;

}

//比较Res 包括DResponse 和SW
int CRunLogDlg::CompareResponse(CString csData)
{
	CString csRes,csSW;

	int iResLen;
	int iRet;

	iResLen = csData.GetLength() -4;

	csRes   = csData.Left(iResLen);

	csSW    = csData.Right(04);


	iRet = CompareExpectSW(csSW);

	if (iRet == false)
		return iRet;



	iRet = CompareExpectResponse(csRes);

	if (iRet == false)
		return iRet;

	return true;
}
int CRunLogDlg::CompareListSW(CString csSW,CString csExpectSW)
{

	int iRet,iCyc,iLen,iListLen;

	CString csTempSW;

	iLen = csExpectSW.GetLength();
	iRet = false;


	for (iCyc = 0; iCyc<iLen;iCyc+= iListLen)
	{

		iListLen = csExpectSW.Find("/",iCyc) - iCyc ;

		if (iListLen<=0)
			iListLen = iLen - iCyc;

		csTempSW = csExpectSW.Mid(iCyc,iListLen);

		if (csTempSW.GetLength()==4)
		{

			if (CompareData(csTempSW,csSW))
			{
				return true;
			}


			//if (_CString2Int(csTempSW) ==_CString2Int(csSW))
			//	return true;
		}

		else
			iRet = CompareRangSW( csSW,  csTempSW);

		if (iRet != false)
			return iRet;

		iListLen +=1;
	}

	return false;
}

int CRunLogDlg::CompareRangSW(CString csSW, CString csExpectSW)
{

	int iRet;

	iRet = _CString2Int(csSW);

	if ((iRet >=  _CString2Int(csExpectSW.Left(04)))&&(iRet<=_CString2Int(csExpectSW.Right(04))))
		iRet = true;
	else 
		iRet = false;

	return iRet;


}
int CRunLogDlg::CompareExpectSW(CString csSW)
{
	int iRet;
	CString csExpSW;

	iRet = GetDlgItemText(IDC_ExpectSW_Edit,csExpSW);
	SetDlgItemText(IDC_CurrentSW_Edit,csSW);
	if (iRet == 0)
		return true;

	if ((iLogType == _CMCC_LOG)&&(csExpSW.GetLength() == 4))
		iRet =  CompareData(csSW,csExpSW);
	else
		iRet = CompareListSW(csSW,csExpSW);





	return  iRet;



}

int CRunLogDlg::CompareExpectResponse(CString csResponse)
{

	int iRet;
	bool bUD = false;
	CString csExpResponse;
	CString csAPDU;
	CString csSW;
	iRet = GetDlgItemText(IDC_CurrentSW_Edit,csSW);
	iRet = GetDlgItemText(IDC_CurrentSend_Edit,csAPDU);
	iRet = GetDlgItemText(IDC_ExpectReponse_Edit,csExpResponse);


	SetDlgItemText(IDC_CurrentResponse_Edit,csResponse);

	if (iRet == 0)
		return true;

	//此处用于抓包数据log 的Select 判断
	if ((bADJRes0xA4 == true)&&
		(iPreIns == 0xA4)&&
		(_CString2Int(csAPDU.Mid(2,2)) == 0xC0)&&
		(iLogType == _Catch_LOG))
	{
		iRet =  CompareFCP(csExpResponse,csResponse);
		if (iRet>= 0)
			return iRet;
	}


	//如果检查级别为03；检查USERDATA

	if (iCheckLevel == 3)
	{
		CString csUD,csExpUD;

		if ( csExpResponse.Mid(0,5) == _T("C_UD:"))
		{

			csExpUD = csExpResponse.Mid(5);
			GetUserDataInSendMessage(csResponse,csUD);	 

		//	//当选择61XX时表示返回SW 为61XX ，所以此处应该是要替换掉XX
		//	if ( (_CString2Int(csExpUD.Right(04))&0xFF00)   == 0x6100)
	//			csExpUD = csUD.Left(csExpUD.GetLength() -2 )+_T("XX");

			return CompareData(csExpUD,csUD);
		}else if ( csExpResponse.Mid(0,5) == _T("C_CD:"))
		{

			csExpUD = csExpResponse.Mid(5);
			GetChannelData(csResponse,csUD);	 
			return CompareData(csExpUD,csUD);

		}

	}


	//部分存在SW 存在于Response
	//此处纠正下
	csExpResponse = DeleteEnterSpace(csExpResponse);
	if (csExpResponse.GetLength() == csResponse.GetLength()+4)
		csResponse = csResponse+csSW;




	return CompareData(csResponse,DeleteEnterSpace(csExpResponse));
}

int CRunLogDlg::CompareData(CString csData1, CString csData2)
{
	int iLen1,iLen2,iCyc,iData1,iData2;
	CString csTemp1,csTemp2;

	iLen1 = csData1.GetLength();
	iLen2 = csData2.GetLength();

	if (iLen1!=iLen2)
		return false;

	for (iCyc = 0 ; iCyc<iLen1 ; iCyc++)
	{

		csTemp1 = csData1.Mid(iCyc,1);
		csTemp2 = csData2.Mid(iCyc,1);

		if ((csTemp1!="X")&&(csTemp1!="x")&&((csTemp2!="X")&&((csTemp2!="x")&&(csTemp1!="?")&&(csTemp2!="?"))))
		{
			iData1 = _ttoi(csTemp1);
			iData2 = _ttoi(csTemp2);
			if (iData1!=iData2)
				return false;	
		}
	}

	return true;

}

int CRunLogDlg::CompareFCP(CString csExFCP,CString csFCP)
{
	
	bool bIsDF;
	CString csFCPV ,csEPCPV;
	CString csDes,csEDes;
	CString csComp,csEComp;

	if (! _GetTLVData(csFCP,0x62,csFCPV))
		return -1;

	if (! _GetTLVData(csExFCP,0x62,csEPCPV))
		return -0x11;

	if (! _GetTLVData(csFCPV,0x82,csDes))
		return -1;

	if (! _GetTLVData(csEPCPV,0x82,csEDes))
		return -0x11;

	//比较文件描述符
	if ((!CompareData(csDes,csEDes))||
		(csDes.IsEmpty()))
		return FALSE;

	bIsDF = ((_CString2Int(csDes.Mid(4,2))&0x38) == 0x38);
	
	if (bIsDF)
	{

		if (( _GetTLVData(csFCPV,0x83,csComp))&&
			  (! _GetTLVData(csEPCPV,0x83,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}
		if (( _GetTLVData(csFCPV,0x84,csComp))&&
			(! _GetTLVData(csEPCPV,0x84,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}

		if (( _GetTLVData(csFCPV,0xA5,csComp))&&
			(! _GetTLVData(csEPCPV,0xA5,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}

		if (( _GetTLVData(csFCPV,0x8A,csComp))&&
			(! _GetTLVData(csEPCPV,0x8A,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}
	}
	else
	{


		if (( _GetTLVData(csFCPV,0x83,csComp))&&
			(! _GetTLVData(csEPCPV,0x83,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}
		if (( _GetTLVData(csFCPV,0xA5,csComp))&&
			(! _GetTLVData(csEPCPV,0xA5,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}

		if (( _GetTLVData(csFCPV,0x8A,csComp))&&
			(! _GetTLVData(csEPCPV,0x8A,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}

		if (( _GetTLVData(csFCPV,0x82,csComp))&&
			(! _GetTLVData(csEPCPV,0x82,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}
		if (( _GetTLVData(csFCPV,0x81,csComp))&&
			(! _GetTLVData(csEPCPV,0x81,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}

		if (( _GetTLVData(csFCPV,0x88,csComp))&&
			(! _GetTLVData(csEPCPV,0x88,csEComp)))
		{
			if ( !CompareData(csComp,csEComp) )
				return FALSE;
		}

	}
	    
	return TRUE;
}


void CRunLogDlg::OnEnSetfocusCtlogEdit()
{

}

void CRunLogDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = -1;

	ContrleBusy(TRUE);

	SetItemFileData2Edit(m_FileTree.GetSelectedItem());

	ContrleBusy(FALSE);

	*pResult = 0;

}


CString CRunLogDlg::GetDIR(void)
{
	BROWSEINFO biDIR;

	CString csDIRPath;
	LPITEMIDLIST llDIRList;

	ZeroMemory(&biDIR,sizeof(BROWSEINFO));
	biDIR.hwndOwner = GetSafeHwnd();
	biDIR.lpszTitle = _T("测试脚本目录");
	biDIR.ulFlags   = BIF_BROWSEFORCOMPUTER|BIF_RETURNONLYFSDIRS|BIF_USENEWUI;

	llDIRList = SHBrowseForFolder(&biDIR);

	if (llDIRList != NULL)
	{
		SHGetPathFromIDList(llDIRList,csDIRPath.GetBuffer(MAX_PATH));
		csDIRPath.ReleaseBuffer();
	}
	else
		csDIRPath.Empty();

	return csDIRPath;

}

int CRunLogDlg::GetLogFileList(CString csDIR,HTREEITEM hDIRTreeItem)
{
	HTREEITEM hFileTreeItem;

	CFileFind cffFile;

	int iRet;

	CString csFilePath;



	if (hDIRTreeItem == TVI_ROOT)
	{
		m_FileTree.DeleteAllItems();
		hDIRTreeItem = m_FileTree.InsertItem(csDIR,Def_Image_DIR,Def_Image_DIR);
	}
	if (csDIR.Right(csDIR.GetLength()-1)!=_T("\\"))
	{
		csDIR +=_T( "\\");
	}
	csDIR  +=   _T("*.*");

	iRet = cffFile.FindFile(csDIR,0);

	while(iRet)
	{

		iRet = cffFile.FindNextFile();
		if (cffFile.IsDots())
		{
			continue;
		}

		if (cffFile.IsDirectory())
		{

			hFileTreeItem = m_FileTree.InsertItem(cffFile.GetFileName(),Def_Image_DIR,Def_Image_DIR,hDIRTreeItem);
			//		hFileTreeItem = m_FileTree.InsertItem(cffFile.GetFilePath(),Def_Image_Path,Def_Image_Path,hFileTreeItem);

			GetLogFileList(cffFile.GetFilePath(),hFileTreeItem);
		}
		else
		{
			csFilePath = cffFile.GetFilePath();

			if ((csFilePath.Right(04)==".log")&&(csFilePath.Right(10)  !="_Error.log"))
			{
				hFileTreeItem = m_FileTree.InsertItem(cffFile.GetFileTitle(),Def_Image_File,Def_Image_File,hDIRTreeItem);
				hFileTreeItem = m_FileTree.InsertItem(cffFile.GetFilePath(),Def_Image_Path,Def_Image_Path,hFileTreeItem);
			}

		}

	}


	cffFile.Close();


	return 0;
}



void CRunLogDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{


	NM_TREEVIEW * pHdr = (NM_TREEVIEW*)pNMHDR;
	CPoint cpPoint;
	UINT uFlag ;
	HTREEITEM hCurrentItem;
	BOOL bCheck;

	GetCursorPos(&cpPoint);
	m_FileTree.ScreenToClient(&cpPoint);
	hCurrentItem = m_FileTree.HitTest(cpPoint,&uFlag);

	if ((hCurrentItem) &&(TVHT_ONITEMSTATEICON&uFlag))
		//	if (hCurrentItem)
	{
		bCheck = m_FileTree.GetCheck(hCurrentItem);

		SetItemCheck(hCurrentItem,!bCheck);

	}



	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CRunLogDlg::SetItemCheck(HTREEITEM hItem, BOOL bCheck)
{
	SetChildItemCheck(hItem,bCheck);
	SetParentCheck(hItem,bCheck);


}

void CRunLogDlg::SetChildItemCheck(HTREEITEM hItem, BOOL bCheck)
{
	HTREEITEM hChildItem;

	hChildItem = m_FileTree.GetChildItem(hItem);

	while(hChildItem)
	{
		m_FileTree.SetCheck(hChildItem,bCheck);
		SetChildItemCheck(hChildItem,bCheck);
		hChildItem = m_FileTree.GetNextItem(hChildItem,TVGN_NEXT);
	}
}

void CRunLogDlg::SetParentCheck(HTREEITEM hItem, BOOL bCheck)
{

	HTREEITEM hParentItem;

	hParentItem = m_FileTree.GetParentItem(hItem);

	if (hParentItem== NULL)
		return ;

	m_FileTree.SetCheck(hParentItem,bCheck);

	SetParentCheck(hParentItem,bCheck);

}


void CRunLogDlg::SetProgress(int iCurrent)
{
	m_Progress.SetPos(iCurrent);
}
void CRunLogDlg::SetProgressGlob(int iCurrent)
{
	m_ProgreeGlob.SetPos(iCurrent);
}

int CRunLogDlg::ThreadRun(void)
{

	CRunLogDlg *NewApp; 


	NewApp = (CRunLogDlg *)AfxGetApp()->GetMainWnd();


	NewApp-> Run1Script();

	return 1;

}

int CRunLogDlg::ThreadAllRun(void)
{

	CRunLogDlg *NewApp; 

	NewApp = (CRunLogDlg *)AfxGetApp()->GetMainWnd();



	NewApp-> RunAllList();


	return 1;

}

int CRunLogDlg::Run1Script(void)
{
	Init_ProgressGlob(1);

	cTimeStart =CTime::GetCurrentTime();

	SetDlgItemText(IDC_CurrentRow,"");

	if (bWriteLog)
	{
		csCurLogFile = GenerateRunLogPath(csTestLogPath,m_FileTree.GetSelectedItem());
		CreateLogFile(csCurLogFile);

		csErrFile    = GenerateRunErrPath(csTestLogPath);
		CreateErrFile(csErrFile);
	}

	RunLOGofEdit();



	MessageBox("测试完成！");



	iThreadEnd = -1;

	ContrleBusy(FALSE);

	ExitThread(0);

	return 1;
}

void CRunLogDlg::RunAllList(void)
{

	HTREEITEM hCurrentItem;

	cTimeStart =CTime::GetCurrentTime();

	hCurrentItem = m_FileTree.GetRootItem();

	Init_ProgressGlob(StatisticFileNumber());

	iFileNum = 0;

	RunCurrentTree(hCurrentItem);


	MessageBox("测试完成！");

	iThreadEnd = -1;

	ContrleBusy(FALSE);

	ExitThread(0); 
}



void CRunLogDlg::RunCurrentTree(HTREEITEM hCurrentItem)
{
	int nImage,nSelectedImage;
	BOOL bRet;





	do 
	{

		if ((CheckPrograssStatus(hThread))&&((iThreadEnd<0)))
			ExitThread(0); 

		bRet = m_FileTree.GetItemImage(hCurrentItem,nImage,nSelectedImage);


		if (!bRet)
			return;

		switch(nImage)
		{
		case Def_Image_DIR:

			RunCurrentTree(m_FileTree.GetChildItem(hCurrentItem));
			break;
		case Def_Image_File:
		case Def_Image_Right:
		case Def_Image_Wrong:

			if (m_FileTree.GetCheck(hCurrentItem))
			{
				SetItemFileData2Edit(hCurrentItem);

				m_FileTree.SelectItem(hCurrentItem);

				m_FileTree.SetItemImage(hCurrentItem,Def_Image_Arrow,Def_Image_Arrow);


				if (bWriteLog)
				{
					csCurLogFile = GenerateRunLogPath(csTestLogPath,hCurrentItem);
					bRet         = CreateLogFile(csCurLogFile);


					csErrFile    = GenerateRunErrPath(csTestLogPath);
					bRet         = CreateErrFile(csErrFile);

				}
				RunLOGofEdit( hCurrentItem);
				SetDlgItemText(IDC_CurrentRow,"");
				iFileNum++;
				SetProgressGlob(iFileNum);
			}
			break;

		default:
			break;
		}

		hCurrentItem = m_FileTree.GetNextItem(hCurrentItem,TVGN_NEXT);

	} while (hCurrentItem);




}
void CRunLogDlg::SetCMCCLog2Edit(CString csFilePath)
{
	//int iRet;
	CMarkup cmFile;
	CString csData,csResult;
	//BOOL bRet;
	int iOffset,iLenght;

	int iTemp;

	CString csAPDU,csExpSW,csSW,csExpResp,csResp;



	cmFile.Load(csFilePath);
	csData = cmFile.GetDoc();

	iOffset = 0;
	iLenght = 0;

	if (csData.Find(_T("APDU:"),iOffset)< 0)
	{
		do 
		{
			//iOffset = csData.Find(_T("APDU:"),iOffset);
			//if (iOffset<0)
			//	break;

			//TIME



			iOffset = csData.Find(_T("</TD> <TD WIDTH=980>("),iOffset);

			if (iOffset <0)
				break;

			iOffset = csData.Find(_T("</TD> <TD WIDTH=980>"),iOffset+9);
			if (iOffset <0)
				break;


			iLenght = csData.Find(_T("</TD"),iOffset+9) -iOffset - 0x14;

			csResult = csData.Mid(iOffset+0x14,iLenght);

			if (csResult.Compare("FEFEFEFE00") == 0)
			{
				csResult = _T("APDU:RESET");
			}
			else
				csResult = _T("APDU:")+csResult;

		
			Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("期望值:"),iOffset);
			if (iOffset<0)
				iOffset = csData.Find(_T("期望状态值:"),iOffset);

			if (iOffset<0)
				break;
			iLenght = csData.Find(_T("</TD"),iOffset)  - iOffset;

			csResult = csData.Mid(iOffset,iLenght);

		
			Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("实际值:"),iOffset);
			if (iOffset<0)
				iOffset = csData.Find(_T("实际状态值:"),iOffset);
			if (iOffset<0)
				break;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

		
			Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("期望响应值:"),iOffset);
			if (iOffset<0)
				break;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);


			
			Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("实际响应值:"),iOffset);
			if (iOffset<0)
				break;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			
			Print2Edit(csResult);
			PriteCurrentTime();

	
	
			//Print2Edit(csAPDU);
			//Print2Edit(csExpSW);
			//Print2Edit(csSW);
			//Print2Edit(csResp);
			//Print2Edit(csExpResp);
			//PriteCurrentTime();
			//csAPDU.Empty();
			//csExpSW.Empty();
			//csSW.Empty();
			//csResp.Empty();
			//csExpResp.Empty();


		} while (iOffset>0);




	}
	else if (csData.Find(_T("期望状态字:"),iOffset)> 0)
	{
		do 
		{
			iOffset = csData.Find(_T("APDU:"),iOffset);
			if (iOffset<0)
				goto PrintToE;


			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			csAPDU = csResult;
			//Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("期望状态值:"),iOffset);
			if (iOffset<0)
				goto PrintToE;

			iLenght = csData.Find(_T("</TD"),iOffset)  - iOffset;

			csResult = csData.Mid(iOffset,iLenght);


			csExpSW = csResult;
			//Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("实际状态值:"),iOffset);
			if (iOffset<0)
				goto PrintToE;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			csSW = csResult;
			//Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("期望响应值:"),iOffset);
			if (iOffset<0)
				goto PrintToE;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);


			csExpResp= csResult;
			//Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("实际响应值:"),iOffset);
			if (iOffset<0)
				goto PrintToE;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			csResp= csResult;
			//Print2Edit(csResult);
			PriteCurrentTime();

			iTemp = csData.Find(_T("期望状态字:"),iOffset);

			if ((iTemp > 0)&&(iTemp<csData.Find(_T("APDU:"),iOffset) ))
			{
				iOffset = csData.Find(_T("期望值:"),iOffset);
				if (iOffset<0)
					goto PrintToE;

				iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

				csResult = csData.Mid(iOffset,iLenght);

				csExpSW= csResult;
				PriteCurrentTime();

			}


PrintToE:	PriteCurrentTime();
			Print2Edit(csAPDU);
			Print2Edit(csExpSW);
			Print2Edit(csSW);
			Print2Edit(csExpResp);
			Print2Edit(csResp);
			csAPDU.Empty();
			csExpSW.Empty();
			csSW.Empty();
			csResp.Empty();
			csExpResp.Empty();
			PriteCurrentTime();

		}while (iOffset>0);
	}
	else
	{

		do 
		{
			iOffset = csData.Find(_T("APDU:"),iOffset);
			if (iOffset<0)
				break;


			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("期望状态值:"),iOffset);
			if (iOffset<0)
				break;

			iLenght = csData.Find(_T("</TD"),iOffset)  - iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("实际状态值:"),iOffset);
			if (iOffset<0)
				break;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("期望响应值:"),iOffset);
			if (iOffset<0)
				break;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			Print2Edit(csResult);
			PriteCurrentTime();

			iOffset = csData.Find(_T("实际响应值:"),iOffset);
			if (iOffset<0)
				break;

			iLenght = csData.Find(_T("</TD"),iOffset) -iOffset;

			csResult = csData.Mid(iOffset,iLenght);

			Print2Edit(csResult);
			PriteCurrentTime();

		} while (iOffset>0);

	}
	


	return;



}

void CRunLogDlg::SetCTLog2Edit(CString csFilePath)
{
	int iRet;
	CStdioFile CSFFile;
	CFileStatus csSta;
	CString csData;

	iRet = CSFFile.Open(csFilePath,CFile::shareDenyRead);
	if (!iRet)
		return ;

	iRet = CSFFile.GetStatus(csFilePath,csSta);
	if (!iRet)
	{
		CSFFile.Close();
		return ;
	}

	do 
	{
		//读取文件内容
		if ((CheckPrograssStatus(hThread))&&((iThreadEnd<0)))
			ExitThread(0); 

		iRet = CSFFile.ReadString(csData);
		if (iRet == FALSE)
			break;
		Print2Edit(csData);
		PriteCurrentTime();
	} while (iRet);

	CSFFile.Close();

}

void CRunLogDlg::SetItemFileData2Edit(HTREEITEM hItem)
{

	HTREEITEM hCurTreeItem,hPathTreeItem ;
	CString csFilePath;
	CString csFileName;
	//	CFileStatus csSta;
	//	CStdioFile CSFFile;
	//	CString csData;
	//	int iRet;
	int iImange,iSelectImange;

	m_FileTree.GetItemImage(hItem,iImange,iSelectImange);

	hPathTreeItem = NULL;
	hCurTreeItem  = NULL;



	switch(iImange)
	{
	case Def_Image_DIR:

		m_FileTree.Expand(hItem, TVE_EXPAND);

		break;

	case Def_Image_Path:

		hPathTreeItem = hItem;
		hCurTreeItem  = m_FileTree.GetParentItem(hItem);

		break;

	case Def_Image_Arrow:
	case Def_Image_File:
	case Def_Image_Right:
	case Def_Image_Wrong:

		hPathTreeItem = m_FileTree.GetChildItem(hItem);
		hCurTreeItem  = hItem;

		break;
	default:

		break;
	}

	if ((hPathTreeItem == NULL)||(hCurTreeItem == NULL))

		return ;


	csFilePath = m_FileTree.GetItemText(hPathTreeItem);

	if (csFilePath.IsEmpty())
		return;
	csFileName = m_FileTree.GetItemText(hCurTreeItem);


	SetDlgItemText(IDC_CTLOG_Edit,"");

	switch(iLogType)
	{
	case _CT_LOG:
	case _Catch_LOG:
	case _Convertion_LOG:SetCTLog2Edit( csFilePath);break;
	case _CMCC_LOG:SetCMCCLog2Edit(csFilePath);break;
	default: SetCTLog2Edit( csFilePath);break;


	}

	m_CTLOG_Edit.SetSel(0,0);
	m_CTLOG_Edit.SetFocus();

	SetDlgItemText(IDC_CurrentFile,_T("当前文件：")+csFileName);


}







void CRunLogDlg::ContrleBusy(BOOL bStatus)
{


	GetDlgItem(IDC_CurrentSend_Edit)->EnableWindow(!bStatus);
	GetDlgItem(IDC_CurrentResponse_Edit)->EnableWindow(!bStatus);
	GetDlgItem(IDC_ExpectReponse_Edit)->EnableWindow(!bStatus);
	GetDlgItem(IDC_ExpectSW_Edit)->EnableWindow(!bStatus);
	GetDlgItem(IDC_CurrentSW_Edit)->EnableWindow(!bStatus);



	switch(iConSta)
	{
	case 0:
		m_PCSCReader_Combo.EnableWindow(FALSE);
		break;

	default:
		m_PCSCReader_Combo.EnableWindow(TRUE);
		break;
	}


	switch(iThreadEnd)
	{
	case  0:
		GetDlgItem(IDC_Restore)->EnableWindow(TRUE);
		GetDlgItem(IDC_Pause)->EnableWindow(FALSE);
		GetDlgItem(IDC_Stop)->EnableWindow(TRUE);
		GetDlgItem(IDC_Connect_Button)->EnableWindow(FALSE);
		GetDlgItem(IDC_OneStep)->EnableWindow(TRUE);
		GetDlgItem(IDC_StartCase)->EnableWindow(FALSE);
		GetDlgItem(IDC_Exit)->EnableWindow(FALSE);
		m_FileTree  .EnableWindow(!bStatus);
		m_CTLOG_Edit.EnableWindow(!bStatus);
		break;
	case  1:
		GetDlgItem(IDC_Restore)->EnableWindow(FALSE);
		GetDlgItem(IDC_Pause)->EnableWindow(TRUE);
		GetDlgItem(IDC_Stop)->EnableWindow(TRUE);
		GetDlgItem(IDC_Connect_Button)->EnableWindow(FALSE);
		GetDlgItem(IDC_OneStep)->EnableWindow(FALSE);
		GetDlgItem(IDC_StartCase)->EnableWindow(FALSE);
		GetDlgItem(IDC_Exit)->EnableWindow(FALSE);
		m_FileTree  .EnableWindow(FALSE);
		m_CTLOG_Edit.EnableWindow(FALSE);
		break;
	default:
		GetDlgItem(IDC_Restore)->EnableWindow(FALSE);
		GetDlgItem(IDC_Pause)->EnableWindow(FALSE);
		GetDlgItem(IDC_Stop)->EnableWindow(FALSE);
		GetDlgItem(IDC_OneStep)->EnableWindow(TRUE);
		GetDlgItem(IDC_StartCase)->EnableWindow(TRUE);
		GetDlgItem(IDC_Exit)->EnableWindow(TRUE);
		GetDlgItem(IDC_Connect_Button)->EnableWindow(!bStatus);
		m_FileTree  .EnableWindow(!bStatus);
		m_CTLOG_Edit.EnableWindow(!bStatus);
		break;
	}



}




void CRunLogDlg::PrintLog2File(CString csPrint)
{

	BOOL bRet;
	if (!bWriteLog)
	{
		return;
	}
	CStdioFile CSFFile;
	bRet = CSFFile.Open(csCurLogFile, CFile::modeWrite | CFile::typeText);
	if (bRet)
	{
		CSFFile.SeekToEnd();
		CSFFile.WriteString(csPrint+"\n\r");
	}




	CSFFile.Close();

}

void CRunLogDlg::PrintErr2File(CString csPrint)
{

	CString csSend,csExResp,csResp,csExSW,csSW;
	BOOL bRet;
	int  iRet;
	if (!bWriteLog)
		return;
	CStdioFile CSFFile;
	bRet = CSFFile.Open(csErrFile, CFile::modeWrite | CFile::typeText);
	if (bRet)
	{
		CSFFile.SeekToEnd();
		CSFFile.WriteString(csPrint+"\n\r");

		iRet = GetDlgItemText(IDC_CurrentSend_Edit,csSend);
		iRet = GetDlgItemText(IDC_ExpectSW_Edit,csExSW);
		iRet = GetDlgItemText(IDC_ExpectReponse_Edit,csExResp);
		iRet = GetDlgItemText(IDC_CurrentSW_Edit,csSW);
		iRet = GetDlgItemText(IDC_CurrentResponse_Edit,csResp);

		CSFFile.WriteString("Send   :" +csSend   +"\n\r");
		CSFFile.WriteString("ExResp :" +csExResp +"\n\r");
		CSFFile.WriteString("ExSW   :" +csExSW   +"\n\r");
		CSFFile.WriteString("Resp   :" +csResp   +"\n\r");
		CSFFile.WriteString("SW     :" +csSW     +"\n\r");

	}




	CSFFile.Close();

}





CString CRunLogDlg::GenerateRunLogPath(CString csDir,HTREEITEM hCurrentItem)
{

	CString csResult,csTime;
	HTREEITEM hParentItem;

	csResult.Empty();

	csTime.Format("%04d%02d%02d%02d%02d%02d",cTimeStart.GetYear(),cTimeStart.GetMonth(),cTimeStart.GetDay(),cTimeStart.GetHour(),cTimeStart.GetMinute(),cTimeStart.GetSecond());

	csDir    = csDir+_T("\\") +csTime;


	hParentItem = m_FileTree.GetParentItem(hCurrentItem);

	while(hParentItem)
	{
		if(hParentItem != m_FileTree.GetRootItem())
			csResult = _T("\\") +m_FileTree.GetItemText(hParentItem) + csResult; 

		hParentItem = m_FileTree.GetParentItem(hParentItem);
	}

	csResult = csDir+csResult;

	_CreateDirectory(csResult);

	csResult = csResult+ _T("\\") +m_FileTree.GetItemText(hCurrentItem)+_T(".TLog");

	return csResult;
}


CString CRunLogDlg::GenerateRunErrPath(CString csDir)
{

	CString csResult,csTime;

	csResult.Empty();

	csTime.Format("%04d%02d%02d%02d%02d%02d",cTimeStart.GetYear(),cTimeStart.GetMonth(),cTimeStart.GetDay(),cTimeStart.GetHour(),cTimeStart.GetMinute(),cTimeStart.GetSecond());

	csDir    = csDir+_T("\\") +csTime;

	_CreateDirectory(csDir);

	csResult = csDir+ _T("\\") +csTime+_T(".Err");

	return csResult;
}


void CRunLogDlg::PriteCurrentTime(void)
{
	CTime ctCurrentTime;
	CTimeSpan ctsTimes;
	CString csTimes;
	int iHour,iMinu,iSeco;

	if (iThreadEnd == 1)
	{
		ctCurrentTime = CTime::GetCurrentTime();
		ctsTimes   = ctCurrentTime-cTimeStart;
		iHour      = (int)ctsTimes.GetTotalHours();
		iMinu      = (int)(ctsTimes.GetTotalMinutes())%60;
		iSeco      = (int)(ctsTimes.GetTotalSeconds())%60;
		csTimes.Format("%02d:%02d:%02d",iHour,iMinu,iSeco);
		SetDlgItemText(IDC_Times,csTimes);
	}

}


void CRunLogDlg::PrintCurrentTransation(void)
{


	BOOL bRet;

	CStdioFile CSFFile;

	CString csPrint;

	CString csSend,csResponse,csSW;
	CString csEResponse,csESW;
	CStringArray csExp;
	CString csFormat;
	int iTime  = 0;
	int iOffset;


	//确保文件被打开，当前出现多次无法打开文件；

	do 
	{
		bRet = CSFFile.Open(csCurLogFile,CFile::modeWrite | CFile::typeText);
		if (!bRet)
		{
			Sleep(10);
			iTime ++;
		}
	} while((!bRet)&&(iTime<10));



	if (bRet)
	{
		CSFFile.SeekToEnd();

		CSFFile.WriteString("---------------------\n");

		GetDlgItemText(IDC_CurrentSend_Edit,csSend);

		if (csPrint != "0012000000")
		{
			 GetDlgItemText(IDC_CurrentResponse_Edit,csResponse);	

			 GetDlgItemText(IDC_CurrentSW_Edit,csSW);	

			 GetDlgItemText(IDC_ExpectReponse_Edit,csEResponse);	

			 GetDlgItemText(IDC_ExpectSW_Edit,csESW);	

			 _ExplainAPDU(csSend,csResponse+csSW,csExp);

			 if (csExp.GetCount()>1)
			 {
				 	 csPrint = csExp.GetAt(0);
					 iOffset = csPrint.Find(":");
					 if ((iOffset<0x10)&&(iOffset>0))
					 {

						 _RepeatCString(" ",0x10 - iOffset,csFormat);
						 csPrint = csPrint.Left(iOffset) + csFormat + csPrint.Mid(iOffset);   
					 }

				  CSFFile.WriteString(csPrint+"\n");
			 }


			 CSFFile.WriteString("Send            :" + csSend +"\n");

			 if(!csResponse.IsEmpty())
				 CSFFile.WriteString("Response        :" + csResponse +"\n");

			 CSFFile.WriteString("SW              :" + csSW +"\n");

			 if(!csEResponse.IsEmpty())
					CSFFile.WriteString("Expect Response :" + csEResponse +"\n");

			 CSFFile.WriteString("Expect SW       :" + csESW +"\n");
			 
			 for (int i = 1 ; i < csExp.GetCount();i++)
			 {
				 csPrint = csExp.GetAt(i);
				 iOffset = csPrint.Find("\\- ");

				 if (iOffset>0)
				 {
					 
					 if (iOffset<0x10)
					 {
						 _RepeatCString(" ",0x10 - iOffset,csFormat);
						 csPrint = csPrint.Left(iOffset) + csFormat + csPrint.Mid(iOffset+1);   
					 }
					 else
						  csPrint = csPrint.Left(iOffset) + csPrint.Mid(iOffset+1);   

				 }
				 else
					 csPrint = RepeatCString(" ",0x12)+csPrint;



				 //if ((iOffset<0x20)&&(iOffset>0))
				 //{

					// _RepeatCString(" ",0x10 - iOffset,csFormat);
					// csPrint = csPrint.Left(iOffset) + csFormat + csPrint.Mid(iOffset+2);   
				 //}
				 //else
					////  csPrint = RepeatCString(" ",0x12)+csPrint;

					// csPrint = RepeatCString(" ",0x12)+csPrint;

				 CSFFile.WriteString(csPrint+"\n");

			 }
			



			//C/*SFFile.WriteString("Send    :"+csPrint+"\n");

			//csAPDU = csPrint;

			//GetDlgItemText(IDC_ExpectSW_Edit,csPrint);
			//if (!csPrint.IsEmpty())
			//	CSFFile.WriteString("ES      :"+csPrint+"\n");

			//GetDlgItemText(IDC_ExpectReponse_Edit,csPrint);
			//if (!csPrint.IsEmpty())
			//	CSFFile.WriteString("ED      :"+csPrint+"\n");

			//GetDlgItemText(IDC_CurrentResponse_Edit,csPrint);		
			//if (!csPrint.IsEmpty())
			//	CSFFile.WriteString("Response:"+csPrint+"\n");

			//csResponse = csPrint;

			//GetDlgItemText(IDC_CurrentSW_Edit,csPrint);
			//if (!csPrint.IsEmpty())
			//	CSFFile.WriteString("SW      :"+csPrint.Right(4)+"\n");

			//csResponse = csResponse+csPrint;

			//_ExplainAPDU(csAPDU,csResponse,csExp);

			//for (int i = 0 ; i < csExp.GetCount();i++)
			//{

			//	  
			//		CSFFile.WriteString(csExp.GetAt(i)+"\n");


			//}*/

		


		}
		else
		{

			GetDlgItemText(IDC_CardATR,csPrint);
			CSFFile.WriteString(csPrint+"\n");
		
			
		}

		if (bWriteTime)
		{


			csPrint.Format("%d",(int)dfTim/1);
			CSFFile.WriteString("Time    :"+csPrint+"ms\n");
		}

	}
	else
	{
		MessageBox("找不到Log文件！");
		bWriteLog = FALSE;
		bWriteTime = FALSE;
	}

	CSFFile.Close();

}




int CRunLogDlg::StatisticFileNumber(void)
{
	return StatisticFileNumberCurrentDIR (m_FileTree.GetRootItem(),0);
}

int CRunLogDlg::StatisticFileNumberCurrentDIR(HTREEITEM hCurrentItem,int iNumber)
{


	int nImage,nSelectedImage;
	BOOL bRet;

	do 
	{

		bRet = m_FileTree.GetItemImage(hCurrentItem,nImage,nSelectedImage);

		if (!bRet)
			return iNumber;

		switch(nImage)
		{
		case Def_Image_DIR:

			iNumber = StatisticFileNumberCurrentDIR(m_FileTree.GetChildItem(hCurrentItem),iNumber);

			break;
		case Def_Image_File:
		case Def_Image_Right:
		case Def_Image_Wrong:
		case Def_Image_Arrow:

			m_FileTree.SetItemImage(hCurrentItem,Def_Image_File,Def_Image_File);

			if (m_FileTree.GetCheck(hCurrentItem))
			{
				iNumber = iNumber+1;
			}
			break;

		default:
			break;
		}
		hCurrentItem = m_FileTree.GetNextItem(hCurrentItem,TVGN_NEXT);

	} while (hCurrentItem);

	return iNumber;
}

void CRunLogDlg::SetStatus(CString csATR)
{

	switch(iConSta)
	{
	case 0:

		SetDlgItemText(IDC_CardATR,"Card ATR: "+csATR);
		SetDlgItemText(IDC_CardStatuse,"卡片已连接");
		m_PCSCReader_Combo.EnableWindow(FALSE);

		break;

	case -1:

		SetDlgItemText(IDC_CardATR,"Card ATR: ");
		SetDlgItemText(IDC_CardStatuse,"卡片未连接");


		break;

	case -2:

		SetDlgItemText(IDC_CardATR,"Card ATR: ");
		SetDlgItemText(IDC_CardStatuse,"未收到正确返回");


		break;
	case -10:

		SetDlgItemText(IDC_CardATR,"Card ATR: ");
		SetDlgItemText(IDC_CardStatuse,"Cold Reset 失败");


		break;

	default:

		SetDlgItemText(IDC_CardATR,"Card ATR: "+csATR);
		SetDlgItemText(IDC_CardStatuse,"卡片状态未知");
		break;

	}



}

CString CRunLogDlg::CalaConfirmSSDData(void)
{

	unsigned char ucSSD[16];
	unsigned char ucAuthBS[3];
	unsigned char ucIMSIS1[3];
	unsigned char ucESN[4];
	unsigned char ucTemp;

	memcpy(ucESN,ucUIMID+1,4);


	ucTemp = ucESN[0];
	ucESN[0] = ucESN[3];
	ucESN[3] = ucTemp;
	ucTemp = ucESN[1];
	ucESN[1] = ucESN[2];
	ucESN[2] = ucTemp;

	ucIMSIS1[0] = ucIMSI_M[5];
	ucIMSIS1[1] = ucIMSI_M[4];
	ucIMSIS1[2] = ucIMSI_M[3];


	InitAKey((char *)ucAKey);
	InitESN((char *) ucESN);
	SSD_Generation(ucRandSSD, (char *)ucSSD, (char *)ucSSD+0x8);

	InitESN((char *) ucESN);
	Auth_Signature(ucRand, ucIMSIS1, ucSSD, 0x00, (char *)ucAuthBS);

	CString csResult;
	_UcHex2CString(ucAuthBS,3,csResult);

	return csResult;
}

int CRunLogDlg::PretreatCurrentCommand(void)
{

	CString csSend,csES,csED;
	CString csP3;
	int iIns,iP1,iP2,iLc;
	int iRet;
	if (!bAdj)
		return bAdj;

	GetDlgItemText(IDC_CurrentSend_Edit,csSend);
	GetDlgItemText(IDC_ExpectSW_Edit,csES);
	GetDlgItemText(IDC_ExpectReponse_Edit,csED);

	iIns = _CString2Int(csSend.Mid(02,02));
	iP1  = _CString2Int(csSend.Mid(04,02));
	iP2  = _CString2Int(csSend.Mid(06,02));
	iLc  = _CString2Int(csSend.Mid(0x8,02));
	iRet = false;


	switch(iIns)
	{
		//Fetch
	case 0x12:

		if (!bADJ0x12)
			break;
		if ((CompareListSW("9000",csES))&&(iFetchLen!=00)&&(csSend.Compare(_T("0012000000"))!=0))
		{
			csP3.Format("%02X",iFetchLen);
			csSend = csSend.Left(0x8)+csP3;
			iRet   = true;
		}


		switch(iPreIns)
		{
		case 0x10:

			if (bADJ0x10)
			{
				SetDlgItemText(IDC_ExpectReponse_Edit,"");
				SetDlgItemText(IDC_ExpectSW_Edit,"");
				break;
			}


		default:

			break;
		}

		if (bADJSTKStartup)
		{
			if ((bInSTKStartup==true)&&(bInSTK == false)&&(iFetchLen!=00))
			{
				SetDlgItemText(IDC_ExpectReponse_Edit,"");
				SetDlgItemText(IDC_ExpectSW_Edit,"6F00");
				bInSTKStartup = false;
			}
		}


		break;


		//TeminalProfile
	case 0x10:

		bInSTKStartup = true;


		break;
		//TeminalResponse
	case 0x14:


		break;

		//ManageSSD
	case 0x82:

		if (!bADJ0x82)
			break;

		if ((iCurDF == 0x7F25)||(iCurDF == 0x7F2E)||((iCurDF == 0x7FF1)&&(iP2 == 01)))
		{
			if (CompareListSW("9000",csES))
			{
				csSend = csSend.Left(10)+CalaConfirmSSDData();

				iRet   = true;
			}
		}


		break;
	case 0x84:
		if (bADJRes0x84)
			SetDlgItemText(IDC_ExpectReponse_Edit,"");
		break;
		iRet   = true;
		break;

		//Auth
	case 0x88:


		break;
		//GetResponse
	case 0xC0:

		if (!bADJ0xC0)
			break;




		switch(iPreIns)
		{



		case 0x8A:
			if (bADJRes0x8A)
				SetDlgItemText(IDC_ExpectReponse_Edit,"");
			break;
		case 0x32:

			if (bADJRes0x32)
			{
				if (csED.GetLength()!=0)
				{
					csP3.Format("%02X",csED.GetLength()/2);
					break;
				}
				else
					goto loop;
			}

			break;

		case 0xA4:
			if((bADJRes0xA4)&&(iLogType != _Catch_LOG))
			{
				if (csED.GetLength()!=0)
				{
					csP3.Format("%02X",csED.GetLength()/2);
					break;
				}
				else
					goto loop;
			}

			break;

		default:
loop:    	if ((CompareListSW("9000",csES))&&(iGetResponseLen!=00))
					{
						csP3.Format("%02X",iGetResponseLen);
						csSend = csSend.Left(0x8)+csP3;
						iRet   = true;
					}

					break;
		}


		break;
		//Statuse
	case 0xF2:
		if (!bADJ0xF2)
			break;
		if ((CompareListSW("9000",csES))&&(iStatusLen!=00))
		{
			csP3.Format("%02X",iStatusLen);
			csSend = csSend.Left(0x8)+csP3;
			iRet   = true;
		}


		break;


	default:

		break;
	}










	if (iRet)
		SetDlgItemText(IDC_CurrentSend_Edit,csSend);

	return iRet;

}


int CRunLogDlg::PostCurrentCommand(void)
{

	CString csSend,csResp,csSW;
	int iIns,iP1,iP2,iLc;
	int iSW;
	CString csData;
	int iRet;
	if (!bAdj)
		return bAdj;
	GetDlgItemText(IDC_CurrentSend_Edit,csSend);
	GetDlgItemText(IDC_CurrentResponse_Edit,csResp);
	GetDlgItemText(IDC_CurrentSW_Edit,csSW);


	iFetchLen       = 00;
	iStatusLen      = 00;
	iGetResponseLen = 00;
	iIns = _CString2Int(csSend.Mid(02,02));
	iP1  = _CString2Int(csSend.Mid(04,02));
	iP2  = _CString2Int(csSend.Mid(06,02));
	iLc  = _CString2Int(csSend.Mid(0x8,02));
	iSW  = _CString2Int(csSW);

	switch(iIns)
	{

	case 0x10:  

		if ((iSW&0xFF00) == 0x9100)
			bInSTK = true;
		break;

	case 0x12:  



		break;

	case 0x14:  
		if (iSW == 0x9000)
			bInSTK = false;
		break;

	case 0x8A:  //base station challenge
		//iRet = _CString2UcHex(csResp.Right(0x08),ucRand);
		break;

	case 0x82:

		if (iCurDF == 0x7FF1)
		{
			if (iP2 == 0)
			{
				ZeroMemory(ucRandSSD,0x7);
				_CString2UcHex(csSend.Mid(10,14),ucRandSSD);
			}

		}
		break;
	case 0x84:
		ZeroMemory(ucRandSSD,0x7);
		_CString2UcHex(csSend.Mid(10,14),ucRandSSD);
		break;

	case 0xA4://Select


		if (iLc==2)
		{
			csData  = csSend.Mid(10,04);
			iCurFid =  _CString2Int(csData);
			if ((iCurFid>0x7F00)&&
				(iCurFid!=0x7FFF))
				iCurDF = iCurFid; 
		}

		if (iLc >5)
		{
			csData  = csSend.Mid(10,10); 
			if (csData == "A000000343")
				iCurDF = 0x7FF1;
			else if (csData == "A000000087")
				iCurDF = 0x7FF0;
		}

		break;

	case 0xB0://ReadBinary


		if ((iCurDF == 0x7FF1)||(iCurDF == 0x7F25)||(iCurDF == 0x7F2E))
		{
			switch(iCurFid)
			{
			case 0x6F31:
				ZeroMemory(ucUIMID,0x8);
				_CString2UcHex(csResp,ucUIMID+_CString2Int(csSend.Mid(6,2)));
				break;
			case 0x6F22:
				ZeroMemory(ucIMSI_M,10);
				_CString2UcHex(csResp,ucIMSI_M);
				break; 
			default:break;
			}
			break;

		}



	case 0xC0:

		switch(iPreIns)
		{
		case 0x8A:



			//	_CString2UcHex("0A9BD2A7",ucRand);

			iRet = _CString2UcHex(csResp.Right(0x08),ucRand);
			break;
		default:break;
		}


		break;

	case 0xC2:

		if ((iSW&0xFF00) == 0x9100)
			bInSTK = true;

		break;


	case 0xF2:
		if (((iSW&0xFF00) == 0x6c00)||((iSW &0xFF00) == 0x6700))
			iStatusLen = iSW&0x00FF;


		break;

	default:
		break;
	}



	if ((iSW&0xFF00) == 0x9100)
		iFetchLen = iSW&0x00FF;

	if (((iSW&0xFF00) == 0x6100)||((iSW&0xFF00) == 0x9F00))
		iGetResponseLen = iSW&0x00FF;

	//	||((iRecP3&0x9f00) == 0x9f00)||((iRecP3&0x6100) == 0x6100)||((iRecP3&0x6c00) == 0x6c00)||((iRecP3&0x6700) == 0x6700))

	iPreIns = iIns;
	return true;
}

void CRunLogDlg::OnAdj0x82()
{

	bADJ0x82 = !bADJ0x82;
}

void CRunLogDlg::OnUpdatePara0x82(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(bADJ0x82);
}

void CRunLogDlg::OnUpdateAdj0x82(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJ0x82);
}

void CRunLogDlg::OnPara0x82()
{

	BOOL bRet ;
	CInputDataDlg *DlgInputData;
	DlgInputData = new CInputDataDlg;
	DlgInputData->Create(IDD_InputDlg,NULL);
	DlgInputData->InitInputDlg();
	DlgInputData->AddPara2List("AKey",UcHex2CString(ucAKey,0x8));
	bRet = DlgInputData->ShowWindow(SW_SHOW);
	if (bRet != 0)
		bRet = DlgInputData->ShowWindow(SW_HIDE);
}

void CRunLogDlg::OnAdj0x10()
{
	bADJ0x10 = !bADJ0x10;

}

void CRunLogDlg::OnUpdateAdj0x10(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJ0x10);

}

void CRunLogDlg::OnAdj0x12()
{
	bADJ0x12 = !bADJ0x12;
}

void CRunLogDlg::OnPara0x12()
{
	BOOL bRet ;
	CInputDataDlg *DlgInputData;
	DlgInputData = new CInputDataDlg;
	DlgInputData->Create(IDD_InputDlg,NULL);
	DlgInputData->InitInputDlg();
	DlgInputData->AddPara2List("KIc",UcHex2CString(ucKIc,0x10));
	DlgInputData->AddPara2List("KID",UcHex2CString(ucKID,0x10));
	bRet = DlgInputData->ShowWindow(SW_SHOW);
	if (bRet != 0)
		bRet = DlgInputData->ShowWindow(SW_HIDE);
}

void CRunLogDlg::OnUpdateAdj0x12(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJ0x12);
}

void CRunLogDlg::OnUpdatePara0x12(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(bADJ0x12);
}


void CRunLogDlg::OnUpdateMenuparse(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd > 0);
}

void CRunLogDlg::OnUpdateMenustop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd >= 0);
}

void CRunLogDlg::OnUpdate1Script(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd < 0);
}

void CRunLogDlg::OnUpdateAllList(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd < 0);
}




void CRunLogDlg::OnMenurestore()
{
	DWORD code;

	if (CurrentStatus()!=0)
		return;

	GetExitCodeThread(hThread,&code);      

	if ((code == STILL_ACTIVE)&&(iThreadEnd == 0))
	{
		iThreadEnd = 1;	
		ResumeThread(hThread);
	}

	b1Step = false;
	ContrleBusy(FALSE);
	return ;
}

void CRunLogDlg::OnUpdateMenurestore(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd == 0);
}

void CRunLogDlg::OnUimUpdatessd()
{

	BOOL bRet ;
	CManageSSDDlg *DlgManageSSDDlg;
	DlgManageSSDDlg = new CManageSSDDlg;
	DlgManageSSDDlg->Create(IDD_ManageSSDDlg,NULL);
	DlgManageSSDDlg->InitManageSSDDlg(0x7F25);
	bRet = DlgManageSSDDlg->ShowWindow(SW_SHOW);
	if (bRet != 0)
		bRet = DlgManageSSDDlg->ShowWindow(SW_HIDE);


}

void CRunLogDlg::OnCsimManagessd()
{
	BOOL bRet ;
	CManageSSDDlg *DlgManageSSDDlg;
	DlgManageSSDDlg = new CManageSSDDlg;
	DlgManageSSDDlg->Create(IDD_ManageSSDDlg,NULL);
	DlgManageSSDDlg->InitManageSSDDlg(0x7FF1);
	bRet = DlgManageSSDDlg->ShowWindow(SW_SHOW);
	if (bRet != 0)
		bRet = DlgManageSSDDlg->ShowWindow(SW_HIDE);
}

bool CRunLogDlg::CheckPrograssStatus(HANDLE hThreadHand)
{

	DWORD code;
	GetExitCodeThread(hThreadHand,&code);

	if (code != STILL_ACTIVE)
		return false;

	return true;
}


int CRunLogDlg::SetAkeyBuffer(CString csAkey)
{
	return _CString2UcHex(csAkey,ucAKey);
}
int CRunLogDlg::SetKIcBuffer(CString csKIc)
{
	return _CString2UcHex(csKIc,ucKIc);
}
int CRunLogDlg::SetKIDBuffer(CString csKID)
{
	return _CString2UcHex(csKID,ucKID);
}


CString CRunLogDlg::GetProtectedData(int iType)
{

	CString csResult;

	switch(iType)
	{
	case 01:
		csResult = UcHex2CString(ucAKey,0x8);
		break;
	case 02:
		csResult = UcHex2CString(ucIMSI_M,0x0A);
		break;
	case 03:
		csResult = UcHex2CString(ucUIMID,0x08);
		break;
	case  04:
		csResult = UcHex2CString(ucRandSSD,0x07);
		break ;
	case  05:
		csResult = UcHex2CString(ucRand,0x04);
		break; 
	default:
		csResult.Empty();
	}
	return csResult;
}





int  CRunLogDlg::Connect_PSCS(CString *csCardATR)
{
	unsigned char ucATR[100];
	int iRet;
	int iReaderNum;
	LPTSTR  szReaderName[10];
	CString csReaderName;

	m_PCSCReader_Combo.EnableWindow(FALSE);

	iReaderNum = m_PCSCReader_Combo.GetCurSel();

	switch (iConSta)
	{
	case 0:

		iRet = PcscReader.DisConnectReader();

		SetDlgItemText(IDC_Connect_Button,"Connect");
		iConSta = -1;
		break;

	case -1:
	case -2:

		iRet =  PcscReader.GetContext();
		iRet =  PcscReader.GetPcsclist(szReaderName);
		iRet =  PcscReader.ConnectReader(szReaderName[iReaderNum]);
		if (iRet != 0)
		{
			MessageBox("Select Reader failed!");
			break;

		}
		iRet = PcscReader.GetCardATR(ucATR);



		if (iRet == 0)
		{
			MessageBox("Get ATR failed!");
			break;
		}
		*csCardATR = UcHex2CString(ucATR,iRet);

		iConSta = 0;

		SetDlgItemText(IDC_Connect_Button,"DisConnect");

		break;

	default:

		break;
	}

	return true;
}



int CRunLogDlg::Connect_CCR5(CString *csCardATR)
{

	//int iSel,iRet;
	int iSel;
	CString csReader;
	//	unsigned int uiRespLen;
	unsigned char ucATR[50];
//	unsigned char ucPTS[4];
	//	BYTE ucResp[50];
//	UINT iTermType;
	//UINT uiATRLen;
	int uiATRLen;

	BYTE ucT_PTS[4] = {"\x12\x00\x02"};
	//	BYTE ucT_ATR[50];
	//	BYTE ucT_Mau[50];
	//	UINT uiT_ATRLen,uiT_MauLen;
	//	BYTE ucCardStatus;


	iSel = m_PCSCReader_Combo.GetCurSel();
	m_PCSCReader_Combo.GetLBText(iSel,csReader);


	switch(iConSta)
	{
	case 0:

		CCR550.PowerOffCCR5();
		SetDlgItemText(IDC_Connect_Button,"Connect");
		iConSta = -1;

	case -1:

		CCR550.GetPortReader(1);

		iConSta = -2;
	case -2:

		CCR550.SetCTBasPara(); 
		CCR550.PowerOn_Ex(ucATR,&uiATRLen);
		SetDlgItemText(IDC_Connect_Button,"DisConnect");
		iConSta = 0;
		break;

	default:
		break;
	}






	//switch (iConSta)
	//{
	//case 0:

	//	iRet = CCR5_PowerOffCard(CCR5Hand);

	//	if (iRet== FALSE)
	//	{			
	//		SetDlgItemText(IDC_Connect_Button,"Connect");
	//		iConSta = -2;
	//	}
	//			
	//	if (CCR5_DisConnectTerminal(CCR5Hand))
	//	{
	//		SetDlgItemText(IDC_Connect_Button,"Connect");
	//		iConSta = -1;
	//	}


	//	break;

	//case -1:

	//	iRet = CCR5_ConnectTerminal(&CCR5Hand,atoi(csReader.Right(csReader.GetLength()-3)),ucT_PTS,0x3,
	//		ucT_ATR,&uiT_ATRLen,&ucCardStatus,ucT_Mau,&uiT_MauLen);
	//	if (iRet != CT_OK)
	//		return -1;


	//	iRet = CCR5_SetupTerminal2Default(CCR5Hand);
	//	if (iRet != CT_OK)
	//		return -1;

	//	iConSta = -2;


	//case -2:

	//	iRet = CCR5_PowerOnCard(CCR5Hand,&ucCardStatus,ucATR,&uiATRLen);
	//	if (iRet != CT_OK)
	//		return -4;
	//	GeneratePPS(0x0,ucATR[2],ucPTS);
	//	ZeroMemory(ucResp,4);

	//	iRet = CCR5_SendPTS2Card(CCR5Hand,ucPTS,4,ucResp,&uiRespLen);

	//	*csCardATR = UcHex2CString(ucATR,uiATRLen);

	//	SetDlgItemText(IDC_Connect_Button,"DisConnect");

	//	iConSta = 0;

	//	break;
	//default:

	//	break;
	//}



	//switch (iConSta)
	//{
	//case 0:

	//	if ( STM_FreeCard(NewHand) == STM_OK)
	//	{

	//		SetDlgItemText(IDC_Connect_Button,"Connect");
	//		iConSta = -2;
	//	}

	//	if (STM_Close(NewHand) == STM_OK)
	//	{
	//		SetDlgItemText(IDC_Connect_Button,"Connect");
	//		iConSta = -1;
	//	}

	//	break;

	//case -1:

	//	iRet = STM_Open(&NewHand,atoi(csReader.Right(csReader.GetLength()-3)) ,STM_EXCL);


	//	if (iRet != STM_OK)
	//		return -1;

	//	iConSta = -2;

	//case -2:

	//	if (STM_Request(NewHand,STM_ICC1,STM_EXCL)!=STM_OK)
	//		return -2;

	//	if (STM_GetTermType(NewHand,&iTermType) != STM_OK)
	//		return -3;

	//	if (STM_GetATR(NewHand,50,&uiATRLen,ucATR) != STM_OK)
	//		return -4;

	//	GeneratePPS(0x0,ucATR[2],ucPTS);

	//	if (STM_ICCSetPTS(NewHand,STM_ICC1,STM_PTS_AUTO_ON,0x4,ucPTS) != STM_OK)
	//		return -5;

	//	*csCardATR = UcHex2CString(ucATR,uiATRLen);

	//	iConSta = 0;

	//	SetDlgItemText(IDC_Connect_Button,"DisConnect");

	//	break;

	//default:

	//	break;
	//}

	return true;
}

int CRunLogDlg::Send_CCR5(unsigned char *ucSend,int iSend,unsigned char *ucResp,int *iResLen)
{


	//	unsigned int uiResLen;
	//int iRet;
	//iRet = STM_SendCard(NewHand,iSend,ucSend,0x102,&uiResLen,ucResp,2,0x9000,0);

	//iRet = CCR5_Send2Card(CCR5Hand,ucSend,iSend,ucResp,&uiResLen);
	int iTime;

	CCR550.SendCommand_T0(ucSend,iSend,ucResp,iResLen,&iTime);
	//	*iResLen = uiResLen;

	return 0;
}


int CRunLogDlg::Connect_Stm(CString *csCardATR)
{

	int iSel,iRet;
	CString csReader;
	unsigned int iTermType ,uiATRLen,uiPort;
	unsigned char ucATR[50];

	iSel = m_PCSCReader_Combo.GetCurSel();
	m_PCSCReader_Combo.GetLBText(iSel,csReader);



	switch (iConSta)
	{
	case 0:

		if ( STM_FreeCard(NewHand) == STM_OK)
		{

			SetDlgItemText(IDC_Connect_Button,"Connect");
			iConSta = -2;
		}

		if (STM_Close(NewHand) == STM_OK)
		{
			SetDlgItemText(IDC_Connect_Button,"Connect");
			iConSta = -1;
		}

		break;

	case -1:
		iRet = STM_OpenEx(&NewHand,csReader,STM_EXCL);

		if (NewHand == NULL)
		{
			if (iRet == STM_ERR_TERM_COMM_CT)
			{
				//iRet = STM_GetPortId(csReader,&uiPort);
				uiPort = atoi(csReader.Right(csReader.GetLength()-7));
				iRet = STM_Open(&NewHand,uiPort ,STM_EXCL);

			}
		}
		if (iRet != STM_OK)
			return -1;

		iConSta = -2;

	case -2:

		if ( STM_Request(NewHand,STM_ICC1,STM_EXCL)!=STM_OK)
			return -2;

		if (STM_GetTermType(NewHand,&iTermType) != STM_OK)
			return -3;

		if (STM_GetATR(NewHand,50,&uiATRLen,ucATR) != STM_OK)
			return -4;	

		*csCardATR = UcHex2CString(ucATR,uiATRLen);

		iConSta = 0;

		SetDlgItemText(IDC_Connect_Button,"DisConnect");

		break;

	default:

		break;
	}

	return true;


}

int CRunLogDlg::Send_Stm(unsigned char *ucSend,int iSend,unsigned char *ucResp,int *iResLen)
{


	unsigned int uiResLen;
	int iRet;
	iRet = STM_SendCard(NewHand,iSend,ucSend,0x102,&uiResLen,ucResp,2,0x9000,0);
	*iResLen = uiResLen;


	return 0;
}



void CRunLogDlg::OnBnClickedExit()
{

	OnBnClickedStop();
	CDialog::OnCancel();
}

void CRunLogDlg::OnBnClickedRestore()
{
	DWORD code;

	if (CurrentStatus()!=0)
		return;

	GetExitCodeThread(hThread,&code);      

	b1Step = false;

	if ((code == STILL_ACTIVE)&&(iThreadEnd == 0))
	{
		iThreadEnd = 1;	
		ResumeThread(hThread);
	}

	ContrleBusy(TRUE);

	return ;

}

void CRunLogDlg::OnAdj0xf2()
{
	bADJ0xF2 = !bADJ0xF2;
}

void CRunLogDlg::OnUpdateAdj0xf2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJ0xF2);
}

void CRunLogDlg::OnAdj0xc0()
{
	bADJ0xC0 = !bADJ0xC0;
}

void CRunLogDlg::OnUpdateAdj0xc0(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJ0xC0);
}

void CRunLogDlg::OnLeve1()
{
	iCheckLevel = 1;
}

void CRunLogDlg::OnUpdateLeve1(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(iCheckLevel == 1);
}

void CRunLogDlg::OnLeve2()
{
	iCheckLevel = 2;
}

void CRunLogDlg::OnUpdateLeve2(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(iCheckLevel == 2);
}
void CRunLogDlg::OnLeve3()
{
	iCheckLevel = 3;
}
void CRunLogDlg::OnUpdateLeve3(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(iCheckLevel == 3);

}


void CRunLogDlg::OnLeve4()
{
	iCheckLevel = 4;
}

void CRunLogDlg::OnUpdateLeve4(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(iCheckLevel == 4);
}

void CRunLogDlg::OnLeve5()
{
	iCheckLevel = 5;
}

void CRunLogDlg::OnUpdateLeve5(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(iCheckLevel == 5);
}


void CRunLogDlg::OnAdjRes0x8a()
{
	bADJRes0x8A = !bADJRes0x8A;
}

void CRunLogDlg::OnUpdateAdjRes0x8a(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJRes0x8A);
}

void CRunLogDlg::OnAdjRes0x84()
{
	bADJRes0x84 = !bADJRes0x84;
}

void CRunLogDlg::OnAdjRes0xa4()
{
	bADJRes0xA4 = !bADJRes0xA4;
}

void CRunLogDlg::OnUpdateAdjRes0xa4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJRes0xA4);
}


void CRunLogDlg::OnAdjRes0x32()
{

	bADJRes0x32 = !bADJRes0x32;
}

void CRunLogDlg::OnUpdateAdjRes0x32(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJRes0x32);
}



void CRunLogDlg::OnUpdateAdjRes0x84(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJRes0x84);
}

void CRunLogDlg::OnUpdateOpenctlog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd == -1);
}

void CRunLogDlg::OnUpdateOpendir(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd == -1);
}

void CRunLogDlg::OnUpdateCatchlog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd == -1);
}


void CRunLogDlg::OnUpdateOpencmcclog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd == -1);
}

void CRunLogDlg::OnUpdateOpencmcclist(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd == -1);
}



void CRunLogDlg::OnUpdateConvertionlog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(iThreadEnd == -1);
}



void CRunLogDlg::OnBnClickedOnestep()
{

	DWORD code;



	if (m_CTLOG_Edit.GetLineCount()<2)
	{
		MessageBox("未发现脚本内容！");
		return;
	}

	if (CurrentStatus()!=0)
		return;


	b1Step = true;

	GetExitCodeThread(hThread,&code);    

	if (code != STILL_ACTIVE)
	{
		m_CTLOG_Edit.SetSel(0,0);
		m_CTLOG_Edit.SetFocus();
		hThread  = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) ThreadRun,NULL, 0, NULL);
	}
	else
		ResumeThread(hThread);


	iThreadEnd = 1;	
	ContrleBusy(TRUE);
}



void CRunLogDlg::Addcmcclist(CString csListFilePath,CString csListDir)
{

	HTREEITEM hDIRTreeItem ;
	m_FileTree.DeleteAllItems();

	//int iRet;
	CMarkup cmFile;
	CString csData,csFilePath,csFileName;
	//BOOL bRet;
	int iOffset,iLenght;

	HTREEITEM hCurTreeItem;

	m_FileTree.DeleteAllItems();
	hDIRTreeItem = m_FileTree.InsertItem(csListFilePath,Def_Image_DIR,Def_Image_DIR);



	cmFile.Load(csListFilePath);
	csData = cmFile.GetDoc();

	iOffset = 0;
	iLenght = 0;

	do 
	{
		iOffset = csData.Find(_T("<A HREF = \""),iOffset);
		if (iOffset<0)
			break;

		iLenght = csData.Find(_T("\">"),iOffset) - iOffset;
		csFilePath = csData.Mid(iOffset+11,iLenght-11);
		iOffset    = iOffset+iLenght +2;
		csFileName = csData.Mid(iOffset,csData.Find(_T("</A>"),iOffset) - iOffset);

		hCurTreeItem = m_FileTree.InsertItem(csFileName,Def_Image_File,Def_Image_File,hDIRTreeItem);
		hCurTreeItem = m_FileTree.InsertItem(csListDir +csFilePath,Def_Image_Path,Def_Image_Path,hCurTreeItem);


	} while (iOffset>0);

}


void CRunLogDlg::OnOpencmcclist()
{

	CString csDir,csPath;
	OnBnClickedStop();

	ContrleBusy(TRUE);

	CFileDialog CFDataFile(TRUE,NULL,_T("*.HTML"),4|1,_T("HTML File文件(*.HTML)"));
	if ( CFDataFile.DoModal() == IDOK)

	{
		csPath  = CFDataFile.GetPathName();
		csDir   = csPath.Left(csPath.GetLength() - CFDataFile.GetFileName().GetLength());

		Addcmcclist(csPath ,csDir);
	}


	ContrleBusy(FALSE);

	iLogType = _CMCC_LOG;


}

void CRunLogDlg::OnAbout()
{

	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CRunLogDlg::OnStkStartup()
{
	bADJSTKStartup = !bADJSTKStartup;

}

void CRunLogDlg::OnUpdateStkStartup(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(bADJSTKStartup);
}


void CRunLogDlg::OnAdj0x14()
{
	bADJ0x14 = !bADJ0x14;
}

void CRunLogDlg::OnUpdateAdj0x14(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bADJ0x14);
}

void CRunLogDlg::OnStnClickedCurrentfile()
{

}

void CRunLogDlg::OnBnClickedStartcase()
{
	OnAllList();
}

void CRunLogDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	SaveSetting(csCurMDir+_T("\\Setting.ini"));

	CDialog::OnCancel();
}

BOOL CRunLogDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;	
	}
	return CDialog::PreTranslateMessage(pMsg);

}


void CRunLogDlg::OnDefaultsetting()
{

	bWrongPause=  true;
	bWriteLog  =  true;
	bWriteTime =  true;


	bAdj       =  true;
	bADJ0x10   =  true;
	bADJ0x12   =  true;
	bADJ0x14   =  true;
	bADJ0x82   =  true;
	bADJ0xC0   =  true;
	bADJ0xF2   =  true;



	bADJRes0x8A = true;
	bADJRes0x84 = true;
	bADJRes0xA4 = true;
	bADJRes0x32 = true;


	bADJSTKStartup = true;


	bInSTK        = FALSE;
	bInSTKStartup = FALSE;


	iCheckLevel=  1;

}

BOOL CRunLogDlg::GetSetting(CString csFile)
{

	CString csData;
	CFileStatus csSta;
	CStdioFile CSFFile;
	int iRet;


	iRet = CSFFile.Open(csFile,CFile::shareDenyRead);

	if (!iRet)
		return FALSE;

	iRet = CSFFile.GetStatus(csFile,csSta);
	if (!iRet)
	{
		CSFFile.Close();
		return FALSE;
	}


	//读取文件头
	iRet = CSFFile.ReadString(csData);

	if ((iRet == FALSE)||(csData!=_T("#Setting File#")))
	{
		MessageBox(_T("不是设置文件"));
		CSFFile.Close();
		return FALSE;
	}

	do 
	{
		//读取文件内容
		iRet = CSFFile.ReadString(csData);
		if (iRet == FALSE)
			break;

		TransSetting(csData);

	} while (iRet);

	CSFFile.Close();
	return TRUE;
}


BOOL CRunLogDlg::TransSetting(CString csData)
{
	BOOL bRet;
	CString csName,csValue;
	int iSum;
	bRet = _RecogniseFileData(csData,&csName,&csValue,&iSum);
	
	

	if (csName == _T("WP"))
	{
		if (csValue == _T("TURE;"))
			bWrongPause = TRUE;
		else
			bWrongPause = FALSE;
	}

	if (csName == _T("WL"))
	{
		if (csValue == _T("TURE;"))
			bWriteLog = TRUE;
		else
			bWriteLog = FALSE;
	}

	if (csName == _T("WT"))
	{
		if (csValue == _T("TURE;"))
			bWriteTime = TRUE;
		else
			bWriteTime = FALSE;
	}


	if (csName == _T("ADJ"))
	{
		if (csValue == _T("TURE;"))
			bAdj = TRUE;
		else
			bAdj = FALSE;
	}
	if (csName == _T("ADJ10"))
	{
		if (csValue == _T("TURE;"))
			bADJ0x10 = TRUE;
		else
			bADJ0x10 = FALSE;
	}

	if (csName == _T("ADJ12"))
	{
		if (csValue == _T("TURE;"))
			bADJ0x12 = TRUE;
		else
			bADJ0x12 = FALSE;
	}

	if (csName == _T("ADJ14"))
	{
		if (csValue == _T("TURE;"))
			bADJ0x14 = TRUE;
		else
			bADJ0x14 = FALSE;
	}


	if (csName == _T("ADJ82"))
	{
		if (csValue == _T("TURE;"))
			bADJ0x82 = TRUE;
		else
			bADJ0x82 = FALSE;
	}

	if (csName == _T("ADJC0"))
	{
		if (csValue == _T("TURE;"))
			bADJ0xC0 = TRUE;
		else
			bADJ0xC0 = FALSE;
	}
	if (csName == _T("ADJF2"))
	{
		if (csValue == _T("TURE;"))
			bADJ0xF2 = TRUE;
		else
			bADJ0xF2 = FALSE;
	}

	if (csName == _T("ADJR8A"))
	{
		if (csValue == _T("TURE;"))
			bADJRes0x8A = TRUE;
		else
			bADJRes0x8A = FALSE;
	}

	if (csName == _T("ADJR84"))
	{
		if (csValue == _T("TURE;"))
			bADJRes0x84 = TRUE;
		else
			bADJRes0x84 = FALSE;
	}

	if (csName == _T("ADJRA4"))
	{
		if (csValue == _T("TURE;"))
			bADJRes0xA4 = TRUE;
		else
			bADJRes0xA4 = FALSE;
	}

	if (csName == _T("ADJR32"))
	{
		if (csValue == _T("TURE;"))
			bADJRes0x32 = TRUE;
		else
			bADJRes0x32 = FALSE;
	}

	if (csName == _T("ADJSTKSU"))
	{
		if (csValue == _T("TURE;"))
			bADJSTKStartup = TRUE;
		else
			bADJSTKStartup = FALSE;
	}
	if (csName == _T("InSTK"))
	{
		if (csValue == _T("TURE;"))
			bInSTK = TRUE;
		else
			bInSTK = FALSE;
	}

	if (csName == _T("InSTKSU"))
	{
		if (csValue == _T("TURE;"))
			bInSTKStartup = TRUE;
		else
			bInSTKStartup = FALSE;
	}

	if (csName == _T("CL"))
	{
		iCheckLevel = _CString2Int(GetCStringData(csValue,1));
	}

	if (csName == _T("CDMA_AKEY"))
	{
		_CString2UcHex(GetCStringData(csValue,1),ucAKey);
	}
	if (csName == _T("CDMA_Rand"))
	{
		_CString2UcHex(GetCStringData(csValue,1),ucRand);
	}

	if (csName == _T("CDMA_UMID"))
	{
		_CString2UcHex(GetCStringData(csValue,1),ucUIMID);
	}
	if (csName == _T("CDMA_SSD"))
	{
		_CString2UcHex(GetCStringData(csValue,1),ucRandSSD);
	}
	if (csName == _T("CDMA_IMSIM"))
	{
		_CString2UcHex(GetCStringData(csValue,1),ucIMSI_M);
	}

	return TRUE;

}

BOOL CRunLogDlg::SaveSetting(CString csFile)
{
	CString csData;
	CString csResult;
	CFileStatus csSta;
	CStdioFile CSFFile;
	int iRet;
	BOOL bRet;


	iRet = CSFFile.Open(csFile,CFile::modeReadWrite);
	if (!iRet)
	{
		iRet = CSFFile.Open(csFile,CFile::modeCreate|CFile::modeReadWrite);
		if (!iRet)
			return FALSE;

		csResult = _T("[WP]:TURE;\n[WL]:TURE;\n[WT]:TURE;\n[ADJ]:TURE;\n[ADJ10]:TURE;\n[ADJ12]:TURE;\n[ADJ14]:TURE;\n[ADJ82]:TURE;\n[ADJC0]:TURE;\n[ADJF2]:TURE;\n[ADJR8A]:TURE;\n[ADJR84]:TURE;\n[ADJRA4]:TURE;\n[ADJR32]:TURE;\n[ADJSTKSU]:TURE;\n[InSTK]:FALSE;\n[InSTKSU]:FALSE;\n[CL]:01;\n[CDMA_AKEY]:7B70B748F0765AB7;\n[CDMA_Rand]:01020304;\n[CDMA_UMID]:0102030405060708;\n[CDMA_SSD]:01020304050607;\n[CDMA_IMSIM]:0102030405060708090A;");
	}
	else
	{
		iRet = CSFFile.GetStatus(csFile,csSta);
		if (!iRet)
		{
			CSFFile.Close();
			return FALSE;
		}

		//读取文件头
		iRet = CSFFile.ReadString(csData);

		if ((iRet == FALSE)||(csData!=_T("#Setting File#")))
		{
			CSFFile.Close();
			return FALSE;
		}

		do 
		{
			//读取文件内容
			iRet = CSFFile.ReadString(csData);
			if (iRet == FALSE)
				break;

			bRet = RebornSetting(csData,&csResult);

		} while (iRet);
	}



	//读取文件头

	if (bRet)
	{

		CSFFile.SeekToBegin();

		csResult = _T("#Setting File#\n")+csResult;

		CSFFile.WriteString(csResult);
	}

	CSFFile.Close();
	return bRet;


}

BOOL CRunLogDlg::RebornSetting(CString csData,CString* csNewInit)
{

	BOOL bRet;
	CString csName,csValue;
	int iSum;
	bRet = _RecogniseFileData(csData,&csName,&csValue,&iSum);

	if (csName == _T("WP"))
	{

		*csNewInit += _T("[WP]:");
		if (bWriteLog)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");

	}

	else if (csName == _T("WL"))
	{
		*csNewInit += _T("[WL]:");
		if (bWriteLog)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");

	}

	else if (csName == _T("WT"))
	{

		*csNewInit += _T("[WT]:");
		if (bWriteTime)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}

	else if (csName == _T("ADJ"))
	{
		*csNewInit += _T("[ADJ]:");
		if (bAdj)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");

	}
	else if (csName == _T("ADJ10"))
	{
		*csNewInit += _T("[ADJ10]:");
		if (bADJ0x10)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJ12"))
	{
		*csNewInit += _T("[ADJ12]:");
		if (bADJ0x12)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJ14"))
	{
		*csNewInit += _T("[ADJ14]:");
		if (bADJ0x14)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJ82"))
	{
		*csNewInit += _T("[ADJ82]:");
		if (bADJ0x82)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJC0"))
	{
		*csNewInit += _T("[ADJC0]:");
		if (bADJ0xC0)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJF2"))
	{
		*csNewInit += _T("[ADJF2]:");
		if (bADJ0xF2)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJR8A"))
	{
		*csNewInit += _T("[ADJR8A]:");
		if (bADJRes0x8A)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJR84"))
	{
		*csNewInit += _T("[ADJR84]:");
		if (bADJRes0x84)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJRA4"))
	{
		*csNewInit += _T("[ADJRA4]:");
		if (bADJRes0xA4)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJR32"))
	{
		*csNewInit += _T("[ADJR32]:");
		if (bADJRes0x32)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("ADJSTKSU"))
	{
		*csNewInit += _T("[ADJSTKSU]:");
		if (bADJSTKStartup)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("InSTK"))
	{
		*csNewInit += _T("[InSTK]:");
		if (bInSTK)
			*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("InSTKSU"))
	{
		*csNewInit += _T("[InSTKSU]:");
		if (bInSTKStartup)
				*csNewInit += _T("TURE;");
		else
			*csNewInit += _T("FALSE;");
	}
	else if (csName == _T("CL"))
	{
		csValue.Format("%02d",iCheckLevel);
		*csNewInit += (_T("[CL]:")+ csValue+";");

	}
	else
		*csNewInit += csData;

	*csNewInit += _T("\n");

	return TRUE;
}




void CRunLogDlg::OnPathSettingButton()
{
	// TODO: 在此添加命令处理程序代码
	CPathDlg* DlgTemp;

	DlgTemp = new CPathDlg;

	DlgTemp->Create(IDD_PathDlg,this);
	//SetLogPath(csTestLogPath);
	DlgTemp->SetLogPath(csTestLogPath);
	if (DlgTemp->ShowWindow(SW_SHOW) != 0)
		 DlgTemp->ShowWindow(SW_HIDE);


}

void CRunLogDlg::SetLogPath(CString csPath)
{
	csTestLogPath = csPath;
}
