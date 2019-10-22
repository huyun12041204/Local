// EventList.cpp: 实现文件
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
END_MESSAGE_MAP()



// CEventList 消息处理程序




int CEventList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	

	return 0;
}
