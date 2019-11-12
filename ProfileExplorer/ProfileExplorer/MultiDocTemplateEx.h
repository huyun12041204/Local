#pragma once
#include "afxwin.h"
class CMultiDocTemplateEx :
	public CMultiDocTemplate
{
public:


	CMultiDocTemplateEx(UINT nIDResource,
		CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass,
		CRuntimeClass* pViewClass)
		:CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
	{



	}


	virtual BOOL GetDocString(CString& rString, enum DocStringIndex index) const
	{
		CString m_strSuffix = _T("MDB");
		if (index == CDocTemplate::filterExt)
		{
			rString = "." + m_strSuffix;
		}
		else if (index == CDocTemplate::fileNewName || index == CDocTemplate::filterName)
		{
			rString = m_strSuffix + "(*." + m_strSuffix + ")";
		}
		else
		{
			__super::GetDocString(rString, index);
		}
		return TRUE;
	};

	CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible)
	{
		CDocument* pDocument = CreateNewDocument();
		if (pDocument == NULL)
		{
			TRACE(traceAppMsg, 0, "CDocTemplate::CreateNewDocument returned NULL.\n");
			AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
			return NULL;
		}
		ASSERT_VALID(pDocument);

		BOOL bAutoDelete = pDocument->m_bAutoDelete;
		pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
		CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
		pDocument->m_bAutoDelete = bAutoDelete;
		if (pFrame == NULL)
		{
			AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
			delete pDocument;       // explicit delete on error
			return NULL;
		}
		ASSERT_VALID(pFrame);

		if (lpszPathName == NULL)
		{
			// create a new document - with default document name
			SetDefaultTitle(pDocument);

			// avoid creating temporary compound file when starting up invisible
			if (!bMakeVisible)
				pDocument->m_bEmbedded = TRUE;

			if (!pDocument->OnNewDocument())
			{
				// user has be alerted to what failed in OnNewDocument
				TRACE(traceAppMsg, 0, "CDocument::OnNewDocument returned FALSE.\n");
				pFrame->DestroyWindow();
				return NULL;
			}

			// it worked, now bump untitled count
			m_nUntitledCount++;
		}
		else
		{
			// open an existing document
			CWaitCursor wait;
			if (!pDocument->OnOpenDocument(lpszPathName))
			{
				// user has be alerted to what failed in OnOpenDocument
				TRACE(traceAppMsg, 0, "CDocument::OnOpenDocument returned FALSE.\n");
				pFrame->DestroyWindow();
				return NULL;
			}
			pDocument->SetPathName(lpszPathName, bAddToMRU);
			pDocument->OnDocumentEvent(CDocument::onAfterOpenDocument);
		}

		//if (lpszPathName!=NULL)
		//	bMakeVisible = FALSE;

		InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
		return pDocument;
	}
};

