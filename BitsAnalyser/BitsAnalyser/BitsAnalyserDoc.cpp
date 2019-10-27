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




// CBitsAnalyserDoc ���л�

void CBitsAnalyserDoc::Serialize(CArchive& ar)
{
	CMainFrame*       _pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	CBitsAnalyserView* pView = (CBitsAnalyserView*)(_pMain->GetActiveView());
	CString _Event,_EventDes;
	BYTE* _Bits;
	int _EventCount;
	int _ViewCount;
	if (ar.IsStoring())
	{
		_EventCount = _pMain->m_wndEventList.GetEventCount();

		for (int i = 0 ; i < _EventCount ; i++)
		{
			_Event.Empty();
			if (_pMain->m_wndEventList.GetEvent(i, _Event, _EventDes)<=0)
			{
				AfxMessageBox("��ȡ�¼�ʧ�ܣ��˴���ֱ���˳���");
				return;
			}
			ar.WriteString(_Event);
		}

		
	}
	else
	{
		_EventDes.Empty();
		while (ar.ReadString(_Event))
			_EventDes += _Event;
		_Bits = new BYTE[_EventDes.GetLength()/2];
		_EventCount =  _CString2UcHex(_EventDes, _Bits);

		for (int i = 0; i < _EventCount; i+= 256)
		{
			_ViewCount = _EventCount - i;

			pView->ViewAPDU(_Bits + i, (_ViewCount > 256) ? 256 : _ViewCount);

		}



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
