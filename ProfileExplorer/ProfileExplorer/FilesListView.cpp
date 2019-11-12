
#include "stdafx.h"
#include "MainFrm.h"
#include "FilesListView.h"
#include "Resource.h"
#include "ProfileExplorer.h"
//#include "FileProperties.h"
//#include "PrivateCardFileStruct.h"



// CFilesList

//IMPLEMENT_DYNAMIC(CFilesList, CSkinListCtrl)
IMPLEMENT_DYNAMIC(CFilesList, CListCtrl)
	//BEGIN_MESSAGE_MAP(CFilesList, CSkinListCtrl)
	BEGIN_MESSAGE_MAP(CFilesList, CListCtrl)
		ON_NOTIFY_REFLECT(NM_CLICK, &CFilesList::OnNMClick)
		ON_NOTIFY_REFLECT(NM_DBLCLK, &CFilesList::OnNMDblclk)
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CFilesList::OnNMCustomdraw)
	END_MESSAGE_MAP()
	// CFilesList 消息处理程序
	void CFilesList::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		// TODO: 在此添加控件通知处理程序代码

		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		CMainFrame * CurMainFrm  = (CMainFrame*) AfxGetApp()->GetMainWnd();

		int iItem = pNMListView->iItem;

		UINT iStatus = GetItemState(iItem , LVNI_STATEMASK);

//#ifdef _DEBUG
//		CurMainFrm->DisplayMassege(_T("->Select Item ")+ Int2CStringDecimal(iItem));
//#endif // _DEBUG
//
//		if(iItem!=-1)
//		{
//			CurMainFrm->_InputInformation2Properties((long)iItem );
//			CurMainFrm->_InputInformation2PageFile  ((long)iItem );	
//		}

	}
	void CFilesList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		// TODO: 在此添加控件通知处理程序代码
		//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		CString csTitle;

		int  nItem =   pNMListView->iItem;

		//UINT iStatus = GetItemState(nItem , LVNI_STATEMASK);
		
		if(nItem!=-1)
		{
			csTitle = GetItemText(nItem,DEF_FILELIST_ROUTE_COLUMN)+GetItemText(nItem,DEF_FILELIST_FID_COLUMN);
		}


		((CProfileExplorerApp*)AfxGetApp())->OpenDocumentFile(_T("#")+csTitle);

		*pResult = 0;
	}
	void CFilesList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
	{
		//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
		//LPNMLVCUSTOMDRAW lpnmcd = (LPNMLVCUSTOMDRAW) pNMHDR;  


		LPNMLVCUSTOMDRAW lpnmcd = (LPNMLVCUSTOMDRAW)pNMHDR;
		if (lpnmcd->nmcd.dwDrawStage == CDDS_PREPAINT)
		{
			*pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
			//	SetSelectedSumItem(-1);
			return;
		}
		else if (lpnmcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			return;
		}
		else if (lpnmcd->nmcd.dwDrawStage == (CDDS_SUBITEM | CDDS_ITEMPREPAINT))
		{

			*pResult = CDRF_DODEFAULT;
			int iItem = (int)lpnmcd->nmcd.dwItemSpec;
			//	int iSubItem = lpnmcd->iSubItem;  

			if ((iItem % 2) == 1)
				lpnmcd->clrTextBk = RGB(209, 230, 255);


			CFont font;
			font.CreateFont(12,   // nHeight  
				0,                         // nWidth  
				0,                         // nEscapement  
				0,                         // nOrientation  
				FW_THIN,                 // nWeight  
				FALSE,                     // bItalic  
				FALSE,                     // bUnderline  
				0,                         // cStrikeOut  
				ANSI_CHARSET,              // nCharSet  
				OUT_DEFAULT_PRECIS,        // nOutPrecision  
				CLIP_DEFAULT_PRECIS,       // nClipPrecision  
				DEFAULT_QUALITY,           // nQuality  
				DEFAULT_PITCH,  // nPitchAndFamily  
				_T("宋体"));

			CDC dc;
			dc.Attach(lpnmcd->nmcd.hdc);
			//	DrawSubItem(&dc,iItem,iSubItem,rSubItem);
			dc.SelectObject(&font);
			dc.Detach();
			return;
		}

		*pResult = 0;

	}



	// 删除所有的列
	int CFilesList::DeleteAllColumn(void)
	{
		int iSum;
		BOOL bRet;
		bRet = TRUE;

		iSum =GetColumnCount();

		for (int i = 0 ; i < iSum ; i++)
			bRet &= DeleteColumn(i);

		return bRet;
	}
	// 获取列的数目
	int CFilesList::GetColumnCount(void)
	{
		int iRet = 0;
		CHeaderCtrl* cTemp;
		cTemp = GetHeaderCtrl() ;
		if (cTemp!= NULL)
			iRet = cTemp->GetItemCount();
		return iRet;
	}



	int CFilesList::_SetCurrentSelected(int iItem)
	{
		if (iItem <0)
			return -1;
		//int iSum = GetItemCount();
		//for (int i = 0; i < iSum; i++)
		//	SetItemState(i, LVNI_ALL, LVNI_STATEMASK);

		//SetItemState(iItem,LVNI_FOCUSED |LVIS_DROPHILITED, LVNI_STATEMASK);
		SetSelectionMark(iItem);

		return _SetCurrentFocused(iItem);
	}


	int CFilesList::_SetCurrentFocused(int iItem)
	{
		if (iItem < 0)
			return -1;
		//int iSum = GetItemCount();
		//for (int i = 0; i < iSum; i++)
		//	SetItemState(i, LVNI_ALL, LVNI_STATEMASK);

		SetItemState(iItem, LVNI_FOCUSED | LVIS_DROPHILITED, LVNI_STATEMASK);

		//取出全View 的大小, 计算出一半处的位置,以及Item数
		CRect cRectt;
		GetWindowRect(&cRectt);
		int iTop = cRectt.bottom - cRectt.top;
		iTop = iTop - 23;
		iTop = iTop / 2;
		iTop = iTop / 18;

		CPoint cPoint;
		if (iItem >= iTop)
			GetItemPosition(iItem - iTop, &cPoint);
		else
			GetItemPosition(0, &cPoint);

		Scroll(CSize(0, cPoint.y - 23));

		return 0;
	}
	// 调整Column 大小
	void CFilesList::AdjustColumnSize(int icx)
	{

		LVCOLUMN pColumn1;
		int iColSum;
		UINT* uiX;
		int i;

		iColSum = ((CHeaderCtrl*)GetHeaderCtrl())->GetItemCount();

		uiX     = new UINT[iColSum];
		ZeroMemory(uiX,iColSum+5);
		icx     = icx;

		if (icx < 70*iColSum)
		{

			uiX[0]     =  icx/iColSum;
			for ( i = 1 ; i<(iColSum-1) ; i++)
				uiX[i] =  uiX[0];
			uiX[iColSum-1] =  icx - (uiX[0]*(iColSum-1));

		}
		else if(iColSum>1)
		{
			uiX[0]     =  70;
			uiX[1]     =  (icx - 70)/(iColSum-1) ;

			for ( i = 2 ; i<(iColSum-1) ; i++)
				uiX[i] =  uiX[1];
			if (iColSum>2) 
				uiX[iColSum-1] =  icx - 70 - (uiX[1]*(iColSum-2));


		}
		else
			uiX[0]     =  icx;

		for (i = 0 ; i < iColSum; i ++)
		{
			pColumn1.mask = LVCF_WIDTH;
			if (GetColumn(i,&pColumn1))
			{
				pColumn1.cx = uiX[i];
				SetColumn(i, &pColumn1);
			}
		}

		delete uiX;
	}
	//void CFilesList::DrawSubItem(CDC *pDC, int nItem, int nSubItem,CRect &rSubItem)  
	//{  
	//	//pDC->SetBkMode(TRANSPARENT);  
	//	//pDC->SetTextColor(RGB(0, 0, 0));  
	//	CFont font;  
	//	font.CreateFont(12,   // nHeight  
	//		0,                         // nWidth  
	//		0,                         // nEscapement  
	//		0,                         // nOrientation  
	//		FW_NORMAL,                 // nWeight  
	//		FALSE,                     // bItalic  
	//		FALSE,                     // bUnderline  
	//		0,                         // cStrikeOut  
	//		ANSI_CHARSET,              // nCharSet  
	//		OUT_DEFAULT_PRECIS,        // nOutPrecision  
	//		CLIP_DEFAULT_PRECIS,       // nClipPrecision  
	//		DEFAULT_QUALITY,           // nQuality  
	//		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily  
	//		_T("宋体"));  
	//	pDC->SelectObject(&font);  
	//	CString strText;  
	//	strText = GetItemText(nItem, nSubItem);  
	//	//取出扩展格式进行提取设置
	//	//	DWORD dwStyle = GetExtendedStyle();
	//	UINT  uiStyle;
	//	//uiStyle = DT_SINGLELINE| DT_VCENTER| DT_END_ELLIPSIS;
	//	////居中 居左 居右
	//	//switch(dwStyle&0x0003)
	//	//{
	//	//case LVCFMT_LEFT: uiStyle |= DT_LEFT;
	//	//case LVCFMT_RIGHT: uiStyle |= DT_RIGHT;
	//	//case LVCFMT_CENTER: uiStyle |= DT_CENTER;
	//	//case LVCFMT_JUSTIFYMASK: uiStyle |= DT_LEFT;
	//	//}
	//	uiStyle = DT_SINGLELINE| DT_VCENTER| DT_END_ELLIPSIS|DT_CENTER;
	//	//pDC->DrawText(strText, strText.GetLength(), &rSubItem, uiStyle);
	//	pDC->DrawText(_T("0"), 1, &rSubItem, uiStyle);
	//}  



class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CFilesListView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

CFilesListView::CFilesListView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CFilesListView::~CFilesListView()
{
}

BEGIN_MESSAGE_MAP(CFilesListView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)

	ON_COMMAND(ID_FILETREE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILETREE_CHECK, OnFileCheck)
	ON_COMMAND(ID_FILETREE_DELETE, OnFileDelete)
	ON_COMMAND(ID_FILETREE_EXPORT, OnFileExport)
	ON_COMMAND(ID_FILETREE_INPORT, OnFileImport)
	ON_COMMAND(ID_FILETREE_REFRESH, OnFileRefresh)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int CFilesListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER  | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;


	if (!m_wndFilesList.Create(dwStyle,rectDummy,this,1))
	{
		TRACE0("创建自绘的 list 控件失败");
		return -1;    
	}
	else

		InitializeFileListCtrl();

	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}



	return 0;
}
int CFilesListView::InitializeFileListCtrl(void)
{
	LONG lStyle;
	lStyle = GetWindowLong(m_wndFilesList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	lStyle |= LVS_SHOWSELALWAYS; //设置style
	//lStyle |= LVS_LIST; //设置style
	//lStyle |=LVS_AUTOARRANGE ;
	SetWindowLong(m_wndFilesList.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_wndFilesList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	 dwStyle |= LVS_EX_HEADERDRAGDROP;


	//dwStyle |=LVCFMT_CENTER;
	//dwStyle1 |= LVS_EX_CHECKBOXES;
	//LVCFMT_CENTER

	m_wndFilesList.SetExtendedStyle(dwStyle); //设置扩展风格
	//先删除所有的内容
	m_wndFilesList.DeleteAllItems();
	m_wndFilesList.DeleteAllColumn();

	m_wndFilesList.InsertColumn(DEF_FILELIST_HANDLE_COLUMN    ,_T("NUMBER"),LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_FID_COLUMN       ,_T("FID")  ,LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_NAME_COLUMN      ,_T("NAME"),LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_ROUTE_COLUMN     ,_T("路径"),LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_TYPE_COLUMN      ,_T("类别"),LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_STRUCTURE_COLUMN ,_T("结构"),LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_STATUS_COLUMN    ,_T("状态"),LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_SIZE_COLUMN      ,_T("大小"),LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_RECORD_NUMBER_COLUMN ,_T("记录条数"),LVCFMT_CENTER,20);
	m_wndFilesList.InsertColumn(DEF_FILELIST_RECORD_LENGTH_COLUMN ,_T("记录大小"),LVCFMT_CENTER,20);

	//SetListCtrlFont();



	return 0;
}

void CFilesListView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
	m_wndFilesList.AdjustColumnSize(cx);
}


void CFilesListView::OnContextMenu(CWnd* pWnd, CPoint point)
{

	CListCtrl* pWndList = &m_wndFilesList;
	ASSERT_VALID(pWndList);

	if (pWnd != pWndList)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptList = point;
		pWndList->ScreenToClient(&ptList);

		UINT flags = 0;

		int iItem = pWndList->HitTest(ptList, &flags);
		//HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (iItem >= 0)
		{
			pWndList->SetSelectionMark(iItem);
		}
	}

	pWndList->SetFocus();
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_FILELIST, point.x, point.y, this, TRUE);
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_FILELIST);

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

void CFilesListView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFilesList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);

}

BOOL CFilesListView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CFilesListView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CFilesListView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CFilesListView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

void CFilesListView::OnClassAddMemberVariable()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFilesListView::OnClassDefinition()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFilesListView::OnClassProperties()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFilesListView::OnNewFolder()
{
	AfxMessageBox(_T("新建文件夹..."));
}

void CFilesListView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndFilesList.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFilesListView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	//m_FileViewImages.SetFocus();
}

void CFilesListView::OnChangeVisualStyle()
{
	m_FilesViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

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

	m_FilesViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FilesViewImages.Add(&bmp, RGB(255, 0, 0));

//	m_ctrlFilesList.SetImageList(&m_FilesViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 锁定*/);
}

void CFilesListView::OnFileOpen()
{

	int iItem = m_wndFilesList.GetSelectionMark();

	if (iItem < 0)
		return;
	CString csPath = _GetFilePath(iItem);

	if (csPath.IsEmpty())
		return;

	((CProfileExplorerApp*)AfxGetApp())->OpenDocumentFile(_T("#") + csPath);
}

void CFilesListView::OnFileCheck()
{
	int iItem = m_wndFilesList.GetSelectionMark();
	if (iItem < 0)
		return;
	CString csPath = _GetFilePath(iItem);

	if (csPath.IsEmpty())
		return;

	PCFS __FIle;
	ProfileClass* __Profile;
	CStringArray csResult;
	CString csFile;
	int iErrorNumber = 0;
	__Profile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	if (__Profile->GetFileFromDoc(csPath, &__FIle) != DEF_PROFILE_SUCCESS)
		return;

	iErrorNumber = __FIle.CheckParameter(csResult);

	CMainFrame* pMain;
	pMain = (CMainFrame*)AfxGetMainWnd();

	for (int i = 0; i < csResult.GetCount(); i++)
		pMain->_Display2Output(csResult.GetAt(i), DEF_ERROR_TAG);


	if ((__FIle.uiTYPE != DEF_FILETYPE_EF) &&
		(!__Profile->CheckDFSFI(csPath)))
		iErrorNumber += 1;


	if (iErrorNumber == 0)
		pMain->_Display2Output(_T("文件") + csPath + _T(" 没有存在错误！"), DEF_MESSAGE_RIGHT);
	else
		pMain->_Display2Output(_T("文件") + csPath + _T(" 存在 ") + Int2CStringDecimal(iErrorNumber) + _T(" 错误！"), DEF_MESSAGE_ERROR);


}

void CFilesListView::OnFileDelete()
{
	int iItem = m_wndFilesList.GetSelectionMark();

	if (iItem < 0)
		return;
	CString csPath = _GetFilePath(iItem);

	if (csPath.IsEmpty())
		return;

	PCFS __FIle;
	ProfileClass* __Profile;
	__Profile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;
	if (__Profile->GetFileFromDoc(csPath, &__FIle) != DEF_PROFILE_SUCCESS)
		return;


	long lRecord = __Profile->GetFileRecord(csPath);
	CMainFrame* pMain;
	pMain = (CMainFrame*)AfxGetMainWnd();
	if ((lRecord > 0) &&
		(__Profile->DeleteDocRecord(lRecord)))
	{
		pMain->_Display2Output(_T("文件") + csPath + _T(" 删除成功！"), DEF_MESSAGE_RIGHT);

		DeleteItem(iItem);

		//以下为同步代码
		CFilesTreeView* p_wndList = (CFilesTreeView*)pMain->GetDockablePane(ID_VIEW_FILETREE);
		if (p_wndList->IsKindOf(RUNTIME_CLASS(CFilesTreeView)))
		{
			HTREEITEM hItem = p_wndList->_GetItem(csPath);
			if (hItem !=NULL)
				p_wndList->DeleteItem(hItem);
		}
	}

}

void CFilesListView::OnFileExport()
{
	//CMainFrame* pMain;
	//CStringArray csArray;
	//CString _FilePath;
	//CString csNewMDBFile;



	//int iItem = m_wndFilesList.GetSelectionMark();

	//if (iItem < 0)
	//	return;
	//CString csPath = _GetFilePath(iItem);

	//if (csPath.IsEmpty())
	//	return;

	//csArray.Add(_FilePath);
	//pMain = (CMainFrame*)AfxGetMainWnd();


	//__GetCurrentDIR(csNewMDBFile);

	//csNewMDBFile = csNewMDBFile + _T("\\") + _FilePath + _T(".mdb");

	//pMain->__ExportFiles(csArray, csNewMDBFile);

}

void CFilesListView::OnFileImport()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	pMain->OnImportFiles();
}

void CFilesListView::OnFileRefresh()
{


	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->NewInsertProfileThread();

}


int CFilesListView::__GetItemCount(void)
{
	return m_wndFilesList.GetItemCount();
}
int CFilesListView::__InsertItem(int nItem, LPCTSTR lpszItem)
{
	return m_wndFilesList.InsertItem(nItem,lpszItem);
}
int CFilesListView::__SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{

	if ((nItem<m_wndFilesList.GetItemCount())&&
		(nSubItem < m_wndFilesList.GetColumnCount()))
		return m_wndFilesList.SetItemText(nItem,nSubItem,lpszText);

	return FALSE;

}
CString CFilesListView::__GetItemText(int nItem,int nSubItem)
{
	return m_wndFilesList.GetItemText(nItem,nSubItem);
}

// 通过FID 获取到文件Number (传说的中的handle)
int CFilesListView::GetFileItem(CString csRoute)
{
	if (csRoute.IsEmpty())
		return -1;

	int iItem = m_wndFilesList.GetItemCount();
	CString csFID,csParent;


	for(int i = 0 ; i < iItem ; i++)
	{
		csFID    = m_wndFilesList.GetItemText(i,DEF_FILELIST_FID_COLUMN);
		csParent = m_wndFilesList.GetItemText(i,DEF_FILELIST_ROUTE_COLUMN);
		csFID    = csParent+csFID;

		csFID  .MakeUpper();
		csRoute.MakeUpper();

		if (csFID.Compare(csRoute) == 0)		
			return _CString2IntDecimal(m_wndFilesList.GetItemText(i,DEF_FILELIST_HANDLE_COLUMN));

	}

	return -1;
}

BOOL CFilesListView::RemoveAllItem()
{
	return m_wndFilesList.DeleteAllItems();

}

BOOL CFilesListView::InsertFile(PCFS __File)
{
	int iCount = GetFileItem(__File.stROUTE + Int2CString(__File.uiFID));
	if (iCount <0)
	{
		iCount = __GetItemCount();
		if(__InsertItem(iCount, Int2CStringDecimal(__File.uiHANDLE)) == -1)
			return FALSE;
	}
	return ModifyFile(iCount, __File);

}

BOOL CFilesListView::ModifyFile(int iItem, PCFS __File)
{
	_variant_t vbuffer;
	BOOL bUsed;
	for (int i = DEF_FILELIST_FID_COLUMN; i <= DEF_FILELIST_RECORD_LENGTH_COLUMN; i++)
	{
		bUsed = FALSE;
		vbuffer = __File.GetPrivateCardFileStructData(i, &bUsed);
		if ((vbuffer.vt != VT_NULL) &&
			(bUsed) &&
			(__SetItemText(iItem, i, (LPCTSTR)(_bstr_t)vbuffer) == FALSE))
			 return FALSE;
	}

	return TRUE;
}

BOOL CFilesListView::ResetStatue(int iNum, int* iItem)
{
	for(int i = 0 ; i< iNum; i++)
		m_wndFilesList.SetItemState(iItem[i], 0, LVIS_DROPHILITED);

	return TRUE;
}

BOOL CFilesListView::SetSelectItem(int iItem)
{
	m_wndFilesList._SetCurrentFocused(iItem);
//	m_wndFilesList.SetItemState(iItem, LVIS_SELECTED , LVIS_SELECTED);
	return TRUE;
}

BOOL CFilesListView::SetBlod(int iNum, int* iItem)
{
	for (int i = 0; i < iNum; i++)
		m_wndFilesList.SetItemState(iItem[i], LVIS_DROPHILITED, LVIS_DROPHILITED);

	return TRUE;

}


int CFilesListView::_FindFile(CString csFID, int *iNum, int * iItem)
{
	csFID.MakeUpper();

	int iCount = m_wndFilesList.GetItemCount();
	*iNum = 0;
	ZeroMemory(iItem, sizeof(iItem));
	for (int i = 0 ; i< iCount; i++)
	{

		if (csFID.Compare(__GetItemText(i, DEF_FILELIST_FID_COLUMN)) == 0)
		{
			if (*iNum <100)
				iItem[*iNum] = i;	
			*iNum += 1;
		}
	}

	return *iNum;
}

int CFilesListView::_FindFile_Name(CString csName, int *iNum, int * iItem)
{

	int iCount = m_wndFilesList.GetItemCount();
	*iNum = 0;
	ZeroMemory(iItem, sizeof(iItem));
	CString _Name;
	for (int i = 0; i < iCount; i++)
	{
		_Name = __GetItemText(i, DEF_FILELIST_NAME_COLUMN);
		if (_Name.GetLength() < csName.GetLength())
			continue;
		_Name = _Name.Mid(0, csName.GetLength());
		if (csName.Compare(_Name) == 0)
		{
			if (*iNum < 100)
				iItem[*iNum] = i;
			*iNum += 1;
		}
	}

	return *iNum;
}
CString CFilesListView::_GetFilePath(int iItem)
{
	CString csFID   = __GetItemText(iItem, DEF_FILELIST_FID_COLUMN);
	CString csRoute = __GetItemText(iItem, DEF_FILELIST_ROUTE_COLUMN);

	return csRoute + csFID;
}

int CFilesListView::GetSelectItemPath(CStringArray& csPath)
{

	int iCount = __GetItemCount();
	int iRet = 0;
	for (int i = 0 ; i < iCount; i++)
	{

		if (m_wndFilesList.GetItemState(i, LVIS_SELECTED) ==LVIS_SELECTED)
		{
			csPath.Add(_GetFilePath(i));
			iRet++;
		}
	}

	return iRet;
}

int CFilesListView::DeleteItem(int iItem)
{
	BOOL bRet = TRUE;
	bRet = m_wndFilesList.DeleteItem(iItem);

	for (int i = iItem; i < m_wndFilesList.GetItemCount(); i++)
	{
		if (!__SetItemText(i, DEF_FILELIST_HANDLE_COLUMN, Int2CStringDecimal(i + 1)))
		{
			bRet = FALSE;
		}
	}
	return bRet;
}
