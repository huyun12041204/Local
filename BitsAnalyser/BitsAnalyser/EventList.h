#pragma once


#define DEF_EVENT_OVER  -1
#define DEF_EVENT_EMPTY  0
#define DEF_EVENT_SUCCESS       1
#define DEF_EVENT_SUCCESS_EXT   2
#define DEF_EVENT_SUCCESS_EXT1  2
#define DEF_EVENT_SUCCESS_EXT2  4
#define DEF_EVENT_SUCCESS_EXT3  8
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
	// ��ȡһ���ж��ٸ��¼�
	int GetEventCount();
	//int GetEvent(int iIndex, CString& csEvent, CString& csByte);
	int GetEvent(int iIndex, CString& csEvent, CString& csByte, CString& csType, CString& csOffset);
	int AddEvent(CString csText);
	int GetVCCForEvent(int iPos, BYTE* ucEvent);
	int SeteEventByte(int iEventIndex, CString csBYTE);
	int SetEventType(int iEventIndex, CString csType);
	int SetEventDescription(int iEventIndex, CString csByte, CString csType, CString csOffset);
	int RemoveAllEvent(void);
	int UpdateEventList(void);
	int ShowEventList();
	int GetNextEvent(int iType, int iStart, CString& csEvent);
	int GetPreviousEvent(int iType, int iStart, CString& csEvent);
	ULONG GetSize();
};


