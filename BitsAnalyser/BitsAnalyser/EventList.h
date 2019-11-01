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


	CStringArray ___EentList;



public:
	CListBox m_pEventList;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateFonts();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 获取一共有多少个事件
	int GetEventCount();
	int GetEvent(int iIndex, CString& csEvent, CString& csDescription);
	int AddEvent(CString csText);
	int SeteEventDes(int iEventIndex, CString csDes);
	int RemoveAllEvent(void);
	int UpdateEventList(void);
	int ShowEventList();
	ULONG GetSize();
};


