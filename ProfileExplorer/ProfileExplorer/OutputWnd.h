
#pragma once
#define DEF_MESSAGE_BUILT      01
#define DEF_MESSAGE_CHANGE_LOG 02
#define DEF_MESSAGE_DOWNLOAD   03
#define DEF_MESSAGE_DEBUG      04


#define  DEF_MESSAGE_NORMAL    0
#define  DEF_MESSAGE_HINT      1
#define  DEF_MESSAGE_WARNNING  2
#define  DEF_MESSAGE_ERROR     3
#define  DEF_MESSAGE_RIGHT     4


#define LBXI_CX_BORDER   2
#define LBXI_CY_BORDER   2

/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口

class COutputList : public CListBoxXI
{
// 构造
public:
	COutputList();
	BOOL bHINT;
	BOOL bWARN;
	BOOL bERROR;
	BOOL bRIGHT;

// 实现
public:
	virtual ~COutputList();
	int AddString(LPCTSTR lpszString, int nImageIndex);
//************************************
// Method:    AdjustHorzScroll
// FullName:  COutputList::AdjustHorzScroll
// Access:    protected 
// Returns:   void
// Qualifier: 增加水平滚动条
//************************************
	void AdjustHorzScroll();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnEditSave();
	afx_msg void OnOutputSetting(UINT iID);
	afx_msg void OnUpdateOutputSetting(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	void RemvoeAllSelect();
	int GetSelectFistItem();
	int GetSelectLastItem();
	void OnLButtonDown(UINT nFlags, CPoint point);


	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// 构造
public:
	COutputWnd();

	void UpdateFonts();

// 特性
protected:
	CMFCTabCtrl	m_wndTabs;
	CImageList  m_OutputImages;
	COutputList m_wndOutputBuild;
	COutputList m_wndOutputChange;
	COutputList m_wndOutputAll;

protected:

	void AdjustHorzScroll(CListBox& wndListBox);

// 实现
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	void DisplayMassege(CString csMessage, int iMessageTag = DEF_MESSAGE_NORMAL);
	void ResetContent();
	void SetImageList();
};

