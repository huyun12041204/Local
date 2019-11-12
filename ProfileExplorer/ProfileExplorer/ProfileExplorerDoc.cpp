// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// ProfileExplorerDoc.cpp: CProfileExplorerDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ProfileExplorer.h"
#endif

#include "ProfileExplorerDoc.h"
#include "MainFrm.h"
#include <propkey.h>
#include "ChildFrm.h"
#include <propkey.h>
//#include <wtypes.h>
#include "afxdatarecovery.h"

#include "FileProperties.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProfileExplorerDoc

IMPLEMENT_DYNCREATE(CProfileExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CProfileExplorerDoc, CDocument)
END_MESSAGE_MAP()


// CProfileExplorerDoc 构造/析构

CProfileExplorerDoc::CProfileExplorerDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CProfileExplorerDoc::~CProfileExplorerDoc()
{
}

BOOL CProfileExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CProfileExplorerDoc 序列化

void CProfileExplorerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CProfileExplorerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CProfileExplorerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CProfileExplorerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CProfileExplorerDoc 诊断

#ifdef _DEBUG
void CProfileExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProfileExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CProfileExplorerDoc 命令


BOOL CProfileExplorerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;
	// TODO:  在此添加您专用的创建代码
	CString csTitel;
	PCFS tempFile;
	if (!IsCardFilesAppPath(lpszPathName,NULL, &csTitel))
	{
//		theApp.pcFile.LoadFile(lpszPathName);

//		((CMainFrame*)theApp.GetMainWnd())->InsertProfile(lpszPathName);
	}
	
	((CProfileExplorerApp*)AfxGetApp())->pcFile.GetFileFromDoc(csTitel,&tempFile);

	if (tempFile.IsEmpty())
		return FALSE;

	CChildFrame* Frm;
	POSITION posOpenDoc = CDocument::GetFirstViewPosition();
	if (posOpenDoc != NULL)
	{
		CView* pView = CDocument::GetNextView(posOpenDoc); // get first one
		ASSERT_VALID(pView);
		Frm = (CChildFrame*)pView->GetParentFrame();
		Frm->SetFileToView(tempFile);
	}

	((CMainFrame*)theApp.GetMainWnd())->OpenReference(csTitel);

	return TRUE;
}

BOOL CProfileExplorerDoc::DoFileSave()
{
	CString csFilePath;

	if (m_strPathName.IsEmpty())
	{
		if (!_GetPathNameFromView(m_strPathName))
			return FALSE;
	}


	if (!IsCardFilesAppPath(m_strPathName, &csFilePath))
		csFilePath = m_strPathName;

	DWORD dwAttrib = GetFileAttributes(csFilePath);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY)
	{
		// we do not have read-write access or the file does not (now) exist
		if (!DoSave(NULL))
		{
			TRACE(traceAppMsg, 0, "Warning: File save with new name failed.\n");
			return FALSE;
		}
	}
	else
	{
		if (!DoSave(m_strPathName,FALSE))
		{
			TRACE(traceAppMsg, 0, "Warning: File save failed.\n");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CProfileExplorerDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CString newName = lpszPathName;

	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(":/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			if (AfxGetApp() && AfxGetApp()->GetDataRecoveryHandler())
			{
				// remove "[recovered]" from the title if it exists
				CString strNormalTitle = AfxGetApp()->GetDataRecoveryHandler()->GetNormalDocumentTitle(this);
				if (!strNormalTitle.IsEmpty())
					newName = strNormalTitle;
			}

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) && !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				int iStart = 0;
				newName += strExt.Tokenize(_T(";"), iStart);
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
				CATCH_ALL(e)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to delete file after failed SaveAs.\n");
				//DELETE_EXCEPTION(e);
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
	{
		SetPathName(newName);
		OnDocumentEvent(onAfterSaveDocument);
	}

	return TRUE;        // success
}


BOOL CProfileExplorerDoc::AddFile(PCFS __New)
{
	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;

	CString csNew = __New.stROUTE + Int2CString(__New.uiFID, 4);
	if (pcFile->GetFileRecord(csNew) != 0)
	{
		AfxMessageBox(_T("File ") + csNew + _T(" is existed!"));
		return FALSE;
	}
		

	

	if (pcFile->AddNewFileToDoc(__New)>=0)
	{
		CFilesListView * _View1 = (CFilesListView *)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILELIST);
		_View1->InsertFile(__New);

		CFilesTreeView *_View2 = (CFilesTreeView *)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILETREE);
		_View2->InsertFile(__New);

		return TRUE;
	}

	return FALSE;
}

BOOL CProfileExplorerDoc::ModifyFile(PCFS __Modified)
{
	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;
	int iModifiedID[DEF_FILELIST_CONTENT_COLUMN];
	int iModifiedCount = 0;
	ZeroMemory(iModifiedID, sizeof(iModifiedID));


	CChildFrame* Frm;
	Frm = (CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (Frm== NULL)
		return FALSE;



	iModifiedCount = __Modified.IsModified(Frm->pcfsOriFile, iModifiedID);

	CString csOriFile = Frm->pcfsOriFile.stROUTE + Int2CString(Frm->pcfsOriFile.uiFID,4);

	for (int i = 0; i < iModifiedCount; i++)
	{

		if ((iModifiedID[i] == DEF_FILELIST_HANDLE_COLUMN) &&
			(pcFile->SetRecordNumberInDoc(Frm->pcfsOriFile.uiHANDLE, __Modified.uiHANDLE) != TRUE))
		{
			MessageBox(NULL, _T("修改文件记录号失败。"), _T("错误"), IDOK | IDNO);
			return FALSE;
		}

		if ((iModifiedID[i] == DEF_FILELIST_2GACC_COLUMN) && (__Modified.uiTYPE == DEF_FILETYPE_EF) && (__Modified.i2GAC < 0))
			__Modified.i2GAC = AppendOneNew2GAC(_T("3F000022"), __Modified.c2GREAD, __Modified.c2GUPDATE, __Modified.c2GINCREATE,
				__Modified.c2GRFM, __Modified.c2GREHAB, __Modified.c2GINVAL, pcFile);

		if ((iModifiedID[i] == DEF_FILELIST_3GACC_COLUMN) && (__Modified.b3G) && (__Modified.i3GAC < 0))

		{
			CString csARRPath;

			if (__Modified.ui3GARRLEVEL == DEF_ARRLEVEL_2F06)
				csARRPath = _T("3F00");
			else
				csARRPath = __Modified.stROUTE.Mid(0, 8);

			csARRPath = csARRPath + _TranslateARRFile(__Modified.ui3GARRLEVEL);

			__Modified.i3GAC = AppendOneNew3GARR(csARRPath, __Modified.c3GREAD, __Modified.c3GUPDATE,
				__Modified.c3GINCREATE, __Modified.c3GACTIVE,
				__Modified.c3GDEACTIVE, pcFile);

		}

	}

	if (iModifiedCount != 0)
	{

		int iRet = pcFile->SetFileSubDataListToDoc(__Modified.uiHANDLE, __Modified, iModifiedCount, (UINT*)iModifiedID);

		if ((iRet != 0) &&
			(MessageBox(NULL, _T("修改 文件的 存在未写入项，是否忽略。"), _T("错误"), IDOK | IDNO) == IDNO))
			return FALSE;

		if (pcFile->PostFileToChildLinkFile(__Modified) < 0)
		{
			MessageBox(NULL, _T("修改 Link 文件的 文件大小，记录长度，记录条数。"), _T("错误"), IDOK | IDNO);
			return FALSE;
		}


		CFilesListView * _View1 = (CFilesListView *)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILELIST);
		if (_View1->IsKindOf(RUNTIME_CLASS(CFilesListView)))
		{
			int iItem = _View1->GetFileItem(csOriFile);
			if ((iItem > 0) &&
				(!_View1->ModifyFile(iItem-1, __Modified)))
				AfxMessageBox(_T("修改List View 失败"));

			
	
		}

		CFilesTreeView *_View2 = (CFilesTreeView *)((CMainFrame*)AfxGetApp()->GetMainWnd())->GetDockablePane(ID_VIEW_FILETREE);

		if (_View2->IsKindOf(RUNTIME_CLASS(CFilesTreeView)))
		{
			HTREEITEM hItem = _View2->_GetItem(csOriFile);

			if ((hItem != NULL)&&
				(!_View2->ModifyFile(hItem, __Modified)))
				AfxMessageBox(_T("修改Tree View 失败"));


		}
			
	}

	return TRUE;
}

BOOL CProfileExplorerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CChildFrame* Frm ;
	Frm = (CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
	if (Frm == NULL)
		return FALSE;

	PCFS __File;

	if (!Frm->GetFileFromView(&__File))
		return FALSE;



		
	ProfileClass* pcFile = &((CProfileExplorerApp*)AfxGetApp())->pcFile;



	//判断是不是新的纪录	
	if (__File.uiHANDLE <= pcFile->GetDocRecordCount())
	{
		if (!ModifyFile(__File))
			return FALSE;
	}
	else
	{
		if (!AddFile(__File))
			return FALSE;
	}


	if (!Frm->SetFileToView(__File, TRUE))
		return FALSE;


	SetPathName(GenerateCardFilesAppPath(pcFile->GetProfilePath(), __File.stROUTE + Int2CString(__File.uiFID,4)),FALSE);

	SetModifiedFlag(FALSE);
	return TRUE;

//	return CDocument::OnSaveDocument(lpszPathName);
}

void CProfileExplorerDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	// TODO: 在此添加专用代码和/或调用基类

	CDocument::SetPathName(lpszPathName, bAddToMRU);

	CString csTitel;
	if (IsCardFilesAppPath(lpszPathName, NULL, &csTitel))
	{
		_AppendSign(csTitel, _T("-"), 2);
		SetTitle(csTitel);
	}
}

BOOL CProfileExplorerDoc::_GetPathNameFromView(CString& lpszPathName)
{

	POSITION posOpenDoc = CDocument::GetFirstViewPosition();
	if (posOpenDoc != NULL)
	{
		lpszPathName = ((CProfileExplorerApp*)AfxGetApp())->pcFile.GetProfilePath();
		CView* pView = CDocument::GetNextView(posOpenDoc); // get first one
		lpszPathName = lpszPathName + _T("#") +
			_variant_t2CString(((CProfileExplorerView*)pView)->__GetPropertyValue(DEF_FILELIST_ROUTE_COLUMN)) +
			_variant_t2CString(((CProfileExplorerView*)pView)->__GetPropertyValue(DEF_FILELIST_FID_COLUMN));

	}
	else
		return FALSE;




	return TRUE;



}

void CProfileExplorerDoc::SetModifiedFlag(BOOL bModified)
{
	CDocument::SetModifiedFlag(bModified);
	CString csTitle = GetTitle();

	if ((bModified) && (csTitle.Right(1) != _T("*")))
		SetTitle(csTitle + _T("*"));
	if ((!bModified) && (csTitle.Right(1) == _T("*")))
		SetTitle(csTitle.Left(csTitle.GetLength() - 1));

}