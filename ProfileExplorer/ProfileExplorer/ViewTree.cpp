
#include "stdafx.h"
#include "FilesTreeView.h"

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
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;

	//NM_TREEVIEW * pHdr = (NM_TREEVIEW*)pNMHDR;
	//CPoint cpPoint;
	//UINT uFlag ;
	//HTREEITEM hItem;
	//CMainFrame *CurMainFrm;
	////	CCardFilesView* CurView;
	//CString csRount;

	//GetCursorPos(&cpPoint);
	//CTreeCtrl::ScreenToClient(&cpPoint);
	//hItem = CTreeCtrl::HitTest(cpPoint,&uFlag);
	//if (hItem) 
	//{


	//	if (TVHT_ONITEM&uFlag)
	//	{
	//		csRount = GetFileRoute(hItem);
	//		CurMainFrm = (CMainFrame*)AfxGetApp()->GetMainWnd();
	//		CurMainFrm->SelectFileInView(csRount);


	//	}

	//}

}

// 通过 ITEM 来获取 Route
CString CViewTree::GetFileRoute(HTREEITEM hItem)
{
	CString csTemp;
	csTemp.Empty();

	while(hItem != NULL)
	{
		csTemp = __GetItemFID(hItem)+csTemp;
		hItem = GetNextItem(hItem,TVGN_PARENT);
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


void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

//	NM_TREEVIEW * pHdr = (NM_TREEVIEW*)pNMHDR;
	//CPoint cpPoint;
	//UINT uFlag ;
	//HTREEITEM hItem;
	//CString csRount;
	//GetCursorPos(&cpPoint);
	//CTreeCtrl::ScreenToClient(&cpPoint);
	//hItem = CTreeCtrl::HitTest(cpPoint,&uFlag);
	CString csRount;
	HTREEITEM hItem;
	if ((_GetPointItem(&hItem)&TVHT_ONITEM)&&(hItem!=NULL))
	{
		csRount = GetFileRoute(hItem);

		((CMainFrame*)AfxGetApp()->GetMainWnd())->_GetView(DEF_FilesList_View)->GetNumber(csRount)


	}
}
