// RunLog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRunLogApp:
// �йش����ʵ�֣������ RunLog.cpp
//

class CRunLogApp : public CWinAppEx
{
public:
	CRunLogApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
	  
	DECLARE_MESSAGE_MAP()
};

extern CRunLogApp theApp;