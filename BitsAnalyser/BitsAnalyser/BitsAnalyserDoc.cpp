// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// BitsAnalyserDoc.cpp : CBitsAnalyserDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "BitsAnalyser.h"
#endif

#include "BitsAnalyserDoc.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBitsAnalyserDoc

IMPLEMENT_DYNCREATE(CBitsAnalyserDoc, CDocument)

BEGIN_MESSAGE_MAP(CBitsAnalyserDoc, CDocument)
END_MESSAGE_MAP()


// CBitsAnalyserDoc 构造/析构

CBitsAnalyserDoc::CBitsAnalyserDoc()
{
	// TODO: 在此添加一次性构造代码

}

CBitsAnalyserDoc::~CBitsAnalyserDoc()
{
}

BOOL CBitsAnalyserDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}


#define DEF_SAVE_SIZE 1024

// CBitsAnalyserDoc 序列化

void CBitsAnalyserDoc::Serialize(CArchive& ar)
{
	CMainFrame*       _pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	CBitsAnalyserView* pView = (CBitsAnalyserView*)(_pMain->GetActiveView());
	CString _Event,_EventByte, _EventType, _EventOffset;
	BYTE* _Bits;
	int _EventCount;

	//Bits总数
	int iBitsSize   = 0;
	int iEventSize  = 0;
	int iHandleSize = 0;

	if (ar.IsStoring())
	{
		//iBitsSize = _pMain->m_wndEventList.GetSize();
		_EventCount = _pMain->m_wndEventList.GetEventCount();
		iBitsSize = _EventCount;
		//此处加5预防超出
		_Bits = new BYTE[DEF_SAVE_SIZE+5];
		int iSaveOffset = 0;
		for (int i = 0 ; i < _EventCount ; i++)
		{



			_Event.Empty();
			if (_pMain->m_wndEventList.GetEvent(i, _Event, _EventByte, _EventType,_EventOffset)<=0)
			{
				AfxMessageBox("获取事件失败，此处将直接退出！");
				return;
			}



			iEventSize = _CString2UcHex(_Event, _Bits+ iSaveOffset);


			iHandleSize += iEventSize;
			iBitsSize += (iEventSize - 1);
			_pMain->SendMessage(ID_MESSAGE_UPDATE_PROGRESS, iHandleSize, iBitsSize);


			iSaveOffset += iEventSize;
			if ((iSaveOffset >= DEF_SAVE_SIZE)||
				(i == (_EventCount-1)))
			{
				ar.Write(_Bits, iSaveOffset);
				iSaveOffset = 0;
			}
				
		}

		delete _Bits;


		
	}
	else
	{

		_pMain -> RemoveAllBitsData();

		_Bits = new BYTE[DEF_SINGLE_VIEW_MAX];
		iEventSize = ar.Read(_Bits, DEF_SINGLE_VIEW_MAX);

	

		iHandleSize = 0;
		CFile* __File = ar.GetFile();
		int iBitsSize = (int)__File->GetLength();

		while (iEventSize>0)
		{
			_pMain->SendMessage(ID_MESSAGE_UPDATE_PROGRESS, iHandleSize, iBitsSize);
 			iEventSize   = ((iEventSize > DEF_SINGLE_VIEW_MAX) ? DEF_SINGLE_VIEW_MAX : iEventSize);
			iHandleSize += iEventSize;
			pView->ViewAPDU(_Bits, iEventSize);
			iEventSize   = ar.Read(_Bits, DEF_SINGLE_VIEW_MAX);
		
			
		}

		_pMain->SendMessage(ID_MESSAGE_UPDATE_PROGRESS, iHandleSize, iBitsSize);
		_pMain->SendMessage(ID_MESSAGE_UPDATE_EVENT, 1, 0);

		delete _Bits;

	
		_pMain->OnEventButton();



			//CurMainFrm->csCard += csTemp;
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CBitsAnalyserDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CBitsAnalyserDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CBitsAnalyserDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CBitsAnalyserDoc 诊断

#ifdef _DEBUG
void CBitsAnalyserDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBitsAnalyserDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBitsAnalyserDoc 命令


BOOL CBitsAnalyserDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码

	return TRUE;
}
