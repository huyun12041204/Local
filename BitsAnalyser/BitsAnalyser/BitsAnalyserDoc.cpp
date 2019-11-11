// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// BitsAnalyserDoc.cpp : CBitsAnalyserDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CBitsAnalyserDoc ����/����

CBitsAnalyserDoc::CBitsAnalyserDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CBitsAnalyserDoc::~CBitsAnalyserDoc()
{
}

BOOL CBitsAnalyserDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}


#define DEF_SAVE_SIZE 1024

// CBitsAnalyserDoc ���л�

void CBitsAnalyserDoc::Serialize(CArchive& ar)
{
	CMainFrame*       _pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	CBitsAnalyserView* pView = (CBitsAnalyserView*)(_pMain->GetActiveView());
	CString _Event,_EventByte, _EventType, _EventOffset;
	BYTE* _Bits;
	int _EventCount;

	//Bits����
	int iBitsSize   = 0;
	int iEventSize  = 0;
	int iHandleSize = 0;

	if (ar.IsStoring())
	{
		//iBitsSize = _pMain->m_wndEventList.GetSize();
		_EventCount = _pMain->m_wndEventList.GetEventCount();
		iBitsSize = _EventCount;
		//�˴���5Ԥ������
		_Bits = new BYTE[DEF_SAVE_SIZE+5];
		int iSaveOffset = 0;
		for (int i = 0 ; i < _EventCount ; i++)
		{



			_Event.Empty();
			if (_pMain->m_wndEventList.GetEvent(i, _Event, _EventByte, _EventType,_EventOffset)<=0)
			{
				AfxMessageBox("��ȡ�¼�ʧ�ܣ��˴���ֱ���˳���");
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
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CBitsAnalyserDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CBitsAnalyserDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CBitsAnalyserDoc ���

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


// CBitsAnalyserDoc ����


BOOL CBitsAnalyserDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  �ڴ������ר�õĴ�������

	return TRUE;
}
