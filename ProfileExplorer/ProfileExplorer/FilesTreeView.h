
#pragma once
#include "PrivateCardFileStruct.h"
/////////////////////////////////////////////////////////////////////////////
// CViewTree 窗口

class CViewTree : public CTreeCtrl
{
	// 构造
public:
	CViewTree();
	virtual     ~CViewTree();

	// 重写
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// 实现
public:

	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()

	//操作
public:
	CString   GetFilePath(HTREEITEM hItem);
	CString __GetItemFID(HTREEITEM hItem);
	UINT     _GetPointItem(HTREEITEM* hItem);
	void    __FileOpen();
};

#pragma once

//#include "ViewTree.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFilesTreeView : public CDockablePane
{
// 构造
public:
	CFilesTreeView();
	virtual ~CFilesTreeView();
	void AdjustLayout();
	void OnChangeVisualStyle();

	int GetFileImage(int iFileType);
	// 特性
protected:

	CViewTree        m_wndFileView;
	CImageList       m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

protected:

// 实现
public:


protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileCheck();
	afx_msg void OnFileDelete();
	afx_msg void OnFileExport();
	afx_msg void OnFileImport();
	afx_msg void OnFileRefresh();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()


//操作
public:
	int       _InsertFile(int iFID, CString csDescription,int iStructure, int iState, HTREEITEM hPareItem=TVI_ROOT, HTREEITEM* hFileItem=NULL);
	int        InsertFile(int iFID, CString csDescription, CString csRoute, int iStructure, int iStatus);
	//************************************
	// Method:    InsertFile
	// FullName:  CFilesTreeView::InsertFile
	// Access:    public 
	// Returns:   int 失败返回 -1 ，成功返回TRUE
	// Qualifier:
	// Parameter: PCFS File
	//************************************
	int        InsertFile(PCFS File);
	int        DeleteAllItem(void);

	HTREEITEM  GetRouteItem(CString csRoute);
	HTREEITEM _GetItem(int iFID,HTREEITEM hParentItem);
	HTREEITEM _GetItem(CString csPath);

	HTREEITEM  GetRootItem();



	CString   _GetItemFID(HTREEITEM hItem);

	BOOL      _SetItemFID(HTREEITEM hItem, CString csFID);
	CString   _GetItemName(HTREEITEM hItem);
	BOOL      _SetItemName(HTREEITEM hItem, CString csName);

	BOOL      ModifyFile(HTREEITEM hItem, PCFS __File);

	BOOL     SetSelectItem(HTREEITEM hItem);
	BOOL     SetBlod(int iNum, HTREEITEM* hItem);
	BOOL     ResetStatue(int iNum, HTREEITEM* hItem);


	BOOL DeleteItem(HTREEITEM hItem);
	int _FindFile(CString csFID, int *iNum, HTREEITEM* hItem);
	int __FindFile(CString csFID, HTREEITEM hParentItem, int* iNum, HTREEITEM * hItem);

	int _FindFile_Name(CString csName, int *iNum, HTREEITEM* hItem);
	int __FindFile_Name(CString csName, HTREEITEM hParentItem, int* iNum, HTREEITEM * hItem);
	// 展开FileList
	void     _ExpandFilesTree(int iMode = 0);
};

