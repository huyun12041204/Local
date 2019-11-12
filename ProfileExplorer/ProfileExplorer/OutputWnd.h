
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
// COutputList ����

class COutputList : public CListBoxXI
{
// ����
public:
	COutputList();
	BOOL bHINT;
	BOOL bWARN;
	BOOL bERROR;
	BOOL bRIGHT;

// ʵ��
public:
	virtual ~COutputList();
	int AddString(LPCTSTR lpszString, int nImageIndex);
//************************************
// Method:    AdjustHorzScroll
// FullName:  COutputList::AdjustHorzScroll
// Access:    protected 
// Returns:   void
// Qualifier: ����ˮƽ������
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
// ����
public:
	COutputWnd();

	void UpdateFonts();

// ����
protected:
	CMFCTabCtrl	m_wndTabs;
	CImageList  m_OutputImages;
	COutputList m_wndOutputBuild;
	COutputList m_wndOutputChange;
	COutputList m_wndOutputAll;

protected:

	void AdjustHorzScroll(CListBox& wndListBox);

// ʵ��
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

