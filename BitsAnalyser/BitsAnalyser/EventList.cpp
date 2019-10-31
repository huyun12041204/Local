﻿// EventList.cpp: 实现文件
//

//#include "pch.h"
#include "stdafx.h"
#include "BitsAnalyser.h"
#include "EventList.h"
#include "..\..\AllUsedSource\BasicConvert\BasicConvert\BasicConvert.h"


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

//#define VIEW__MODE 


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

void CEventList::UpdateFonts()
{
	
	m_pEventList.SetFont(&afxGlobalData.fontRegular);

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

#ifdef VIEW__MODE
	return m_pEventList.GetCount();
#else
	return ___EentList.GetCount();
#endif // VIEW__MODE
	
}

#define DEF_EVENT_OVER  -1
#define DEF_EVENT_EMPTY  0
#define DEF_EVENT_SUCCESS      1
#define DEF_EVENT_SUCCESS_EXT  2
int CEventList::GetEvent(int iIndex, CString& csEvent, CString& csDescription)
{
	// TODO: 在此处添加实现代码.


#ifdef VIEW__MODE
	if (iIndex > m_pEventList.GetCount())
		return DEF_EVENT_OVER;

	_DeleteEnterSpace(csEvent);
	;
	m_pEventList.GetText(iIndex, csEvent);

	if (csEvent.IsEmpty())
		return DEF_EVENT_EMPTY;

	int iDes = csEvent.Find(_T("["));

	if (iDes <= 0)
	{
		csDescription.Empty();
		return DEF_EVENT_SUCCESS;
	}
	else
	{
		csDescription = csEvent.Mid(iDes + 1);
		csDescription = csDescription.Left(csDescription.GetLength() - 1);
		csEvent = csEvent.Mid(0, iDes);

	}

	return DEF_EVENT_SUCCESS_EXT;
#else
	if(iIndex<0)
		return DEF_EVENT_OVER;

	if (iIndex >= ___EentList.GetCount())
		return DEF_EVENT_OVER;

	_DeleteEnterSpace(csEvent);


	csEvent = ___EentList.GetAt(iIndex);

	if (csEvent.IsEmpty())
		return DEF_EVENT_EMPTY;

	int iDes = csEvent.Find(_T("["));

	if (iDes <= 0)
	{
		csDescription.Empty();
		return DEF_EVENT_SUCCESS;
	}
	else
	{
		csDescription = csEvent.Mid(iDes + 1);
		csDescription = csDescription.Left(csDescription.GetLength() - 1);
		csEvent = csEvent.Mid(0, iDes);

	}

	return DEF_EVENT_SUCCESS_EXT;
#endif // VIEW__MODE
}


int CEventList::AddEvent(CString csText)
{
	// TODO: 在此处添加实现代码.	
	

#ifdef VIEW__MODE
	return m_pEventList.AddString(csText);
#else
	return ___EentList.Add(csText);
#endif // VIEW__MODE

}


#define DEF_Des_Exsited 2

int CEventList::SeteEventDes(int iEventIndex, CString csDes)
{


#ifdef VIEW__MODE
	CString csEvent, csEventDes;
	int iRet = GetEvent(iEventIndex, csEvent, csEventDes);
	if (iRet != DEF_EVENT_SUCCESS)
		return iRet;

	//EventDes.Empty();
	//csEventDes = csEvent + _T("                    ");
	//csEventDes = csEventDes.Left(20);
	csDes = _T("[") + csDes + _T("]");

	csEvent = csEvent + csDes;



	m_pEventList.DeleteString(iEventIndex);
	m_pEventList.InsertString(iEventIndex, csEvent);

	return 1;
#else
	CString csEvent, csEventDes;
	int iRet = GetEvent(iEventIndex, csEvent, csEventDes);
	if (iRet != DEF_EVENT_SUCCESS)
		return iRet;

	csDes = _T("[") + csDes + _T("]");

	csEvent = csEvent + csDes;

	___EentList.RemoveAt(iEventIndex);
	___EentList.InsertAt(iEventIndex, csEvent);

	return 1;


#endif // VIEW__MODE


}

int CEventList::RemoveAllEvent(void)
{
#ifdef VIEW__MODE
	m_pEventList.ResetContent();
#else
	m_pEventList.ResetContent();
    ___EentList.RemoveAll();
#endif // VIEW__MODE

	
	return TRUE;
}

int CEventList::UpdateEventList(void)
{
#ifdef VIEW__MODE
	//m_pEventList.ResetContent();
#else
	
	for ( int i = m_pEventList.GetCount(); i < ___EentList.GetCount(); i++)
	{
		m_pEventList.AddString(___EentList.GetAt(i));
	}




#endif // VIEW__MODE


	return TRUE;
}

int CEventList::ShowEventList()
{
	m_pEventList.ResetContent();

	int iCount = ___EentList.GetCount();

	for (int i = 0 ; i< iCount; i++ )
		m_pEventList.AddString(___EentList.GetAt(i));

	return iCount;
}