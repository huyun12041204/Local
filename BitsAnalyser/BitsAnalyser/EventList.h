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
	CStringArray ___VCCList;

	
public:
	CListBox m_pEventList;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateFonts();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 获取一共有多少个事件
	int GetEventCount();
	int GetEvent(int iIndex, CString& csEvent, CString& csByte);
	int GetEvent(int iIndex, CString& csEvent, CString& csByte, CString& csType);
	int AddEvent(CString csText);
	int GetVCCForEvent(int iPos, BYTE* ucEvent);
	int SeteEventByte(int iEventIndex, CString csBYTE);
	int SetEventType(int iEventIndex, CString csType);
	int SetEventDescription(int iEventIndex, CString csByte, CString csType);
	int RemoveAllEvent(void);
	int UpdateEventList(void);
	int ShowEventList();
	int GetNextEvent(int iType, int iStart, CString& csEvent);
	int GetPreviousEvent(int iType, int iStart, CString& csEvent);
	ULONG GetSize();
};


