
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "ProfileExplorer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_3D_VS2005, rectDummy, this, ID_OUTPUT_TAB))
	{
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}

	// 创建输出窗格:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | LBS_MULTIPLESEL |
		WS_CHILD | WS_VISIBLE | WS_HSCROLL |
		WS_VSCROLL | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS;
	//const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | LBS_MULTIPLESEL |
	//	WS_CHILD | WS_VISIBLE | WS_HSCROLL |
	//	WS_VSCROLL | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		

	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, ID_OUTPUT_LIST1) ||
		!m_wndOutputChange.Create(dwStyle, rectDummy, &m_wndTabs, ID_OUTPUT_LIST2) ||
		!m_wndOutputAll.Create(dwStyle, rectDummy, &m_wndTabs, ID_OUTPUT_LIST3))
	{
		TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}



	UpdateFonts();

	CString strTabName;
	//BOOL bNameValid;

	// 将列表窗口附加到选项卡:
	//bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	//ASSERT(bNameValid);
	strTabName = _T("提示");
	m_wndTabs.AddTab(&m_wndOutputBuild, strTabName);
	//bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	//ASSERT(bNameValid);
	strTabName = _T("操作");
	m_wndTabs.AddTab(&m_wndOutputChange, strTabName);
	//bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	//ASSERT(bNameValid);
	strTabName = _T("全部");
	m_wndTabs.AddTab(&m_wndOutputAll, strTabName);

	((CProfileExplorerApp*)AfxGetApp())->pcFile.InstallOutput(&m_wndOutputBuild, &m_wndOutputChange, &m_wndOutputAll);

	SetImageList();
	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::UpdateFonts()
{
	m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputChange.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputAll.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::DisplayMassege(CString csMessage, int iMessageTag)
{

	if (iMessageTag >= DEF_MESSAGE_HINT)
	{
	
		m_wndOutputBuild.AddString(csMessage, iMessageTag);
	}

	m_wndOutputAll.AddString(csMessage, iMessageTag);




}

void COutputWnd::ResetContent()
{
	m_wndOutputBuild.ResetContent();
	m_wndOutputChange.ResetContent();
	m_wndOutputAll .ResetContent();

}

void COutputWnd::SetImageList()
{


	m_OutputImages.DeleteImageList();

	//	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	UINT uiBmpId = IDB_OUTPUT_VIEW;

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

	m_OutputImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_OutputImages.Add(&bmp, RGB(0, 0, 0));

	m_wndOutputAll.SetImageList(&m_OutputImages);
	m_wndOutputChange.SetImageList(&m_OutputImages);
	m_wndOutputBuild.SetImageList(&m_OutputImages);
}



/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
	 bHINT  = TRUE;
	 bWARN  = TRUE;
	 bERROR = TRUE;
	 bRIGHT = TRUE;
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBoxXI)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_SAVE, &COutputList::OnEditSave)
    ON_COMMAND_RANGE(ID_HINT_VIEW, ID_RIGHT_VIEW, OnOutputSetting)
	ON_UPDATE_COMMAND_UI_RANGE(ID_HINT_VIEW, ID_RIGHT_VIEW,OnUpdateOutputSetting)
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SETFOCUS()

END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	int iIndex,iLen;
	CString csCopyData;
	CString csTemp;

	iIndex = GetCurSel();
	if (iIndex<0)
		return;

	int nCount = GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	GetSelItems(nCount, aryListBoxSel.GetData()); 
	AFXDUMP(aryListBoxSel);

	//int iSum = GetSelCount();
	csCopyData.Empty();
	for (int i = 0 ; i < nCount ; i++)
	{
		csTemp.Empty();
		iLen   = GetTextLen(aryListBoxSel.GetAt(i));
		GetText(aryListBoxSel.GetAt(i),csTemp.GetBuffer(iLen));
		csTemp.ReleaseBuffer();
		csTemp += _T("\r\n");
		csCopyData += csTemp;
	}



	if(OpenClipboard())
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, csCopyData.GetLength()+1); 
		buffer = (char*)GlobalLock(clipbuffer); 
		memcpy(buffer, csCopyData,csCopyData.GetLength()); 
		GlobalUnlock(clipbuffer); 
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();

	}
	else
		MessageBox(_T("复制失败"));
}
void COutputList::OnEditClear()
{
	COutputWnd* pWnd = (COutputWnd*) (this->GetParent()->GetParent());
	pWnd->ResetContent();

	//pWnd
	//CMainFrame* pMain = (CMainFrame*) AfxGetApp()->GetMainWnd();
	//if (pWnd == (pMain->_GetView(DEF_OutputWnd_View)))
	//	((COutputWnd*)pWnd) ->ResetContent();
	//else
	//	ResetContent();


	MessageBox(_T("清除完成"));
}
void COutputList::OnEditSave()
{
	CString csFileName;
	CString csFileData;
	CStdioFile CSFFile;
	int iIndex,iIndexLen;
	CFileDialog CFDataFile(FALSE,_T("*.txt|.txt"),NULL,4|2,_T("文本文件(*.txt)|*.txt||"));


	if ( CFDataFile.DoModal() == IDOK)
	{
		csFileName = CFDataFile.GetPathName();
		if (CSFFile.Open(csFileName,CFile::modeCreate|CFile::modeWrite))
		{

			iIndex = GetCount();

			for (int i= 0 ;i <iIndex; i++ )
			{
				iIndexLen = GetTextLen(i); 
				GetText(i,csFileData.GetBuffer(iIndexLen));
				csFileData.ReleaseBuffer();
				CSFFile.WriteString(csFileData);
				CSFFile.WriteString(_T("\n"));
			}
			CSFFile.Close();
		}
		else
			MessageBox(_T("保存失败"));

	}
}

 void COutputList::OnOutputSetting(UINT iID)
{

	 switch (iID)
	 {
	 case ID_HINT_VIEW:    bHINT  = !bHINT; 
		 SetImageStatue(DEF_HINT_TAG, bHINT ? _LISTBOX_XI_SHOW : _LISTBOX_XI_HIDE); break;
	 case ID_WARNING_VIEW: bWARN  = !bWARN; 
		 SetImageStatue(DEF_WARN_TAG, bWARN ? _LISTBOX_XI_SHOW : _LISTBOX_XI_HIDE); break;
	 case ID_ERROR_VIEW:   bERROR = !bERROR; 
		 SetImageStatue(DEF_ERROR_TAG, bERROR ? _LISTBOX_XI_SHOW : _LISTBOX_XI_HIDE); break;
	 case ID_RIGHT_VIEW:   bRIGHT = !bRIGHT; 
		 SetImageStatue(DEF_RIGHT_TAG, bRIGHT ? _LISTBOX_XI_SHOW : _LISTBOX_XI_HIDE); break;
	 }

	 for (int i = 0; i< GetCount(); i++)
	 {

		 switch (GetItemImage(i))
		 {
		 case DEF_HINT_TAG: SetItemStatue(i, bHINT  ? _LISTBOX_XI_SHOW  : _LISTBOX_XI_HIDE);  break;
		 case DEF_WARN_TAG: SetItemStatue(i, bWARN  ? _LISTBOX_XI_SHOW  : _LISTBOX_XI_HIDE);  break;
		 case DEF_ERROR_TAG:SetItemStatue(i, bERROR ? _LISTBOX_XI_SHOW : _LISTBOX_XI_HIDE);  break;
		 case DEF_RIGHT_TAG:SetItemStatue(i, bRIGHT ? _LISTBOX_XI_SHOW : _LISTBOX_XI_HIDE);  break;
		 default:
			 break;
		 }
	 }


}
 void COutputList::OnUpdateOutputSetting(CCmdUI* pCmdUI)
 {

	 switch (pCmdUI->m_nID)
	 {
	 case ID_HINT_VIEW:    pCmdUI->SetCheck(bHINT); break;
	 case ID_WARNING_VIEW: pCmdUI->SetCheck(bWARN); break;
	 case ID_ERROR_VIEW:   pCmdUI->SetCheck(bERROR); break;
	 case ID_RIGHT_VIEW:   pCmdUI->SetCheck(bRIGHT); break;
	 }


 }
void COutputList::RemvoeAllSelect()
{


	int nCount = GetSelCount();

	if (nCount <= 0)
		return ;

	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	GetSelItems(nCount, aryListBoxSel.GetData()); 
	AFXDUMP(aryListBoxSel);


	for (int i = 0 ; i < nCount ; i++)
	{
		SetSel(aryListBoxSel.GetAt(i),FALSE);
	}

}
int COutputList::GetSelectFistItem()
{
	int nCount = GetSelCount();
	if (nCount <=0)
		return -1;

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	GetSelItems(nCount, aryListBoxSel.GetData()); 
	AFXDUMP(aryListBoxSel);
	return aryListBoxSel.GetAt(0);
}
int COutputList::GetSelectLastItem()
{
	int nCount = GetSelCount();
	if (nCount <=0)
		return -1;

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	GetSelItems(nCount, aryListBoxSel.GetData()); 
	AFXDUMP(aryListBoxSel);
	return aryListBoxSel.GetAt(nCount-1);
}

void COutputList::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//此处需要加入Focus 否则滚轮会失效， 
	//必须把当前控件作为焦点
	SetFocus();

	BOOL bOut;
	int  iSelItem;
	iSelItem = CListBoxXI::ItemFromPoint(point, bOut);
	if (bOut)
	{
		RemvoeAllSelect();
		return;
	}

	if ((::GetKeyState(VK_CONTROL) >= 0) && ((::GetKeyState(VK_SHIFT) >= 0)))
		RemvoeAllSelect();
	if ((::GetKeyState(VK_SHIFT) < 0))
	{
		int iFirstSel = GetSelectFistItem();
		int iLastSel = GetSelectLastItem();

		if (iSelItem < iFirstSel)
			for (int i = iFirstSel; i >= iSelItem; i--)
				SetSel(i, TRUE);

		if (iLastSel < iSelItem)
			for (int i = iLastSel; i <= iSelItem; i++)
				SetSel(i, TRUE);
		return;
	}

	BOOL bSeled = GetSel(iSelItem);
	SetSel(iSelItem, !bSeled);

}

int COutputList::AddString(LPCTSTR lpszString, int nImageIndex)
{




	return CListBoxXI::_AddString(lpszString, nImageIndex);
}

void COutputList::AdjustHorzScroll()
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < CListBox::GetCount(); i++)
	{
		CString strItem;
		CListBox::GetText(i, strItem);
		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	CListBox::SetHorizontalExtent(cxExtentMax + 20);
	dc.SelectObject(pOldFont);
}
void COutputList::OnSetFocus(CWnd* pOldWnd)
{
	CListBoxXI::OnSetFocus(pOldWnd);

	AdjustHorzScroll();
}


