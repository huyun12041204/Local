// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.cpp: CMainFrame 类的实现
//

#include "stdafx.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)

	ON_COMMAND_RANGE(ID_VIEW_FILELIST, ID_VIEW_PROPERTY, &CMainFrame::OnViewShow)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_FILELIST, ID_VIEW_PROPERTY, &CMainFrame::OnUpdateViewShow)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_COMPILE_COMPILE, &CMainFrame::OnCompileCompile)
	ON_COMMAND_RANGE(ID_COMPILE_REBUILT_AC, ID_COMPILE_REBUILT_ADDRESS, &CMainFrame::OnCheckSetting)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COMPILE_REBUILT_AC, ID_COMPILE_REBUILT_ADDRESS, &CMainFrame::OnUpdateCheckSetting)

	ON_COMMAND_RANGE(ID_MODIFY_FIXEDAC, ID_MODIFY_FIXEDARR, &CMainFrame::OnCheckSetting)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MODIFY_FIXEDAC, ID_MODIFY_FIXEDARR, &CMainFrame::OnUpdateCheckSetting)

	ON_COMMAND_RANGE(ID_CHECK_ADDRESS, ID_CHECK_CHARACTERISTIC, &CMainFrame::OnCheckSetting)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CHECK_ADDRESS, ID_CHECK_CHARACTERISTIC, &CMainFrame::OnUpdateCheckSetting)

	ON_COMMAND_RANGE(ID_CHECK_DIR, ID_CHECK_PRL, &CMainFrame::OnCheckSetting)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CHECK_DIR, ID_CHECK_PRL, &CMainFrame::OnUpdateCheckSetting)

	ON_COMMAND_RANGE(ID_CHECK_19001F01, ID_CHECK_19001F11, &CMainFrame::OnCheckSetting)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CHECK_19001F01, ID_CHECK_19001F11, &CMainFrame::OnUpdateCheckSetting)

	ON_COMMAND_RANGE(ID_CHECK_0002, ID_CHECK_0040, &CMainFrame::OnCheckSetting)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CHECK_0002, ID_CHECK_0040, &CMainFrame::OnUpdateCheckSetting)

	ON_COMMAND(ID_CHECK_CHECK, &CMainFrame::OnCheckCheck)
	ON_COMMAND(ID_DELETE_FILE, &CMainFrame::OnDeleteFile)
	ON_COMMAND(ID_MODULE_SAVE, &CMainFrame::OnModuleSave)

	ON_COMMAND_RANGE(ID_MODULE_SAVE1, ID_MODULE_SAVE10, &CMainFrame::_OnModuleSave)
	ON_COMMAND(ID_MODULE_FETCH, &CMainFrame::OnModuleFetch)
	ON_COMMAND_RANGE(ID_MODULE_FETCH1, ID_MODULE_FETCH10, &CMainFrame::_OnModuleFetch)
	ON_UPDATE_COMMAND_UI(ID_MODULE_FETCH, &CMainFrame::OnUpdateModuleFetch)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MODULE_FETCH1, ID_MODULE_FETCH10, &CMainFrame::_OnUpdateModuleFetch)


	ON_COMMAND(ID_MODIFY_COMFIRM, &CMainFrame::OnModifyComfirm)
	ON_COMMAND(ID_NEW_PROJECT, &CMainFrame::OnNewProject)


	ON_COMMAND_RANGE(ID_NEW_EF,ID_NEW_MF, &CMainFrame::OnNew)

	//ON_COMMAND(ID_NEW_EF, &CMainFrame::OnNewEf)
	//ON_COMMAND(ID_NEW_DF, &CMainFrame::OnNewDf)
	ON_COMMAND(ID_NEW_MODULE, &CMainFrame::OnNewModule)
	ON_COMMAND(ID_FIND_FIND, &CMainFrame::OnFindFind)
	ON_COMMAND(ID_FIND_REPEAT, &CMainFrame::OnFindRepeat)
	ON_COMMAND(ID_IMPORT_FILES, &CMainFrame::OnImportFiles)
	ON_COMMAND(ID_EXPORT_FILES, &CMainFrame::OnExportFiles)

	ON_COMMAND_RANGE(ID_COMPILE_BUILT, ID_COMPILE_BUILT_ADPU, &CMainFrame::OnCompileBuilt)
	ON_UPDATE_COMMAND_UI(ID_MODULE_SAVE, &CMainFrame::OnUpdateModuleSave)
	ON_WM_TIMER()
	ON_COMMAND(ID_CLOST_ALL, &CMainFrame::OnClostAll)
	ON_COMMAND(ID_CLOSE_CURRENT, &CMainFrame::OnCloseCurrent)
	ON_COMMAND(ID_CLOSE_OTHER, &CMainFrame::OnCloseOther)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WINDOWS_7);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	uiChildViewWide = 300;


	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!CreateStatueBar())
	{
		return -1;
	}


	//m_wndStatusBar.RedrawWindow();

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndFilesListView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFilesTreeView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFilesListView);
	CDockablePane* pTabbedBar = nullptr;
	m_wndFilesTreeView.AttachToTabWnd(&m_wndFilesListView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndReference.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndReference);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
	// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
	ModifyStyle(0, FWS_PREFIXTITLE);

	m_wndFind.Create(IDD_FINDBOX);
	m_wndFind.ShowWindow(SW_HIDE);
	
	SetTimer(DEF_Event_Process, 100, NULL);
	
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	InitParameter();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 创建类视图
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	strClassView = _T("文件结构");
	if (!m_wndFilesTreeView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, 
		ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“类视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建文件视图
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFilesListView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, 
		ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建输出窗口
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}


	// 创建参考窗口
	CString strReferenceWnd;
	bNameValid = strReferenceWnd.LoadString(IDS_REFERENCE_WND);
	ASSERT(bNameValid);
	if (!m_wndReference.Create(strReferenceWnd, this, CRect(0, 0, 200, 200), TRUE, 
		ID_VIEW_REFERENCEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“参考”窗口\n");
		return FALSE; // 未能创建
	}



	


	// 创建属性窗口
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), 
		TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“属性”窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_FILELIST_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFilesListView.SetIcon(hFileViewIcon, FALSE);

//	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_STATUEBAR_PROCESS), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
//	m_wndFilesListView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_FILETREE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFilesTreeView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_OUTPUT_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hReferenceBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_REFERENCE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndReference.SetIcon(hReferenceBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_PROPERTIES_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

BOOL CMainFrame::CreateStatueBar()
{
	BOOL bNameValid;
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return FALSE;      // 未能创建
	}



	CString strTitlePane1;
	CString strTitlePane2;
	CString strTitlePane3;
	CString strTitlePane4;

	bNameValid = strTitlePane1.LoadString(IDS_STATUS_WORK);
	ASSERT(bNameValid);

	bNameValid = strTitlePane2.LoadString(IDS_STATUS_SUBWORK);
	ASSERT(bNameValid);

	bNameValid = strTitlePane3.LoadString(IDS_STATUS_RESERVE);
	ASSERT(bNameValid);

	bNameValid = strTitlePane4.LoadString(IDS_STATUS_FILE);
	ASSERT(bNameValid);

	HICON hIcon = (HICON) ::LoadImage(AfxGetResourceHandle(),
		MAKEINTRESOURCE(IDI_STATUEBAR_PROCESS),
		IMAGE_ICON,
		::GetSystemMetrics(SM_CXSMICON), 
		::GetSystemMetrics(SM_CYSMICON), 
		LR_DEFAULTCOLOR);
	m_wndStatusBar.SetIcon(hIcon, FALSE);

	m_wndStatusBar.AddElement(
		new CMFCRibbonStatusBarPane(ID_STATUSBAR_WORK,
			_T(""),
			IDB_STATUEBAR_PROCESS,
			16,
			RGB(255, 255, 255),
			hIcon,TRUE, TRUE),
		_T("进程运行中"),
		TRUE);

	m_wndStatusBar.AddElement(
		new CMFCRibbonStatusBarPane(ID_STATUSBAR_SUBWORK,strTitlePane1, 
			FALSE,NULL, RepeatCString(_T(" "), 20)),
		_T("当前状态"),TRUE);


	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_RESERVE, strTitlePane3, TRUE), strTitlePane3);

	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_FILE, strTitlePane4, FALSE, NULL,
		RepeatCString(_T("  "), 100)), strTitlePane4);

	m_ElementWork = (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetElement(0);

	m_ElementSubWork = (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetElement(1);

	m_ElementReserve = (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetExElement(0);

	m_ElementFile    = (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetExElement(1);


	m_wndStatusBar.AddSeparator();

	return TRUE;

}

void CMainFrame::SetStatueBarWindowIcons(BOOL bHiColorIcons)
{

	//HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndFilesListView.SetIcon(hFileViewIcon, FALSE);

	//m_ElementWork->SetIcon(hFileViewIcon, FALSE);

	//m_wndStatusBar.SetIcon(hFileViewIcon,FALSE);
}

void CMainFrame::LoadSetting(int iID)
{

	CString csPath = _T("Settings");
	int iSetting = FALSE;

	switch (iID)
	{
	case ID_MODIFY_FIXEDARR: csPath = _T("Settings\\Modify"); break;
	case ID_MODIFY_FIXEDAC:  csPath = _T("Settings\\Modify"); break;
	case ID_COMPILE_REBUILT_ARR: csPath = _T("Settings\\Compile"); break;
	case ID_COMPILE_REBUILT_AC: csPath = _T("Settings\\Compile"); break;
	case ID_COMPILE_REBUILT_ADDRESS:  csPath = _T("Settings\\Compile"); break;
	case ID_COMPILE_REBUILT_OTA: csPath = _T("Settings\\Compile"); break;


	case ID_CHECK_ADDRESS: csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_HADLE: csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_STRUCTURE:  csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_SFI: csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_LINKS: csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_CHARACTERISTIC:csPath = _T("Settings\\Check\\Normal"); break;

	case ID_CHECK_ARR:  csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_DIR: csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_PBR: csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_SST:csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_UST: csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_CST:csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_PRL: csPath = _T("Settings\\Check\\Public"); break;


	case ID_CHECK_19001F01:csPath = _T("Settings\\Check\\STK"); break;
	case ID_CHECK_19001F04: csPath = _T("Settings\\Check\\STK"); break;
	case ID_CHECK_19001F10:csPath = _T("Settings\\Check\\STK"); break;
	case ID_CHECK_19001F11: csPath = _T("Settings\\Check\\STK"); break;

	case ID_CHECK_0002: csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_0018: csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_0019: csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_001A:csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_0022:csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_0040: csPath = _T("Settings\\Check\\Protected"); break;
	}


	iSetting = theApp.GetProfileInt(csPath, Int2CStringDecimal(iID), TRUE);


	switch (iID)
	{
	case ID_MODIFY_FIXEDARR: b_mFixedARR = iSetting; break;
	case ID_MODIFY_FIXEDAC:  b_mFixedAC  = iSetting; break;
	case ID_COMPILE_REBUILT_ARR: b_bRebuiltARR = iSetting; break;
	case ID_COMPILE_REBUILT_AC: b_bRebuiltAC = iSetting; break;
	case ID_COMPILE_REBUILT_ADDRESS: b_bRebuiltAddress = iSetting; break;
	case ID_COMPILE_REBUILT_OTA: b_bRebuiltOTA = iSetting; break;


	case ID_CHECK_ADDRESS: b_cAddress = iSetting; break;
	case ID_CHECK_HADLE:b_cHandle = iSetting; break;
	case ID_CHECK_STRUCTURE:b_cStructure = iSetting; break;
	case ID_CHECK_SFI:b_cSFI = iSetting; break;
	case ID_CHECK_LINKS:b_cLinks = iSetting; break;
	case ID_CHECK_CHARACTERISTIC:b_cCharacteristic = iSetting; break;

	case ID_CHECK_ARR: b_cARR = iSetting; break;
	case ID_CHECK_DIR :b_cDIR = iSetting; break;
	case ID_CHECK_PBR:b_cPBR = iSetting; break;
	case ID_CHECK_SST:b_cSST = iSetting; break;
	case ID_CHECK_UST:b_cUST = iSetting; break;
	case ID_CHECK_CST:b_cCST = iSetting; break;
	case ID_CHECK_PRL:b_cPRL = iSetting; break;


	case ID_CHECK_19001F01:b_c19001F01 = iSetting; break;
	case ID_CHECK_19001F04:b_c19001F04 = iSetting; break;
	case ID_CHECK_19001F10:b_c19001F10 = iSetting; break;
	case ID_CHECK_19001F11:b_c19001F11 = iSetting; break;

	case ID_CHECK_0002:b_c0002 = iSetting; break;
	case ID_CHECK_0018:b_c0018 = iSetting; break;
	case ID_CHECK_0019:b_c0019 = iSetting; break;
	case ID_CHECK_001A:b_c001A = iSetting; break;
	case ID_CHECK_0022:b_c0022 = iSetting; break;
	case ID_CHECK_0040:b_c0040 = iSetting; break;
	}



}

void CMainFrame::SaveSetting(int iID)
{
	int iSetting = TRUE;
	CString csPath;
	switch (iID)
	{
	case ID_MODIFY_FIXEDARR: iSetting = b_mFixedARR ;csPath = _T("Settings\\Modify"); break;
	case ID_MODIFY_FIXEDAC:  iSetting = b_mFixedAC ; csPath = _T("Settings\\Modify"); break;

	case ID_COMPILE_REBUILT_ARR:   iSetting = b_bRebuiltARR; csPath = _T("Settings\\Compile"); break;
	case ID_COMPILE_REBUILT_AC:   iSetting = b_bRebuiltAC; csPath = _T("Settings\\Compile"); break;
	case ID_COMPILE_REBUILT_ADDRESS:  iSetting = b_bRebuiltAddress; csPath = _T("Settings\\Compile"); break;
	case ID_COMPILE_REBUILT_OTA:  iSetting = b_bRebuiltOTA; csPath = _T("Settings\\Compile"); break;


	case ID_CHECK_ADDRESS:  iSetting = b_cAddress; csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_HADLE: iSetting = b_cHandle; csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_STRUCTURE:  iSetting = b_cStructure; csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_SFI: iSetting = b_cSFI; csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_LINKS: iSetting = b_cLinks; csPath = _T("Settings\\Check\\Normal"); break;
	case ID_CHECK_CHARACTERISTIC: iSetting = b_cCharacteristic; csPath = _T("Settings\\Check\\Normal"); break;

	case ID_CHECK_DIR:  iSetting = b_cDIR; csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_ARR:  iSetting = b_cARR; csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_PBR:  iSetting = b_cPBR; csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_SST:  iSetting = b_cSST; csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_UST:  iSetting = b_cUST; csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_CST:  iSetting = b_cCST; csPath = _T("Settings\\Check\\Public"); break;
	case ID_CHECK_PRL:  iSetting = b_cPRL; csPath = _T("Settings\\Check\\Public"); break;


	case ID_CHECK_19001F01:iSetting = b_c19001F01; csPath = _T("Settings\\Check\\STK"); break;
	case ID_CHECK_19001F04:iSetting = b_c19001F04; csPath = _T("Settings\\Check\\STK"); break;
	case ID_CHECK_19001F10:iSetting = b_c19001F10; csPath = _T("Settings\\Check\\STK"); break;
	case ID_CHECK_19001F11:iSetting = b_c19001F11; csPath = _T("Settings\\Check\\STK"); break;

	case ID_CHECK_0002: iSetting = b_c0002; csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_0018: iSetting = b_c0018; csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_0019: iSetting = b_c0019; csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_001A: iSetting = b_c001A; csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_0022: iSetting = b_c0022; csPath = _T("Settings\\Check\\Protected"); break;
	case ID_CHECK_0040: iSetting = b_c0040; csPath = _T("Settings\\Check\\Protected"); break;
	default:return;
	}

	

	theApp.WriteProfileInt(csPath, Int2CStringDecimal(iID), iSetting);

}

void CMainFrame::OpenReference(CString csPath)
{

	//如果没有安装adobe插件 ，此处会无法打开
	if (!m_wndReference.IsValid())
		return;

	CString csRoute, csFID;




	csRoute = csPath.Mid(0, csPath.GetLength() - 4);
	csFID   = csPath.Mid(csRoute.GetLength());
	csRoute.MakeUpper();
	if ((csRoute.GetLength()>4)&&
		(csRoute.Mid(0,4) == _T("3F00")))
	{
		csRoute = csRoute.Mid(4);
	}

	m_wndReference.OpenReference(csRoute, csFID);
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

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

	m_wndOutput.UpdateFonts();
	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnViewShow(UINT iID)
{
	CDockablePane* m_wnd;
	switch (iID)
	{
	case ID_VIEW_FILELIST:b_vFileList   = !b_vFileList; m_wnd  = &m_wndFilesListView ;   break;
	case ID_VIEW_FILETREE :b_vFileTree  = !b_vFileTree; m_wnd  = &m_wndFilesTreeView; break;
	case ID_VIEW_OUTPUT   :b_vOutput    = !b_vOutput;   m_wnd  = &m_wndOutput; break;
	case ID_VIEW_REFERENCE:b_vReference = !b_vReference;m_wnd = &m_wndReference; break;
	case ID_VIEW_PROPERTY :b_vProperty  = !b_vProperty; m_wnd  = &m_wndProperties; break;
	default:break;
	}

	if (m_wnd->IsVisible())
	{
		m_wnd->ShowPane(FALSE, FALSE, TRUE);
	}
	else
	{
		m_wnd->SetAutoHideMode(FALSE, CBRS_ALIGN_ANY);
		m_wnd->ShowPane(TRUE, FALSE, TRUE);
	}



	//if (m_wndOutput.IsVisible())
	//	m_wndOutput.ShowPane(FALSE, FALSE, TRUE);
	//else
	//{
	//	m_wndOutput.SetAutoHideMode(FALSE, CBRS_ALIGN_ANY);
	//	m_wndOutput.ShowPane(TRUE, FALSE, TRUE);
	//}

	

}
void CMainFrame::OnUpdateViewShow(CCmdUI* pCmdUI)
{

	switch ( pCmdUI->m_nID)
	{
	case ID_VIEW_FILELIST :pCmdUI->SetCheck(m_wndFilesListView.IsVisible());break;
	case ID_VIEW_FILETREE :pCmdUI->SetCheck(m_wndFilesTreeView.IsVisible());break;
	case ID_VIEW_OUTPUT   :pCmdUI->SetCheck(m_wndOutput.IsVisible()       );break;
	case ID_VIEW_REFERENCE:pCmdUI->SetCheck(m_wndReference.IsVisible()    );break;
	case ID_VIEW_PROPERTY :pCmdUI->SetCheck(m_wndProperties.IsVisible()   );break;
	default:break;
	}


}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

int CMainFrame::InsertProfile(BOOL bCurrent /*= TRUE*/)
{
	int iRet = TRUE;
	ProfileClass* Profile = &(theApp.pcFile);
	PCFS SubFile;


	m_wndFilesListView.RemoveAllItem();

	m_wndFilesTreeView.DeleteAllItem();

	
	long lSubFile = Profile->GetDocRecordCount();
	long lFlashContent = 0;

	for (long i = 1; i <= lSubFile; i++)
	{

		SubFile.EmptyFile();

		if (Profile->GetSimpleFileFromDocFast(i==1, &SubFile)!=DEF_PROFILE_SUCCESS)
		{
			iRet = FALSE;
			if( MessageBox(_T("文件打开失败，是否继续"),_T("WARNNING"), MB_YESNO) == IDYES)
				continue;
			else
				break;	
		}

		if (!SubFile.IsEmpty())
		{

			m_wndFilesListView.InsertFile(SubFile);

			m_wndFilesTreeView.InsertFile(SubFile);

		}

		if ((SubFile.uiTYPE == DEF_FILETYPE_EF)&&
			(!SubFile.IsLinkFile()))
		{
			lFlashContent += SubFile.uiSIZE;
		}


	}

	//此处需要重新绘制控件TREE

	m_wndFilesTreeView.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE|RDW_ALLCHILDREN);


	if (bCurrent)
	{
		InsertFRU(lSubFile, lFlashContent);

		CString  strTitlePane;
		if (iRet)
		{
			_Display2StatusBar(Profile->GetProfilePath(), DEF_STATUEBAR_FILE);
			ASSERT(strTitlePane.LoadString(IDS_STATUS_WORK));
			_Display2StatusBar(strTitlePane);

		}
		else
		{

			ASSERT(strTitlePane.LoadString(IDS_STATUS_FILE));
			_Display2StatusBar(strTitlePane, DEF_STATUEBAR_FILE);
		}

	}
		


	//;
	
		

	return iRet;
}

int CMainFrame::InsertFRU(long lSubFile,long lFlashContent)
{
	ProfileClass* Profile = &(theApp.pcFile);
	_variant_t __Data;
	int iReserveFile = 0;
	for (int i = 0; i < 9; i++)
	{
		__Data.Clear();
		if (Profile->GetFRUData(i, __Data))
		{
			m_wndProperties.SetKey(i, __Data);
		}
		if (i == 7)
		{
			iReserveFile = _variant_t2Int(__Data);
		}

		if (i == 8)
		{
			lFlashContent += _variant_t2Int(__Data);
		}


	}

	m_wndProperties.SetKey(9, (_variant_t)(iReserveFile + lSubFile));

	m_wndProperties.SetKey(10, (_variant_t)((iReserveFile + lSubFile) * 18 + lFlashContent));

	return TRUE;

}

int CMainFrame::InitParameter()
{

	 b_mFixedARR = TRUE;
	 b_mFixedAC = TRUE;

	 b_bRebuiltARR = TRUE;
	 b_bRebuiltAC = TRUE;
	 b_bRebuiltOTA = TRUE;
	 b_bRebuiltAddress = TRUE;


	 b_cAddress = TRUE;
	 b_cHandle = TRUE;
	 b_cStructure = TRUE;
	 b_cSFI = TRUE;
	 b_cLinks = TRUE;
	 b_cCharacteristic = TRUE;

	 b_cDIR = TRUE;
	 b_cARR = TRUE;
	 b_cPBR = TRUE;
	 b_cSST = TRUE;
	 b_cUST = TRUE;
	 b_cCST = TRUE;
	 b_cPRL = TRUE;

	 b_c19001F01 = TRUE;
	 b_c19001F04 = TRUE;
	 b_c19001F10 = TRUE;
	 b_c19001F11 = TRUE;

	 b_c0002 = TRUE;
	 b_c0018 = TRUE;
	 b_c0019 = TRUE;
	 b_c001A = TRUE;
	 b_c0022 = TRUE;
	 b_c0040 = TRUE;
	 return TRUE;
}

void CMainFrame::OnNewProject()
{
	// TODO: 在此添加命令处理程序代码
}

void CMainFrame::OnNew(UINT iID)
{


	((CProfileExplorerApp*)AfxGetApp())->OnFileNew();

	PCFS __newef;

	__newef.EmptyFile();

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	switch (iID)
	{
	case ID_NEW_EF:    __newef.CreateTestData(pcFile->GetDocRecordCount() + 1); break;
	case ID_NEW_BINARY:__newef.CreateBinaryFile(pcFile->GetDocRecordCount() + 1); break;
	case ID_NEW_LINFIX:__newef.CreateLinfixFile(pcFile->GetDocRecordCount() + 1); break;
	case ID_NEW_CYCLIC:__newef.CreateCyclicFile(pcFile->GetDocRecordCount() + 1); break;
	case ID_NEW_INTER: __newef.CreateTestData(pcFile->GetDocRecordCount() + 1);  break;
	case ID_NEW_ADF:   __newef.CreateADF(pcFile->GetDocRecordCount() + 1); break;
	case ID_NEW_DF:    __newef.CreateDF(pcFile->GetDocRecordCount() + 1); break;
	case ID_NEW_MF:    __newef.CreateMF(pcFile->GetDocRecordCount() + 1); break;
	default:	break;
	}
	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if (SubFrame->SetFileToView(__newef))
	{
		
		((CProfileExplorerDoc*)SubFrame->GetActiveDocument())->SetPathName(GenerateCardFilesAppPath(pcFile->GetProfilePath(), __newef.stROUTE + Int2CString(__newef.uiFID)), FALSE);
		((CProfileExplorerDoc*)SubFrame->GetActiveDocument())->SetModifiedFlag(TRUE);

	}

	


}

void CMainFrame::OnNewEf()
{
	PCFS __newef;
	__newef.EmptyFile();

	((CProfileExplorerApp*)AfxGetApp())->OnFileNew();

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	__newef.CreateTestData(pcFile->GetDocRecordCount() + 1);

	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if ((SubFrame == NULL) ||
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))
	{
		MessageBox(_T("没有打开任何文件！"));
		return;
	}

	SubFrame->SetFileToView(__newef);
}

void CMainFrame::OnNewDf()
{
	PCFS __newFile;
	__newFile.EmptyFile();

	((CProfileExplorerApp*)AfxGetApp())->OnFileNew();

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	__newFile.CreateDF(pcFile->GetDocRecordCount() + 1);

	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if ((SubFrame == NULL) ||
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))
	{
		MessageBox(_T("没有打开任何文件！"));
		return;
	}

	SubFrame->SetFileToView(__newFile);
}

void CMainFrame::OnNewModule()
{
	// TODO: 在此添加命令处理程序代码
}

void CMainFrame::OnModuleSave()
{

	BOOL bRet = FALSE;

	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if ((SubFrame == NULL)||
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))

	{
		MessageBox(_T("没有打开任何文件！"));
		return;
	}

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	PCFS __File;

	SubFrame->GetFileFromView(&__File);

	int iCount = pcFile->GetModuleRecordCount();

	if (iCount < 10)
	{
		int iNum = pcFile->GetModuleValidRecordCount();
		if (iNum != iCount)
			pcFile->DeleteModuleInvalidRecord();

		__File.uiHANDLE = iNum + 1;
		if (pcFile->AddNewFileToModule(__File) < 0)
		{
			MessageBox(_T("保存失败!"));
			return;
		}
			
	}
	else
	{
	
		if (MessageBox(_T("超出保存限制,是否将保存到位置1"), _T("Warinning"), MB_YESNO) == IDYES)
		{
			if (pcFile->SetFileToModule(1, __File)<0)
			{
				MessageBox(_T("保存失败!"));
				return;
			}
		
		}
		else
			return;
		

	}

	MessageBox(_T("保存成功!"));
}

void CMainFrame::_OnModuleSave(UINT iID)
{
	BOOL bRet = FALSE;

	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if ((SubFrame == NULL) || 
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))

	{
		MessageBox(_T("没有打开任何文件！"));
		return;
	}

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	PCFS __File;

	if (!SubFrame->GetFileFromView(&__File))
	{
		MessageBox(_T("获取文件内容失败！"));
		return;
	}

	;

	__File.uiHANDLE = (iID%10 +1);


	if (pcFile->SetFileToModule((iID % 10 + 1), __File)<0)
	{
		MessageBox(_T("保存失败！"));
	}
	else
		MessageBox(_T("保存成功！"));


	CMFCRibbonButton * pButton = DYNAMIC_DOWNCAST(
		CMFCRibbonButton, m_wndRibbonBar.FindByID(ID_MODULE_SAVE));

	pButton->RemoveAllSubItems();

}

void CMainFrame::OnUpdateModuleSave(CCmdUI *pCmdUI)
{
	CMFCRibbonButton * pButton = DYNAMIC_DOWNCAST(
		CMFCRibbonButton, m_wndRibbonBar.FindByID(ID_MODULE_SAVE));

	if (pButton == NULL)
		return;

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	if (!pcFile->IsValid())
		return;


	long lCount = pcFile->GetModuleValidRecordCount();

	int iCout = (pButton->GetSubItems()).GetCount();

	if (iCout == lCount)
		return;

	pButton->RemoveAllSubItems();
	PCFS __File;
	for (long i = 0; i < lCount; i++)
	{

		__File.EmptyFile();
		if (((CProfileExplorerApp *)AfxGetApp())->pcFile.GetFileFromModule(i + 1, &__File) >= 0)
		{
			CString csText = Int2CString(__File.uiFID, 4) + _T("(") + __File.stFILENAME + _T(")");
			pButton->AddSubItem(new CMFCRibbonButton(ID_MODULE_SAVE1 + i, csText));
		}
	}



}


void CMainFrame::OnModuleFetch()
{
	BOOL bRet = FALSE;

	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if ((SubFrame == NULL) ||
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))
	{
		MessageBox(_T("没有打开任何文件！"));
		return;
	}

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	PCFS __File;

	SubFrame->GetFileFromView(&__File);

	int iRecord = __File.uiHANDLE;

	__File.EmptyFile();

	int iNum = pcFile->GetModuleValidRecordCount();

	if (iNum < 1)
	{
		MessageBox(_T("没有找到保存的模板!"));
		return;
	}

	int iRet = pcFile->GetFileFromModule(1, &__File);

	__File.uiHANDLE = iRecord;

	SubFrame->SetFileToView(__File);
}

void CMainFrame::_OnModuleFetch(UINT iID)
{
	BOOL bRet = FALSE;

	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if ((SubFrame == NULL)||
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))
	{
		MessageBox(_T("没有打开任何文件！"));
		return;
	}

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	PCFS __File;

	SubFrame->GetFileFromView(&__File);

	int iRecord = __File.uiHANDLE;
	__File.EmptyFile();

	int iRet = pcFile->GetFileFromModule(iID%10 +1, &__File);

	if (iRet < 0)
	{
		MessageBox(_T("读取模板失败!"));
		return;
	}
	__File.uiHANDLE = iRecord;
	SubFrame->SetFileToView(__File);
}

void CMainFrame::OnUpdateModuleFetch(CCmdUI *pCmdUI)
{
	CMFCRibbonButton* pButton = DYNAMIC_DOWNCAST(
		CMFCRibbonButton, m_wndRibbonBar.FindByID(ID_MODULE_FETCH));


	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	if (!pcFile->IsValid())
		return;

	long lCount = pcFile->GetModuleValidRecordCount();

	int iCout = (pButton->GetSubItems()).GetCount();

	if (iCout == lCount)
		return;

	pButton->RemoveAllSubItems();

	PCFS __File;
	for (long i = 0; i < lCount; i++)
	{
		__File.EmptyFile();
		if (((CProfileExplorerApp *)AfxGetApp())->pcFile.GetFileFromModule(i + 1, &__File, FALSE) > 0)
		{
			CString csText = Int2CString(__File.uiFID, 4) + _T("(") + __File.stFILENAME + _T(")");

			CMFCRibbonButton* NewButton = new CMFCRibbonButton(ID_MODULE_FETCH1 + i, csText);

			pButton->AddSubItem(NewButton);
		}
	}


}

void CMainFrame::_OnUpdateModuleFetch(CCmdUI *pCmdUI)
{



}
void CMainFrame::OnDeleteFile()
{
	// TODO: 在此添加命令处理程序代码
	BOOL bRet = FALSE;

	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if ((SubFrame == NULL)||
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))
	{
		MessageBox(_T("没有打开文件！"));
		return;
	}


	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;
	CString csFile = SubFrame->pcfsOriFile.stROUTE + Int2CString(SubFrame->pcfsOriFile.uiFID, 4);
	if (MessageBox(_T("是否确定删除 文件 ")+ csFile+_T("?"),_T("Warnning"),MB_YESNO)!=IDYES)
		return;



	long lRet = SubFrame->pcfsOriFile.uiHANDLE;
	if (lRet > 0)
	{
		bRet = pcFile->DeleteDocRecord(lRet);
	}

	if (!bRet)
	{

		MessageBox(_T("删除失败！"));
	}
	else
	{
		SubFrame->DestroyWindow();
		MessageBox(_T("删除成功！"));
	}
	NewInsertProfileThread();

}


void CMainFrame::OnFindFind()
{
	m_wndFind.ShowWindow(TRUE);



}


void CMainFrame::OnFindRepeat()
{
	// TODO: 在此添加命令处理程序代码
}

void CMainFrame::OnImportFiles()
{
	// TODO: 在此添加命令处理程序代码

	CString csFileName;
	CString csFileData;
	CFileDialog CFDataFile(TRUE, _T("(*.MDB)|*.MDB"), _T(""), 4 | 2, _T("(*.MDB)|*.MDB||"));
	CString csTemp;

	if (CFDataFile.DoModal() == IDOK)
	{
		csFileName = CFDataFile.GetPathName();

		ProfileClass _ImportFiles;
		ProfileClass* _ProFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

		if (_ImportFiles.LoadFile(csFileName) <= 0)
		{
			MessageBox(_T("文件导入失败，未能正常打开文件！"));
			return;
		}

		long lCount = _ImportFiles.GetDocRecordCount();

		if (lCount <= 0)
		{
			MessageBox(_T("文件为空！"));
			_ImportFiles.DisconnectProfile();
			return;
		}

		PCFS _ImportFile;

		for (long i = 1 ; i <= lCount ; i++)
		{
			_ImportFile.EmptyFile();
			if (_ImportFiles.GetFileFromDoc(i, &_ImportFile,FALSE)<0)
				continue;

			_ImportFile.uiHANDLE = (UINT)_ProFile->GetDocRecordCount() + 1;

			CString csFilePath = _ImportFile.stROUTE + Int2CString(_ImportFile.uiFID,4);

			if (_ProFile->GetFileRecord(csFilePath)> 0)
			{
				//MessageBox(csFilePath+_T("已经存在此处跳过"));
				continue;
			}

			if (_ProFile->AddNewFileToDoc(_ImportFile)<DEF_PROFILE_SUCCESS)
			{
				MessageBox(csFilePath + _T("安装失败！"));
				continue;

			}

		}
		MessageBox(_T("导入完成"));
		_ImportFiles.DisconnectProfile();
		//InsertProfile();
		NewInsertProfileThread();
		return;

	}
}

BOOL CMainFrame::__ExportFiles(const CStringArray& csPath, CString csProfile)
{

	ProfileClass* Profile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;
	ProfileClass __ExportFile;
	PCFS __File;

	if (!CopyFile(_T("template\\template.mdb"), csProfile, TRUE))
	{
		CString Path;
		__GetCurrentDIR(Path);
		if (!CopyFile(Path + _T("\\template\\template.mdb"), csProfile, TRUE))
		{
			MessageBox(_T("创建文件失败!"));
			return FALSE;
		}

	}

	if (__ExportFile.LoadFile(csProfile) < 1)
	{
		MessageBox(_T("打开文件失败!"));
		return FALSE;
	}


	for (int i = 0; i < csPath.GetCount(); i++)
	{
		__File.EmptyFile();

		if (Profile->GetFileFromDoc(csPath.GetAt(i), &__File) < DEF_PROFILE_SUCCESS)
			continue;


		if (__ExportFile.AddNewFileToDoc(__File) < DEF_PROFILE_SUCCESS)
		{
			_Display2Output(_T("导出存在错误！"), DEF_ERROR_TAG);
			return FALSE;
		}
	}
	__ExportFile.DisconnectProfile();

	MessageBox("写入导出文件完成!");

	return TRUE;
}

void CMainFrame::_ExportFiles( const CStringArray& csPath)
{

	CString csFileName;
	CString csFileData;
	CStdioFile CSFFile;
	CFileDialog CFDataFile(FALSE, _T("(*.MDB)|*.MDB"), _T("Profile"), 4 | 2, _T("(*.MDB)|*.MDB||"));
	if (CFDataFile.DoModal() == IDOK)
	{
		csFileName = CFDataFile.GetPathName();
	

		__ExportFiles(csPath, csFileName);
		
	}

	return;


}

void CMainFrame::OnExportFiles()
{

	/*CString csFileName;
	CString csFileData;
	CStdioFile CSFFile;
	CFileDialog CFDataFile(FALSE, _T("(*.MDB)|*.MDB"), _T("Profile"), 4 | 2, _T("(*.MDB)|*.MDB||"));


	if (CFDataFile.DoModal() == IDOK)
	{
		csFileName = CFDataFile.GetPathName();
		if (!CopyFile( _T("template\\template.mdb"), csFileName, TRUE))
		{
			CString Path;
			__GetCurrentDIR(Path);
			if (!CopyFile(Path+_T("\\template\\template.mdb"), csFileName, TRUE))
			{
				MessageBox(_T("创建文件失败!"));
				return;
			}

		}

		ProfileClass __ExportFile;
		ProfileClass pcFile = ((CProfileExplorerApp*)AfxGetApp())->pcFile;
		if (__ExportFile.LoadFile(csFileName)<1)
		{
			MessageBox(_T("打开文件失败!"));
			return;
		}
		CStringArray csPathArray;
		PCFS __File;

		int _SelCount = m_wndFilesListView.GetSelectItemPath(csPathArray);



		for (int i= 0 ; i < _SelCount; i++)
		{
			__File.EmptyFile();

			if (pcFile.GetFileFromDoc(csPathArray.GetAt(i), &__File)< DEF_PROFILE_SUCCESS)
				continue;


			if (__ExportFile.AddNewFileToDoc(__File) < DEF_PROFILE_SUCCESS)
			{
				MessageBox("写入导出文件失败!");
				continue;
			}
		}
		__ExportFile.DisconnectProfile();

		MessageBox("写入导出文件完成!");
	}

	return;*/

	CStringArray csPathArray;
	if (m_wndFilesListView.GetSelectItemPath(csPathArray)>0)
	{
		_ExportFiles(csPathArray);
	}


}




void CMainFrame::OnModifyComfirm()
{

	CChildFrame* SubFrame = (CChildFrame*)GetActiveFrame();

	if ((SubFrame == NULL) ||
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))
	{
		MessageBox(_T("没有打开任何文件！"));
		return;
	}
	CDocument* SubDoc = SubFrame->GetActiveDocument();

	if ((SubFrame == NULL) ||
		(!SubFrame->IsKindOf(RUNTIME_CLASS(CChildFrame))))
	{
		MessageBox(_T("没有打开任何文件！"));
		return;
	}

	SubDoc->DoFileSave();
}

void CMainFrame::OnCheckSetting(UINT iID)
{
	switch (iID)
	{
	case ID_MODIFY_FIXEDAC:         b_mFixedAC = !b_mFixedAC; break;
	case ID_MODIFY_FIXEDARR:        b_mFixedARR = !b_mFixedARR; break;
	case ID_COMPILE_REBUILT_AC:     b_bRebuiltAC = !b_bRebuiltAC; break;
	case ID_COMPILE_REBUILT_ARR:    b_bRebuiltARR = !b_bRebuiltARR; break;
	case ID_COMPILE_REBUILT_OTA:    b_bRebuiltOTA = !b_bRebuiltOTA; break;
	case ID_COMPILE_REBUILT_ADDRESS:b_bRebuiltAddress = !b_bRebuiltAddress; break;
	case ID_CHECK_ADDRESS:          b_cAddress = !b_cAddress; break;
	case ID_CHECK_HADLE:            b_cHandle = !b_cHandle; break;
	case ID_CHECK_STRUCTURE:        b_cStructure = !b_cStructure; break;
	case ID_CHECK_SFI:              b_cSFI = !b_cSFI; break;
	case ID_CHECK_LINKS:            b_cLinks = !b_cLinks; break;
	case ID_CHECK_CHARACTERISTIC:   b_cCharacteristic = !b_cCharacteristic; break;
	case ID_CHECK_DIR:              b_cDIR = !b_cDIR; break;
	case ID_CHECK_ARR:              b_cARR = !b_cARR; break;
	case ID_CHECK_PBR:              b_cPBR = !b_cPBR; break;
	case ID_CHECK_SST:              b_cSST = !b_cSST; break;
	case ID_CHECK_UST:              b_cUST = !b_cUST; break;
	case ID_CHECK_CST:              b_cCST = !b_cCST; break;
	case ID_CHECK_PRL:              b_cPRL = !b_cPRL; break;
	case ID_CHECK_19001F01:         b_c19001F01 = !b_c19001F01; break;
	case ID_CHECK_19001F04:         b_c19001F04 = !b_c19001F04; break;
	case ID_CHECK_19001F10:         b_c19001F10 = !b_c19001F10; break;
	case ID_CHECK_19001F11:         b_c19001F11 = !b_c19001F11; break;
	case ID_CHECK_0002:             b_c0002 = !b_c0002; break;
	case ID_CHECK_0018:             b_c0018 = !b_c0018; break;
	case ID_CHECK_0019:             b_c0019 = !b_c0019; break;
	case ID_CHECK_001A:             b_c001A = !b_c001A; break;
	case ID_CHECK_0022:             b_c0022 = !b_c0022; break;
	case ID_CHECK_0040:             b_c0040 = !b_c0040; break;


	default:break;
	}

}

void CMainFrame::OnUpdateCheckSetting(CCmdUI* pCmdUI)
{

	switch (pCmdUI->m_nID)
	{
	case ID_MODIFY_FIXEDAC:          pCmdUI->SetCheck(b_mFixedAC); break;
	case ID_MODIFY_FIXEDARR:         pCmdUI->SetCheck(b_mFixedARR); break;
	case ID_COMPILE_REBUILT_AC:      pCmdUI->SetCheck(b_bRebuiltAC); break;
	case ID_COMPILE_REBUILT_ARR:     pCmdUI->SetCheck(b_bRebuiltARR); break;
	case ID_COMPILE_REBUILT_OTA:     pCmdUI->SetCheck(b_bRebuiltOTA); break;
	case ID_COMPILE_REBUILT_ADDRESS: pCmdUI->SetCheck(b_bRebuiltAddress); break;
	case ID_CHECK_ADDRESS:           pCmdUI->SetCheck(b_cAddress); break;
	case ID_CHECK_HADLE:             pCmdUI->SetCheck(b_cHandle); break;
	case ID_CHECK_STRUCTURE:         pCmdUI->SetCheck(b_cStructure); break;
	case ID_CHECK_SFI:               pCmdUI->SetCheck(b_cSFI); break;
	case ID_CHECK_LINKS:             pCmdUI->SetCheck(b_cLinks); break;
	case ID_CHECK_CHARACTERISTIC:    pCmdUI->SetCheck(b_cCharacteristic); break;
	case ID_CHECK_DIR:               pCmdUI->SetCheck(b_cDIR); break;
	case ID_CHECK_ARR:               pCmdUI->SetCheck(b_cARR); break;
	case ID_CHECK_PBR:               pCmdUI->SetCheck(b_cPBR); break;
	case ID_CHECK_SST:               pCmdUI->SetCheck(b_cSST); break;
	case ID_CHECK_UST:               pCmdUI->SetCheck(b_cUST); break;
	case ID_CHECK_CST:               pCmdUI->SetCheck(b_cCST); break;
	case ID_CHECK_PRL:               pCmdUI->SetCheck(b_cPRL); break;
	case ID_CHECK_19001F01:          pCmdUI->SetCheck(b_c19001F01); break;
	case ID_CHECK_19001F04:          pCmdUI->SetCheck(b_c19001F04); break;
	case ID_CHECK_19001F10:          pCmdUI->SetCheck(b_c19001F10); break;
	case ID_CHECK_19001F11:          pCmdUI->SetCheck(b_c19001F11); break;
	case ID_CHECK_0002:              pCmdUI->SetCheck(b_c0002); break;
	case ID_CHECK_0018:              pCmdUI->SetCheck(b_c0018); break;
	case ID_CHECK_0019:              pCmdUI->SetCheck(b_c0019); break;
	case ID_CHECK_001A:              pCmdUI->SetCheck(b_c001A); break;
	case ID_CHECK_0022:              pCmdUI->SetCheck(b_c0022); break;
	case ID_CHECK_0040:              pCmdUI->SetCheck(b_c0040); break;
	default:break;
	}


}

int CMainFrame::CompileCompile()
{
	int iResult  = 0; 
	int iRet;
	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	if (b_bRebuiltARR)
	{
		_Display2Output(_T("开始编译 3G 文件权限 (ARR)"), DEF_MESSAGE_HINT);
		 iRet = pcFile->ReInput3GACText();

		if (iRet == DEF_PROFILE_SUCCESS)
		{
			iRet = pcFile->ComputeProfileARR();
		}
		_Display2Output(_T("结束编译 3G 文件权限 (ARR)"), iRet!= DEF_PROFILE_SUCCESS? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}

	if (iRet != DEF_PROFILE_SUCCESS)
		iResult = 1;




	if (b_bRebuiltAC)
	{
		_Display2Output(_T("开始编译 2G 文件权限"), DEF_MESSAGE_HINT);
		iRet =  pcFile->ComputeProfileAC();
		_Display2Output(_T("结束编译 2G 文件权限"), iRet != TRUE ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
		
	if (iRet != TRUE)
		iResult += 2;

	if (b_bRebuiltOTA)
	{
		_Display2Output(_T("开始编译 OTA应用"), DEF_MESSAGE_HINT);
		iRet =  pcFile->ComputeProfileOTA();
		_Display2Output(_T("结束编译 OTA应用"), iRet < 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);

		if (iRet <0)
			iResult += 4;

		_Display2Output(_T("开始编译 固定应用"), DEF_MESSAGE_HINT);
		iRet =  pcFile->ComputeProfileFixed();
		_Display2Output(_T("结束编译 固定应用"), iRet < 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);

		if (iRet <0)
			iResult += 8;
	}

	if (b_bRebuiltAddress)
	{
		_Display2Output(_T("开始编译 文件地址"), DEF_MESSAGE_HINT);
		iRet =  pcFile->ComputeProfileNotLinkAddress();
		if (iRet != DEF_PROFILE_SUCCESS)
			iResult += 0x10;
		iRet =  pcFile->ComputeProfileLinkAddress();
		if (iRet <TRUE)
			iResult += 0x20;
		_Display2Output(_T("结束编译 文件地址"), iRet < TRUE ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
	_Display2Output(_T("编译完成!"), DEF_MESSAGE_HINT);

	return iResult;
}

void CMainFrame::OnCompileCompile()
{
	//NewStatusThread();
	NewCompileThread();
}

void CMainFrame::CompileBuilt()
{

	CString csCardHead, csFileHead, csFileContent;

	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	int iFileHead = 0;

	pcFile->BuiltFRU(csCardHead, iFileHead);

	pcFile->BuiltFile(csFileHead, iFileHead);

	pcFile->BuiltContent(csFileContent);


	CString csTitle = pcFile->GetProfilePath();

	if (CreateWriteFile(csTitle.Mid(0, csTitle.GetLength() - 4) + _T(".BIN"), csCardHead + csFileHead + csFileContent) != FALSE)
	{
		MessageBox(_T("生成 ") + csTitle + _T(".BIN 成功！"));
	}
	else
	{
		MessageBox(_T("生成 ") + csTitle + _T(".BIN 失败！"));
	}

}

afx_msg void CMainFrame::OnCompileBuilt(UINT iID)
{
	NewBuiltThread( iID);
}

int CMainFrame::CheckCheck()
{
	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	int iResult = 0;
	int iRet;
	if (b_cAddress)
	{
		_Display2Output(_T("开始检查 文件地址"), DEF_MESSAGE_HINT);
		iRet = pcFile->CheckProfileAddress(FALSE);
		_Display2Output(_T("结束检查 文件地址"), iRet <= 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
		
	if (iRet <= 0)
	{
		iResult += 1;
	}


	if (b_cHandle)
	{

		_Display2Output(_T("开始检查 文件句柄 --->"), DEF_MESSAGE_HINT);
		iRet = pcFile->CheckProfileHandle(FALSE);
		_Display2Output(_T("结束检查 文件句柄"), iRet <= 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
		
	if (iRet<=0)
	{
		iResult += 2;
	}

	if (b_cStructure)
	{

		_Display2Output(_T("开始检查 文件系统结构"), DEF_MESSAGE_HINT);
		iRet = pcFile->CheckProfileStructure();
		_Display2Output(_T("结束检查 文件系统结构"), iRet <= 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
	if (iRet <= 0)
	{
		iResult += 4;
	}


	if (b_cSFI)
	{
		_Display2Output(_T("开始检查 各个目录下短文件标示"), DEF_MESSAGE_HINT);
		iRet = pcFile->CheckProfileSFI();
		_Display2Output(_T("结束检查 各个目录下短文件标示"), iRet <= 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
	
	if (iRet <= 0)
	{
		iResult += 8;
	}

	if (b_cLinks)
	{
		_Display2Output(_T("开始检查 文件关联"), DEF_MESSAGE_HINT);
		iRet = pcFile->CheckProfileLinks();
		_Display2Output(_T("结束检查 文件关联"), iRet <= 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
		
	if (iRet <= 0)
	{
		iResult += 0x10;
	}

	if (b_cCharacteristic)
	{
		_Display2Output(_T("开始检查 单个文件结构"), DEF_MESSAGE_HINT);
		iRet = pcFile->CheckProfileSingleFile();
		_Display2Output(_T("结束检查 单个文件结构"), iRet <= 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
	if (iRet <= 0)
	{
		iResult += 0x20;
	}
		

	if (b_cDIR)

	{
		_Display2Output(_T("开始检查 目录文件（2F00）"), DEF_MESSAGE_HINT);
		iRet = pcFile->CheckDIR2F00();
		_Display2Output(_T("结束检查 目录文件（2F00）"), iRet <= 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
	if (iRet <= 0)
	{
		iResult += 0x40;
	}

	if (b_cPBR)
	{
		CString csPBR = _T("3F007F105F3A");
		_Display2Output(_T("开始检查 号簿文件（")+ csPBR+_T(")"), DEF_MESSAGE_HINT);
		iRet = pcFile->CheckPBR(csPBR);
		_Display2Output(_T("结束检查 号簿文件（") + csPBR + _T(")") , iRet <= 0 ? DEF_MESSAGE_ERROR : DEF_MESSAGE_RIGHT);
	}
		
	if (iRet <= 0)
	{
		iResult += 0x80;
	}

	return iResult;
}


void CMainFrame::OnCheckCheck()
{
	//NewStatusThread();
	NewCheckThread();
}


CDockablePane* CMainFrame::GetDockablePane(int iID)
{
	CDockablePane* _Pane;

	switch (iID)
	{
	case ID_VIEW_FILELIST:_Pane = &m_wndFilesListView; break;
	case ID_VIEW_FILETREE:_Pane = &m_wndFilesTreeView; break;
	case ID_VIEW_PROPERTY:_Pane = &m_wndProperties; break;
	default:_Pane = NULL; break;
	}
	

	return _Pane;

}


int CMainFrame::CreateWriteFile(CString csTitle, CString &csReuslt)
{
	BOOL bRet = TRUE;

	FILE *filetemp;

	filetemp = fopen(csTitle, "wb+");
	if (filetemp == NULL)
		return FALSE;

	int iLen = csReuslt.GetLength();

	BYTE* ucByte = new BYTE[iLen];

	_CString2UcHex(csReuslt, ucByte);

	if (fwrite(ucByte, 1, iLen / 2, filetemp) <= 0)
		bRet = FALSE;

	fclose(filetemp);

	return bRet;
}

void CMainFrame::InsertProfileThread()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();

	pMain->InsertProfile(FALSE);

	iThread = Def_Thread_Free;
}


void CMainFrame::NewInsertProfileThread()
{
	DWORD code;


	GetExitCodeThread(hStatusThread, &code);

	if (code != STILL_ACTIVE)
	{
		hStatusThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InsertProfileThread, NULL, 0, NULL);
	}

	iThread = Def_Thread_InsertProfile;
	return;
}


void CMainFrame::StatusBarThread()
{


	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	UINT uiNumber = 0;
	CString csTag;

	while (iThread != Def_Thread_Free)
	{
		csTag = RepeatCString(_T("."),uiNumber/10);
		switch (iThread)
		{
		case Def_Thread_Compile: pMain->_Display2StatusBar(_T("编译") + csTag); break;
		case Def_Thread_Built:   pMain->_Display2StatusBar(_T("生成") + csTag); break;
		case Def_Thread_Check: 	 pMain->_Display2StatusBar(_T("检查") + csTag); break;
		}

		uiNumber++;
		Sleep(10);
		if (uiNumber > 49)
			uiNumber = 0;
	}
	pMain->_Display2StatusBar(_T("就绪"));

}


void CMainFrame::NewStatusThread()
{
	DWORD code;


	GetExitCodeThread(hStatusThread, &code);

	if (code != STILL_ACTIVE)
	{
		hStatusThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StatusBarThread, NULL, 0, NULL);
	}

	return;
}

void CMainFrame::NewCompileThread()
{
	DWORD code;


	GetExitCodeThread(hWorkThread, &code);

	if (code != STILL_ACTIVE)
	{
		hWorkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CompileThread, NULL, 0, NULL);
	}
	else
		ResumeThread(hWorkThread);

	iThread = Def_Thread_Compile;

	return;
}

void CMainFrame::CompileThread()
{
	//	iThread = Def_Thread_Compile;

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();

	pMain->CompileCompile();

	iThread = Def_Thread_Free;

}


void CMainFrame::NewBuiltThread(UINT iID)
{
	DWORD code;


	GetExitCodeThread(hWorkThread, &code);

	if (code != STILL_ACTIVE)
	{
		switch (iID)
		{
		case ID_COMPILE_BUILTONLY :hWorkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BuiltThreadOnly, NULL, 0, NULL); break;
		case ID_COMPILE_BUILT:     hWorkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BuiltThread    , NULL, 0, NULL); break;
		case ID_COMPILE_BUILT_ADPU:hWorkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BuiltThreadAPDU, NULL, 0, NULL); break;
		default:
			break;
		}

		
	}
	else
		ResumeThread(hWorkThread);

	iThread = Def_Thread_Built;

	return;
}

void CMainFrame::BuiltThreadOnly()
{

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();


	pMain->CompileBuilt();

	iThread = Def_Thread_Free;

}
void CMainFrame::BuiltThread()
{

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();

	if ((pMain->CompileCompile()!=0)&&
		(pMain->MessageBox(_T("编译发现错误，是否继续？"), _T("WARNNING"), MB_YESNO) == IDNO))
	{
		return;	
	}

	if ((pMain->CheckCheck() != 0) &&
		(pMain->MessageBox(_T("检查发现错误，是否继续？"), _T("WARNNING"), MB_YESNO) == IDNO))
	{
		return;
	}
	pMain->CompileBuilt();

	iThread = Def_Thread_Free;

}

void CMainFrame::BuiltThreadAPDU()
{

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();

	pMain->CompileBuilt();

	iThread = Def_Thread_Free;

}

void CMainFrame::NewCheckThread()
{
	DWORD code;


	GetExitCodeThread(hWorkThread, &code);

	if (code != STILL_ACTIVE)
	{
		hWorkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckThread, NULL, 0, NULL);
	}
	else
		ResumeThread(hWorkThread);

	iThread = Def_Thread_Check;

	return;
}

void CMainFrame::CheckThread()
{

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();

	pMain->CheckCheck();

	iThread = Def_Thread_Free;

}


void CMainFrame::_Display2Output(CString csMessage, int iTag)
{

	if (iThread == Def_Thread_Free)
	{

		if (!m_wndOutput.IsVisible())
		{
			m_wndOutput.SetAutoHideMode(FALSE, CBRS_ALIGN_ANY);
			m_wndOutput.ShowPane(TRUE, FALSE, TRUE);
			
		}

		//if (m_wndOutput.IsAutohideAllEnabled())
		//{
		//	m_wndOutput.SetActiveInGroup(TRUE);

		//	m_wndOutput.is



		//}

	}


	m_wndOutput.DisplayMassege(csMessage, iTag);
	//m_wndOutput.DisplayMassege(_T("-->")+Int2CString(iTag) + csMessage, iTag);
}

void CMainFrame::_Display2StatusBar(CString csText, int iIndex/*= DEF_StatueBar_Thread*/)
{

	//bool bRet;
	CDC __temp;
	CSize iSize;
	switch (iIndex)
	{
	case DEF_STATUEBAR_WORK:m_ElementWork->SetText(csText); break;
	case DEF_STATUEBAR_SUBWORK:m_ElementSubWork->SetText(csText); 
//	m_ElementSubWork->SetTextAlign(TA_LEFT);
		break;
	case DEF_STATUEBAR_RESERVE:m_ElementReserve->SetText(csText); break;
	case DEF_STATUEBAR_FILE:
		m_ElementFile->SetText(csText);
		m_ElementFile->SetTextAlign(TA_RIGHT);
	//	m_ElementFile->Redraw();
		break;
	}
	m_wndStatusBar.RedrawWindow();

}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值



	if (nIDEvent == DEF_Event_Process)
	{
	
		if (iThread == Def_Thread_Free)
		{
			if (m_ElementWork->IsAnimation())
				m_ElementWork->StopAnimation();
			m_ElementSubWork->SetText(_T("就绪"));
		}
	else
	{
		if (!m_ElementWork->IsAnimation())
		{
			//	CBitmap bitmap;
			//	bitmap.LoadBitmap(IDB_STATUEBAR_PROCESS);
			m_ElementWork->SetAnimationList(IDB_STATUEBAR_PROCESS);
			m_ElementWork->StartAnimation(100);
		}
		CString csText;
		CString csFormat;
		switch (iThread)
		{
		case Def_Thread_Compile:csText = _T("编译"); break;
		case Def_Thread_Built:  csText = _T("生成"); break;
		case Def_Thread_Check: 	csText = _T("检查"); break;
		case Def_Thread_InsertProfile: 	csText = _T("打开文件");; break;
		}

		csFormat = m_ElementSubWork->GetText();
		int iOff = csFormat.Find(".");
		if (iOff < 0)
		{
			csFormat = _T(".");
		}
		else
		{
			iOff = csFormat.GetLength() - iOff;
			if (iOff > 5)
				csFormat.Empty();
			else
				csFormat = RepeatCString(_T("."), iOff + 1);
		}

		m_ElementSubWork->SetText(csText + csFormat);

		}
		

	}
		
	m_wndStatusBar.RedrawWindow();
	CMDIFrameWndEx::OnTimer(nIDEvent);
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((m_hAccel)&&
	    (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg)))
			return (TRUE);
	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}


BOOL CMainFrame::OnShowMDITabContextMenu(CPoint point, DWORD dwAllowedItems, BOOL bTabDrop)
{
	// TODO: 在此添加专用代码和/或调用基类


	CMenu menu;
	menu.LoadMenu(IDR_POPUP_CLOSE);
	//menu.LoadMenu(IDR_OUTPUT_POPUP);
	

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return true;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
	
	return CMDIFrameWndEx::OnShowMDITabContextMenu(point, dwAllowedItems, bTabDrop);
}


void CMainFrame::OnClostAll()
{
	CChildFrame* CFram;
	
	do 
	{
		CFram = (CChildFrame*)GetActiveFrame();
		if (CFram->IsKindOf(RUNTIME_CLASS(CChildFrame)))
			CFram->OnClose();
		else
			return;
	} while (1);

	
	//((CMDIFrameWndEx*)AfxGetMainWnd())->CloseDo
	
}


void CMainFrame::OnCloseCurrent()
{
	CChildFrame* CFram = (CChildFrame*)GetActiveFrame();

	CFram->OnClose();
	//CFram->Close

	//CProfileExplorerDoc* CDoc = (CProfileExplorerDoc*)GetActiveDocument();
//	OnFileClose();

	//CDoc->OnCloseDocument();
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnCloseOther()
{
	CChildFrame* CCurrent = (CChildFrame*)GetActiveFrame();
	CChildFrame* CFram;

	int iCount = AfxGetApp()->GetOpenDocumentCount();

	for (int i = 1 ; i< iCount; 1)
	{
		//AfxGetApp()->
		MDINext();
		CFram = (CChildFrame*)GetActiveFrame();
		if (!CFram->IsKindOf(RUNTIME_CLASS(CChildFrame)))
			return;
		if (CFram != CCurrent)
		{
			CFram->OnClose();
			i++;
		}


	}
}
