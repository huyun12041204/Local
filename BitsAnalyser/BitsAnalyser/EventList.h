#pragma once


// CEventList

class CEventList : public CDockablePane
{
	DECLARE_DYNAMIC(CEventList)

public:
	CEventList();
	virtual ~CEventList();

protected:
	DECLARE_MESSAGE_MAP()

	CListBox m_pEventList;



public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 获取一共有多少个事件
	int GetEventCount();
	int GetEvent(int iIndex, CString& csEvent, CString& csDescription);
	int AddEvent(CString csText);
};


