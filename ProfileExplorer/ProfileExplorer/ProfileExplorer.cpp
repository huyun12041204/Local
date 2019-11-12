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

// ProfileExplorer.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ProfileExplorer.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ProfileExplorerDoc.h"
#include "ProfileExplorerView.h"
#include "MultiDocTemplateEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


#include "stdafx.h"
#include "ProfileExplorer.h"
#include "afxdialogex.h"


// CFindFileDialog 对话框

IMPLEMENT_DYNAMIC(CFindFileDialog, CDialogEx)

CFindFileDialog::CFindFileDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDBOX, pParent)
{

}

CFindFileDialog::~CFindFileDialog()
{
}

void CFindFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFindFileDialog, CDialogEx)
	ON_BN_CLICKED(IDC_FIND_FIND, &CFindFileDialog::OnBnClickedFindFind)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_FIND_NEXT, &CFindFileDialog::OnBnClickedFindNext)
END_MESSAGE_MAP()

void CFindFileDialog::OnBnClickedFindFind()
{

	CFilesTreeView* __View1;
	CFilesListView* __View2;

	__View1 = (CFilesTreeView*)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILETREE);

	__View2 = (CFilesListView*)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILELIST);


	CString csFIND;
	BOOL bFID = FALSE;
	GetDlgItemText(IDC_FID_EDIT, csFIND);

	if (_IsAllHex(csFIND)&& 
		(csFIND.GetLength()== 4))
		bFID = TRUE;


	__View1->ResetStatue(iNumber, hItem);



	if (__View1->IsKindOf(RUNTIME_CLASS(CFilesTreeView)))
	{
		iNumber = 0;
		ZeroMemory(hItem, iNumber);
		if (bFID == TRUE)
			__View1->_FindFile(csFIND, &iNumber, hItem);
		if (iNumber == 0)
			__View1->_FindFile_Name(csFIND, &iNumber, hItem);


		if (iNumber != 0)
		{
			__View1->SetSelectItem(hItem[0]);
			__View1->SetBlod(iNumber, hItem);
		}





	}


	if (__View2->IsKindOf(RUNTIME_CLASS(CFilesListView)))
	{
		iNumber = 0;
		ZeroMemory(iItem, iNumber);

		//if (bFID == TRUE)
		//{
		//	__View2->_FindFile(csFIND, &iNumber, iItem);
		//	__View2->SetSelectItem(iItem[0]);
		//	__View2->SetBlod(iNumber, iItem);
		//}

		if (bFID == TRUE)
			__View2->_FindFile(csFIND, &iNumber, iItem);
		if (iNumber == 0)
			__View2->_FindFile_Name(csFIND, &iNumber, iItem);

		if (iNumber != 0)
		{
			__View2->SetSelectItem(iItem[0]);
			__View2->SetBlod(iNumber, iItem);
		}


	}
	iCurrent += 1;
	
}

void CFindFileDialog::OnBnClickedFindNext()
{
	CFilesTreeView* __View1;
	CFilesListView* __View2;

	__View1 = (CFilesTreeView*)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILETREE);

	__View2 = (CFilesListView*)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILELIST);

	if (__View1->IsKindOf(RUNTIME_CLASS(CFilesTreeView)))
		__View1->SetSelectItem(hItem[iCurrent]);
	if (__View2->IsKindOf(RUNTIME_CLASS(CFilesListView)))
		__View2->SetSelectItem(iItem[iCurrent]);

	iCurrent += 1;

	if (iCurrent >= iNumber)
		iCurrent = 0;

}


void CFindFileDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CFilesTreeView* __View1;

	__View1 = (CFilesTreeView*)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILETREE);

	if (__View1->IsKindOf(RUNTIME_CLASS(CFilesTreeView)))
	{
		if (bShow)
		{
			ZeroMemory(hItem, sizeof(hItem));
			iNumber  = 0;
			iCurrent = 0;
		}
	}



}
void CFindFileDialog::OnDestroy()
{
	CDialogEx::OnDestroy();	
}
void CFindFileDialog::OnClose()
{
	CFilesTreeView* __View1;
	__View1 = (CFilesTreeView*)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILETREE);
	if (__View1->IsKindOf(RUNTIME_CLASS(CFilesTreeView)))
		__View1->ResetStatue(iNumber, hItem);

	CFilesListView* __View2;
	__View2 = (CFilesListView*)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILELIST);
	if (__View2->IsKindOf(RUNTIME_CLASS(CFilesListView)))
		__View2->ResetStatue(iNumber, iItem);

	


	CDialogEx::OnClose();
}


// CFindFileDialog 消息处理程序


// CProfileExplorerApp
BOOL IsCardFilesAppPath(LPCTSTR lpszPathName, CString* csFilePath, CString* csTitle)
{

	BOOL bRet;
	int Item = ((CString)lpszPathName).Find(_T("#"));

	bRet = (Item >= 0);

	if ((bRet) && (csFilePath != NULL))
		*csFilePath = ((CString)lpszPathName).Left(Item);


	if ((bRet) && (csTitle != NULL))
		*csTitle = ((CString)lpszPathName).Mid(Item + 1);

	return bRet;
}

CString GenerateCardFilesAppPath(CString csProfile, CString csFilePath)
{
	_DeleteEnterSpace(csFilePath);

	return csProfile + _T("#") + csFilePath;


}

BEGIN_MESSAGE_MAP(CProfileExplorerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CProfileExplorerApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CProfileExplorerApp::OnFileOpen)
END_MESSAGE_MAP()


// CProfileExplorerApp 构造

CProfileExplorerApp::CProfileExplorerApp() noexcept
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ProfileExplorer.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CProfileExplorerApp 对象

CProfileExplorerApp theApp;
UINT uiChildViewWide;
int iThread;



// CProfileExplorerApp 初始化

BOOL CProfileExplorerApp::InitInstance()
{
	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Giesecke&Devrient"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接


	uiChildViewWide = 300;

	CMultiDocTemplateEx* pDocTemplate;
	pDocTemplate = new CMultiDocTemplateEx(IDR_ProfileExplorerTYPE,
		RUNTIME_CLASS(CProfileExplorerDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CProfileExplorerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();



	return TRUE;
}

int CProfileExplorerApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CProfileExplorerApp 消息处理程序





// 用于运行对话框的应用程序命令
void CProfileExplorerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CProfileExplorerApp 自定义加载/保存方法

void CProfileExplorerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CProfileExplorerApp::LoadCustomState()
{
	CMainFrame* p_mMain = (CMainFrame*)AfxGetMainWnd();

	for (int i = ID_MODIFY_FIXEDARR ; i<= ID_MODIFY_FIXEDAC;i++)
		p_mMain->LoadSetting(i);

	for (int i = ID_COMPILE_REBUILT_AC; i <= ID_COMPILE_REBUILT_ADDRESS; i++)
		p_mMain->LoadSetting(i);

	for (int i = ID_CHECK_ADDRESS; i <= ID_CHECK_CHARACTERISTIC; i++)
		p_mMain->LoadSetting(i);

	for (int i = ID_CHECK_DIR; i <= ID_CHECK_PRL; i++)
		p_mMain->LoadSetting(i);

	for (int i = ID_CHECK_0002; i <= ID_CHECK_0019; i++)
		p_mMain->LoadSetting(i);

	for (int i = ID_CHECK_19001F11; i <= ID_CHECK_19001F10; i++)
		p_mMain->LoadSetting(i);





}

void CProfileExplorerApp::SaveCustomState()
{

	CMainFrame* p_mMain = (CMainFrame*)AfxGetMainWnd();

	for (int i = ID_MODIFY_FIXEDARR; i <= ID_MODIFY_FIXEDAC; i++)
		p_mMain->SaveSetting(i);

	for (int i = ID_COMPILE_REBUILT_AC; i <= ID_COMPILE_REBUILT_ADDRESS; i++)
		p_mMain->SaveSetting(i);

	for (int i = ID_CHECK_ADDRESS; i <= ID_CHECK_CHARACTERISTIC; i++)
		p_mMain->SaveSetting(i);

	for (int i = ID_CHECK_DIR; i <= ID_CHECK_PRL; i++)
		p_mMain->SaveSetting(i);

	for (int i = ID_CHECK_0002; i <= ID_CHECK_0019; i++)
		p_mMain->SaveSetting(i);

	for (int i = ID_CHECK_19001F11; i <= ID_CHECK_19001F10; i++)
		p_mMain->SaveSetting(i);


}

// CProfileExplorerApp 消息处理程序

CDocument* CProfileExplorerApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	CString csTitel;
	CString csPath;
	if (!IsCardFilesAppPath(lpszFileName, &csPath, &csTitel))
	{


		int iRet = pcFile.LoadFile(lpszFileName);

		if (iRet == FALSE)
		{
			return NULL;
		}
		else if (iRet == TRUE)
		{
			((CMainFrame*)GetMainWnd())->InsertProfile(TRUE);
			CString csPath = lpszFileName;
			CWinAppEx::AddToRecentFileList(csPath);
			return ((CDocument*)NULL + 1);
		}
		else
			return  ((CDocument*)NULL + 2);
			
	}
	else
	{
		if (pcFile.GetProfilePath().IsEmpty())
			return NULL;


		if ((csTitel.GetLength() > 8) &&
			(csTitel.Left(4) == "3F00"))
			csTitel = csTitel.Mid(4);

	}

	return CWinAppEx::OpenDocumentFile(pcFile.GetProfilePath()+lpszFileName, FALSE);
}

void CProfileExplorerApp::AddToRecentFileList(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类

	CWinAppEx::AddToRecentFileList(lpszPathName);
}

void CProfileExplorerApp::OnFileNew()
{
	CWinAppEx::OnFileNew();
}

void CProfileExplorerApp::OnFileOpen()
{
	CWinAppEx::OnFileOpen();
}




