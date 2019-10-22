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



public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


