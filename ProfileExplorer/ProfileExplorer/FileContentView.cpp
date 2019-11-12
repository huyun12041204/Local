// FileContentView.cpp: 实现文件
//

#include "stdafx.h"
#include "ProfileExplorer.h"
#include "FileContentView.h"


// CFileContentView

IMPLEMENT_DYNCREATE(CFileContentView, CEditView)

CFileContentView::CFileContentView()
{

}

CFileContentView::~CFileContentView()
{
}

BEGIN_MESSAGE_MAP(CFileContentView, CEditView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CFileContentView 绘图

void CFileContentView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// CFileContentView 诊断

#ifdef _DEBUG
void CFileContentView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CFileContentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileContentView 消息处理程序


int CFileContentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	SetWindowLong(m_hWnd, GWL_STYLE, GetStyle()&~(ES_AUTOHSCROLL | WS_HSCROLL));
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//SetClassLong(GetEditCtrl(), GCL_STYLE, GetStyle()&~WS_HSCROLL);
	return 0;
}
