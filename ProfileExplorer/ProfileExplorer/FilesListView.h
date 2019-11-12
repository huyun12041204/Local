
#pragma once
#include "PrivateCardFileStruct.h"

// CFilesList

//class CFilesList : public CSkinListCtrl
class CFilesList : public CListCtrl
{
	DECLARE_DYNAMIC(CFilesList)

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);


	int _SetCurrentSelected(int iItem);

	int _SetCurrentFocused(int iItem);
	int GetColumnCount(void);
	int DeleteAllColumn(void);
	void AdjustColumnSize(int icx);
	
	//void DrawSubItem(CDC *pDC, int nItem, int nSubItem,CRect &rSubItem);
};

#pragma once

//#include "ViewTree.h"

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFilesListView : public CDockablePane
{
public:
	CFilesListView();
	virtual ~CFilesListView();

	void      AdjustLayout();
	void      OnChangeVisualStyle();
	int     __GetItemCount(void);
	int     __InsertItem(int nItem, LPCTSTR lpszItem);
	int     __SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	CString __GetItemText(int nItem,int nSubItem);
	int       GetFileItem(CString csRoute);
	BOOL      RemoveAllItem();


	//************************************
	// Method:    InsertFile
	// FullName:  CFilesListView::InsertFile
	// Access:    public 
	// Returns:   int ,成功为 TRUE,其他为失败，返回为0 to -n, n为序号
	// Qualifier: 安装一行，一行为一个文件
	// Parameter: PCFS File
	//************************************
	BOOL InsertFile(PCFS __File);
	BOOL ModifyFile(int iItem, PCFS __File);
	BOOL ResetStatue(int iNum, int * iItem);
	BOOL SetSelectItem(int iItem);
	BOOL SetBlod(int iNum, int* iItem);

	int _FindFile(CString csFID, int *iNum, int * iItem);

	int _FindFile_Name(CString csName, int *iNum, int * iItem);
	CString _GetFilePath(int iItem);
	int GetSelectItemPath(CStringArray& csPath);
	int DeleteItem(int iItem);
protected:
	CClassToolBar m_wndToolBar;
	CFilesList    m_wndFilesList;
	CImageList    m_FilesViewImages;
	UINT          m_nCurrSort;

// 重写
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	int InitializeFileListCtrl(void);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	afx_msg void OnFileOpen();
	afx_msg void OnFileCheck();
	afx_msg void OnFileDelete();
	afx_msg void OnFileExport();
	afx_msg void OnFileImport();
	afx_msg void OnFileRefresh();


	DECLARE_MESSAGE_MAP()
};

