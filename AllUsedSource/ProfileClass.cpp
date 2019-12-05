#include "stdafx.h"
#include "ProfileClass.h"

ProfileClass::ProfileClass()
{
	//csProfileaPath = csPath;


	m_pMainwnd = NULL;
	m_pHintwnd = NULL;
	m_pAllwnd = NULL;

}

ProfileClass::~ProfileClass()
{
	_DisconnectAccess();
}



BOOL ProfileClass::IsValid()
{

	if ((m_pConnection == NULL)||
		(m_pDocRecordset == NULL)||
		(m_pFRURecordset == NULL))
		return FALSE;
	return TRUE;
}

void ProfileClass::InstallOutput(CWnd* m_pMain, CWnd* m_pHint, CWnd* m_pAll)
{

	//m_pAllwnd = m_pAll;
	//m_pHintwnd = m_pHint;
	//m_pMainwnd = m_pMain;
	if (m_pMain->IsKindOf(RUNTIME_CLASS(CListBox)))
	{
		m_pMainwnd = (CListBox*)m_pMain;
		//ASSERT(m_pMainwnd -> IsKindOf(RUNTIME_CLASS(CListBox)));
	}
	if (m_pHint->IsKindOf(RUNTIME_CLASS(CListBox)))
	{
		m_pHintwnd = (CListBox*)m_pHint;
		//ASSERT(m_pMainwnd -> IsKindOf(RUNTIME_CLASS(CListBox)));
	}
	if (m_pAll->IsKindOf(RUNTIME_CLASS(CListBox)))
	{
		m_pAllwnd = (CListBox*)m_pAll;
		//ASSERT(m_pMainwnd -> IsKindOf(RUNTIME_CLASS(CListBox)));
	}

//	m_pAllwnd  = & (CListBox)m_pAll;
//	m_pHintwnd = &(CListBox)m_pHint;

}

void ProfileClass::OutputToWnd(UINT uiTAG,CString csText)
{

#ifndef _LISTBOX_XI

	switch (uiTAG)
	{
	case DEF_ERROR_TAG:	csText = DEF_RESULT_ERROR + csText; break;
	case DEF_WARN_TAG:  csText = DEF_RESULT_WARN + csText; break;
	case DEF_HINT_TAG:  csText = DEF_RESULT_HINT + csText; break;
	case DEF_NORMAL_TAG:csText = DEF_RESULT_NORMAL + csText; break;
	}

	switch (uiTAG)
	{
	case DEF_ERROR_TAG:
	case DEF_WARN_TAG:
		if (m_pMainwnd != NULL)
			m_pMainwnd->AddString(csText);
	case DEF_HINT_TAG:
		if (m_pHintwnd != NULL)
			m_pHintwnd->AddString(csText);
	case DEF_NORMAL_TAG:
		if (m_pAllwnd != NULL)
			m_pAllwnd->AddString(csText);
	}
#else
	
#if _LISTBOX_XI_EX
	if (uiTAG!= DEF_NORMAL_TAG)
		((CListBoxXI*)m_pMainwnd)->_AddString(csText, uiTAG);

	((CListBoxXI*)m_pAllwnd)->_AddString(csText, uiTAG);
#endif
#endif

	

}

void ProfileClass::DisplayAccessError(_com_error e)
{

	CString errormessage;
	errormessage.Format(_T("数据库操作失败.\r错误信息：%s"), e.ErrorMessage());
	TRACE(errormessage);
	OutputToWnd(DEF_ERROR_TAG, errormessage);
}

int ProfileClass::LoadFile(const CString csPath)
{
	if (csPath == this->GetProfilePath())
		return 2;

	if (!GetProfilePath().IsEmpty())
		_DisconnectAccess();

	csProfileaPath = csPath;

	if (!ConnectProfile())
	{
		csProfileaPath.Empty();
		
		return FALSE;
	}
	
	return TRUE;
}


CString ProfileClass::GenerateARRPath(CString csFID, CString csRoute)
{
	CString csResult;
	//PCFS pARRFile;
	if ((_CString2Int(csFID) & 0xFF00) == 0x2F00)
		csResult = _T("3F00") + csFID;
	else if (csRoute.GetLength() >= 8)
		csResult = csRoute.Mid(0, 8) + csFID;
	else
		csResult.Empty();

	return csResult;
}

int  ProfileClass::ConnectProfile()
{
	return _ConnecteAccess(csProfileaPath);
}

int  ProfileClass::DisconnectProfile()
{
	return _DisconnectAccess();
}

int  ProfileClass::_ConnecteAccess(const CString& csPath)
{
	BOOL bRet = TRUE;

	//CString csFilePath;
	if ((m_pConnection == NULL) || (!m_pConnection->State))
	{
		if (!_CreateConnection(csPath))
			return FALSE;
	}
	if ((m_pDocRecordset == NULL) || (!m_pDocRecordset->State))
		if (!_CreateRecordset(m_pDocRecordset,(CString)_T("AllFiles")))
			return -0x11 ;

	if ((m_pFRURecordset == NULL) || (!m_pFRURecordset->State))
		if (!_CreateRecordset(m_pFRURecordset,(CString)_T("FRU")))
			return -0x12;

	if ((m_pModRecordset == NULL) || (!m_pModRecordset->State))
		if (!_CreateRecordset(m_pModRecordset,(CString)_T("[Module]")))
			return -0x13;

	if (m_pDocRecordset->GetRecordCount() > 0)
		m_pDocRecordset->MoveFirst();

	if (m_pFRURecordset->GetRecordCount() > 0)
		m_pFRURecordset->MoveFirst();

	if (m_pModRecordset->GetRecordCount() > 0)
		m_pModRecordset->MoveFirst();


	return TRUE;


}

BOOL ProfileClass::_CreateConnection(const CString &csConnstr)
{
	//char* PtConnectStr;
	CString _temp(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + csConnstr);
//	PtConnectStr = _temp.GetBuffer(0);


	try
	{
		if (m_pConnection == NULL)
			m_pConnection.CreateInstance(__uuidof(ADODB::Connection));
		if (!m_pConnection->State)
			m_pConnection->Open(_temp.GetBuffer(), "", "", ADODB::adModeUnknown);
	}
	catch (_com_error e)
	{
		DisplayAccessError(e);
		return FALSE;
	}
	return TRUE;
}

BOOL ProfileClass::_CreateRecordset(ADODB::_RecordsetPtr& m_pRecord, CString csFrom)
{

	try
	{

		m_pRecord.CreateInstance(__uuidof (ADODB::Recordset));
		csFrom = _T("select * from ") + csFrom;
		m_pRecord->Open(csFrom.GetBuffer(),
			//_variant_t((IDispatch*)m_pConnection, true),
			m_pConnection.GetInterfacePtr(),
			ADODB::adOpenStatic,
			ADODB::adLockOptimistic,
			ADODB::adCmdText);


	}
	catch (_com_error e)
	{
		DisplayAccessError(e);
		return FALSE;
	}
	return TRUE;
}

void ProfileClass::_DisconnectRecordset(ADODB::_RecordsetPtr& m_pRecord)
{
	if (m_pRecord->State)
		m_pRecord->Close();
}

BOOL ProfileClass::_DisconnectAccess()
{
	try
	{

		if ((m_pDocRecordset != NULL) && (m_pDocRecordset->State))
			_DisconnectRecordset(m_pDocRecordset);
		if ((m_pFRURecordset != NULL) && (m_pFRURecordset->State))
			_DisconnectRecordset(m_pFRURecordset);
		if ((m_pModRecordset != NULL) && (m_pModRecordset->State))
			_DisconnectRecordset(m_pModRecordset);
		if ((m_pConnection != NULL) && (m_pConnection->State))
			m_pConnection->Close();
	}
	catch (_com_error(e))
	{
	//	DisplayAccessError(e);
		return FALSE;

	}


	return TRUE;


}

BOOL ProfileClass::_SelectRecord(ADODB::_RecordsetPtr& m_pRecord, long nRecord, BOOL brRelative)
{
	BOOL bRet;
	if (nRecord <= 0)
		return FALSE;

	if ((m_pRecord==NULL)||(!m_pRecord->State))
		return FALSE;


	long lCount = m_pRecord->GetRecordCount();

	try
	{

		if (brRelative)
		{
			_variant_t _temp;
			_temp.Clear();

			//先检查当条是否符合
			//if ((_GetCurrentRecordSubData(m_pRecord, DEF_FILELIST_HANDLE_COLUMN, _temp) == DEF_PROFILE_SUCCESS) &&
			//	(nRecord == _variant_t2Int(_temp)))
			//	return TRUE;

			//通过绝对记录条数查询,如果符合直接结果

			if (nRecord<lCount)
			{
				m_pRecord->MoveFirst();
				m_pRecord->Move(nRecord - 1);
			}


			if ((_GetCurrentRecordSubData(m_pRecord, DEF_FILELIST_HANDLE_COLUMN, _temp) == DEF_PROFILE_SUCCESS) &&
				(nRecord == _variant_t2Int(_temp)))
				return TRUE;

			//一条一条查询,符合直接结果
			m_pRecord->MoveFirst();

			do
			{
				//此处防止溢出操作，通过EOF判断似乎不准确
				if (m_pDocRecordset->GetAbsolutePosition() > GetDocRecordCount())
					return FALSE;

				if ((_GetCurrentRecordSubData(m_pRecord, DEF_FILELIST_HANDLE_COLUMN, _temp) == DEF_PROFILE_SUCCESS) &&
					(nRecord == _variant_t2Int(_temp)))
					return TRUE;

				bRet = m_pRecord->adoEOF;
				if (!bRet)
					m_pRecord->MoveNext();
			} while (!m_pRecord->adoEOF);


		}
		else
		{
			//if (nRecord > lCount)
			//	return FALSE;
			//if ((m_pRecord->GetAbsolutePosition()) != nRecord)
			//{
				m_pRecord->MoveFirst();
				m_pRecord->Move(nRecord - 1);
		//	}
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		DisplayAccessError(e);
		return FALSE;
	}


	return FALSE;


}

BOOL ProfileClass::_DeleteCurrentRecord(ADODB::_RecordsetPtr& m_pRecord)
{
	if ((m_pRecord == NULL) || (!m_pRecord->State))
		return FALSE;
	try
	{
		m_pRecord->Delete(ADODB::adAffectCurrent);
		m_pRecord->Update();
	}
	catch (_com_error e)
	{
		DisplayAccessError(e);
		return FALSE;
	}
	return TRUE;
}

BOOL ProfileClass::_DeleteRecord(ADODB::_RecordsetPtr& m_pRecord,long lRecord)
{
	if (!_SelectRecord(m_pRecord, lRecord))
		return FALSE;
	return _DeleteCurrentRecord(m_pRecord);

}

BOOL ProfileClass::DeleteDocRecord(long lRecord)
{
	long lCount = GetDocRecordCount();

	if (!_DeleteRecord(m_pDocRecordset, lRecord))
		return FALSE;

	for (int i = lRecord; i < lCount;i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i+1))
			return FALSE;
		if (!_SetCurrentRecordSubData(m_pDocRecordset,DEF_FILELIST_HANDLE_COLUMN, Int2_variant_t(VT_I4,i)))
			return FALSE;
	}
	
	return TRUE;

}

CString ProfileClass::GetProfilePath()
{

	return csProfileaPath;
}

long ProfileClass::GetDocRecordCount()
{
	if ((m_pDocRecordset == NULL) || (!m_pDocRecordset->State))
		return 0;
	return m_pDocRecordset->GetRecordCount();
}

long ProfileClass::GetModuleRecordCount()
{
	if ((m_pModRecordset == NULL) || (!m_pModRecordset->State))
		return 0;
	return m_pModRecordset->GetRecordCount();
}

long ProfileClass::GetModuleValidRecordCount()
{
	int iRet = 0;
	long lCount = GetModuleRecordCount();

	for (long i = 1; i <= lCount; i++)
	{
		if (!_SelectRecord(m_pModRecordset, i, FALSE))
			break;
		_variant_t _temp;
		if (_GetCurrentRecordSubData(m_pModRecordset, DEF_FILELIST_HANDLE_COLUMN, _temp)!= DEF_PROFILE_SUCCESS)
			break;

		if (_variant_t2Int(_temp) != 0)
			iRet++;
	}
	return iRet;
}

long ProfileClass::DeleteModuleInvalidRecord()
{

	int iRet = 0;
	long lCount = GetModuleRecordCount();

	for (long i = 1; i <= lCount; i++)
	{
		if (!_SelectRecord(m_pModRecordset, i, FALSE))
			break;
		_variant_t _temp;
		if (_GetCurrentRecordSubData(m_pModRecordset, DEF_FILELIST_HANDLE_COLUMN, _temp) != DEF_PROFILE_SUCCESS)
			break;

		if (_variant_t2Int(_temp) == 0)
			_DeleteCurrentRecord(m_pModRecordset);
	}
	return iRet;


}

long ProfileClass::GetFileRecord(CString csPath)
{
	if (csPath.IsEmpty())
		return 0;

	CString csFile;
	_variant_t __FID, __ROUTE, __REOCRD;
	long lCount = GetDocRecordCount();




	m_pDocRecordset->MoveFirst();


	
	
	long lRet = 0;
	for (long i = 0 ; i < lCount; i++)
	{
	 
		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_HANDLE_COLUMN, __REOCRD)!=0)||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_FID_COLUMN, __FID) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ROUTE_COLUMN, __ROUTE) != 0))
		{
			lRet = -1;
			break;
		}
		csFile.Empty();

		if (__ROUTE.vt == VT_BSTR)
			csFile += _variant_t2CString(__ROUTE);

		if (__FID.vt == VT_BSTR)
			csFile += _variant_t2CString(__FID);

		csPath.MakeUpper();
		csFile.MakeUpper();
		if (csPath.Compare(csFile)==0)
		{
			lRet = (long)__REOCRD;
			break;
		}
		m_pDocRecordset->MoveNext();
	}

	return lRet;


}


long ProfileClass::_GetCurrentRecordSubData(ADODB::_RecordsetPtr& m_pRecord, long nItem, _variant_t& varOutput)
{
	if ((m_pRecord == NULL) || (!m_pRecord->State))
		return FALSE;
	long lSize = 0;
	try
	{
		ADODB::FieldPtr    m_pFieldset = m_pRecord->GetFields()->GetItem((_variant_t)(nItem));
		ADODB::adoDataTypeEnum  adoDTE = m_pFieldset->GetType();
		varOutput.Clear();
	

		//如果为WChar 或者 Int 直接获取数据
		if ((adoDTE == ADODB::adVarWChar) ||
			(adoDTE == ADODB::adInteger))
			varOutput = m_pFieldset->GetValue();
		//如果是长的二进制数据则需要返回长度
		else if (adoDTE == ADODB::adLongVarBinary)
		{
			lSize = m_pFieldset->ActualSize;
			if (lSize > 0)
				varOutput = m_pFieldset->GetChunk(lSize);

		}
		else
			return -1;
	}
	catch (_com_error(e))
	{

		DisplayAccessError(e);
		return -2;
	}

	

	return lSize;
}

long ProfileClass::_GetRecordSubData(ADODB::_RecordsetPtr& m_pRecord,long nRecord, long nItem, _variant_t& varOutput)
{
	////int iRet = m_pRecord->GetAbsolutePosition();
	//if (nRecord<=0)
	//	return FALSE;
	//if (m_pRecord->GetAbsolutePosition() != nRecord)
	//{
	//	m_pRecord->MoveFirst();
	//	m_pRecord->Move(nRecord-1);
	//}

	if (!_SelectRecord(m_pRecord,nRecord))
	{
		return FALSE;
	}

	return _GetCurrentRecordSubData(m_pRecord, nItem, varOutput);

}

long ProfileClass::GetRecordSubDataFromDoc(long nRecord, long nItem, _variant_t& varOutput)
{

	return _GetRecordSubData(m_pDocRecordset, nRecord, nItem, varOutput);

}

int ProfileClass::_GetSimpleFileFromTab(ADODB::_RecordsetPtr& m_pRecord, PCFS* pcfsFile)
{
	int iRet = 0;
	_variant_t _temp;
	long lRet = FALSE;
	pcfsFile->EmptyFile();

	for (long i = DEF_FILELIST_HANDLE_COLUMN; i <= DEF_FILELIST_RECORD_LENGTH_COLUMN; i++)
	{
		if (i == DEF_FILELIST_CHVKEYDF_COLUMN)
			continue;
		_temp.Clear();
		if ((_GetCurrentRecordSubData(m_pRecord, i, _temp) < 0) ||
			(pcfsFile->SetPrivateCardFileStructData(i, _temp) == FALSE))
			iRet = iRet + 1;
	}
	return iRet;
}


int ProfileClass::GetSimpleFileFromDocFast(bool bFirst, PCFS* pcfsFile)
{

	if (bFirst)
		m_pDocRecordset->MoveFirst();
	else if (!m_pDocRecordset->adoEOF)
	{
		m_pDocRecordset->MoveNext();
	}

	

	return _GetSimpleFileFromTab(m_pDocRecordset, pcfsFile);
}




int ProfileClass::_GetFileFromTable(ADODB::_RecordsetPtr& m_pRecord,long nRecord, PCFS* pcfsFile, BOOL brRelative /*= TRUE*/, UINT *uiFails /*== NULL*/)
{

	int iRet = 0;
	_variant_t _temp;
	long lRet = FALSE;
	pcfsFile->EmptyFile();
	if (!_SelectRecord(m_pRecord, nRecord, brRelative))
		return -1;
	for (long i = DEF_FILELIST_HANDLE_COLUMN; i <= DEF_FILELIST_CONTENT_COLUMN; i++)
	{
		if (i == DEF_FILELIST_CHVKEYDF_COLUMN)
			continue;
		_temp.Clear();
		if ((_GetCurrentRecordSubData(m_pRecord, i, _temp) < 0) ||
			(pcfsFile->SetPrivateCardFileStructData(i, _temp) == FALSE))
		{
			if (uiFails != NULL)
				uiFails[iRet] = (unsigned int)i;
			iRet = iRet + 1;
		}
	}

	if (pcfsFile->IsLinkFile())
	{
		PCFS __File;
		__File.EmptyFile();

		if (GetFileFromDoc(pcfsFile->stLINKROUTE, &__File) < 0)
		{
			if (uiFails != NULL)
				uiFails[iRet] = (unsigned int)DEF_FILELIST_CONTENT_COLUMN;
			iRet = iRet + 1;
		}
		else
			pcfsFile->stContent = __File.stContent;
	}
	return iRet;
}

int  ProfileClass::GetFileFromDoc(long nRecord, PCFS* pcfsFile, BOOL brRelative,UINT *uiFails  /*== NULL*/)
{

	return _GetFileFromTable(m_pDocRecordset, nRecord, pcfsFile, brRelative, uiFails);

}

int ProfileClass::GetFileFromModule(long nRecord, PCFS* pcfsFile, BOOL brRelative /*= TRUE */, UINT *uiFails /*= NULL*/)
{
	return _GetFileFromTable(m_pModRecordset, nRecord, pcfsFile, brRelative, uiFails);

}

int  ProfileClass::GetFileFromDoc(CString csPath, PCFS* pcfsFile, UINT *uiFails)
{
	pcfsFile->EmptyFile();
	long lRecord = GetFileRecord(csPath);
	if (lRecord <= 0)
		return -1;
	return GetFileFromDoc(lRecord, pcfsFile);
}

BOOL ProfileClass::GetRecordFileData(CString csPath, int* iRecord, int* iRecordLength, CStringArray* csRecords)
{
	PCFS pFile;
	if (this->GetFileFromDoc(csPath, &pFile) < 0)
		return FALSE;

	if (iRecord!=NULL)
		*iRecord = pFile.uiRECORDNUMBER;

	if (iRecordLength != NULL)
		*iRecordLength = pFile.uiRECORDLENGTH;


	if (csRecords != NULL)
	{
		CString csContent = pFile.stContent;

		_DeleteNote(csContent);
		_DeleteEnterSpace(csContent);

		for (int i = 0; i <= pFile.uiRECORDNUMBER; i++)
			csRecords->Add(csContent.Mid(i* pFile.uiRECORDLENGTH * 2, pFile.uiRECORDLENGTH * 2));
	}

	return TRUE;
}

BOOL ProfileClass::RetrieveARRFile(CString csFID, CString csRoute, int* iRecord,CStringArray* csRecords)
{

	if (csFID.IsEmpty())
		return FALSE;

	csFID =GenerateARRPath(csFID, csRoute);

	if (csFID.IsEmpty())
		return FALSE;

	return GetRecordFileData(csFID, iRecord, NULL, csRecords);

}

UINT ProfileClass::GetDFList(CStringArray& csDF, BOOL bADF /*= TRUE*/, BOOL bMF /*= FALSE*/)
{

	PCFS _tempFile;
	long lCount = GetDocRecordCount();
	int iRet;
	UINT uiCounter = 0;
	for (long i = 1; i <= lCount; i++)
	{
		_tempFile.EmptyFile();
		iRet = GetFileFromDoc(i, &_tempFile);

		if ((_tempFile.uiTYPE == DEF_FILETYPE_MF) && bMF)
		{
			uiCounter += 1;
			csDF.Add(_tempFile.stROUTE + Int2CString(_tempFile.uiFID,4));
		}
			
		if ((_tempFile.uiTYPE == DEF_FILETYPE_ADF) && bADF)
		{
			uiCounter += 1;
			csDF.Add(_tempFile.stROUTE + Int2CString(_tempFile.uiFID, 4));
		}
		if ((_tempFile.uiTYPE == DEF_FILETYPE_DF))
		{
			uiCounter += 1;
			csDF.Add(_tempFile.stROUTE + Int2CString(_tempFile.uiFID, 4));
		}
	
	}
	return uiCounter;

}

int ProfileClass::_SetCurrentRecordSubData(ADODB::_RecordsetPtr& m_pRecord, long nItem, _variant_t varInput)
{
	ADODB::FieldPtr    m_pFieldset = m_pRecord->GetFields()->GetItem((_variant_t)(nItem));
	ADODB::adoDataTypeEnum  adoDTE = m_pFieldset->GetType();
	if ((m_pRecord == NULL) || (!m_pRecord->State))
		return FALSE;

	if (((adoDTE == ADODB::adVarWChar) && (varInput.vt == VT_BSTR))
		|| ((adoDTE == ADODB::adInteger) && (varInput.vt == VT_I4)))
		m_pFieldset->PutValue(varInput);
	else if ((adoDTE == ADODB::adLongVarBinary) && (varInput.vt == (VT_UI1|VT_ARRAY)))
	{
		if (!SUCCEEDED(m_pFieldset->AppendChunk(varInput)))
			return FALSE;
	}
	else
	{
		varInput = _T("");
		m_pFieldset->PutValue(varInput);
		return FALSE;
	}
	return TRUE;

}

int ProfileClass::_SetRecordSubData(ADODB::_RecordsetPtr& m_pRecord, long nRecord, long nItem, _variant_t varOutput)
{
	 
	if (!_SelectRecord(m_pRecord, nRecord, TRUE))
		return FALSE;
	return _SetCurrentRecordSubData(m_pRecord, nItem, varOutput);

}

int ProfileClass::_SetFileToTable(ADODB::_RecordsetPtr& m_pRecord,long nRecord, PCFS pcfsFile, BOOL brRelative /*= TRUE */, UINT *uiFails /*== NULL*/)
{


	int iRet = 0;
	_variant_t _temp;
	BOOL bUsed = FALSE;

	if (!_SelectRecord(m_pRecord, nRecord, brRelative))
		return -1;

	for (long i = DEF_FILELIST_FID_COLUMN; i <= DEF_FILELIST_CONTENT_COLUMN; i++)
	{
		_temp = pcfsFile.GetPrivateCardFileStructData(i, &bUsed);

		if (bUsed&&_SetCurrentRecordSubData(m_pRecord, i, _temp))
		{
			if (uiFails != NULL)
				uiFails[iRet] = (unsigned int)i;
			iRet = iRet + 1;
		}
	}
	m_pRecord->Update();
	return iRet;


}


int ProfileClass::SetFileToDoc(long nRecord, PCFS pcfsFile, BOOL brRelative /*= TRUE */, UINT *uiFails/*= NULL /*== NULL*/)
{

	return _SetFileToTable(m_pDocRecordset, nRecord, pcfsFile, brRelative, uiFails);

}

int ProfileClass::SetFileToModule(long nRecord, PCFS pcfsFile, BOOL brRelative /*= TRUE*/, UINT *uiFails /*= NULL /*== NULL*/)
{

	return _SetFileToTable(m_pModRecordset, nRecord, pcfsFile, brRelative, uiFails);

}


int ProfileClass::SetFileSubDataListToDoc(long nRecord, PCFS pcfsFile, UINT iSubDataNumber, UINT * iSubDataList, UINT *uiFails )
{
	int iRet = 0;

	for (UINT i = 0 ; i < iSubDataNumber ; i++)
	{
		if (!_SetRecordSubData(m_pDocRecordset, nRecord, iSubDataList[i], pcfsFile.GetPrivateCardFileStructData(iSubDataList[i])))
		{
			if (uiFails!= NULL)
				uiFails[iRet] = iSubDataList[i];
			iRet++;
		}
	}
	m_pDocRecordset->Update();
	return iRet;

}

int ProfileClass::_AddNewFileToTable(ADODB::_RecordsetPtr& m_pRecord,PCFS pcfsFile)
{
	_variant_t _temp;
	BOOL bUsed;
	int iRet = DEF_PROFILE_SUCCESS;
	try
	{
		if (m_pRecord->GetRecordCount()!=0)
		{
			if (!m_pRecord->adoEOF)
				m_pRecord->MoveLast();
			else
			{
				m_pRecord->MoveFirst();
				m_pRecord->MoveLast();
			}
		}

	
			
		
		m_pRecord->AddNew();

		for (long i = DEF_FILELIST_HANDLE_COLUMN; i <= DEF_FILELIST_CONTENT_COLUMN; i++)
		{
			_temp = pcfsFile.GetPrivateCardFileStructData(i, &bUsed);

			if (bUsed&&_SetCurrentRecordSubData(m_pRecord, i, _temp))
				iRet = iRet + 1;
		}
		m_pRecord->Update();
	}
	catch (_com_error(e))
	{
		DisplayAccessError(e);
		return -1;
		
	}

	return iRet;

}

int ProfileClass::AddNewFileToDoc(PCFS pcfsFile)
{

	return _AddNewFileToTable(m_pDocRecordset, pcfsFile);
 
}

int ProfileClass::AddNewFileToModule(PCFS pcfsFile)
{

	return _AddNewFileToTable(m_pModRecordset, pcfsFile);

}

int ProfileClass::ChangeRecordNumberOnly(long lOriNumber, long lDesNumber)
{

	if (lOriNumber<1)
		return FALSE;
	_variant_t cTemp = lDesNumber;
	cTemp.vt         = VT_I4;
	m_pDocRecordset->MoveFirst();
	//其实味第一条 是0;
	m_pDocRecordset->Move(lOriNumber-1);
	return _SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_HANDLE_COLUMN, cTemp);

}

int  ProfileClass::SetRecordNumberInDoc(long lOriNumber, long lDesNumber)
{

	int iFileCounter = GetDocRecordCount();


	try
	{

		//m_pDocRecordset->MoveLast();
		//m_pDocRecordset->AddNew();

		//新建一条不数据零时放入到最后
		if (!ChangeRecordNumberOnly(lOriNumber, iFileCounter + 1))
			return -1;

		for (int i = 1 ; i < lDesNumber;i++)
		{
			if (!ChangeRecordNumberOnly(i, i))
			{
				return -2;
			}

		}
		if (!ChangeRecordNumberOnly(iFileCounter, lDesNumber))
			return -3;

		m_pDocRecordset->Update();

	}
	catch (_com_error(e))
	{
		DisplayAccessError(e);
		return FALSE;
	}



	return TRUE;

}

BOOL ProfileClass::_CurrentRecordIsLinkRecord(ADODB::_RecordsetPtr& m_pRecord)
{
	_variant_t __temp;
	if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_TYPE_COLUMN, __temp) != DEF_PROFILE_SUCCESS) &&
		(_ExChangeFileType(__temp) != DEF_FILETYPE_EF))
		return FALSE;

	if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_STRUCTURE_COLUMN, __temp) != DEF_PROFILE_SUCCESS)
		return FALSE;

	int iSTRUCTURE = _ExChangeFileStructure(__temp);

	if ((iSTRUCTURE != DEF_FILESTRUCTURE_TLINK) &&
		(iSTRUCTURE != DEF_FILESTRUCTURE_CLINK) &&
		(iSTRUCTURE != DEF_FILESTRUCTURE_LLINK))
		return FALSE;

	return TRUE;

}

BOOL ProfileClass::RecordIsLinkRecord(long lRecord, CString& csPLinkFile)
{
	_variant_t __temp;
	if (!_SelectRecord(m_pDocRecordset, lRecord))
		return FALSE;
	
	if (!_CurrentRecordIsLinkRecord(m_pDocRecordset))
		return FALSE;

	if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_LINKFILE_COLUMN, __temp) != DEF_PROFILE_SUCCESS)
		return FALSE;

	csPLinkFile = _variant_t2CString(__temp);

	return TRUE;
}

BOOL ProfileClass::RecordIsLinkRecord(long lRecord, long* lPLinkNumber)
{
	CString csPLINK;
	if (!RecordIsLinkRecord(lRecord, csPLINK))
		return FALSE;
	if (lPLinkNumber!=NULL)
		*lPLinkNumber = GetFileRecord(csPLINK);
	return TRUE;

}

int  ProfileClass::PostFileToChildLinkFile(PCFS pcfsFile)
{
	static UINT uiList[3] = { DEF_FILELIST_SIZE_COLUMN,DEF_FILELIST_RECORD_NUMBER_COLUMN ,DEF_FILELIST_RECORD_LENGTH_COLUMN };

	long lCount = GetDocRecordCount();
	long lLinkReocrd;
	int iRet = 0;
	int iWrong = 0;
	for (long i = 1 ;  i<= lCount ; i++)
	{
		lLinkReocrd = 0;

		if (RecordIsLinkRecord(i, &lLinkReocrd)&&
		   (lLinkReocrd == pcfsFile.uiHANDLE))
		{
			if (SetFileSubDataListToDoc(i, pcfsFile, 3, uiList) == DEF_PROFILE_SUCCESS)
				iRet   += 1;
			else
				iWrong += 1;
				
		}
	}

	if (iWrong >0)
		return -iWrong;
		
	return iRet;
}

BOOL ProfileClass::GetFileBasicInformation(ADODB::_RecordsetPtr& m_pRecord, CString& csText)
{

	try
	{
		_variant_t __FID, __Route, __Record;

		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_HANDLE_COLUMN, __Record) == DEF_PROFILE_SUCCESS) &&
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_FID_COLUMN, __FID) == DEF_PROFILE_SUCCESS) &&
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ROUTE_COLUMN, __Route) == DEF_PROFILE_SUCCESS))

		{
			csText.Empty();
			csText = (_T("Record ") + Int2CStringDecimal((int)__Record) + _T(" - "));
			csText += _T("(File ");
			if (__Route.vt == VT_BSTR)
				csText += _variant_t2CString(__Route);

			if (__FID.vt == VT_BSTR)
				csText += (_variant_t2CString(__FID)+ _T(")"));
		}
		else
			return FALSE;

	}
	catch (_com_error(e))
	{
		DisplayAccessError(e);
		return FALSE;
	}


	return TRUE;
}

int ProfileClass::ComputeProfileAC()
{
	CStringArray csACArray;
	CString csText,csAC;
	_variant_t __Type,__2GAC;
	_variant_t __Read, __Update, __Increase, __RFM, __Rehab, __Inval;
	 BOOL bInArray = FALSE;

	long lCounter = GetDocRecordCount();
	csACArray.RemoveAll();

	for (long i = 1; i <= lCounter; i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i))
		{
			return -1;
		}

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;

		if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_TYPE_COLUMN, __Type) != 0)

		{
			OutputToWnd( DEF_ERROR_TAG,  csText + _T("获取文件类别失败，此处跳出并结束。"));
			return -10;
		}

		if (_ExChangeFileType(__Type) != DEF_FILETYPE_EF)
		{
			OutputToWnd( DEF_NORMAL_TAG, csText + _T("不是EF，此处跳过。"));
			continue;
		}

		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GREAD_COLUMN, __Read) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GUPDATE_COLUMN, __Update) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GINCREASE_COLUMN, __Increase) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GRFM_COLUMN, __RFM) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GREHAB_COLUMN, __Rehab) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GINVAL_COLUMN, __Inval) != 0)||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GACC_COLUMN, __2GAC) != 0))
		{
			OutputToWnd( DEF_ERROR_TAG,  csText + _T("获取GSM下文件权限失败，此处跳出并结束。"));
			continue;
		}
			
		bInArray = FALSE;
		csAC = Generate2GACRecord(_Translate2GACText(_variant_t2CString(__Read)), _Translate2GACText(_variant_t2CString(__Update)),
			_Translate2GACText(_variant_t2CString(__Increase)), _Translate2GACText(_variant_t2CString(__RFM)),
			_Translate2GACText(_variant_t2CString(__Inval)), _Translate2GACText(_variant_t2CString(__Rehab)), TRUE);

		int iRecord;
		for (iRecord = 0; iRecord < csACArray.GetCount(); iRecord++)
		{

			csAC.MakeUpper();
			if (csAC.Left(6).Compare(csACArray.GetAt(iRecord).Left(6)) == 0)
			{
				bInArray = TRUE;
				break;
			}

		}

		if (bInArray)
		{
			if (_variant_t2Int(__2GAC) != iRecord+1)
			{
				try
				{
					_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GACC_COLUMN, Int2_variant_t(VT_I4, iRecord + 1));
					m_pDocRecordset->Update();
				}
				catch (_com_error(e))
				{
					OutputToWnd( DEF_ERROR_TAG,  csText + _T("写入当前文件AC Number 失败，此处跳出并结束。") );
					DisplayAccessError(e);
					return -31;
				}
				OutputToWnd(DEF_NORMAL_TAG,csText + _T("写入当前文件AC Number ") + Int2CStringDecimal(iRecord + 1));
			}
			else
				OutputToWnd(DEF_NORMAL_TAG,csText + _T("当前文件AC Number 与编译出相同，此处跳过。"));
		}
		else
		{

			csACArray.Add(csAC);

			OutputToWnd(DEF_NORMAL_TAG,(_T("新建一条新的AC") + csAC));
			try
			{
				_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_2GACC_COLUMN, Int2_variant_t(VT_I4, csACArray.GetCount()));
				m_pDocRecordset->Update();
			}
			catch (_com_error(e))
			{
				OutputToWnd( DEF_ERROR_TAG,  csText + _T("写入当前文件AC Number 失败，此处跳出并结束。"));
				DisplayAccessError(e);
				return -32;
			}

			OutputToWnd(DEF_NORMAL_TAG, csText + _T("写入当前文件AC Number") + Int2CStringDecimal(csACArray.GetCount()));

		}

	}

	PCFS _tempAC;
	if ((GetFileFromDoc(_T("3F000022"), &_tempAC) >= 0) &&
		!_tempAC.IsEmpty())
	{
		_tempAC.stContent.Empty();
		_tempAC.uiRECORDNUMBER = csACArray.GetCount();
		_tempAC.uiRECORDLENGTH = 3;
		_tempAC.uiSIZE = 3 * csACArray.GetCount();
		for (int i = 0; i < _tempAC.uiRECORDNUMBER; i++)
			_tempAC.stContent += csACArray.GetAt(i) + _T("\r\n");
		static UINT uiList[4] = { DEF_FILELIST_SIZE_COLUMN,DEF_FILELIST_RECORD_NUMBER_COLUMN ,DEF_FILELIST_RECORD_LENGTH_COLUMN ,DEF_FILELIST_CONTENT_COLUMN };

		_tempAC.CorrectPCFS();



		if (SetFileSubDataListToDoc(_tempAC.uiHANDLE, _tempAC, 4, uiList) >= DEF_PROFILE_SUCCESS)
		{
			OutputToWnd(DEF_HINT_TAG, _T("3F000002 文件写入新的数据:"));

			for (int i = 0; i < _tempAC.uiRECORDNUMBER; i++)
				OutputToWnd(DEF_NORMAL_TAG, csACArray.GetAt(i));

		

		}
	}
	else
		return FALSE;

	return TRUE;

}

int ProfileClass::ReInput3GACText()
{

	int iRet = 0;

	long lCounter = GetDocRecordCount();

	for (long i = 1 ; i <= lCounter;i++)
	{

		if (_ReInpur3GACText(i) < DEF_PROFILE_SUCCESS)
			iRet += 1;

	
	}



	return iRet;
}

int ProfileClass::_ReInpur3GACText(long i)
{
	if (!_SelectRecord(m_pDocRecordset, i))
	{
		return -1;
	}


	CString csText;
	_variant_t __be3G;
	_variant_t __Read, __Update, __Increase, __Active, __Deactive;
	_variant_t __Route, __ARRLEVEL, __ARRC;
	if (!GetFileBasicInformation(m_pDocRecordset, csText))
		return -2;


	//如果此条获取3G标志位失败，或者3G标志为0；则忽略当条
	if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GMODE_COLUMN, __be3G) != 0) ||
		(_variant_t2Int(__be3G) != 1))
	{
		OutputToWnd(DEF_NORMAL_TAG, csText + _T("非UICC配置文件，此处忽略。"));

		return DEF_PROFILE_UNNECESSARY;
	}



	//如果获取3G权限出错，直接返回
	if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GREAD_COLUMN, __Read) != 0) ||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GUPDATE_COLUMN, __Update) != 0) ||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GINCREASE_COLUMN, __Increase) != 0) ||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GREHAB_COLUMN, __Active) != 0) ||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GINVAL_COLUMN, __Deactive) != 0))
	{

		OutputToWnd(DEF_ERROR_TAG, csText + _T("获取UICC权限失败，此处跳出并结束。"));
		return -2;
	}


	if ((!_variant_t2CString(__Read).IsEmpty()) && (!_variant_t2CString(__Update).IsEmpty()) &&
		(!_variant_t2CString(__Increase).IsEmpty()) && (!_variant_t2CString(__Active).IsEmpty()) &&
		(!_variant_t2CString(__Deactive).IsEmpty()))
	{
		OutputToWnd(DEF_NORMAL_TAG, csText + _T("已存在UICC权限，此处不重写，此处忽略。"));
		return DEF_PROFILE_UNNECESSARY;
	}


	//如果获取3G权限出错，直接返回
	//此处修改为 continue 因为ARR Counter 和LEVEL 可能会获取失败
	if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GACC_COLUMN, __ARRC) != 0) ||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GARRFile_COLUMN, __ARRLEVEL) != 0) ||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ROUTE_COLUMN, __Route) != 0))
	{
		OutputToWnd(DEF_ERROR_TAG, csText + _T("已存在UICC权限配置失败，此处跳出并结束。"));
		return -3;
	}

	CStringArray csArray;
	if (!RetrieveARRFile(_variant_t2CString(__ARRLEVEL),
		_variant_t2CString(__Route), NULL, &csArray))
	{
		OutputToWnd(DEF_ERROR_TAG, csText + _T("获取UICC的ARR指引文件（ARR File）失败，此处跳出并结束。"));
		return -10;
	}


	if (_variant_t2Int(__ARRC) > csArray.GetCount())
	{
		OutputToWnd(DEF_ERROR_TAG, csText + _T("文件指示ARR记录超出ARR文件记录，此处跳出并结束。"));
		return -11;
	}

	if (_variant_t2Int(__ARRC) <= 0)
	{
		OutputToWnd(DEF_ERROR_TAG, csText + _T("文件ARR 记录存在不能为零或者负数。"));
		return -12;
	}

	CString csRecord = csArray.GetAt(_variant_t2Int(__ARRC) - 1);
	CString csAC;
	int iOther;

	//READ
	csAC.Empty();
	_GetACFromARR(01, csRecord, csAC, &iOther);
	__Read = CString2_variant_t(VT_BSTR, csAC);

	//UPDATE
	csAC.Empty();
	_GetACFromARR(02, csRecord, csAC, &iOther);
	__Update = CString2_variant_t(VT_BSTR, csAC);

	//Increase
	csAC.Empty();
	_GetACFromARR(0x32, csRecord, csAC, &iOther, true);
	__Increase = CString2_variant_t(VT_BSTR, csAC);

	//Deactive
	csAC.Empty();
	_GetACFromARR(0x8, csRecord, csAC, &iOther);
	__Deactive = CString2_variant_t(VT_BSTR, csAC);

	//Active
	csAC.Empty();
	_GetACFromARR(0x10, csRecord, csAC, &iOther);
	__Active = CString2_variant_t(VT_BSTR, csAC);

	if (!_SelectRecord(m_pDocRecordset, i))
	{
		OutputToWnd(DEF_ERROR_TAG, csText + _T("重新选择记录失败，此处跳出并结束。"));
		return -21;
	}


	try
	{
		_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GREAD_COLUMN, __Read);
		_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GUPDATE_COLUMN, __Update);
		_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GINCREASE_COLUMN, __Increase);
		_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GREHAB_COLUMN, __Active);
		_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GINVAL_COLUMN, __Deactive);
		m_pDocRecordset->Update();
	}
	catch (_com_error(e))
	{
		OutputToWnd(DEF_ERROR_TAG, csText + _T("写入记录数据失败，此处跳出并结束。"));
		DisplayAccessError(e);
		return -31;
	}

	OutputToWnd(DEF_HINT_TAG, csText + _T("成功重新写入。"));

	return DEF_PROFILE_SUCCESS;
}

int ProfileClass::GetARRFileList(CStringArray& csPathArray)
{
	long lCount = GetDocRecordCount();
	_variant_t __FID, __ROUTE;
	m_pDocRecordset->MoveFirst();
	for (long i = 1 ; i <= lCount;i++)
	{
	
		if(!_SelectRecord(m_pDocRecordset, i))
			continue;

		if (i == 61)
		{
			i = 61;
		}


		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_FID_COLUMN, __FID) == DEF_PROFILE_SUCCESS) &&
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ROUTE_COLUMN, __ROUTE) == DEF_PROFILE_SUCCESS)&&
			!_CurrentRecordIsLinkRecord(m_pDocRecordset))
		{
			if ((_variant_t2CString(__FID).Compare(_T("2F06")) == 0)||
				(_variant_t2CString(__FID).Compare(_T("6F06")) == 0))
				csPathArray.Add(_variant_t2CString(__ROUTE) + _variant_t2CString(__FID));
		}
		else
			continue;


	//	m_pDocRecordset->MoveNext();
	}

	return csPathArray.GetCount();

}

int ProfileClass::ComputeARRFile(CString csPath)
{
	BOOL bInArray;
	CString csText;
	CStringArray csArray;
	long lCount = GetDocRecordCount();
	csArray.RemoveAll();

	_variant_t __be3G, __3GAC;
	_variant_t __Read, __Update, __Increase, __Active, __Deactive;
	_variant_t __Route, __ARRLEVEL, __ARRC;

	//m_pDocRecordset->MoveFirst();

	for (long i = 1; i <= lCount; i++)
	{

		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;

		//如果此条获取3G标志位失败，或者3G标志为0；则忽略当条
		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GMODE_COLUMN, __be3G) != 0) ||
			(_variant_t2Int(__be3G) != 1))
		{
			OutputToWnd(DEF_NORMAL_TAG, csText + _T("非UICC配置文件，此处忽略。"));
			continue;
		}



		//如果获取3G权限出错，直接返回
		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GREAD_COLUMN, __Read) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GUPDATE_COLUMN, __Update) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GINCREASE_COLUMN, __Increase) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GREHAB_COLUMN, __Active) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GINVAL_COLUMN, __Deactive) != 0))
		{

			OutputToWnd( DEF_ERROR_TAG,  csText + _T("获取UICC权限失败，此处跳出并结束。"));
			return -2;
		}

		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ROUTE_COLUMN, __Route) != 0) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GARRFile_COLUMN, __ARRLEVEL) != 0))
		{
			OutputToWnd( DEF_ERROR_TAG,  csText + _T("获取ARR文件失败，此处跳出并结束。"));
			return -3;
		}

		//此处判断如果是LINK文件需要指引过去
		CString _tempFile = GenerateARRPath(_variant_t2CString(__ARRLEVEL), _variant_t2CString(__Route));
		CString _SureFile;

		if (!RecordIsLinkRecord(GetFileRecord(_tempFile), _SureFile))
			_SureFile = _tempFile;

		if (_SureFile.IsEmpty())
		{
			OutputToWnd( DEF_ERROR_TAG,  csText + _T(" ARR 指引存在问题" + _variant_t2CString(__Route)+_variant_t2CString(__ARRLEVEL) + ",此处跳出并结束。"));
			return -4;
		}

	

		if (csPath.Compare(_SureFile)!=0)
			continue;


		if ((_variant_t2CString(__Read).IsEmpty())     || (_variant_t2CString(__Update).IsEmpty()) ||
			(_variant_t2CString(__Increase).IsEmpty()) || (_variant_t2CString(__Active).IsEmpty()) ||
			(_variant_t2CString(__Deactive).IsEmpty()))
		{
			OutputToWnd(DEF_NORMAL_TAG, csText + _T("权限存在空值，此处忽略。"));
			continue;
		}

		CString csRecord = Generate3GARRRecord(_Translate3GACText(_variant_t2CString(__Read)),
			_Translate3GACText(_variant_t2CString(__Update)),
			_Translate3GACText(_variant_t2CString(__Increase)),
			_Translate3GACText(_variant_t2CString(__Active)),
			_Translate3GACText(_variant_t2CString(__Deactive)));

		int iRecord;
		bInArray = FALSE;
		for (iRecord = 0; iRecord < csArray.GetCount(); iRecord++)
		{
			
			csRecord.MakeUpper();
			if (csRecord.Compare(csArray.GetAt(iRecord)) == 0)
			{
				bInArray = TRUE;
				break;
			}
		}



		if (bInArray)
		{
			if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GACC_COLUMN, __3GAC) != 0)&&
				(_variant_t2Int(__3GAC) != iRecord + 1))
			{
				try
				{
					_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GACC_COLUMN, Int2_variant_t(VT_I4, iRecord + 1));
					m_pDocRecordset->Update();
				}
				catch (_com_error(e))
				{
					OutputToWnd( DEF_ERROR_TAG,  csText + _T("写入当前文件3G AC Number 失败，此处跳出并结束。"));
					DisplayAccessError(e);
					return -31;
				}
				OutputToWnd(DEF_NORMAL_TAG, csText + _T("写入当前文件3G AC Number ") + Int2CStringDecimal(iRecord + 1));
			}
			else
				OutputToWnd(DEF_NORMAL_TAG, csText + _T("当前文件3G AC Number 与编译出相同，此处跳过。"));
		}
		else
		{

			csArray.Add(csRecord);

			OutputToWnd( DEF_NORMAL_TAG,(_T("新建一条新的AC")+csRecord));
			try
			{
				_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GACC_COLUMN, Int2_variant_t(VT_I4, csArray.GetCount()));
				m_pDocRecordset->Update();
			}
			catch (_com_error(e))
			{
				OutputToWnd( DEF_ERROR_TAG,  csText + _T("写入当前文件3G AC Number 失败，此处跳出并结束。"));
				DisplayAccessError(e);
				return -32;
			}

			OutputToWnd(DEF_NORMAL_TAG, csText + _T("写入当前文件3G AC  Number") + Int2CStringDecimal(csArray.GetCount()));

		}
	}

	PCFS _tempAC;
	if ((GetFileFromDoc(csPath, &_tempAC) >= 0) &&
		!_tempAC.IsEmpty())
	{
		

		_tempAC.stContent.Empty();
		_tempAC.uiRECORDNUMBER = csArray.GetCount();
		//_tempAC.uiRECORDLENGTH = 3;
		_tempAC.uiSIZE = _tempAC.uiRECORDLENGTH * csArray.GetCount();
		for (int i = 0; i < _tempAC.uiRECORDNUMBER; i++)
			_tempAC.stContent += FormatCString(csArray.GetAt(i), _tempAC.uiRECORDLENGTH*2)  + _T("\r\n");
		static UINT uiList[3] = { DEF_FILELIST_SIZE_COLUMN,DEF_FILELIST_RECORD_NUMBER_COLUMN  ,DEF_FILELIST_CONTENT_COLUMN };

		_tempAC.CorrectPCFS();
		SetFileSubDataListToDoc(_tempAC.uiHANDLE, _tempAC, 3, uiList);

		OutputToWnd(DEF_HINT_TAG, csPath+_T(" 文件写入新的数据:"));

		for (int i = 0; i < _tempAC.uiRECORDNUMBER; i++)
			OutputToWnd(DEF_NORMAL_TAG, FormatCString(csArray.GetAt(i), _tempAC.uiRECORDLENGTH * 2));


	}
	else
		return FALSE;

	return TRUE;
}

int ProfileClass::ComputeProfileARR()
{
	CStringArray csFileArray;
	int iNumber = GetARRFileList(csFileArray);
	int iRet = 0;

	for (int i = 0 ; i < iNumber; i++)
	{
		if (ComputeARRFile(csFileArray.GetAt(i))!= TRUE)
		{
			OutputToWnd( DEF_ERROR_TAG,  csFileArray.GetAt(i)+_T("编译时跳出,此处直接跳过"));
			iRet++;
		}
	}





	return iRet;
}

int ProfileClass::ComputeProfileNotLinkAddress()
{

	
	CString    csText;
	_variant_t __Type, __Size, __Address;
	long lCount   = GetDocRecordCount();
	UINT iADDRESS = 0;
	m_pDocRecordset->MoveFirst();

	for (long i = 1 ; i <= lCount;i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;

		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_TYPE_COLUMN, __Type) != DEF_PROFILE_SUCCESS) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_SIZE_COLUMN, __Size) != DEF_PROFILE_SUCCESS) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ADDRESS_COLUMN, __Address) != DEF_PROFILE_SUCCESS))
		{
			OutputToWnd( DEF_ERROR_TAG,  csText + _T("获取文件相关信息出错，此处跳出并结束。"));
			return -2;
		}
			
		if (_ExChangeFileType(__Type) != DEF_FILETYPE_EF)
		{
		
			if (_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ADDRESS_COLUMN, Int2_variant_t(VT_I4, 0)))
			{
				OutputToWnd(DEF_NORMAL_TAG, csText + _T("不是EF，把0写入ADDRESS。"));
				m_pDocRecordset->Update();
				continue;
			}
			else
			{
				OutputToWnd( DEF_ERROR_TAG,  csText + _T("不是EF，把0写入ADDRESS。但是没有成功！"));
				return -11;
			}
			
		}
	
			
		if (_CurrentRecordIsLinkRecord(m_pDocRecordset))
		{
			OutputToWnd(DEF_NORMAL_TAG, csText + _T(" LINK文件，此处忽略！"));
			continue;
		}
			

		if ((int)__Address == iADDRESS)
		{
			OutputToWnd(DEF_NORMAL_TAG, csText + _T(" 地址相同不做重复写入"));
		}
		else if (!_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ADDRESS_COLUMN, Int2_variant_t(VT_I4, iADDRESS)))
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 写入地址 ") + Int2CStringDecimal(iADDRESS)+_T("失败")));
		else
		{
			m_pDocRecordset->Update();
			OutputToWnd(DEF_NORMAL_TAG, (csText + _T(" 重写地址 ") + Int2CStringDecimal(iADDRESS)));
		}

		iADDRESS += _variant_t2Int(__Size);
	}

	return DEF_PROFILE_SUCCESS;

}

int ProfileClass::ComputeProfileLinkAddress()
{
	_variant_t _ADDRESS;
	CString csText;
	long lCount = GetDocRecordCount();
	long lPRecord;
	UINT iADDRESS = 0;
	m_pDocRecordset->MoveFirst();

	for (long i = 1; i <= lCount; i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;

		if (!RecordIsLinkRecord(i, &lPRecord))
		{
			OutputToWnd( DEF_NORMAL_TAG, + (csText + _T(" 不是LINK文件,此处跳过! ")));
			continue;
		}
		if (!_SelectRecord(m_pDocRecordset, lPRecord))
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 找寻LINK文件失败,此处跳出并结束! ")));
			return -10;
		}

		if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ADDRESS_COLUMN, _ADDRESS) != DEF_PROFILE_SUCCESS)
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 获取LINK文件地址失败,此处跳出并结束! ")));
			return -11;
		}

		if (!_SelectRecord(m_pDocRecordset, i))
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 重新选择文件失败,此处跳出并结束! ")));
			return -1;
		}
			

		if (_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ADDRESS_COLUMN,  _ADDRESS))
		{
			OutputToWnd(DEF_NORMAL_TAG, (csText + _T(" 重新写入地址:")+Int2CStringDecimal(_variant_t2Int(_ADDRESS))));
			m_pDocRecordset->Update();
		}
		else
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 重新写入地址 失败!")));
			return -20;
		}

	}

	return TRUE;
}

int ProfileClass::ComputeProfileOTA()
{
	CString csText;
	CString csContent;
	_variant_t __temp,__APID;
	long lCount   = GetDocRecordCount();
	int iNumber = 0;
	int iLength;

	if (GetFileRecord(_T("3F0019001F04")) <= 0)
	{
		OutputToWnd(DEF_ERROR_TAG, (CString)(_T("未能找到OTA应用列表文件 19001F04，此处跳出并结束！")));
		return -10;
	}


	if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_LENGTH_COLUMN, __temp) != DEF_PROFILE_SUCCESS)
	{
		OutputToWnd(DEF_ERROR_TAG, (CString)(_T("获取OTA应用列表文件 19001F04 记录条数失败，此处跳出并结束！")));
		return -10;
	}

	iLength = (int)__temp;

	if (iLength < 5)
	{
		iLength = 5;
		__temp  = 5;
		if (_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_LENGTH_COLUMN, __temp) != DEF_PROFILE_SUCCESS)
		{
			OutputToWnd(DEF_ERROR_TAG, (CString)(_T("写入OTA应用列表文件 19001F04 记录长度失败，此处跳出并结束！")));
			return -10;
		}

	}
		




	__temp.Clear();

	m_pDocRecordset->MoveFirst();
	for (long i = 1 ; i <= lCount; i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;
		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;
		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_FRU3_COLUMN,__temp)!=  DEF_PROFILE_SUCCESS)||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_FRU4_COLUMN, __APID) != DEF_PROFILE_SUCCESS))
		{
			OutputToWnd( DEF_NORMAL_TAG, + (csText + _T(" 获取文件应用状态失败，此处忽略! ")));
			continue;
		}

		if (_TanslateApplicationType(_variant_t2CString(__temp)) == DEF_APPLICAITON_TYPE_OTA)
		{
			csContent += (_variant_t2CString(__APID) + Int2CString(i, iLength*2 - 6));
			csContent += _T("\r\n");
			iNumber++;
		}
	}


	if (GetFileRecord(_T("3F0019001F04")) <= 0)
	{
		OutputToWnd( DEF_ERROR_TAG,  (CString)(_T("未能找到OTA应用列表文件 19001F04，此处跳出并结束！")));
		return -10;
	}

	if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_NUMBER_COLUMN, __temp) != DEF_PROFILE_SUCCESS)
	{
		OutputToWnd( DEF_ERROR_TAG,  (CString)(_T("获取OTA应用列表文件 19001F04 记录条数失败，此处跳出并结束！")));
		return -10;
	}

	if (((int)__temp<iNumber)&&
		!_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_CONTENT_COLUMN, Int2_variant_t(VT_I4,iNumber)))
	{
		OutputToWnd( DEF_ERROR_TAG,  (CString)(_T("重写OTA应用列表文件 19001F04 记录条数失败，此处跳出并结束！")));
		return -20;
	}
	else
	{
		csContent = RepeatCString(_T("0000000000\r\n"),  (int)__temp- iNumber)+ csContent;
	}
	
	if (!_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_CONTENT_COLUMN, CString2_variant_t(VT_ARRAY | VT_UI1, csContent)))
	{
		OutputToWnd( DEF_ERROR_TAG,  (CString)(_T("重写OTA应用列表文件 19001F04 失败，此处跳出并结束！")));
		return -30;
	}
	else
	{
		_DeleteEnterSpace(csContent);
		OutputToWnd(DEF_HINT_TAG, (_T("重写OTA应用列表文件 19001F04 成功:")));
		for (int i = 0 ; i< (int)__temp;i++)
		{
			OutputToWnd(DEF_NORMAL_TAG, csContent.Mid(i*10,10));
		}
	}
		
	return iNumber;
}

int ProfileClass::ComputeProfileFixed()
{

	CString csText;
	CString csContent;
	_variant_t __temp, __APID;
	long lCount = GetDocRecordCount();
	int iNumber = 0;
	m_pDocRecordset->MoveFirst();
	for (long i = 1; i <= lCount; i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;
		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;
		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_FRU3_COLUMN, __temp) != DEF_PROFILE_SUCCESS) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_FRU4_COLUMN, __APID) != DEF_PROFILE_SUCCESS))
		{
			OutputToWnd( DEF_NORMAL_TAG, + (csText + _T(" 获取文件应用状态失败，此处忽略! ")));
			continue;
		}

		if (_TanslateApplicationType(_variant_t2CString(__temp)) == DEF_APPLICAITON_TYPE_FIXED)
		{
			csContent += (_variant_t2CString(__APID) + Int2CString(i, 4));
			csContent += _T("\r\n");
			iNumber++;
		}
	}


	if (GetFileRecord(_T("3F0019001F10")) <= 0)
	{
		OutputToWnd( DEF_ERROR_TAG,  (CString)(_T("未能找到OTA应用列表文件 19001F10，此处跳出并结束！")));
		return -10;
	}


	if (!_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_SIZE_COLUMN,          Int2_variant_t(VT_I4,iNumber*5))||
		!_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_NUMBER_COLUMN, Int2_variant_t(VT_I4,iNumber))||
		!_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_CONTENT_COLUMN, CString2_variant_t(VT_ARRAY | VT_UI1, csContent)))
	{
		OutputToWnd( DEF_ERROR_TAG,  (CString)(_T("重写OTA应用列表文件 19001F10 失败，此处跳出并结束！")));
		return -30;
	}
	else
	{
		_DeleteEnterSpace(csContent);
		OutputToWnd(DEF_HINT_TAG, (_T("重写OTA应用列表文件 19001F10 成功，")));
		for (int i = 0; i < iNumber; i++)
		{
			OutputToWnd(DEF_NORMAL_TAG, csContent.Mid(i * 10, 10));
		}
	}

	return iNumber;

}




CString ProfileClass::__BuiltFile(int iHandle /*= 0*/, int iType /*= 0x10*/, int iStructure /*= 0x0*/,
	int iFID /*= 0x3f00*/, int iLinkHandle /*= 0*/, int iSize /*= 0*/,
	int iAddress/*=0*/, int iStatus /*= 0x33*/, int iACCRecord/*= 1*/,
	int iRecordLength /*= 0*/, int iARRRecords /*= 0*/, int iARRLevel /*= 0*/,
	int iSFI /*= 0*/)
{
	CString csFile;

	if (iACCRecord < 0)
	{
		iACCRecord = 0;
	}
	if (iARRRecords < 0)
	{
		iARRRecords = 0;
	}

	if ((iARRLevel < 0)||(iARRLevel>7))
	{
		iARRLevel = 0;
	}


	csFile += Int2CString(iHandle, 4);
	csFile += Int2CString(iLinkHandle, 4);
	csFile += Int2CString(iFID, 4);
	csFile += Int2CString(iSize, 4);
	csFile += Int2CString(iAddress, 0x8);
	csFile += Int2CString(iType + iStructure, 2);
	csFile += Int2CString(iStatus, 2);
	csFile += Int2CString(iACCRecord, 2);
	csFile += Int2CString(iRecordLength, 2).Right(02);
	csFile += Int2CString(iARRRecords, 2);
	csFile += Int2CString(iARRLevel * 0x20 + iSFI, 2);

	if (csFile.GetLength()!=36)
	{
		csFile.Empty();
	}

	return csFile;
}


CString ProfileClass::_BuiltMF(int iHandle, int iPINList, int iFID,
	int iStatus, int iARRRecords, int iARRLevel,
	int iPINNumber, int iDFNumber, int iEFNumber)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_MF, NULL, iFID, iPINNumber, iEFNumber, iPINList, iStatus, iDFNumber, NULL, iARRRecords, iARRLevel);
}

CString ProfileClass::_BuiltDF(int iHandle, int iPINList, int iFID,
	int iStatus, int iARRRecords, int iARRLevel,
	int iPINNumber, int iDFNumber, int iEFNumber)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_DF, NULL, iFID, iPINNumber, iEFNumber, iPINList, iStatus, iDFNumber, NULL, iARRRecords, iARRLevel);
}

CString ProfileClass::_BuiltADF(int iHandle, int iPINList, int iFID,
	int iStatus, int iARRRecords, int iARRLevel,
	int iPINNumber, int iDFNumber, int iEFNumber, int iAIDRecords)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_ADF, NULL, iFID, iPINNumber, iEFNumber, iPINList, iStatus, iDFNumber, iAIDRecords, iARRRecords, iARRLevel);
}



CString ProfileClass::_BuiltEF_Transparent(int iHandle, int iFID, int iSize, int iAddress,
	int iStatus, int iACCRecord, int iARRRecords,
	int iARRLevel, int iSFI)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_EF, DEF_FILESTRUCTURE_TRANSPARENT, iFID, 01, iSize, iAddress, iStatus, iACCRecord, 0, iARRRecords, iARRLevel, iSFI);
}


CString ProfileClass::_BuiltEF_Liner(int iHandle,
	int iFID, int iSize, int iAddress,
	int iStatus, int iACCRecord, int iRecordLength,
	int iARRRecords, int iARRLevel, int iSFI)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_EF, DEF_FILESTRUCTURE_LINER, iFID, 01, iSize, iAddress, iStatus, iACCRecord, iRecordLength, iARRRecords, iARRLevel, iSFI);
}

CString ProfileClass::_BuiltEF_Cyclic(int iHandle,
	int iFID, int iSize, int iAddress,
	int iStatus, int iACCRecord, int iRecordLength,
	int iARRRecords, int iARRLevel, int iSFI)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_EF, DEF_FILESTRUCTURE_CYCLIC, iFID, 01, iSize, iAddress, iStatus, iACCRecord, iRecordLength, iARRRecords, iARRLevel, iSFI);
}


CString ProfileClass::_BuiltEF_TLINK(int iHandle, int iFID, int iSize,
	int iAddress, int iLinkHandle, int iStatus,
	int iACCRecord, int iRecordLength, int iARRRecords,
	int iARRLevel, int iSFI)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_EF, DEF_FILESTRUCTURE_TLINK, iFID, iLinkHandle, iSize, iAddress, iStatus, iACCRecord, iRecordLength, iARRRecords, iARRLevel, iSFI);
}

CString ProfileClass::_BuiltEF_LLINK(int iHandle, int iFID, int iSize,
	int iAddress, int iLinkHandle, int iStatus,
	int iACCRecord, int iRecordLength, int iARRRecords,
	int iARRLevel, int iSFI)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_EF, DEF_FILESTRUCTURE_LLINK, iFID, iLinkHandle, iSize, iAddress, iStatus, iACCRecord, iRecordLength, iARRRecords, iARRLevel, iSFI);
}

CString ProfileClass::_BuiltEF_CLINK(int iHandle, int iFID, int iSize,
	int iAddress, int iLinkHandle, int iStatus,
	int iACCRecord, int iRecordLength, int iARRRecords,
	int iARRLevel, int iSFI)
{
	return __BuiltFile(iHandle, DEF_FILETYPE_EF, DEF_FILESTRUCTURE_CLINK, iFID, iLinkHandle, iSize, iAddress, iStatus, iACCRecord, iRecordLength, iARRRecords, iARRLevel, iSFI);
}



BOOL ProfileClass::_BuiltFile(long nRecord, CString&csFileHead, const int iAddOff)
{
	BOOL bRet = TRUE;
	PCFS __File;
	int iParent;
	int iLink;
	int iPinList = 0x01810A;
	csFileHead.Empty();

	if (GetFileFromDoc(nRecord, &__File) < 0)
		return FALSE;

	if (!__File.stROUTE.IsEmpty())
	{
		iParent = GetFileRecord(__File.stROUTE);
		if (iParent < 1)
			return FALSE;
		iParent = iParent - 1;
	}
	else
		iParent = 0;

	switch (__File.uiTYPE)
	{

	case DEF_FILETYPE_EF:
		iLink = 0;
		if (!RecordIsLinkRecord(nRecord, (long*)&iLink))
			iLink = 0;
		else
			iLink = iLink - 1;//此处是Record所以进行编译时要减1成为卡内HANDLE

		switch (__File.uiSTRUCT)
		{
		case DEF_FILESTRUCTURE_TRANSPARENT: csFileHead = _BuiltEF_Transparent(iParent,
			__File.uiFID, __File.uiSIZE, __File.ulADDRESS + iAddOff,
			__File.uiEFSTATUS, __File.i2GAC, __File.i3GAC,
			__File.ui3GARRLEVEL, __File.ui3GSFI); break;
		case DEF_FILESTRUCTURE_LINER:		csFileHead = _BuiltEF_Liner(iParent,
			__File.uiFID, __File.uiSIZE, __File.ulADDRESS + iAddOff,
			__File.uiEFSTATUS, __File.i2GAC, __File.uiRECORDLENGTH,
			__File.i3GAC, __File.ui3GARRLEVEL, __File.ui3GSFI); break;

		case DEF_FILESTRUCTURE_CYCLIC:      csFileHead = _BuiltEF_Cyclic(iParent,
			__File.uiFID, __File.uiSIZE, __File.ulADDRESS + iAddOff,
			__File.uiEFSTATUS, __File.i2GAC, __File.uiRECORDLENGTH,
			__File.i3GAC, __File.ui3GARRLEVEL, __File.ui3GSFI); break;

		case DEF_FILESTRUCTURE_TLINK:       csFileHead = _BuiltEF_TLINK(iParent,
			__File.uiFID, __File.uiSIZE, __File.ulADDRESS + iAddOff, iLink,
			__File.uiEFSTATUS, __File.i2GAC, __File.uiRECORDLENGTH,
			__File.i3GAC, __File.ui3GARRLEVEL, __File.ui3GSFI); break;
		case DEF_FILESTRUCTURE_LLINK:       csFileHead = _BuiltEF_LLINK(iParent,
			__File.uiFID, __File.uiSIZE, __File.ulADDRESS + iAddOff, iLink,
			__File.uiEFSTATUS, __File.i2GAC, __File.uiRECORDLENGTH,
			__File.i3GAC, __File.ui3GARRLEVEL, __File.ui3GSFI); break;
		case DEF_FILESTRUCTURE_CLINK:       csFileHead = _BuiltEF_CLINK(iParent,
			__File.uiFID, __File.uiSIZE, __File.ulADDRESS + iAddOff, iLink,
			__File.uiEFSTATUS, __File.i2GAC, __File.uiRECORDLENGTH,
			__File.i3GAC, __File.ui3GARRLEVEL, __File.ui3GSFI); break;
		default:bRet = FALSE; break;


		}
		break;
	case DEF_FILETYPE_MF:csFileHead = _BuiltMF(iParent, iPinList,
		__File.uiFID, __File.uiDFSTATUS, __File.i3GAC, __File.ui3GARRLEVEL,
		__File.uiCHVNUMBER, __File.uiDFNUMBER, __File.uiEFNUMBER); break;
	case DEF_FILETYPE_DF:
		if (!__File.b3G)
			iPinList = 0x01000A;
		csFileHead = _BuiltDF(iParent, iPinList,
		__File.uiFID, __File.uiDFSTATUS, __File.i3GAC, __File.ui3GARRLEVEL,
		__File.uiCHVNUMBER, __File.uiDFNUMBER, __File.uiEFNUMBER); break;
	case DEF_FILETYPE_ADF:csFileHead = _BuiltADF(iParent, iPinList,
		__File.uiFID, __File.uiDFSTATUS, __File.i3GAC, __File.ui3GARRLEVEL,
		__File.uiCHVNUMBER, __File.uiDFNUMBER, __File.uiEFNUMBER,
		__File.uiAIDNUMBER); break;
	default:bRet = FALSE; break;
	}

	return bRet;

}

BOOL ProfileClass::BuiltFile(CString&csFileHead, const int iAddOff)
{
	CString __temp;
	long lCount = GetDocRecordCount();
	csFileHead.Empty();
	for (long i = 1; i <= lCount; i++)
	{
		__temp.Empty();
		if (!_BuiltFile(i, __temp, iAddOff))
			return FALSE;
		csFileHead += __temp;
	}

	//补充预留位置
	_variant_t _temp;
	if (!GetFRUData(DEF_RFU_RFU_FILE, _temp))
		return FALSE;
	csFileHead += RepeatCString("FF", _variant_t2Int(_temp) * 18);


	return TRUE;
}

BOOL ProfileClass::__BuiltContent(ADODB::_RecordsetPtr& m_pRecord, CString& csContent)
{
	CString csResult;
	_variant_t __Content,__Type,__Structure;

	if (_GetCurrentRecordSubData(m_pRecord, DEF_FILELIST_TYPE_COLUMN, __Type) != DEF_PROFILE_SUCCESS)
		return FALSE;

	if (_ExChangeFileType(__Type) != DEF_FILETYPE_EF )
		return TRUE;


	if (_GetCurrentRecordSubData(m_pRecord, DEF_FILELIST_STRUCTURE_COLUMN, __Structure) != DEF_PROFILE_SUCCESS)
		return FALSE;

	if ((_ExChangeFileStructure(__Structure) == DEF_FILESTRUCTURE_TLINK)||
		(_ExChangeFileStructure(__Structure) == DEF_FILESTRUCTURE_LLINK)||
		(_ExChangeFileStructure(__Structure) == DEF_FILESTRUCTURE_CLINK))
		return TRUE;

	if (_GetCurrentRecordSubData(m_pRecord, DEF_FILELIST_CONTENT_COLUMN, __Content)<0)
		return FALSE;

	csResult = _variant_t2CString(__Content);
	
	_DeleteNote(csResult);
	_DeleteEnterSpace(csResult);

	csContent = csResult;

	return TRUE;
}

BOOL ProfileClass::_BuiltContent(long nRecord, CString& csContent)
{
	
	if (!_SelectRecord(m_pDocRecordset, nRecord, FALSE))
		return FALSE;

	return __BuiltContent(m_pDocRecordset, csContent);

}

BOOL ProfileClass::BuiltContent(CString& csContent)
{
	CString __temp;
	long lCount = GetDocRecordCount();
	csContent.Empty();
	
	for (long i = 1 ; i <= lCount; i++)
	{
		__temp.Empty();

		if (!_BuiltContent(i, __temp))
			return FALSE;
		csContent += __temp;
	}

	return TRUE;
}

BOOL ProfileClass::BuiltFlashSize(long*  lSize)
{
	long lCount = GetDocRecordCount();
	_variant_t __temp;
	long __Size = 0;
	try
	{
		m_pDocRecordset->MoveFirst();
		for (long i = 0; i < lCount; i++)
		{
			__temp.Clear();
			if (i!=0)
				m_pDocRecordset->MoveNext();

			if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_TYPE_COLUMN, __temp) != DEF_PROFILE_SUCCESS) 
				continue;
		

			if (_ExChangeFileType(__temp) != DEF_FILETYPE_EF)
				continue;

			if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_STRUCTURE_COLUMN, __temp) != DEF_PROFILE_SUCCESS)
				continue;



			int iSTRUCTURE = _ExChangeFileStructure(__temp);

			if ((iSTRUCTURE == DEF_FILESTRUCTURE_TLINK) ||
				(iSTRUCTURE == DEF_FILESTRUCTURE_CLINK) ||
				(iSTRUCTURE == DEF_FILESTRUCTURE_LLINK))
				continue;

			__temp.Clear();

			if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_SIZE_COLUMN, __temp) != DEF_PROFILE_SUCCESS)
				continue;

			__Size = __Size + _variant_t2Int(__temp);
		}

	}
	catch (_com_error(e))
	{ 
		DisplayAccessError(e);
		return FALSE;
	}
	*lSize = __Size;
	return TRUE;
}


BOOL ProfileClass::BuiltFRU(CString& csFRU, int& iAoff)
{
	_variant_t _temp;
	int iFileNumber = GetDocRecordCount();

	if (!GetFRUData(DEF_RFU_RFU_FILE, _temp))
		return FALSE;

	iFileNumber = iFileNumber + _variant_t2Int(_temp);

	iAoff  = iFileNumber * 18 + 0x200;

	long lSpace = 0;

	if (!BuiltFlashSize(&lSpace))
		return FALSE;
	else
		lSpace = lSpace + iAoff;


	CString csPin1;
	CString csPuk1;
	CString csPin2;
	CString csPuk2;
	CString csADM;
	CString csPS;
	CString csKI;

	if (GetFRUData(DEF_RFU_CHV1, _temp))
		csPin1 = _variant_t2CString(_temp);
	else
		return FALSE;

	if (GetFRUData(DEF_RFU_UNCHV1, _temp))
		csPuk1 = _variant_t2CString(_temp);
	else
		return FALSE;

	if (GetFRUData(DEF_RFU_CHV2, _temp))
		csPin2 = _variant_t2CString(_temp);
	else
		return FALSE;

	if (GetFRUData(DEF_RFU_UNCHV2, _temp))
		csPuk2 = _variant_t2CString(_temp);
	else
		return FALSE;

	if (GetFRUData(DEF_RFU_ADM1, _temp))
		csADM = _variant_t2CString(_temp);
	else
		return FALSE;

	if (GetFRUData(DEF_RFU_ADM2, _temp))
		csPS = _variant_t2CString(_temp);
	else
		return FALSE;

	if (GetFRUData(DEF_RFU_KI, _temp))
		csKI = _variant_t2CString(_temp);
	else
		return FALSE;


	return _BuiltFRU(csFRU, iFileNumber, lSpace, csPin1, csPuk1, csPin2, csPuk2, csADM, csPS, csKI);
}

BOOL ProfileClass::_BuiltFRU(CString &csFRU,
	int iFileNumber, long lSpace,
	CString csPin1, CString csPuk1,
	CString csPin2, CString csPuk2,
	CString csADM, CString csPS,
	CString csKI,
	int iStaPin1, int iStaPuk1,
	int iStaPin2, int iStaPuk2,
	int iStaADM, int iStaPS,
	int iEnablePin1, int iEnablePin2,
	int iEx1, int iEx2)
{
	CString csTemp;
	csFRU.Empty();
	csFRU += RepeatCString(_T("FF"), 0x8);
	csFRU += Int2CString(iFileNumber, 4);
	csFRU += RepeatCString(_T("FF"), 12);
	csFRU += Int2CString(iEx1, 4);
	csFRU += Int2CString(iEx2, 4);
	csFRU += RepeatCString(_T("FF"), 6);
	csFRU += csPin1;
	csFRU += csPuk1;
	csFRU += csPin2;
	csFRU += csPuk2;
	csFRU += csADM;
	csFRU += csKI;
	csFRU += Int2CString(iStaPin1, 2);
	csFRU += Int2CString(iStaPuk1, 2);
	csFRU += Int2CString(iEnablePin1, 2);
	csFRU += Int2CString(iStaPin2, 2);
	csFRU += Int2CString(iStaPuk2, 2);
	csFRU += Int2CString(iEnablePin2, 2);
	csFRU += Int2CString(iStaADM, 2);
	csFRU += Int2CString(iStaPS, 2);
	csFRU += csPS;
	csFRU += RepeatCString(_T("FF"), 0x98);
	csTemp.Format("%010X", lSpace);
	csFRU += csTemp;
	csFRU += RepeatCString(_T("FF"), 0xFB);

	return TRUE;
}






int ProfileClass::CheckProfileAddress(BOOL bModify )
{
	_variant_t __Type, __Size, __Address;
	CString csText;
	long lCount = GetDocRecordCount();
	UINT iADDRESS = 0;
	int iRet = TRUE;


	m_pDocRecordset->MoveFirst();

	for (long i = 1; i <= lCount; i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;


		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_TYPE_COLUMN, __Type) != DEF_PROFILE_SUCCESS) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_SIZE_COLUMN, __Size) != DEF_PROFILE_SUCCESS) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ADDRESS_COLUMN, __Address) != DEF_PROFILE_SUCCESS))
		{
			OutputToWnd( DEF_ERROR_TAG,  csText + _T("获取文件相关信息出错，此处跳出并结束。"));
			return -2;
		}

		if (_ExChangeFileType(__Type) != DEF_FILETYPE_EF)
		{
			OutputToWnd( DEF_NORMAL_TAG, + (csText + _T(" 不是EF,此处跳过! ")));
			continue;
		}

		if (_CurrentRecordIsLinkRecord(m_pDocRecordset))
		{
			OutputToWnd( DEF_NORMAL_TAG, + (csText + _T(" LINK文件,此处跳过! ")));
			continue;
		}

		if ((int)__Address!= (int)iADDRESS)
		{
			OutputToWnd( DEF_WARN_TAG, (csText + _T(" 文件地址并非连续，可能存在问题！")));
			iRet = FALSE;
			if (bModify)
			{
				if (_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ADDRESS_COLUMN, __Address))
				{
					OutputToWnd(DEF_NORMAL_TAG, (csText + _T(" 修改文件地址为") + Int2CStringDecimal((int)__Address)));
					iRet = 10;
				}
				else
				{
					OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 修改文件地址失败")));
					iRet = -10;
				}	
			}
				
		}
		else
			OutputToWnd( DEF_NORMAL_TAG, + (csText + _T(" 检查完成! ")));
		iADDRESS += (int)__Size;
	}

	return iRet;
}

int ProfileClass::CheckProfileHandle(BOOL bModify )
{
	CString csText;
	long lCount = GetDocRecordCount();
	UINT iADDRESS = 0;
	int iRet = TRUE;
	_variant_t __RECORD;

	for (long i = 1; i <= lCount; i++)
	{
	
		if (i == 1)
			m_pDocRecordset->MoveFirst();
		else
			m_pDocRecordset->MoveNext();

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;

		if(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_HANDLE_COLUMN, __RECORD) != DEF_PROFILE_SUCCESS)
		{
			OutputToWnd( DEF_ERROR_TAG,  csText + _T("获取文件相关信息出错，此处跳出并结束。"));
			return -2;
		}

		if ((int)__RECORD != i)
		{
			OutputToWnd( DEF_WARN_TAG, (csText + _T(" 文件句柄可能存在问题！")));
			iRet = FALSE;
			if (bModify)
			{
				if (_SetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_HANDLE_COLUMN, Int2_variant_t(VT_I4, i)))
				{
					OutputToWnd(DEF_HINT_TAG, (csText + _T(" 修改文件句柄为") + Int2CStringDecimal((int)i)));
					iRet = 10;
				}

				else
				{
					OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 修改文件句柄失败")));
					iRet = -10;
				}
					
			}
		}
		else
			OutputToWnd( DEF_NORMAL_TAG,  csText + _T("文件句柄正确。"));


	}


		
	return iRet;
}



int ProfileClass::CheckProfileStructure( )
{

	_variant_t __Route,__Type;
	CString csText;
	CStringArray csRoute;
	long lCount = GetDocRecordCount();
	int iNumber = GetDFList(csRoute, TRUE, TRUE);
	int iRet = TRUE;
	int iCur = FALSE;


	m_pDocRecordset->MoveFirst();

	for (long i = 1; i <= lCount; i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;


		if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_TYPE_COLUMN, __Type) != DEF_PROFILE_SUCCESS)
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 获取文件类别失败")));
			continue;
		}

		if (_ExChangeFileType(__Type) == DEF_FILETYPE_MF)
		{
			OutputToWnd( DEF_NORMAL_TAG, + (csText + _T(" 是MF，此处忽略。")));
			continue;
		}

		if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ROUTE_COLUMN, __Route)!= DEF_PROFILE_SUCCESS)
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 获取路径失败")));
			continue;
		}


		iCur = FALSE;
		for (int j = 0 ; j < iNumber; j++)
		{
			if (csRoute.GetAt(j).Compare(_variant_t2CString(__Route)) == 0)
			{
				OutputToWnd( DEF_NORMAL_TAG, (csText + _T(" 路径正确！")));
				iCur = TRUE;
				continue;
			}
		}

		if (!iCur)
		{
			OutputToWnd( DEF_WARN_TAG, (csText + _T(" 路径存在问题，此处无法自动修改，请手动更正！")));
			iRet = FALSE;
		}

	}

	

	return TRUE;

}

int ProfileClass::CheckDFSFI(CString csRoute )
{ 
	CString csText;
	CStringArray _SFIFile;
	_variant_t __ROUTE, __SFI;
	long lCount = GetDocRecordCount();
	int iRet = TRUE;

	for (int _iSFI = 1; _iSFI < 0x1F; _iSFI++)
	{
		_SFIFile.RemoveAll();
		m_pDocRecordset->MoveFirst();
		for (long i = 1; i < lCount; i++)
		{
			if (!_SelectRecord(m_pDocRecordset, i))
				return -1;

			if (!GetFileBasicInformation(m_pDocRecordset,csText))
				break;

			if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ROUTE_COLUMN, __ROUTE) != DEF_PROFILE_SUCCESS)
			{
				OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 获取路径失败，此处跳出并结束！")));
				return -2;
			}

			if (_variant_t2CString(__ROUTE).Compare(csRoute) != 0)
				continue;

			if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GSFI_COLUMN, __SFI) != DEF_PROFILE_SUCCESS)
			{
				OutputToWnd( DEF_ERROR_TAG, (csText + _T(" 获取路径失败，此处跳过！")));
				continue;
			}
			if (__SFI == (_variant_t)Int2CStringDecimal(_iSFI))
				_SFIFile.Add(csText);
		}
		if (_SFIFile.GetCount()>1)
		{

			CString csWarn;

			csWarn.Empty();

			for (int i = 0 ; i < _SFIFile.GetCount(); i++)
			{
				csWarn += _T(" ");
				csWarn += _SFIFile.GetAt(i);
			}

			csWarn = _T("SFI - Warnning! ") + csWarn + _T(" are all defined to ") + Int2CStringDecimal(_iSFI);

		//	csWarn = _T("SFI of ") + csWarn + _T(" is repetition! (") + Int2CString(_iSFI)+_T(")");
			OutputToWnd( DEF_WARN_TAG, csWarn);
			OutputToWnd( DEF_WARN_TAG, CString (_T("此处无法自动修改，需要手动修改！")));
			iRet = FALSE;
		}
		else
		{
			if (_SFIFile.GetCount() == 0)
			{
				OutputToWnd(DEF_NORMAL_TAG, csRoute  +_T("下没有文件定义") + Int2CStringDecimal(_iSFI));
			}
			else
				OutputToWnd(DEF_NORMAL_TAG, csRoute+ _T(" 下 ")+_SFIFile.GetAt(0)+_T("定义成") + Int2CStringDecimal(_iSFI));

			

		}

	}

	return iRet;
}

int ProfileClass::CheckProfileSFI()
{
	CStringArray csRoute;
	int iNumber = GetDFList(csRoute, TRUE, TRUE);
	int iRet = TRUE;


	for (int i = 0 ; i < iNumber ; i++)
	{
		iRet&=CheckDFSFI(csRoute.GetAt(i));
	}
	return iRet;



}

int ProfileClass::CheckProfileLinks()
{

	CString csText;
	long lCount = GetDocRecordCount();
	int iRet = TRUE;

	_variant_t __SIZE, __RECORDNUMBER, __RECORDLENGTH;
	_variant_t __PSIZE, __PRECORDNUMBER, __PRECORDLENGTH;

	long lPlink;
	m_pDocRecordset->MoveFirst();

	for (long i = 1; i <= lCount; i++)
	{
		lPlink = 0;
		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;

		if (!_CurrentRecordIsLinkRecord(m_pDocRecordset))
			continue;
	

		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_SIZE_COLUMN, __SIZE) != DEF_PROFILE_SUCCESS)||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_NUMBER_COLUMN, __RECORDNUMBER) != DEF_PROFILE_SUCCESS)||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_LENGTH_COLUMN, __RECORDLENGTH) != DEF_PROFILE_SUCCESS))
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 获取文件大小失败")));
			continue;
		}

		if ((!RecordIsLinkRecord(i, &lPlink))||
			(!_SelectRecord(m_pDocRecordset, i)))
	
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 获取 关联文件失败")));
			continue;
		}

		if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_SIZE_COLUMN, __PSIZE) != DEF_PROFILE_SUCCESS) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_NUMBER_COLUMN, __PRECORDNUMBER) != DEF_PROFILE_SUCCESS) ||
			(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_LENGTH_COLUMN, __PRECORDLENGTH) != DEF_PROFILE_SUCCESS))
		{
			OutputToWnd( DEF_ERROR_TAG,  (csText + _T(" 获取关联文件大小失败")));
			continue;
		}

		if ((__PSIZE != __SIZE) ||
			(__PRECORDNUMBER != __RECORDNUMBER) ||
			(__PRECORDLENGTH != __RECORDLENGTH))

		{
			OutputToWnd( DEF_WARN_TAG, csText + _T(" 和关联文件相关属性不同，(大小，记录长度，记录条数)！"));
			iRet = FALSE;
		}
		else
			OutputToWnd(DEF_NORMAL_TAG, csText + _T(" 和关联文件相关属性相同，(大小，记录长度，记录条数)！"));
	}

	return iRet;

}

int ProfileClass::CheckProfileSingleFile()
{

	CString csText;
	CStringArray csFileError;
	long lCount = GetDocRecordCount();
	int iRet = TRUE;
	PCFS _tempFile;
	for (long i = 1; i<lCount; i++)
	{
		if (!_SelectRecord(m_pDocRecordset, i))
			return -1;

		if (!GetFileBasicInformation(m_pDocRecordset,csText))
			break;
		//csText = _T("Record ")+Int2CStringDecimal(i) +_T(" file");

		_tempFile.EmptyFile();
		if (GetFileFromDoc(i, &_tempFile)<0)
		{
			OutputToWnd( DEF_WARN_TAG, csText + _T(" 获取文件失败，此处跳过！"));
			continue;
		}


		csFileError.RemoveAll();
		if (_tempFile.CheckParameter(csFileError)>0)
		{
			OutputToWnd( DEF_ERROR_TAG,  csText + _T("  属性存在错误，具体如下！"));

			for (int j = 0 ; j< csFileError.GetCount();j++)
				OutputToWnd(DEF_ERROR_TAG,csFileError.GetAt(j));

			iRet = FALSE;
			//continue;
		}
		else
		{
			OutputToWnd( DEF_NORMAL_TAG, csText + _T("  未找出错误属性！"));
		}



	}
	return iRet;

}

int ProfileClass::CheckDIR2F00()
{
	_variant_t __DIR,__RecordNumber,__RecordLength,__AIDList,__AIDNumber,__AIDLength;
	CStringArray csDIRList,csFIDList;
	CString _temp1,_temp2,_temp3;
	BOOL bRet = TRUE;
	if (GetFileRecord(_T("3F002F00")) <= 0)
	{
		OutputToWnd(DEF_WARN_TAG, _T("有没有找到DIR(3F002F00)文件！"));
		return FALSE;
	}


	if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_CONTENT_COLUMN, __DIR)<=0)||
	    (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_NUMBER_COLUMN, __RecordNumber)!=0)||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_LENGTH_COLUMN, __RecordLength)!= 0))
	{
		OutputToWnd(DEF_WARN_TAG, _T("获取DIR(3F002F00)文件内容失败！"));
		return FALSE;
	}

	if (GetFileRecord(_T("3F000018")) <= 0)
	{
		OutputToWnd(DEF_WARN_TAG, _T("有没有找到AID List(3F000018)文件！"));
		return FALSE;
	}


	if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_CONTENT_COLUMN, __AIDList) <= 0)||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_NUMBER_COLUMN, __AIDNumber) != 0)||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_LENGTH_COLUMN, __AIDLength) != 0))
	{
		OutputToWnd(DEF_WARN_TAG, _T("获取AID List(3F000018)文件内容失败！"));
		return FALSE;
	}



	for (int i = 0; i < _variant_t2Int(__AIDNumber); i++)
	{
		_temp1 = _variant_t2CString(__AIDList);
		_DeleteEnterSpace(_temp1);
		_temp2 = _temp1.Mid(i*(int)__AIDLength * 2, (int)__AIDLength * 2);
		_temp3 = _temp2.Mid(2, _CString2Int(_temp2.Left(2)) * 2);

		csDIRList.Add(_temp3);

		csFIDList.Add(_temp2.Right(4));
	}

	for (int i = 0; i < _variant_t2Int(__RecordNumber); i++)
	{
		_temp1 = _variant_t2CString(__DIR);
		_DeleteEnterSpace(_temp1);
		_temp2 = _temp1.Mid(i*(int)__RecordLength * 2, (int)__RecordLength * 2);

		if (!_GetTLVData(_temp2, 0x61, _temp3))
			continue;
		if (!_GetTLVData(_temp3, 0x4F, _temp2))
			continue;
		bRet = FALSE;
		int j;
		for ( j = 0; j < csDIRList.GetCount(); j++)
		{
			if (csDIRList.GetAt(j).Compare(_temp2) == 0)
			{
				bRet = TRUE;
				break;
			}
				
		}

		if (bRet)
		{
			OutputToWnd(DEF_HINT_TAG, _T("Record ")+ Int2CStringDecimal(i+1) + _T(" 找到对应AID(") + _temp2 +_T(") 关联 ADF \"")+ csFIDList.GetAt(j)+_T("\""));
		}
		else
		{
			OutputToWnd(DEF_WARN_TAG, _T("Record ") + Int2CStringDecimal(i) + _T(" 没有找到对应AID指引！"));
		}
	}
	return bRet;
}

int ProfileClass::CheckARR()
{

	BOOL bRet = TRUE;
	CStringArray csDF;
	CStringArray csArrayARR;

	int iFileCount = GetDocRecordCount();
	int iCount = GetDFList(csDF, TRUE, TRUE);

	//获取ARR文件
	iCount = GetARRFileList(csArrayARR);

	if (iCount == 0)
	{
		OutputToWnd(DEF_ERROR_TAG,_T("没有找到ARR文件！"));
		return -1;
	}


	iCount = csArrayARR.GetCount();

	//开始遍历 ARR文件RECORD
	for (int i = 0; i< iCount; i++)
	{
		_variant_t __RecordNumber;
		if (GetFileRecord(csArrayARR.GetAt(i)) <= 0)
		{
			OutputToWnd(DEF_WARN_TAG, csArrayARR.GetAt(i) + _T("选择失败，此处跳过！"));
			continue;
		}


		if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_NUMBER_COLUMN, __RecordNumber) != 0)
		{
			OutputToWnd(DEF_WARN_TAG, csArrayARR.GetAt(i ) + _T("获取文件属性，此处跳过！"));
			continue;
		}


		for (int j = 0; j< (int) __RecordNumber;j++)
		{
			CStringArray csFile_ARRRECORD;

			for (int k = 1; k <= iFileCount ; k++)
			{
				
				if (!_SelectRecord(m_pDocRecordset, k))
					continue;
				_variant_t __3G;
				if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GMODE_COLUMN, __3G) != 0) ||
					(_variant_t2Int(__3G) != 1))
					continue;
				_variant_t __Route, __ARRFile,__ARRC,__FID;
				if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_FID_COLUMN, __FID) != 0) ||
					(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_ROUTE_COLUMN, __Route) != 0) ||
					(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GARRFile_COLUMN, __ARRFile) != 0)||
					(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GACC_COLUMN, __ARRC) != 0))
					continue;

				//此处判断如果是LINK文件需要指引过去
				CString _tempFile = GenerateARRPath(_variant_t2CString(__ARRFile), _variant_t2CString(__Route));
				CString _SureFile;

				if (!RecordIsLinkRecord(GetFileRecord(_tempFile), _SureFile))
					_SureFile = _tempFile;

				if(csArrayARR.GetAt(i).Compare(_SureFile) != 0)
					continue;
				if (j+1 != _variant_t2Int(__ARRC))
					continue;


				csFile_ARRRECORD.Add(_variant_t2CString(__Route) + _variant_t2CString(__FID));

			}

			if (csFile_ARRRECORD.GetCount()==0)
			{
				bRet = FALSE;
				OutputToWnd(DEF_WARN_TAG, csArrayARR.GetAt(i) + _T(" record ")+Int2CStringDecimal(j+1) +_T("没有文件引用！"));
			}
			else
			{
				OutputToWnd(DEF_NORMAL_TAG, csArrayARR.GetAt(i) + _T(" record ") + Int2CStringDecimal(j+1) + _T("引用的文件如下:"));

				for (int k = 0 ; k < csFile_ARRRECORD.GetCount() ;k++)
					OutputToWnd(DEF_NORMAL_TAG, csFile_ARRRECORD.GetAt(k));

			}


			
		}
	
	}
	return bRet;
}


int ProfileClass::CheckTypeFilesIsExisted(CString csRoute, CString csType, UINT iType, UINT uiNumber)
{
	CString __FILE, __FID, __SFI;
	int iRet = TRUE;
	CString csName;

	for (UINT i = 0xC0; i <= 0xCC; i++)
	{
		__FILE = GetTLVData(csType, i);

		if (__FILE.IsEmpty())
			continue;



		__FID = __FILE.Mid(0, 4);
		__SFI = __FILE.Mid(4);

		switch (i&0xF)
		{
		case 0:csName = _T("ADN");break;
		case 1:csName = _T("IAP"); break;
		case 2:csName = _T("EXT1"); break;
		case 3:csName = _T("SNE"); break;
		case 4:csName = _T("ANR"); break;
		case 5:csName = _T("PBC"); break;
		case 6:csName = _T("GRP"); break;
		case 7:csName = _T("AAS"); break;
		case 8:csName = _T("GAS"); break;
		case 9:csName = _T("SNE"); break;
		case 0xA:csName = _T("UID"); break;
		case 0xB:csName = _T("CPP1"); break;
		case 0xC:csName = _T("PURI"); break;
		default: continue;

		}

		switch (iType)
		{

		case 1:if ((i == 0xC7) || (i == 0xC2) || (i == 0xC8) || (i == 0xCB))
				OutputToWnd(DEF_ERROR_TAG, _T("TYPE1 的数据不应该存在")+ csName);
			break;
		case 2:
			if ((i != 0xC4) &&(i != 0xCA) && (i != 0xC3) && (i != 0xCC))
				OutputToWnd(DEF_ERROR_TAG, _T("TYPE2 的数据不应该存在") + csName);
			break;
		case 3:
			if ((i != 0xC7) && (i != 0xC8) && (i != 0xC2) && (i != 0xCB))
				OutputToWnd(DEF_ERROR_TAG, _T("TYPE3 的数据不应该存在") + csName);
			break;
		}


		csName += (_T("-")+Int2CStringDecimal(uiNumber));

		if (GetFileRecord(csRoute + __FID)<=0)
		{
			OutputToWnd(DEF_ERROR_TAG, csRoute+_T("目录下没有")+ csName +_T("文件的FID是")+__FID);
			iRet = FALSE;
		}

		if(__SFI.IsEmpty())
			continue;

		_variant_t _SFI;
		if (_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_3GSFI_COLUMN, _SFI)!= 0)
		{
			OutputToWnd(DEF_ERROR_TAG, csName + _T("(") + csRoute + __FID + _T(")SFI 为0"));
			iRet = FALSE;
			continue;
		}

		if (_CString2Int(__SFI)!=  (int)_SFI)
		{
			OutputToWnd(DEF_ERROR_TAG, csName + _T("(") + csRoute + __FID + _T(")SFI 应该为") + __SFI + _T("(16进制),实际为") + Int2CString((int)_SFI) + _T("(16进制)!"));
			iRet = FALSE;
			continue;
		}

		OutputToWnd(DEF_NORMAL_TAG, csName + _T("(") + csRoute + __FID + _T(") 文件 PBR指引正确"));
	}

	return iRet;
}

int ProfileClass::CheckPBRRecord(CString csRoute,CString csRecord, UINT uiNumber)
{
	CString _Type1, _Type2, _Type3;
	int iRet = TRUE;

	_Type1 = GetTLVData(csRecord, 0xA8);

	if(!_Type1.IsEmpty())
		iRet &= CheckTypeFilesIsExisted(csRoute, _Type1, 1, uiNumber);

	_Type2 = GetTLVData(csRecord, 0xA9);

	if (!_Type2.IsEmpty())
		iRet &= CheckTypeFilesIsExisted(csRoute, _Type2, 2, uiNumber);

	
	_Type3 = GetTLVData(csRecord, 0xAA);

	if (!_Type3.IsEmpty())
		iRet &= CheckTypeFilesIsExisted(csRoute, _Type3, 3, uiNumber);

	return iRet;
}

int ProfileClass::CheckPBR(CString csRoute)
{
	_variant_t __Content,__Length,__Number;
	CString csContent;
	CString csRecord;
	int iRet = TRUE;

	if (GetFileRecord(csRoute+_T("4F30")) <= 0)
	{
		OutputToWnd(DEF_WARN_TAG, _T("有没有找到PBR(")+ csRoute +_T("4F30)文件！"));
		return -1;
	}

	 
	if ((_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_LENGTH_COLUMN, __Length) != DEF_PROFILE_SUCCESS)||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_RECORD_NUMBER_COLUMN, __Number) != DEF_PROFILE_SUCCESS)||
		(_GetCurrentRecordSubData(m_pDocRecordset, DEF_FILELIST_CONTENT_COLUMN      ,__Content) <= DEF_PROFILE_SUCCESS))
	{
		OutputToWnd(DEF_WARN_TAG, _T("获取PBR(") + csRoute + _T("4F30)文件内容失败！"));
		return -2;
	}

	csContent = _variant_t2CString(__Content);

	_DeleteEnterSpace(csContent);

	for (int i = 0 ; i < (int)__Number ; i++)
	{
		csRecord = csContent.Mid(i*(int)__Length * 2, (int)__Length * 2);

		iRet &= CheckPBRRecord(csRoute, csRecord, i+1);
	}

	return iRet;
}

int ProfileClass::GetFRUData(int iID, _variant_t& __data)
{

	try
	{
		__data.Clear();

		m_pFRURecordset->MoveFirst();

		if (_GetCurrentRecordSubData(m_pFRURecordset, iID, __data) !=  DEF_PROFILE_SUCCESS)
			return FALSE;

	}
	catch (_com_error (e))
	{
		DisplayAccessError(e);
		return FALSE;
		
	}


	return TRUE;
}

int ProfileClass::SetFRUData(int iID, _variant_t __data)
{


	try
	{

		m_pFRURecordset->MoveFirst();
		if (!_SetCurrentRecordSubData(m_pFRURecordset, iID, __data))
			return FALSE;
	}
	catch (_com_error(e))
	{
		DisplayAccessError(e);
		return FALSE;

	}

	m_pFRURecordset->Update();
	return TRUE;


}