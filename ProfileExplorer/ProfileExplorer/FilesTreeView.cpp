


#include "stdafx.h"
#include "FilesTreeView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ProfileExplorer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &CViewTree::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDblclk)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 消息处理程序

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


void CViewTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{


}


void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	__FileOpen();

}

// 通过 ITEM 来获取 Route
//************************************
// Method:    GetFileRoute
// FullName:  CViewTree::GetFileRoute
// Access:    public 
// Returns:   CString
// Qualifier: 包含父目录和文件FID
// Parameter: HTREEITEM hItem
//************************************
CString CViewTree::GetFilePath(HTREEITEM hItem)
{
	CString csTemp;
	csTemp.Empty();

	while(hItem != NULL)
	{
		csTemp = __GetItemFID(hItem)+csTemp;
		hItem  = GetNextItem(hItem,TVGN_PARENT);
	}
	return csTemp;
}


// 获取 ITEM 的 FID （Text的 前4）
CString CViewTree::__GetItemFID(HTREEITEM hItem)
{
	CString csText;

	csText = GetItemText(hItem);
	return csText.Left(4);
}

UINT CViewTree::_GetPointItem(HTREEITEM* hItem)
{
	CPoint cpPoint;
	UINT uFlag ;
	GetCursorPos(&cpPoint);
	CTreeCtrl::ScreenToClient(&cpPoint);
	*hItem = CTreeCtrl::HitTest(cpPoint,&uFlag);
	return uFlag;
}

void CViewTree::__FileOpen()
{
	CString csPath;
	HTREEITEM hItem;
	if ((_GetPointItem(&hItem)&TVHT_ONITEM) &&
		(hItem != NULL))
	{
		csPath = GetFilePath(hItem);
		if (!csPath.IsEmpty())
			((CProfileExplorerApp*)AfxGetApp())->OpenDocumentFile(_T("#") + csPath);
	}

}


#include "stdafx.h"
#include "mainfrm.h"
#include "FilesTreeView.h"
#include "Resource.h"
#include "ProfileExplorer.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFilesTreeView::CFilesTreeView()
{
}

CFilesTreeView::~CFilesTreeView()
{
}

BEGIN_MESSAGE_MAP(CFilesTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	//ON_COMMAND(ID_OPEN, OnFileOpen)
//	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
//	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
//	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
//	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
//	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()

	ON_COMMAND(ID_FILETREE_OPEN,  OnFileOpen)
	ON_COMMAND(ID_FILETREE_CHECK, OnFileCheck)
	ON_COMMAND(ID_FILETREE_DELETE, OnFileDelete)
	ON_COMMAND(ID_FILETREE_EXPORT, OnFileExport)
	ON_COMMAND(ID_FILETREE_INPORT, OnFileImport)
	ON_COMMAND(ID_FILETREE_REFRESH, OnFileRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int CFilesTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像:
	m_FileViewImages.Create(IDB_FILETREE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	AdjustLayout();

	return 0;
}

void CFilesTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFilesTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_FILETREE, point.x, point.y, this, TRUE);
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_FILETREE);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CViewTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString csRount;
	HTREEITEM hItem;
	if ((_GetPointItem(&hItem)&TVHT_ONITEM) &&
		(hItem != NULL))
	{
		csRount = GetFilePath(hItem);
		((CProfileExplorerApp*)AfxGetApp())->OpenDocumentFile(_T("#") + csRount);
	}
}

void CFilesTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFilesTreeView::OnProperties()
{
	AfxMessageBox(_T("属性...."));

}

void CFilesTreeView::OnFileOpen()
{

	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	if (hItem == NULL)
		return;
	CString csPath = m_wndFileView.GetFilePath(hItem);
	if (csPath.IsEmpty())
		return;
		
	((CProfileExplorerApp*)AfxGetApp())->OpenDocumentFile(_T("#") + csPath);
}

void CFilesTreeView::OnFileCheck()
{
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	if (hItem == NULL)
		return;
	CString csPath = m_wndFileView.GetFilePath(hItem);
	if (csPath.IsEmpty())
		return;

	PCFS __FIle;
	ProfileClass* __Profile;
	CStringArray csResult;
	CString csFile;
	int iErrorNumber = 0;
	__Profile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	if (__Profile->GetFileFromDoc(csPath, &__FIle)!= DEF_PROFILE_SUCCESS)
		return;

	iErrorNumber = __FIle.CheckParameter(csResult);

	CMainFrame* pMain;
	pMain = (CMainFrame*)AfxGetMainWnd();

	for (int i = 0 ; i< csResult.GetCount(); i++)
		pMain->_Display2Output(csResult.GetAt(i), DEF_MESSAGE_ERROR);


	if ((__FIle.uiTYPE != DEF_FILETYPE_EF) &&
		(!__Profile->CheckDFSFI(csPath)))
		iErrorNumber += 1;


	if (iErrorNumber == 0)
		pMain->_Display2Output(_T("文件") + csPath + _T(" 没有存在错误！"), DEF_MESSAGE_RIGHT);
	else
		pMain->_Display2Output(_T("文件") + csPath + _T(" 存在 ")+Int2CStringDecimal(iErrorNumber)+_T(" 错误！"), DEF_MESSAGE_ERROR);


}

void CFilesTreeView::OnFileDelete()
{
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	if (hItem == NULL)
		return;
	CString csPath = m_wndFileView.GetFilePath(hItem);
	if (csPath.IsEmpty())
		return;


	ProfileClass* __Profile;
	__Profile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;


	long lRecord = __Profile->GetFileRecord(csPath);
	CMainFrame* pMain;
	pMain = (CMainFrame*)AfxGetMainWnd();
	if ((lRecord > 0)&&
		(__Profile->DeleteDocRecord(lRecord)))
	{
		pMain->_Display2Output(_T("文件") + csPath + _T(" 删除成功！"), DEF_NORMAL_TAG);

		DeleteItem(hItem);

		//以下为同步代码
		CFilesListView* p_wndList = (CFilesListView*)pMain->GetDockablePane(ID_VIEW_FILELIST);
		if (p_wndList->IsKindOf(RUNTIME_CLASS(CFilesListView)))
		{
			int iItem = p_wndList->GetFileItem(csPath);
			if (iItem>=0)
				p_wndList->DeleteItem(iItem-1);
		}		
	}
	else
	{
		DeleteItem(hItem);
	}
}

void CFilesTreeView::OnFileExport()
{
	CMainFrame* pMain;
	CStringArray csArray;
	CString _FilePath;
	CString csNewMDBFile;
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();

	if (hItem == NULL)
		return;

	_FilePath = m_wndFileView.GetFilePath(hItem);
	if (_FilePath.IsEmpty())
		return;

	csArray.Add(_FilePath);
	pMain = (CMainFrame*)AfxGetMainWnd();

	
	__GetCurrentDIR(csNewMDBFile);

	csNewMDBFile = csNewMDBFile + _T("\\") + _FilePath + _T(".mdb");

	pMain->__ExportFiles(csArray, csNewMDBFile);

}

void CFilesTreeView::OnFileImport()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	pMain->OnImportFiles();
}

void CFilesTreeView::OnFileRefresh()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->NewInsertProfileThread();

}

void CFilesTreeView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFilesTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFilesTreeView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_FileViewImages.DeleteImageList();

//	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	UINT uiBmpId = IDB_FILETREE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(0, 0, 0));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


int CFilesTreeView::GetFileImage(int iFileType)
{
	int iRet = 0;
	switch (iFileType)
	{
	case DEF_FILETYPE_EF + DEF_FILESTRUCTURE_TRANSPARENT:iRet = 0; break;
	case DEF_FILETYPE_EF + DEF_FILESTRUCTURE_LINER:iRet = 1; break;
	case DEF_FILETYPE_EF + DEF_FILESTRUCTURE_CYCLIC:iRet = 2; break;
	case DEF_FILETYPE_EF + DEF_FILESTRUCTURE_TLINK:iRet = 3; break;
	case DEF_FILETYPE_EF + DEF_FILESTRUCTURE_LLINK:iRet = 4; break;
	case DEF_FILETYPE_EF + DEF_FILESTRUCTURE_CLINK:iRet = 5; break;
	default:

		switch (iFileType&0xF0)
		{
		case DEF_FILETYPE_DF: iRet = 6; break;
		case DEF_FILETYPE_ADF: iRet = 7; break;	
		case DEF_FILETYPE_MF: iRet = 8; break;
		default:iRet = 9; break;
		}
		break;
	}
	return iRet;

}


int CFilesTreeView::_InsertFile( int iFID, CString csDescription,int iStructure, int iState, HTREEITEM hPareItem/*=TVI_ROOT*/, HTREEITEM* hFileItem/*=NULL*/ )
{
	HTREEITEM hCurFileItem;
	CString csFileTitle;

	csFileTitle.Format(_T("%04X"),iFID);

	csFileTitle += _T("  ");
	csFileTitle += csDescription;
	
	int iImage = GetFileImage(iStructure);


	hCurFileItem = m_wndFileView.InsertItem(csFileTitle, iImage, iImage+10,hPareItem);
	if (!iState)
	{
		m_wndFileView.SetItemState(hCurFileItem, TVIS_CUT, TVIS_CUT);
	}
	if (hFileItem!= NULL)
		*hFileItem = hCurFileItem;


	return TRUE;
}
int CFilesTreeView:: InsertFile(int iFID, CString csDescription, CString csRoute, int iStructure, int iStatus)
{
	HTREEITEM hRoute = NULL;
	HTREEITEM hFile  = NULL;
	CString csPath = csRoute + Int2CString(iFID, 4);

	
	hFile = _GetItem(csPath);

	if (hFile!=NULL)
	{
		int iImage = GetFileImage(iStructure);
		_SetItemName(hFile, csDescription);
		m_wndFileView.SetItemState(hFile, 0, TVIS_CUT);
		m_wndFileView.SetItemImage(hFile, iImage, iImage+10);
		return TRUE;
	}
	else
	{

		if (csRoute.IsEmpty())
			hRoute = GetRootItem();
		else
		{
			hRoute = GetRouteItem(csRoute);
			if (hRoute == NULL)
			{
				if(!InsertFile(_CString2Int(csRoute.Right(4)), "未出现目录", csRoute.Mid(0, csRoute.GetLength() - 4), DEF_FILETYPE_DF, 0))
					return -1;
				hRoute = GetRouteItem(csRoute);
			}
				
				
		}
		_ExpandFilesTree();
		return _InsertFile(iFID, csDescription, iStructure, iStatus, hRoute, NULL);

	}

//	return TRUE;
}

int CFilesTreeView::InsertFile(PCFS File)
{

	return InsertFile(File.uiFID, File.stFILENAME, File.stROUTE, File.uiTYPE+ File.uiSTRUCT, TRUE);
	
}

int CFilesTreeView:: DeleteAllItem(void)
{

	//m_wndFileView.DeleteItem(TVI_ROOT);
	return m_wndFileView.DeleteAllItems();
}



// 通过FID
HTREEITEM CFilesTreeView::_GetItem(int iFID,HTREEITEM hParentItem)
{
	HTREEITEM hChildItem;
	CString csFID;
	hChildItem = m_wndFileView.GetChildItem(hParentItem);

	while(hChildItem!=NULL)
	{

		csFID = _GetItemFID(hChildItem);
		if ((!csFID.IsEmpty())&&(_CString2Int(csFID) == iFID))
			return hChildItem;
		hChildItem = m_wndFileView.GetNextItem(hChildItem,TVGN_NEXT);
	}

	return NULL;
}
HTREEITEM CFilesTreeView::_GetItem(CString csPath)
{
	HTREEITEM hFile;
	hFile = TVI_ROOT;
	int iStart,iEnd;
	iStart = 0 ;


	do 
	{
		iEnd = iStart+4;

		if (iEnd < 0)
			iEnd = csPath.GetLength();

		if((iEnd - iStart) == 0x4 ) 
			hFile = _GetItem(_CString2Int(csPath.Mid(iStart,4)),hFile);


		iStart = iEnd;
	} while (iEnd < csPath.GetLength());


	return hFile;
}
HTREEITEM CFilesTreeView:: GetRouteItem(CString csRoute)
{

	if (csRoute.IsEmpty())
		return NULL;
	else
		return _GetItem(csRoute);
}
HTREEITEM CFilesTreeView:: GetRootItem()
{
	return m_wndFileView.GetRootItem();
}



CString CFilesTreeView::  _GetItemFID(HTREEITEM hItem)
{
	CString csText;

	csText.Empty();

	if ((hItem != NULL)&&
		(hItem != TVI_ROOT))
	{
		csText = m_wndFileView.GetItemText(hItem);

		csText = csText.Left(4);
	}


	return csText;
}
BOOL CFilesTreeView::_SetItemFID(HTREEITEM hItem,CString csFID)
{
	CString csText;

	csText = m_wndFileView.GetItemText(hItem);

	return m_wndFileView.SetItemText(hItem, csFID + csText.Mid(4));


}

CString CFilesTreeView::_GetItemName(HTREEITEM hItem)
{
	CString csText;

	csText.Empty();

	if ((hItem != NULL) &&
		(hItem != TVI_ROOT))
	{
		csText = m_wndFileView.GetItemText(hItem);
		int iOS = csText.Find(_T("  "), 0);
		if (iOS >0)
			csText = csText.Mid(iOS + 2);

		
	}


	return csText;

}

BOOL CFilesTreeView::_SetItemName(HTREEITEM hItem, CString csName)
{
	CString csText;

	csText = m_wndFileView.GetItemText(hItem);

	return m_wndFileView.SetItemText(hItem, csText.Left(4) + _T("(") + csName + _T(")"));


}
BOOL CFilesTreeView::ModifyFile(HTREEITEM hItem, PCFS __File)
{
	BOOL bRet = TRUE;
	CString csItemPath = m_wndFileView.GetFilePath(hItem);
	if (__File.stROUTE.Compare(csItemPath.Left(csItemPath.GetLength()-4))!=0)
	{
		m_wndFileView.DeleteItem(hItem);
		bRet = InsertFile(__File);
	}
	else
	{
		bRet &= _SetItemFID(hItem, Int2CString(__File.uiFID,4));
		bRet &= _SetItemName(hItem, __File.stFILENAME);
	}
	return bRet;
}

int CFilesTreeView::_FindFile(CString csFID, int *iNum, HTREEITEM* hItem)
{
	HTREEITEM _Item = TVI_ROOT;
	HTREEITEM hRet = NULL;
	csFID.MakeUpper();
	return __FindFile(csFID, _Item, iNum, hItem);

}
int CFilesTreeView::__FindFile(CString csFID, HTREEITEM hParentItem, int* iNum, HTREEITEM * hItem)
{
	HTREEITEM _Item = m_wndFileView.GetNextItem(hParentItem, TVGN_CHILD);
	CString _FID;
	int iCNum = 0;
	while (_Item != NULL)
	{
		iCNum += __FindFile(csFID, _Item, iNum, hItem);
		_Item = m_wndFileView.GetNextItem(_Item, TVGN_NEXT);
	}

	_FID = _GetItemFID(hParentItem);
	if (_FID.Compare(csFID) == 0)
	{
		if (*iNum < 100)
			hItem[*iNum] = hParentItem;

		*iNum += 1;
		iCNum += 1;
	}


	return iCNum;

}

int CFilesTreeView::_FindFile_Name(CString csName, int *iNum, HTREEITEM* hItem)
{
	HTREEITEM _Item = TVI_ROOT;
	HTREEITEM hRet = NULL;
	return __FindFile_Name(csName, _Item, iNum, hItem);

}
int CFilesTreeView::__FindFile_Name(CString csName, HTREEITEM hParentItem, int* iNum, HTREEITEM * hItem)
{
	HTREEITEM _Item = m_wndFileView.GetNextItem(hParentItem, TVGN_CHILD);
	CString _Name;
	int iCNum = 0;
	while (_Item != NULL)
	{
		iCNum += __FindFile_Name(csName, _Item, iNum, hItem);
		_Item = m_wndFileView.GetNextItem(_Item, TVGN_NEXT);
	}

	_Name = _GetItemName(hParentItem);

	if (_Name.GetLength() < csName.GetLength())
		return iCNum;

	_Name = _Name.Mid(0, csName.GetLength());

	if (_Name.Compare(csName) == 0)
	{
		if (*iNum < 100)
			hItem[*iNum] = hParentItem;

		*iNum += 1;
		iCNum += 1;
	}


	return iCNum;

}

BOOL CFilesTreeView::SetSelectItem(HTREEITEM hItem)
{
	m_wndFileView.Select(hItem, TVGN_FIRSTVISIBLE);
	m_wndFileView.SelectItem(hItem);
//  m_wndFileView.SetItemState(hItem, TVIS_BOLD | TVIS_DROPHILITED, TVIS_BOLD | TVIS_DROPHILITED);
	return TRUE;
}

BOOL CFilesTreeView::SetBlod(int iNum, HTREEITEM* hItem)
{
	for (int i = 0; i < iNum; i++)
		m_wndFileView.SetItemState(hItem[i], TVIS_BOLD | TVIS_DROPHILITED, TVIS_BOLD | TVIS_DROPHILITED);

	return TRUE;

}

BOOL CFilesTreeView::ResetStatue(int iNum, HTREEITEM* hItem)
{
	for (int i = 0; i < iNum; i++)
	{
		m_wndFileView.SetItemState(hItem[i], 0, TVIS_BOLD );
		m_wndFileView.SetItemState(hItem[i], 0, TVIS_DROPHILITED);
	}
		


	return TRUE;

}

// 展开FileList
void CFilesTreeView::     _ExpandFilesTree(int iMode /*= 0*/)
{
	switch(iMode)
	{
	case 0:m_wndFileView.Expand(m_wndFileView.GetRootItem() ,TVE_EXPAND);break;
	default:break;
	}
}

BOOL CFilesTreeView::DeleteItem(HTREEITEM hItem)
{
	BOOL bRet = TRUE;
	if (m_wndFileView.GetNextItem(hItem,TVGN_CHILD)!= NULL)
		bRet = _SetItemName(hItem, _T("未出现目录"));
	else
		bRet = m_wndFileView.DeleteItem(hItem);
	return bRet;
}



