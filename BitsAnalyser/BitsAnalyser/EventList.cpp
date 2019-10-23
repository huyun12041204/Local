﻿// EventList.cpp: 实现文件
//

//#include "pch.h"
#include "stdafx.h"
#include "BitsAnalyser.h"
#include "EventList.h"


// CEventList

IMPLEMENT_DYNAMIC(CEventList, CDockablePane)

CEventList::CEventList()
{

}

CEventList::~CEventList()
{
}


BEGIN_MESSAGE_MAP(CEventList, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CEventList 消息处理程序




int CEventList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;


	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	m_pEventList.Create(dwStyle, rectDummy, this, IDC_EVENTLIST);
	

	return 0;
}


void CEventList::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_pEventList.SetWindowPos(this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	// TODO: 在此处添加消息处理程序代码
}


// 获取一共有多少个事件
int CEventList::GetEventCount()
{
	// TODO: 在此处添加实现代码.
	return m_pEventList.GetCount();
}

#define DEF_EVENT_OVER  -1
#define DEF_EVENT_EMPTY  0
#define DEF_EVENT_SUCCESS      1
#define DEF_EVENT_SUCCESS_EXT  2
int CEventList::GetEvent(int iIndex, CString& csEvent, CString& csDescription)
{
	// TODO: 在此处添加实现代码.

	if (iIndex > m_pEventList.GetCount())
		return DEF_EVENT_OVER;

	m_pEventList.GetText(iIndex, csEvent);

	if (csEvent.IsEmpty())
		return DEF_EVENT_EMPTY;

	int iDes = csEvent.Find(_T("-->"));

	if (iDes <=0)
	{
		csDescription.Empty();
		return DEF_EVENT_SUCCESS;
	}
	else
	{
		csDescription = csEvent.Mid(iDes + 3);
		csEvent = csEvent.Mid(0, iDes);
	}

	return DEF_EVENT_SUCCESS_EXT;
}


int CEventList::AddEvent(CString csText)
{
	// TODO: 在此处添加实现代码.	
	return m_pEventList.AddString(csText);
}
