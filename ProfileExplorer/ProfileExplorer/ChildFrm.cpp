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

// ChildFrm.cpp: CChildFrame 类的实现
//

#include "stdafx.h"
#include "ProfileExplorer.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CONTEXTMENU()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame() noexcept
{
	pcfsOriFile.EmptyFile();
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	CRect rectWindows;

	GetWindowRect(&rectWindows);

	int ix, iy;
	int iLX, iRX;

	ix = rectWindows.Width();
	iy = rectWindows.Height();

	int iWide = uiChildViewWide;

	iLX = (ix > iWide ? iWide : ix / 2);
	iRX = ix - iLX;



	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CProfileExplorerView), CSize(iLX, iy), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CFileContentView), CSize(iRX, iy), pContext))

	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}


	m_wndProfileExplorerView    = (CProfileExplorerView*)m_wndSplitter.GetPane(0, 0);
	m_wndFileContentView        = (CFileContentView*)    m_wndSplitter.GetPane(0, 1);




	return TRUE;

	//return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

BOOL CChildFrame::SetFileToView(PCFS pcfsFile, BOOL bDisplay /*= FALSE*/)
{
	pcfsOriFile = pcfsFile;
	BOOL bLink = FALSE;

	if (((pcfsFile.uiSTRUCT) == DEF_FILESTRUCTURE_TLINK)
		|| ((pcfsFile.uiSTRUCT) == DEF_FILESTRUCTURE_LLINK)
		|| ((pcfsFile.uiSTRUCT) == DEF_FILESTRUCTURE_CLINK))
		bLink = TRUE;


	m_wndProfileExplorerView->InputFileInformation(pcfsFile);
	//m_wndFileContentView->SetDlgItemText(IDC_Content_Edit, pcfsFile.stContent);
	m_wndProfileExplorerView->_SetPropertyStatueOfLink(bLink);
	m_wndProfileExplorerView->UpdateViewStatus();

m_wndFileContentView->GetEditCtrl().SetWindowText(pcfsFile.stContent);

	m_wndFileContentView->GetEditCtrl().EnableWindow(!bLink);
	m_wndFileContentView->GetEditCtrl().ShowWindow(SW_SHOW);
	return TRUE;
}


BOOL CChildFrame::GetFileFromView(PCFS* pcfsFile,BOOL bCorrect)
{

	try
	{
		*pcfsFile = pcfsOriFile;

		GetFilePropertyFromView(pcfsFile);

		GetFileContentFromView(pcfsFile);

		pcfsFile->CorrectPCFS();
	}

	catch(_com_error(e))
	{
		return FALSE;
	}
	return TRUE;
	
}

BOOL CChildFrame::GetFilePropertyFromView(PCFS* pcfsFile)
{
	return m_wndProfileExplorerView->_GetFile(pcfsFile);
}

BOOL CChildFrame::GetFileContentFromView(PCFS* pcfsFile)
{
	CString csContent;
	m_wndFileContentView->GetEditCtrl().GetWindowText(csContent);
	pcfsFile->stContent = csContent;

	return TRUE;

}


void CChildFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CMDIChildWndEx::OnClose();
}
