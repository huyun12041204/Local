#include "stdafx.h"
#include "PrivateCardFileStruct.h"


int _ExChangeFileStructure(_variant_t cTemp)
{
	CString csTemp;
	int iRet = 0;
	if (!__variant_t2CString(cTemp,csTemp))
		return iRet;
	if (csTemp.Compare(_T("Transparent"))== 0)
		iRet = DEF_FILESTRUCTURE_TRANSPARENT;
	if (csTemp.Compare(_T("Linear Fixed"))== 0)
		iRet = DEF_FILESTRUCTURE_LINER;
	if (csTemp.Compare(_T("Cycle"))== 0)
		iRet = DEF_FILESTRUCTURE_CYCLIC;
	if (csTemp.Compare(_T("TLink"))== 0)
		iRet = DEF_FILESTRUCTURE_TLINK;
	if (csTemp.Compare(_T("LLink"))== 0)
		iRet = DEF_FILESTRUCTURE_LLINK;
	if (csTemp.Compare(_T("CLink"))== 0)
		iRet = DEF_FILESTRUCTURE_CLINK; 
	return iRet;
}
 
int _ExChangeFileType(_variant_t cTemp)
{
	CString csTemp;
	int iRet = 0;

	if (!__variant_t2CString(cTemp,csTemp))
		return 0;
	csTemp.MakeUpper();
	if (csTemp.Compare(_T("MF")) == 0)
		return DEF_FILETYPE_MF;
	if (csTemp.Compare(_T("DF"))== 0)
		return DEF_FILETYPE_DF;
	if (csTemp.Compare(_T("ADF"))== 0)
		return DEF_FILETYPE_ADF;
	if (csTemp.Compare(_T("EF"))== 0)
		iRet = DEF_FILETYPE_EF;
	return iRet;
}

_variant_t _TranslateFileStructure(int iCode)
{
	_variant_t _temp;
	switch (iCode)
	{
	case  DEF_FILESTRUCTURE_TRANSPARENT: _temp = CString2_variant_t(VT_BSTR,_T("Transparent"));break;
	case  DEF_FILESTRUCTURE_LINER:       _temp = CString2_variant_t(VT_BSTR,_T("Linear Fixed"));break;
	case  DEF_FILESTRUCTURE_CYCLIC:      _temp = CString2_variant_t(VT_BSTR,_T("Cycle"));break;
	case  DEF_FILESTRUCTURE_TLINK:       _temp = CString2_variant_t(VT_BSTR,_T("TLink"));break;
	case  DEF_FILESTRUCTURE_LLINK:       _temp = CString2_variant_t(VT_BSTR,_T("LLink"));break;
	case  DEF_FILESTRUCTURE_CLINK:       _temp = CString2_variant_t(VT_BSTR,_T("CLink"));break;
	default: _temp.vt = VT_EMPTY;
	}
	return _temp;
}
_variant_t _TranslateFileType(int iCode)
{
	_variant_t _temp;
	switch (iCode)
	{
	case  DEF_FILETYPE_EF:       _temp = CString2_variant_t(VT_BSTR,_T("EF"));break;
	case  DEF_FILETYPE_DF:       _temp = CString2_variant_t(VT_BSTR,_T("DF"));break;
	case  DEF_FILETYPE_MF:       _temp = CString2_variant_t(VT_BSTR,_T("MF"));break;
	case  DEF_FILETYPE_ADF:      _temp = CString2_variant_t(VT_BSTR,_T("ADF"));break;
	default: _temp.vt = VT_EMPTY;
	}
	return _temp;

}

CString _Translate2GAC(char cAC, BOOL bFormat /*= FALSE*/)
{

	CString csAC;
	switch(cAC)
	{
	case 0:   csAC = _T("ALWAYS");break;
	case 1:   csAC = _T("CHV1");break;
	case 2:   csAC = _T("CHV2");break;
	case 4:   csAC = _T("CHV4");break;
	case 0xF: csAC = _T("NEVER");break;
	case -1:  csAC = _T("");break;
	default:  csAC = _T("ERROR");break;
	}

	if (bFormat)
	{
		csAC = csAC + _T("         ");
		csAC = csAC.Left(6);
	}

	return csAC;


}

CString _Translate3GAC(char cAC)
{

	CString csAC;
	switch(cAC)
	{
	case 0:   csAC = _T("ALWAYS");break;
	case 1:   csAC = _T("PIN 01");break;
	case 2:   csAC = _T("PIN 81");break;
	case 4:   csAC = _T("PIN 0A");break;
	case 0xA: csAC = _T("PIN 0A");break;
	case 0xF: csAC = _T("NEVER");break;
	case -1:  csAC = _T("");break;
	case -0xF:csAC = _T("ABSENCE");break;
	default:  csAC = _T("ERROR");break;
	}
	return csAC;
}

char _Translate2GACText(CString csText)
{
	DeleteEnterSpace(csText);
	char cAC = 0xF;
	csText.MakeUpper();
	if (csText.Compare(_T("AlWAY")) == 0)
		cAC = 0;
	if (csText.Compare(_T("ALWAYS")) == 0)
		cAC = 0;
	if (csText.Compare(_T("CHV1")) == 0)
		cAC = 1;
	if (csText.Compare(_T("CHV2")) == 0)
		cAC = 2;
	if (csText.Compare(_T("CHV4")) == 0)
		cAC = 4;
	if (csText.Compare(_T("NEVER")) == 0)
		cAC = 0xF;
	if (csText.Compare(_T("")) == 0)
		cAC = -1;
	return cAC;
}

char _Translate3GACText(CString csText)
{

	char cAC = -0xF;
	csText.MakeUpper();
	if (csText.Compare(_T("ALWAYS")) == 0)
		cAC = 0;
	if (csText.Compare(_T("PIN 01")) == 0)
		cAC = 1;
	if (csText.Compare(_T("PIN 81")) == 0)
		cAC = 2;
	if (csText.Compare(_T("PIN 0A")) == 0)
		cAC = 0xA;
	if (csText.Compare(_T("NEVER")) == 0)
		cAC = 0xF;
	if (csText.Compare(_T("ABSENCE")) == 0)
		cAC = -0xF;
	if (csText.Compare(_T("")) == 0)
		cAC = -1;
	return cAC;

}

int _TranslateARRFileLevel(CString csFID)
{
	int iLevel = 127;
	if (csFID.GetLength() ==4)
	{
		switch(_CString2Int(csFID))
		{
		case 0x2F06: iLevel= DEF_ARRLEVEL_2F06 ; break;
		case 0x6F06: iLevel= DEF_ARRLEVEL_6F06 ; break;
		case 0x4F06: iLevel= DEF_ARRLEVEL_4F06 ; break;
		}
	}
	return iLevel;
}

CString _TranslateARRFile(int iLevel)
{
	CString csFID;
	switch(iLevel)
	{
	case DEF_ARRLEVEL_2F06: csFID = _T("2F06");break;
	case DEF_ARRLEVEL_4F06: csFID = _T("4F06");break;
	case DEF_ARRLEVEL_6F06: csFID = _T("6F06");break;
	default:csFID.Empty(); break;
	}
	return csFID;
}



UINT _TanslateApplicationType(CString csText)
{
	UINT uiRet = DEF_APPLICAITON_TYPE_NOT;
	if (csText.Compare(_T("固定应用")) == 0)
		uiRet = DEF_APPLICAITON_TYPE_FIXED;
	if (csText.Compare(_T("OTA应用")) == 0)
		uiRet = DEF_APPLICAITON_TYPE_OTA;
	return uiRet;
}

CString _TanslateSTKAPPCode(UINT uiCode)
{

	CString csText;
	csText.Empty();
	if (uiCode == 1)
		csText = _T("固定应用");
	if (uiCode == 2)
		csText = _T("OTA应用");
	return csText;

}

CString Generate2GACRecord(int iRead, int iUpdate, int iIncreate, int iRFM, int iInval, int iReha, BOOL bComment /*= FALSE*/)
{
	CString csComment;

	csComment.Empty();
	if (bComment)
	{
		//csComment = _T("\r\n");
		csComment += _T("    //读:") + _Translate2GAC(iRead,TRUE) + _T(";");
		csComment += _T("  写:") + _Translate2GAC(iUpdate, TRUE) + _T(";");
		csComment += _T("  增加:") + _Translate2GAC(iIncreate, TRUE) + _T(";");
		csComment += _T("  RFM:") + _Translate2GAC(iRFM, TRUE) + _T(";");
		csComment += _T("  恢复:") + _Translate2GAC(iReha, TRUE) + _T(";");
		csComment += _T("  无效:") + _Translate2GAC(iInval, TRUE) + _T(";");


	}


	return Int2CString(iRead, 1) + Int2CString(iUpdate, 1) +
		  Int2CString(iIncreate, 1) + Int2CString(iRFM, 1) +
		  Int2CString(iReha, 1) + Int2CString(iInval, 1)+ csComment;
}

CString _Generate3GACData(int iRead, int iUpdate, int iInval, int iReha, int iAC, CString csAC)
{
	CString csARR;
	csARR.Empty();
	int iCommand = 0;

	if (iAC == iRead)
		iCommand += 1;
	if (iAC == iUpdate)
		iCommand += 2;
	if (iAC == iInval)
		iCommand += 0x8;
	if (iAC == iReha)
		iCommand += 0x10;
	if (iCommand != 0)
	{
		csARR = _T("8001") + Int2CString(iCommand, 2);
		csARR = csARR + csAC;
	}

	return csARR;
}

CString Generate3GARRRecord(int iRead, int iUpdate, int iIncreate, int iInval, int iReha)
{
	CString csResult;
	csResult.Empty();
	csResult += _Generate3GACData(iRead, iUpdate, iInval, iReha, 0, _T("9000"));
	csResult += _Generate3GACData(iRead, iUpdate, iInval, iReha, 1, _T("A406830101950108"));
	csResult += _Generate3GACData(iRead, iUpdate, iInval, iReha, 2, _T("A406830181950108"));
	csResult += _Generate3GACData(iRead, iUpdate, iInval, iReha, 0xA, _T("A40683010A950108"));
	csResult += _Generate3GACData(iRead, iUpdate, iInval, iReha, 0xF, _T("9700"));

	switch (iIncreate)
	{
	case 0: csResult += _T("8401329000"); break;
	case 1: csResult += _T("840132A406830101950108"); break;
	case 2: csResult += _T("840132A406830181950108"); break;
	case 0x81: csResult += _T("840132A406830181950108"); break;
	case 0xF:  csResult += _T("8401329700"); break;
	default:break;
	}
	return csResult;
}

int PrivateCardFileStruct::SetPrivateCardFileStructData(int iCode,_variant_t _Input)
{
	BOOL bRet =TRUE;

	if (_Input.vt == VT_EMPTY)
	{
		if ((iCode != DEF_FILELIST_CONTENT_COLUMN) &&
			(iCode != DEF_FILELIST_NAME_COLUMN)    &&
			(iCode != DEF_FILELIST_ROUTE_COLUMN))
			return FALSE;
		else
			return TRUE;
	}

	switch(iCode)
	{
	case DEF_FILELIST_HANDLE_COLUMN:uiHANDLE     = _variant_t2Int(_Input);break;
	case DEF_FILELIST_FID_COLUMN:   uiFID        = _CString2Int(_Input);break;
	case DEF_FILELIST_NAME_COLUMN:  stFILENAME   = _variant_t2CString(_Input);       break;
	case DEF_FILELIST_ROUTE_COLUMN: stROUTE      = _variant_t2CString(_Input);       break;
	case DEF_FILELIST_TYPE_COLUMN : uiTYPE       = _ExChangeFileType(_Input);break;
	case DEF_FILELIST_STRUCTURE_COLUMN:uiSTRUCT  = _ExChangeFileStructure(_Input);break;
	case DEF_FILELIST_STATUS_COLUMN:uiEFSTATUS   = _variant_t2Int(_Input);break;
	case DEF_FILELIST_SIZE_COLUMN:  uiSIZE       = _variant_t2Int(_Input);break;
	case DEF_FILELIST_RECORD_LENGTH_COLUMN:uiRECORDLENGTH = _variant_t2Int(_Input);break;
	case DEF_FILELIST_RECORD_NUMBER_COLUMN:uiRECORDNUMBER = _variant_t2Int(_Input);break;
	case DEF_FILELIST_2GACC_COLUMN: i2GAC                 =  _variant_t2Int(_Input);break;
	case DEF_FILELIST_3GACC_COLUMN: i3GAC                 =  _variant_t2Int(_Input);break;
	case DEF_FILELIST_3GARRFile_COLUMN: ui3GARRLEVEL      =  _TranslateARRFileLevel(_variant_t2CString(_Input));break;
	case DEF_FILELIST_ADDRESS_COLUMN: ulADDRESS           = _variant_t2Int(_Input);break;
	case DEF_FILELIST_3GMODE_COLUMN: b3G                  = (bool)(_variant_t2Int(_Input)&true);break;
	case DEF_FILELIST_3GSFI_COLUMN:  ui3GSFI              = _variant_t2Int(_Input);break;
	case DEF_FILELIST_CHVNUMBER_COLUMN:uiCHVNUMBER = _variant_t2Int(_Input);break;
	case DEF_FILELIST_EFNUMBER_COLUMN:uiEFNUMBER = _variant_t2Int(_Input);break;
	case DEF_FILELIST_DFNUMBER_COLUMN:uiDFNUMBER = _variant_t2Int(_Input);break;
	case DEF_FILELIST_FILECHARDF_COLUMN:uiDFSTATUS = _variant_t2Int(_Input);break;
	case DEF_FILELIST_AID_COLUMN:uiAIDNUMBER  = _variant_t2Int(_Input);break;
	case DEF_FILELIST_LINKFILE_COLUMN :stLINKROUTE = _variant_t2CString(_Input);break;
	case DEF_FILELIST_FRU1_COLUMN:      FRU1 =  _CString2Int(_variant_t2CString(_Input)) ;break;
	case DEF_FILELIST_FRU2_COLUMN:      FRU2 =  _CString2Int(_variant_t2CString(_Input)) ;break;


	case DEF_FILELIST_FRU3_COLUMN:      FRU3 = _TanslateApplicationType (_variant_t2CString(_Input)) ;break;
	case DEF_FILELIST_FRU4_COLUMN:      FRU4 = _CString2Int(_variant_t2CString(_Input)) ;break;
	case DEF_FILELIST_CONTENT_COLUMN:stContent     = _variant_t2CString(_Input);break;

	case DEF_FILELIST_2GREAD_COLUMN:      c2GREAD     = _Translate2GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_2GUPDATE_COLUMN:    c2GUPDATE   = _Translate2GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_2GINCREASE_COLUMN:  c2GINCREATE = _Translate2GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_2GRFM_COLUMN:       c2GRFM      = _Translate2GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_2GINVAL_COLUMN:     c2GINVAL    = _Translate2GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_2GREHAB_COLUMN:     c2GREHAB    = _Translate2GACText(_variant_t2CString(_Input));break;

	case DEF_FILELIST_3GREAD_COLUMN:      c3GREAD     = _Translate3GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_3GUPDATE_COLUMN:    c3GUPDATE   = _Translate3GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_3GINCREASE_COLUMN:  c3GINCREATE = _Translate3GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_3GINVAL_COLUMN:     c3GDEACTIVE = _Translate3GACText(_variant_t2CString(_Input));break;
	case DEF_FILELIST_3GREHAB_COLUMN:     c3GACTIVE   = _Translate3GACText(_variant_t2CString(_Input));break;
	default:bRet = FALSE;
	}
	return bRet;
}

_variant_t PrivateCardFileStruct::GetPrivateCardFileStructData(int iCode,BOOL* bUsed)
{
	_variant_t _temp;


	if (bUsed != NULL)
	{
		*bUsed =TRUE;
		switch(this->uiTYPE&0xF0)
		{
		case DEF_FILETYPE_EF:

			if ((iCode>=DEF_FILELIST_CHVNUMBER_COLUMN)&&
				(iCode<=DEF_FILELIST_AID_COLUMN))
				*bUsed = FALSE;

			if ((!this->b3G)&&
				(iCode>=DEF_FILELIST_3GREAD_COLUMN)&&
				(iCode<=DEF_FILELIST_3GSFI_COLUMN))
				*bUsed = FALSE;

			if (((this->uiTYPE&0xF)<DEF_FILESTRUCTURE_TLINK)&&
				(iCode==DEF_FILELIST_LINKFILE_COLUMN))
				*bUsed = FALSE;
			break;

		case DEF_FILETYPE_DF:

		case DEF_FILETYPE_MF:
			if (iCode == DEF_FILELIST_AID_COLUMN)
				*bUsed = FALSE;

		case DEF_FILETYPE_ADF:
			if ((iCode>=DEF_FILELIST_SIZE_COLUMN)&&
				(iCode<=DEF_FILELIST_ADDRESS_COLUMN))
				*bUsed = FALSE;
			if ((!this->b3G)&&
				(iCode>=DEF_FILELIST_3GREAD_COLUMN)&&
				(iCode<=DEF_FILELIST_3GSFI_COLUMN))
				*bUsed = FALSE;
			if ((iCode>=DEF_FILELIST_2GREAD_COLUMN)&&
				(iCode<=DEF_FILELIST_2GACC_COLUMN))
				*bUsed = FALSE;
			if(iCode==DEF_FILELIST_LINKFILE_COLUMN)
				*bUsed = FALSE;

			break;
		default:*bUsed = FALSE;break;
		}
	}

	

	switch(iCode)
	{
	case DEF_FILELIST_HANDLE_COLUMN:        _temp = Int2_variant_t(VT_I4,uiHANDLE) ;break;
	case DEF_FILELIST_SIZE_COLUMN:          _temp = Int2_variant_t(VT_I4,uiSIZE);break;
	case DEF_FILELIST_RECORD_LENGTH_COLUMN: _temp = Int2_variant_t(VT_I4,uiRECORDLENGTH);break;
	case DEF_FILELIST_RECORD_NUMBER_COLUMN: _temp = Int2_variant_t(VT_I4,uiRECORDNUMBER);break;
	case DEF_FILELIST_2GACC_COLUMN:         _temp = Int2_variant_t(VT_I4,i2GAC);break;
	case DEF_FILELIST_ADDRESS_COLUMN:       _temp = Int2_variant_t(VT_I4,ulADDRESS);break;
	case DEF_FILELIST_3GACC_COLUMN:         _temp = Int2_variant_t(VT_I4,i3GAC);break;
	case DEF_FILELIST_CHVNUMBER_COLUMN:     _temp = Int2_variant_t(VT_I4,uiCHVNUMBER);break;
	case DEF_FILELIST_EFNUMBER_COLUMN:      _temp = Int2_variant_t(VT_I4,uiEFNUMBER);break;
	case DEF_FILELIST_DFNUMBER_COLUMN:      _temp = Int2_variant_t(VT_I4,uiDFNUMBER);break;
	case DEF_FILELIST_AID_COLUMN:           _temp = Int2_variant_t(VT_I4,uiAIDNUMBER);break;
	case DEF_FILELIST_NAME_COLUMN:          _temp = CString2_variant_t(VT_BSTR,stFILENAME);break;
	case DEF_FILELIST_ROUTE_COLUMN:         _temp = CString2_variant_t(VT_BSTR,stROUTE);break;
	case DEF_FILELIST_LINKFILE_COLUMN:      _temp = CString2_variant_t(VT_BSTR,stLINKROUTE);break;
	case DEF_FILELIST_3GARRFile_COLUMN:    _temp = _TranslateARRFile(ui3GARRLEVEL);break;
	case DEF_FILELIST_TYPE_COLUMN:          _temp = _TranslateFileType(uiTYPE);break;
	case DEF_FILELIST_STRUCTURE_COLUMN:     _temp = _TranslateFileStructure(uiSTRUCT);break;

	case DEF_FILELIST_FID_COLUMN:          _temp = CString2_variant_t(VT_BSTR,Int2CString(uiFID,4));break;
	case DEF_FILELIST_STATUS_COLUMN:       _temp =  Int2_variant_t(VT_BSTR,uiEFSTATUS);break;

	case DEF_FILELIST_3GMODE_COLUMN:       _temp =  Int2_variant_t(VT_BSTR,b3G);break;
	case DEF_FILELIST_3GSFI_COLUMN:        _temp =  Int2_variant_t(VT_BSTR,ui3GSFI);break;
	case DEF_FILELIST_FILECHARDF_COLUMN:   _temp =  Int2_variant_t(VT_BSTR,uiDFSTATUS);break;

	case DEF_FILELIST_FRU1_COLUMN:        _temp =  CString2_variant_t(VT_BSTR,Int2CString(FRU1,4));break;
	case DEF_FILELIST_FRU2_COLUMN:        _temp =  CString2_variant_t(VT_BSTR,Int2CString(FRU2,4));break;
	case DEF_FILELIST_FRU3_COLUMN:        _temp =  _TanslateSTKAPPCode(FRU3);break;
	case DEF_FILELIST_FRU4_COLUMN:        _temp =  CString2_variant_t(VT_BSTR,Int2CString(FRU4,6));break;
	case DEF_FILELIST_CONTENT_COLUMN:      _temp =  CString2_variant_t(VT_ARRAY|VT_UI1,stContent);break;

	case DEF_FILELIST_2GREAD_COLUMN:      _temp = _Translate2GAC(c2GREAD);break;
	case DEF_FILELIST_2GUPDATE_COLUMN:    _temp = _Translate2GAC(c2GUPDATE);break;
	case DEF_FILELIST_2GINCREASE_COLUMN:  _temp = _Translate2GAC(c2GINCREATE);break;
	case DEF_FILELIST_2GRFM_COLUMN:       _temp = _Translate2GAC(c2GRFM);break;
	case DEF_FILELIST_2GINVAL_COLUMN:     _temp = _Translate2GAC(c2GINVAL);break;
	case DEF_FILELIST_2GREHAB_COLUMN:     _temp = _Translate2GAC(c2GREHAB);break;

	case DEF_FILELIST_3GREAD_COLUMN:      _temp = _Translate3GAC(c3GREAD);break;
	case DEF_FILELIST_3GUPDATE_COLUMN:    _temp = _Translate3GAC(c3GUPDATE);break;
	case DEF_FILELIST_3GINCREASE_COLUMN:  _temp = _Translate3GAC(c3GINCREATE);break;
	case DEF_FILELIST_3GINVAL_COLUMN:     _temp = _Translate3GAC(c3GDEACTIVE);break;
	case DEF_FILELIST_3GREHAB_COLUMN:     _temp = _Translate3GAC(c3GACTIVE);break;

	default:_temp.vt = VT_EMPTY;break;
	}
	return _temp;
}

int PrivateCardFileStruct::CreateTestData( int iHandle )
{
	 uiHANDLE        = iHandle;
	 uiFID           = 0x2FFF  ; 
	 uiTYPE          = DEF_FILETYPE_EF;
	 uiSTRUCT        = DEF_FILESTRUCTURE_TRANSPARENT ;
	 uiEFSTATUS      = 33;
	 uiSIZE          = 01; 
	 uiRECORDNUMBER  = 00;
	 uiRECORDLENGTH  = 00;
	 ulADDRESS       = 00;

	 b3G             = true;
	 i2GAC           = 1;
	 i3GAC           = 1;
	 ui3GARRLEVEL    = DEF_ARRLEVEL_2F06;
	 ui3GSFI         = 0;

	 uiCHVNUMBER     = 0;
	 uiEFNUMBER      = 0;
	 uiDFNUMBER      = 0;
	 uiDFSTATUS      = 0;
	 uiAIDNUMBER     = 0;
	 uiROUTE         = 0;
	 uiLINKROUTE     = 0;

	 FRU1            = 0;
	 FRU2            = 0;
	 FRU3            = 0;
	 FRU4            = 0;

	  stFILENAME     = _T("Test File ");
	  stContent      = _T("FF //TEST Content");
	  stROUTE        = _T("3F00");;
	  stLINKROUTE.Empty();
	 return TRUE;
}

int PrivateCardFileStruct::CreateBinaryFile( int iHandle )
{
	uiHANDLE        = iHandle;
	uiFID           = 0x2FFF  ; 
	uiTYPE          = DEF_FILETYPE_EF;
	uiSTRUCT        = DEF_FILESTRUCTURE_TRANSPARENT ;
	uiEFSTATUS      = 33;
	uiSIZE          = 10; 
	uiRECORDNUMBER  = 00;
	uiRECORDLENGTH  = 00;
	ulADDRESS       = 00;

	b3G             = true;
	i2GAC           = 1;
	i3GAC           = 1;
	ui3GARRLEVEL    = DEF_ARRLEVEL_2F06;
	ui3GSFI         = 0;

	uiCHVNUMBER     = 0;
	uiEFNUMBER      = 0;
	uiDFNUMBER      = 0;
	uiDFSTATUS      = 0;
	uiAIDNUMBER     = 0;
	uiROUTE         = 0;
	uiLINKROUTE     = 0;

	FRU1            = 0;
	FRU2            = 0;
	FRU3            = 0;
	FRU4            = 0;

	stFILENAME     = _T("Binary File ");
	stContent      = _T("FFFFFFFFFFFFFFFFFFFF //Binary data");
	stROUTE        = _T("3F00");;
	stLINKROUTE.Empty();
	return TRUE;

}

int PrivateCardFileStruct::CreateLinfixFile( int iHandle )
{
	uiHANDLE        = iHandle;
	uiFID           = 0x2FFF  ; 
	uiTYPE          = DEF_FILETYPE_EF;
	uiSTRUCT        = DEF_FILESTRUCTURE_LINER ;
	uiEFSTATUS      = 33;
	uiSIZE          = 10; 
	uiRECORDNUMBER  = 01;
	uiRECORDLENGTH  = 10;
	ulADDRESS       = 00;

	b3G             = true;
	i2GAC           = 1;
	i3GAC           = 1;
	ui3GARRLEVEL    = DEF_ARRLEVEL_2F06;
	ui3GSFI         = 0;

	uiCHVNUMBER     = 0;
	uiEFNUMBER      = 0;
	uiDFNUMBER      = 0;
	uiDFSTATUS      = 0;
	uiAIDNUMBER     = 0;
	uiROUTE         = 0;
	uiLINKROUTE     = 0;

	FRU1            = 0;
	FRU2            = 0;
	FRU3            = 0;
	FRU4            = 0;

	stFILENAME     = _T("Linfix File ");
	stContent      = _T("FFFFFFFFFFFFFFFFFFFF //Record 01 \r\n");
	stROUTE        = _T("3F00");;
	stLINKROUTE.Empty();
	return TRUE;

}

int PrivateCardFileStruct::CreateCyclicFile( int iHandle )
{
	uiHANDLE        = iHandle;
	uiFID           = 0x2FFF  ; 
	uiTYPE          = DEF_FILETYPE_EF;
	uiSTRUCT        = DEF_FILESTRUCTURE_CYCLIC;
	uiEFSTATUS      = 33;
	uiSIZE          = 06; 
	uiRECORDNUMBER  = 02;
	uiRECORDLENGTH  = 03;
	ulADDRESS       = 00;

	b3G             = true;
	i2GAC           = 9;
	i3GAC           = 12;
	ui3GARRLEVEL    = DEF_ARRLEVEL_2F06;
	ui3GSFI         = 0;

	uiCHVNUMBER     = 0;
	uiEFNUMBER      = 0;
	uiDFNUMBER      = 0;
	uiDFSTATUS      = 0;
	uiAIDNUMBER     = 0;
	uiROUTE         = 0;
	uiLINKROUTE     = 0;

	FRU1            = 0;
	FRU2            = 0;
	FRU3            = 0;
	FRU4            = 0;

	stFILENAME     = _T("Cyclic File ");
	stContent      = _T("FFFFFF //CycLic Record 01 \r\nFFFFFF //CycLic Record 02 \r\n");
	stROUTE        = _T("3F00");;
	stLINKROUTE.Empty();
	return TRUE;

}

int PrivateCardFileStruct::EmptyFile()
{
	uiHANDLE        = 0;
	uiFID           = 0  ; 
	uiTYPE          = 0;
	uiSTRUCT        = 0;
	uiEFSTATUS      = 0;
	uiSIZE          = 0; 
	uiRECORDNUMBER  = 0;
	uiRECORDLENGTH  = 0;
	ulADDRESS       = 00;

	b3G             = false;
	i2GAC           = 0;
	i3GAC           = 0;
	ui3GARRLEVEL    = 127;
	ui3GSFI         = 0;

	uiCHVNUMBER     = 0;
	uiEFNUMBER      = 0;
	uiDFNUMBER      = 0;
	uiDFSTATUS      = 0;
	uiAIDNUMBER     = 0;
	uiROUTE         = 0;
	uiLINKROUTE     = 0;

	FRU1            = 0;
	FRU2            = 0;
	FRU3            = 0;
	FRU4            = 0;

 
	c2GREAD         =-1;  
	c2GUPDATE       =-1;  
	c2GINCREATE     =-1; 
	c2GRFM          =-1; 
	c2GREHAB        =-1; 
	c2GINVAL        =-1; 

	c3GREAD         =-1; 
	c3GUPDATE       =-1; 
	c3GINCREATE     =-1; 
	c3GDEACTIVE     =-1; 
	c3GACTIVE       =-1; 

	stFILENAME.Empty();
	stContent.Empty();
	stROUTE.Empty();
	stLINKROUTE.Empty();
	return TRUE;

}

int PrivateCardFileStruct::IsEmpty()
{
	PrivateCardFileStruct tempfile;
	tempfile.EmptyFile();

	
	return (this->ComparePCFS(tempfile)==1);

}

int PrivateCardFileStruct::CreateMF( int iHandle )
{
	uiHANDLE        = iHandle;
	uiFID           = 0x3F00  ; 
	uiTYPE          = DEF_FILETYPE_MF;
	uiSTRUCT        = 0;
	uiEFSTATUS      = 0;
	uiSIZE          = 0; 
	uiRECORDNUMBER  = 0;
	uiRECORDLENGTH  = 0;
	ulADDRESS       = 00;

	b3G             = true;
	i2GAC           = -1;
	i3GAC           = 1;
	ui3GARRLEVEL    = DEF_ARRLEVEL_2F06;
	ui3GSFI         = 0;

	uiCHVNUMBER     = 3;
	uiEFNUMBER      = 0;
	uiDFNUMBER      = 0;
	uiDFSTATUS      = 51;
	uiAIDNUMBER     = 0;
	uiROUTE         = 0;
	uiLINKROUTE     = 0;

	FRU1            = 0;
	FRU2            = 0;
	FRU3            = 0;
	FRU4            = 0;

	stFILENAME     = _T("Test MF ");
	stContent      = _T("//Test MF");
	stROUTE        = _T("");;
	stLINKROUTE.Empty();
	return TRUE;

}

int PrivateCardFileStruct::CreateDF( int iHandle )
{
	uiHANDLE        = iHandle;
	uiFID           = 0x7FF1  ; 
	uiTYPE          = DEF_FILETYPE_DF;
	uiSTRUCT        = 0;
	uiEFSTATUS      = 0;
	uiSIZE          = 0; 
	uiRECORDNUMBER  = 0;
	uiRECORDLENGTH  = 0;
	ulADDRESS       = 00;

	b3G             = true;
	i2GAC           = -1;
	i3GAC           = 1;
	ui3GARRLEVEL    = DEF_ARRLEVEL_2F06;
	ui3GSFI         = 0;

	uiCHVNUMBER     = 3;
	uiEFNUMBER      = 0;
	uiDFNUMBER      = 0;
	uiDFSTATUS      = 51;
	uiAIDNUMBER     = 0;
	uiROUTE         = 0;
	uiLINKROUTE     = 0;

	FRU1            = 0;
	FRU2            = 0;
	FRU3            = 0;
	FRU4            = 0;

	stFILENAME     = _T("Test DF ");
	stContent      = _T("//Test DF");
	stROUTE        = _T("3F00");;
	stLINKROUTE.Empty();
	return TRUE;

}

int PrivateCardFileStruct::CreateADF( int iHandle )
{
	uiHANDLE        = iHandle;
	uiFID           = 0x7FF1  ; 
	uiTYPE          = DEF_FILETYPE_ADF;
	uiSTRUCT        = 0;
	uiEFSTATUS      = 0;
	uiSIZE          = 0; 
	uiRECORDNUMBER  = 0;
	uiRECORDLENGTH  = 0;
	ulADDRESS       = 00;

	b3G             = true;
	i2GAC           = -1;
	i3GAC           = 1;
	ui3GARRLEVEL    = DEF_ARRLEVEL_2F06;
	ui3GSFI         = 0;

	uiCHVNUMBER     = 3;
	uiEFNUMBER      = 0;
	uiDFNUMBER      = 0;
	uiDFSTATUS      = 51;
	uiAIDNUMBER     = 1;
	uiROUTE         = 0;
	uiLINKROUTE     = 0;

	FRU1            = 0;
	FRU2            = 0;
	FRU3            = 0;
	FRU4            = 0;

	stFILENAME     = _T("Test ADF ");
	stContent      = _T("// Test ADF");
	stROUTE        = _T("3F00");;
	stLINKROUTE.Empty();
	return TRUE;

}

int PrivateCardFileStruct::ComparePCFS(PrivateCardFileStruct _file)
{

	for (int i = DEF_FILELIST_HANDLE_COLUMN ; i < DEF_FILELIST_CONTENT_COLUMN ; i++)
	{
		if (this->GetPrivateCardFileStructData(i) != _file.GetPrivateCardFileStructData(i))
			return (-i);
	}

	if (this->stContent != _file.stContent)
		return (-38);
	return 1;
}

int PrivateCardFileStruct::IsModified(PrivateCardFileStruct _file,int* iModifiedID)
{
	int iRet = 0;

	for (int i = DEF_FILELIST_HANDLE_COLUMN ; i < DEF_FILELIST_CONTENT_COLUMN ; i++)
	{
		if (this->GetPrivateCardFileStructData(i) != _file.GetPrivateCardFileStructData(i))
		{
			iModifiedID[iRet] = i;
			iRet++;
		}		
	}

	if (this->stContent!= _file.stContent)
	{
		iModifiedID[iRet] = DEF_FILELIST_CONTENT_COLUMN;
		iRet++;
	}
	return iRet;
}

//************************************
// Method:    CorrectPCFS
// FullName:  PrivateCardFileStruct::CorrectPCFS
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: BOOL b3GMode   非3G模式, 修改 SFI ARRLEVEL,ARR Record 复成0
// Parameter: BOOL bContent  非根据 实际内容吧,修改 Size 成实际数据;
// Parameter: BOOL bType     如果为 DF MF ADF 修改 结构为0;Status也相应修改 为DF 把EFStatus 修改为0 , 如果为EF 把DFStatus修改为0
// Parameter: BOOL bStruct   如果为 BINARY ,修改 RecordLen 为 0;
// Parameter: BOOL bRecord   如果为 Record ,修改 Size;  
//************************************
int PrivateCardFileStruct::CorrectPCFS(BOOL b3GMode,BOOL bContent,BOOL bType,BOOL bStruct,BOOL bRecord)
{
	if ((b3GMode)&&(!b3G))
	{
		ui3GSFI      = 0;
		ui3GARRLEVEL = 0;
		i3GAC        = 0;
	}

	if ((bContent)&&
		(uiTYPE== DEF_FILETYPE_EF)&&
		(uiSTRUCT != DEF_FILESTRUCTURE_TLINK)&&
		(uiSTRUCT != DEF_FILESTRUCTURE_CLINK)&&
		(uiSTRUCT != DEF_FILESTRUCTURE_LLINK))
	{

		CString csTemp;
		int iLength;
		csTemp = stContent;
		_DeleteNote(csTemp);
		_DeleteEnterSpace(csTemp);
		iLength = csTemp.GetLength();
		if ((iLength%2)!=0)
		{
			stContent = stContent+_T("\r\nF");
			csTemp = stContent;
			_DeleteNote(csTemp);
			_DeleteEnterSpace(csTemp);
			iLength = csTemp.GetLength();
		}

		if (iLength/2 != uiSIZE)
			uiSIZE = iLength/2;

	}

	if (bType)
	{
		if (uiTYPE != DEF_FILETYPE_EF)
		{
			uiSTRUCT   = 0;
			uiEFSTATUS = 0; 

			if (uiTYPE == DEF_FILETYPE_ADF)
			{
				if (uiAIDNUMBER == 0)
					uiAIDNUMBER = 1;
			}
			else
				uiAIDNUMBER = 0;
		}
		else
		{
			uiDFSTATUS  = 0; 
		    uiCHVNUMBER = 0; 
	        uiEFNUMBER  = 0; 
	        uiDFNUMBER  = 0; 
	        uiDFSTATUS  = 0; 
	        uiAIDNUMBER = 0; 
			
		}
	}

	if (bStruct)
	{

		if ((uiSTRUCT == DEF_FILESTRUCTURE_TRANSPARENT)||
			(uiSTRUCT==DEF_FILESTRUCTURE_TLINK))
		{
			uiRECORDLENGTH = uiSIZE;
			uiRECORDNUMBER = 1;
		}else if (((uiSTRUCT == DEF_FILESTRUCTURE_LINER)||
			       (uiSTRUCT != DEF_FILESTRUCTURE_LLINK)||
			       (uiSTRUCT != DEF_FILESTRUCTURE_CYCLIC)||
		           (uiSTRUCT != DEF_FILESTRUCTURE_CLINK))&&
			      ((uiRECORDLENGTH == 0)|| 
			       (uiRECORDNUMBER == 0)))
		{
			uiRECORDLENGTH = uiSIZE;
			uiRECORDNUMBER = 1;
		}


		//if ((uiSTRUCT != DEF_FILESTRUCTURE_LINER)&&
		//	(uiSTRUCT != DEF_FILESTRUCTURE_LLINK)&&
		//	(uiSTRUCT != DEF_FILESTRUCTURE_CYCLIC)&&
		//	(uiSTRUCT != DEF_FILESTRUCTURE_CLINK))
		//{
		//	uiRECORDLENGTH = uiSIZE;
		//	uiRECORDNUMBER = 1;
		//}
		//else if((uiRECORDLENGTH == 0)||(uiRECORDNUMBER == 0))
		//{
		//	uiRECORDLENGTH = uiSIZE;
		//	uiRECORDNUMBER = 1;
		//}

	}


	if ((bRecord)&&
		((uiSTRUCT == DEF_FILESTRUCTURE_LINER)||
		(uiSTRUCT == DEF_FILESTRUCTURE_LLINK)||
		(uiSTRUCT == DEF_FILESTRUCTURE_CYCLIC)||
		(uiSTRUCT == DEF_FILESTRUCTURE_CLINK)))
	{
		if (uiRECORDLENGTH != 0)
		{
			uiRECORDNUMBER = uiSIZE / uiRECORDLENGTH;
		}
		else
		{
			uiRECORDNUMBER =0;
			uiRECORDLENGTH =0;
		}
		
	}


	return 0;
}

int PrivateCardFileStruct::GetErrorNumber(int i2GACMax,int i3GACMax, int iAIDMax,
										   CStringArray& csErrorInformation,
										   const CStringArray& csRouteArray)
{
	int iRet = 0;
	int iCode = uiTYPE|uiSTRUCT;
	//CString csRecord = _T(" of file whitch record ")+Int2CStringDecimal(uiHANDLE);
	CString csRecord;
	_Int2CStringDecimal(uiHANDLE,csRecord);

	csRecord =  _T(" of file whitch record ")+csRecord;
	//第一个检查项目
	if ((uiTYPE != DEF_FILETYPE_EF)&&
		(uiTYPE!= DEF_FILETYPE_DF)&&
		(uiTYPE!= DEF_FILETYPE_ADF)&&
		(uiTYPE!= DEF_FILETYPE_MF))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"Type\" ")+ csRecord);
	}

	//第二个检查项 文件结构

	if ((uiSTRUCT!= DEF_FILESTRUCTURE_TRANSPARENT)&&
		(uiSTRUCT!= DEF_FILESTRUCTURE_TLINK)&&
		(uiSTRUCT!= DEF_FILESTRUCTURE_LINER)&&
		(uiSTRUCT!= DEF_FILESTRUCTURE_LLINK)&&
		(uiSTRUCT!= DEF_FILESTRUCTURE_CYCLIC)&&
		(uiSTRUCT!= DEF_FILESTRUCTURE_CLINK))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"Structure\" ")+csRecord);
	}

	//第三个检查项目短文件标识
	if (ui3GSFI >= 31)
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"SFI\" ")+csRecord);
	}

	//第4个 文件大小 和文件内容 相互匹配 
	CString _stContent = stContent;
	_DeleteNote(_stContent);
	_DeleteEnterSpace(_stContent);
	
	if(((iCode == (DEF_FILETYPE_EF|DEF_FILESTRUCTURE_CYCLIC))||
		(iCode == (DEF_FILETYPE_EF|DEF_FILESTRUCTURE_LINER))||
		(iCode == (DEF_FILETYPE_EF|DEF_FILESTRUCTURE_TRANSPARENT)))&&
		((_stContent.GetLength()/2)!=uiSIZE))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"Content or Size\" ")+csRecord);
	}

	//第5个 检查项目 ， Record 和 Number 不匹配 Size

	if(((iCode == (DEF_FILETYPE_EF|DEF_FILESTRUCTURE_CYCLIC))||
	    (iCode == (DEF_FILETYPE_EF|DEF_FILESTRUCTURE_LINER))||
	    (iCode == (DEF_FILETYPE_EF|DEF_FILESTRUCTURE_CLINK))||
	    (iCode == (DEF_FILETYPE_EF|DEF_FILESTRUCTURE_LLINK)))
	    &&
	    ((uiRECORDLENGTH*uiRECORDNUMBER)!= uiSIZE))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"Record length Record number or Size\" ")+csRecord);
	}

	//第6个检查项目， 2G AC记录号， 必须输入 0022 的记录总数
	if((uiTYPE == DEF_FILETYPE_EF)&&(i2GAC > i2GACMax))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"2G AC Record\" ")+csRecord);

	}

	//第7个检查项目， 3G AC记录号， 必须输入 ARR 的记录总数

	if((b3G)&& 
	 ((i3GAC > i3GACMax)||
	    i3GAC <= 0))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"3G AC Record\" ")+csRecord);
	}

	//第8个检查项文件路径
	BOOL bRouteErr = TRUE;
	for (int i = 0;i < csRouteArray.GetCount(); i++)
	{
		if ((stROUTE.IsEmpty())||
		    (stROUTE.Compare(csRouteArray.GetAt(i))==0))
		{
			bRouteErr = FALSE;
			break;
		}
	}

	if (bRouteErr)
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"Route\" ")+csRecord);
	}
	//第9个检查项AID记录号
	if ((uiTYPE == DEF_FILETYPE_ADF)&&
		(uiAIDNUMBER > iAIDMax))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"AID Record\" ")+csRecord);
	}

	//第10 项 DF 文件状态

	if ((uiTYPE != DEF_FILETYPE_EF)&&
	   ((uiDFSTATUS&0xDF) != 0x13))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"DF Status\" ")+csRecord);
	}

	//第11 项 EF 文件状态

	if ((uiTYPE == DEF_FILETYPE_EF)&&
		((uiDFSTATUS&0x12) != 0))
	{
		iRet += 1;
		csErrorInformation.Add(_T("There is error in \"DF Status\" ")+csRecord);
	}


	return iRet;

}


int PrivateCardFileStruct::CheckParameter(CStringArray& csErrorInformation)
{
	int iRet = 0;
	int iCode = uiTYPE | uiSTRUCT;
	CString csRecord;
	_Int2CStringDecimal(uiHANDLE, csRecord);

	csRecord = _T("Record ") + csRecord;
	csRecord += _T("(") + this->stROUTE + Int2CString(this->uiFID) + this->stFILENAME +_T(")");
	//第一个检查项目
	if ((uiTYPE != DEF_FILETYPE_EF) &&
		(uiTYPE != DEF_FILETYPE_DF) &&
		(uiTYPE != DEF_FILETYPE_ADF) &&
		(uiTYPE != DEF_FILETYPE_MF))
	{
		iRet += 1;
		csErrorInformation.Add(_T("\"Type\"(") + Int2CString(uiTYPE)  + _T(")is Invalid value!"));
	}

	//第二个检查项 文件结构

	if ((uiSTRUCT != DEF_FILESTRUCTURE_TRANSPARENT) &&
		(uiSTRUCT != DEF_FILESTRUCTURE_TLINK) &&
		(uiSTRUCT != DEF_FILESTRUCTURE_LINER) &&
		(uiSTRUCT != DEF_FILESTRUCTURE_LLINK) &&
		(uiSTRUCT != DEF_FILESTRUCTURE_CYCLIC) &&
		(uiSTRUCT != DEF_FILESTRUCTURE_CLINK))
	{
		iRet += 1;
		csErrorInformation.Add(_T("\"Structure\"(") + Int2CString(uiSTRUCT) + _T(")is Invalid value!"));
	}

	//第三个检查项目短文件标识
	if (ui3GSFI >= 31)
	{
		iRet += 1;
		csErrorInformation.Add(_T("\"SFI\"(") + Int2CString(ui3GSFI) + _T(")is Invalid value!"));
	}

	//第4个 文件大小 和文件内容 相互匹配 
	CString _stContent = stContent;
	_DeleteNote(_stContent);
	_DeleteEnterSpace(_stContent);

	if (((iCode == (DEF_FILETYPE_EF | DEF_FILESTRUCTURE_CYCLIC)) ||
		(iCode == (DEF_FILETYPE_EF | DEF_FILESTRUCTURE_LINER)) ||
		(iCode == (DEF_FILETYPE_EF | DEF_FILESTRUCTURE_TRANSPARENT))) &&
		((_stContent.GetLength() / 2) != uiSIZE))
	{
		iRet += 1;
		csErrorInformation.Add(_T("\"SIZE\"(") + Int2CString(ui3GSFI) + _T(")is accord with \"content\"!"));
	}

	//第5个 检查项目 ， Record 和 Number 不匹配 Size

	if (((iCode == (DEF_FILETYPE_EF | DEF_FILESTRUCTURE_CYCLIC)) ||
		(iCode == (DEF_FILETYPE_EF | DEF_FILESTRUCTURE_LINER)) ||
		(iCode == (DEF_FILETYPE_EF | DEF_FILESTRUCTURE_CLINK)) ||
		(iCode == (DEF_FILETYPE_EF | DEF_FILESTRUCTURE_LLINK)))
		&&
		((uiRECORDLENGTH*uiRECORDNUMBER) != uiSIZE))
	{
		iRet += 1;
		csErrorInformation.Add(_T("\"SIZE\"(") + Int2CString(ui3GSFI) +
			_T(")is accord with \"record size\"") + Int2CString(uiRECORDLENGTH) + csRecord+
			_T("and \"record number\" !") +Int2CString(uiRECORDNUMBER) + csRecord);
	}

	//第6项 检查 3G 模式下，必须要有 3GARRFile 和3G ARR record

	if (b3G)
	{
		if (this->i3GAC == 0)
		{
			iRet += 1;
			csErrorInformation.Add(_T("\"ARR record\"(") + Int2CString(i3GAC) + _T(")is Invalid value!"));
		}

		if (this->ui3GARRLEVEL > DEF_ARRLEVEL_6F06)
		{
			iRet += 1;
			csErrorInformation.Add(_T("\"ARR file\"(") + _TranslateARRFile(ui3GARRLEVEL) + _T(")is Invalid value!"));
		}
	}

	//第10 项 DF 文件状态

	if ((uiTYPE != DEF_FILETYPE_EF) &&
		((uiDFSTATUS & 0xDF) != 0x13))
	{
		iRet += 1;
		csErrorInformation.Add(_T("\"DF status\"(") + Int2CString(uiDFSTATUS) + _T(")is Invalid value!"));
	}

	//第11 项 EF 文件状态

	if ((uiTYPE == DEF_FILETYPE_EF) &&
		((uiEFSTATUS & 0x12) != 0))
	{
		iRet += 1;
		csErrorInformation.Add(_T("\"EF status\"(") + Int2CString(uiEFSTATUS) + _T(")is Invalid value!"));
	}


	return iRet;

}



BOOL PrivateCardFileStruct::IsLinkFile()
{
	if ((uiTYPE == DEF_FILETYPE_EF) &&
		((uiSTRUCT == DEF_FILESTRUCTURE_CLINK) ||
		(uiSTRUCT == DEF_FILESTRUCTURE_LLINK) ||
			(uiSTRUCT == DEF_FILESTRUCTURE_TLINK)))
		return TRUE;
	return FALSE;
}
