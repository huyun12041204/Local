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
	
#ifdef VIEW__MODE
	m_pEventList.SetRedraw(FALSE);
#endif

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


//int CEventList::GetEvent(int iIndex, CString& csEvent, CString& csByte)
//{
//	// TODO: 在此处添加实现代码.
//
//
//#ifdef VIEW__MODE
//	if (iIndex > m_pEventList.GetCount())
//		return DEF_EVENT_OVER;
//
//	_DeleteEnterSpace(csEvent);
//	;
//	m_pEventList.GetText(iIndex, csEvent);
//
//	if (csEvent.IsEmpty())
//		return DEF_EVENT_EMPTY;
//
//	int iDes = csEvent.Find(_T("["));
//
//	if (iDes <= 0)
//	{
//		csByte.Empty();
//		return DEF_EVENT_SUCCESS;
//	}
//	else
//	{
//		csByte = csEvent.Mid(iDes + 1);
//		csByte = csByte.Left(csByte.GetLength() - 1);
//		csEvent = csEvent.Mid(0, iDes);
//
//	}
//
//	return DEF_EVENT_SUCCESS_EXT;
//#else
//	if(iIndex<0)
//		return DEF_EVENT_OVER;
//
//	if (iIndex >= ___EentList.GetCount())
//		return DEF_EVENT_OVER;
//
//	_DeleteEnterSpace(csEvent);
//
//
//	csEvent = ___EentList.GetAt(iIndex);
//
//	if (csEvent.IsEmpty())
//		return DEF_EVENT_EMPTY;
//
//	int iDes = csEvent.Find(_T("["));
//
//	if (iDes <= 0)
//	{
//		csByte.Empty();
//		return DEF_EVENT_SUCCESS;
//	}
//	else 
//	{
//		csByte = csEvent.Mid(iDes + 1,2);
//		//csByte = csByte.Left(csByte.GetLength() - 1);
//		csEvent = csEvent.Mid(0, iDes);
//
//	}
//
//	return DEF_EVENT_SUCCESS_EXT;
//#endif // VIEW__MODE
//}


//************************************
// Method:    GetEvent
// FullName:  CEventList::GetEvent
// Access:    public 
// Returns:   int  
//DEF_EVENT_OVER,
//DEF_EVENT_EMPTY,
//DEF_EVENT_SUCCESS
//DEF_EVENT_SUCCESS_EXT1
//DEF_EVENT_SUCCESS_EXT2
// Qualifier:
// Parameter: int iIndex 
// Parameter: CString & csEvent
// Parameter: CString & csByte
// Parameter: CString & csType
//************************************
int CEventList::GetEvent(int iIndex, CString& csEvent, CString& csByte,CString& csType, CString& csOffset)
{
	// TODO: 在此处添加实现代码.


#ifdef VIEW__MODE
	if (iIndex > m_pEventList.GetCount())
		return DEF_EVENT_OVER;

	_DeleteEnterSpace(csEvent);
	CString _Event;
	m_pEventList.GetText(iIndex, _Event);

	if (_Event.IsEmpty())
		return DEF_EVENT_EMPTY;

	int iStart = _Event.Find(_T("["));

	if (iStart <= 0)
	{
		csByte.Empty();
		csEvent = _Event;
		return DEF_EVENT_SUCCESS;
	}
	else
	{
		csByte = _Event.Mid(iStart + 1, 2);
		//csByte = csByte.Left(csByte.GetLength() - 1);
		csEvent = _Event.Mid(0, iStart);

	}

	iStart = _Event.Find(_T("("));

	if (iStart <= 0)
	{
		csType.Empty();
		return DEF_EVENT_SUCCESS_EXT1;
	}
	else
	{
		csType = _Event.Mid(iStart + 1, 2);

	}

	return DEF_EVENT_SUCCESS_EXT2;
#else
	if (iIndex < 0)
		return DEF_EVENT_OVER;

	if (iIndex >= ___EentList.GetCount())
		return DEF_EVENT_OVER;

	//_DeleteEnterSpace(csEvent);


	CString _Event = ___EentList.GetAt(iIndex);


	if (_Event.IsEmpty())
		return DEF_EVENT_EMPTY;

	


	int iRet = DEF_EVENT_SUCCESS;;
	int iStart = _Event.Find(_T("["));
	int iOffset = _Event.GetLength();


	if (iStart <= 0)
	{
		csByte.Empty();	
	}
	else
	{
		if (iStart < iOffset)
			iOffset = iStart;
		csByte = _Event.Mid(iStart + 1, 2);
		iRet |= DEF_EVENT_SUCCESS_EXT1;

	}

	 iStart = _Event.Find(_T("("));



	if (iStart <= 0)
	{
		csType.Empty();
		//iRet =  DEF_EVENT_SUCCESS_EXT1;
	}
	else
	{
		if ( iStart < iOffset)
			iOffset = iStart;
		csType = _Event.Mid(iStart + 1, 3);
	
		iRet |= DEF_EVENT_SUCCESS_EXT2;

	}

	iStart = _Event.Find(_T("<"));

	if (iStart <= 0)
	{
		csOffset.Empty();
		//iRet = DEF_EVENT_SUCCESS_EXT2;
	}
	else
	{
		if (iStart < iOffset)
			iOffset = iStart;
		csOffset = _Event.Mid(iStart + 1, 2);
		iRet |= DEF_EVENT_SUCCESS_EXT3;

	}

	csEvent = _Event.Left(iOffset);

	return iRet;

#endif // VIEW__MODE
}

int CEventList::AddEvent(CString csText)
{
	// TODO: 在此处添加实现代码.	
	

#ifdef VIEW__MODE
	return m_pEventList.AddString(csText);
#else
	if ((_CString2Int(csText.Mid(1,1))&0x8 )!= 0)
	{
		CString csCount;
		csCount.Format("%x", ___EentList.GetCount());
		___VCCList.Add(csCount);
	}
	return ___EentList.Add(csText);
#endif // VIEW__MODE

}

int  CEventList::GetVCCForEvent(int iPos,BYTE* ucEvent)
{
	int iRet = 0;
	int iCount = ___VCCList.GetCount();
	int iVCCSqu = 0;
	for (int i = iCount; i>0 ;i--)
	{
		iVCCSqu = _CString2Int(___VCCList.GetAt(i - 1));
		if (iVCCSqu < iPos)
			break;
	}

	CString csEvent,csByte,_EventType, _EventOffset;;

	if ((GetEvent(iVCCSqu, csEvent, csByte, _EventType, _EventOffset)& DEF_EVENT_SUCCESS) != DEF_EVENT_SUCCESS)
	{
		return -1;
	}

	return _CString2UcHex(csEvent, ucEvent);

}


#define DEF_Des_Exsited 2

int CEventList::SeteEventByte(int iEventIndex, CString csBYTE)
{


#ifdef VIEW__MODE
	CString csEvent, csEventByte;
	int iRet = GetEvent(iEventIndex, csEvent, csEventByte, TODO);
	if (iRet != DEF_EVENT_SUCCESS)
		return iRet;

	//EventDes.Empty();
	//csEventDes = csEvent + _T("                    ");
	//csEventDes = csEventDes.Left(20);
	csBYTE = _T("[") + csBYTE + _T("]");

	csEvent = csEvent + csBYTE;



	m_pEventList.DeleteString(iEventIndex);
	m_pEventList.InsertString(iEventIndex, csEvent);

	return 1;
#else
	CString csEvent, csEventByte,csEventType, csEventOffset;
	int iRet = GetEvent(iEventIndex, csEvent, csEventByte, csEventType,csEventOffset);
	if (iRet != DEF_EVENT_SUCCESS)
		return iRet;

	csBYTE = _T("[") + csBYTE + _T("]");

	csEvent = csEvent + csBYTE;

	___EentList.RemoveAt(iEventIndex);
	___EentList.InsertAt(iEventIndex, csEvent);

	return 1;


#endif // VIEW__MODE


}

int CEventList::SetEventType(int iEventIndex, CString csType)
{


#ifdef VIEW__MODE
	CString csEvent, csEventByte, csEventType;
	int iRet = GetEvent(iEventIndex, csEvent, csEventByte, csEventType, TODO);
	if (iRet == DEF_EVENT_SUCCESS_EXT2)
		return iRet;

	csType = _T("(") + csType + _T(")");

	csEvent = csEvent + csType;


	m_pEventList.DeleteString(iEventIndex);
	m_pEventList.InsertString(iEventIndex, csEvent);

	return 1;
#else

	CString csEvent, csEventByte,csEventType,csEventOffset;
	int iRet = GetEvent(iEventIndex, csEvent, csEventByte,csEventType, csEventOffset);
	if (iRet == DEF_EVENT_SUCCESS_EXT2)
		return iRet;

	csType = _T("(") + csType + _T(")");

	csEvent = csEvent + csType;

	___EentList.RemoveAt(iEventIndex);
	___EentList.InsertAt(iEventIndex, csEvent);

	return 1;

#endif

}

int CEventList::SetEventDescription(int iEventIndex, CString csByte,CString csType, CString csOffset)
{
#ifdef VIEW__MODE
	CString csEvent, csEventByte;
	int iRet = GetEvent(iEventIndex, csEvent, csEventByte, TODO);
	if (iRet != DEF_EVENT_SUCCESS)
		return iRet;

	//EventDes.Empty();
	//csEventDes = csEvent + _T("                    ");
	//csEventDes = csEventDes.Left(20);
	csByte = _T("[") + csByte + _T("]");

	csType = _T("(") + csType + _T(")");

	csEvent = csEvent + csType;



	m_pEventList.DeleteString(iEventIndex);
	m_pEventList.InsertString(iEventIndex, csEvent);

	return 1;
#else

	CString csEvent, csEventByte, csEventType,csEventOffset;
	int iRet = GetEvent(iEventIndex, csEvent, csEventByte, csEventType, csEventOffset);
	if ((iRet& DEF_EVENT_SUCCESS) != DEF_EVENT_SUCCESS)
		return iRet;

	if (!csByte.IsEmpty())
		csByte = _T("[") + csByte + _T("]");
	
	if (!csType.IsEmpty())
		csType = _T("(") + csType + _T(")");
	
	if (!csOffset.IsEmpty())
		csOffset = _T("<") + csOffset + _T(">");


	csEvent = csEvent + csByte +csType+ csOffset;

	___EentList.RemoveAt(iEventIndex);
	___EentList.InsertAt(iEventIndex, csEvent);

	return 1;

#endif

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

#ifdef VIEW__MODE
	m_pEventList.SetRedraw(TRUE);
	m_pEventList.UpdateWindow();
	return m_pEventList.GetCount();

#else

	//m_pEventList.ResetContent();

	int iCount = ___EentList.GetCount();

	m_pEventList.SetRedraw(FALSE);
	for (int i = m_pEventList.GetCount(); i < iCount; i++)
		m_pEventList.AddString(___EentList.GetAt(i));

	m_pEventList.SetRedraw(TRUE);
	m_pEventList.UpdateWindow();

	return iCount;
#endif // VIEW__MODE
	
}

ULONG CEventList::GetSize()
{
#ifdef VIEW__MODE

	return m_pEventList.GetSize();
#else

	return ___EentList.GetSize();
#endif // VIEW__MODE

}

int CEventList::GetNextEvent(int iType, int iStart,CString& csEvent)
{
	int iCount = GetEventCount();
	CString  __Byte, __Type, __Offset;
	CString csType;

	csType.Format("%03x", iType);


	for (int i = iStart;i<iCount;i++)
	{
		if (((GetEvent(i, csEvent,__Byte, __Type, __Offset)& DEF_EVENT_SUCCESS_EXT2) == DEF_EVENT_SUCCESS_EXT2)&&
			(csType.Compare(__Type) == 0))
			return i;
	}
	
	csEvent.Empty();
	return -1;
}

int CEventList::GetPreviousEvent(int iType, int iStart, CString& csEvent)
{
	int iCount = GetEventCount();
	CString  __Byte, __Type,__Offset;
	CString csType;

	csType.Format("%03x", iType);


	for (int i = iStart; i >= 0; i--)
	{
		if (((GetEvent(i, csEvent, __Byte, __Type, __Offset) & DEF_EVENT_SUCCESS_EXT2) == DEF_EVENT_SUCCESS_EXT2) &&
			(csType.Compare(__Type) == 0))
			return i;
	}

	csEvent.Empty();
	return -1;
}