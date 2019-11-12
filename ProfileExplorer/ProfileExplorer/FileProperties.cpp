#pragma once
#include "stdafx.h"
#include "FileProperties.h"
#include "PrivateCardFileStruct.h"



CMFCPropertyGridProperty* CreateBasicInformation() 
{

	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("文件基本信息"));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("文件序号"), _T(""), _T(""));
//	pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("文件名称")  , _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("文件 ID")  , _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("文件类别")  , _T(""), _T(""));
	pProp->AddOption(_T("EF"));
	pProp->AddOption(_T("DF"));
	pProp->AddOption(_T("ADF"));
	pProp->AddOption(_T("MF"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("文件路径"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("文件模式"), _T(""), _T(""));
	pProp->AddOption(_T("GSM 模式"));
	pProp->AddOption(_T("UICC 模式"));
	pGroup1->AddSubItem(pProp);

	pGroup1->AddSubItem(CreateDFInformation());
	pGroup1->AddSubItem(CreateEFInformation());



	return pGroup1;

}

// //创建EF文件基本信息 包含文件结构， 文件路径 连接文件，短文件标示，文件2G 3G标示
//************************************
// Method:    CreateEFInformation，CreateDFInformation，CreateSpeciality，CreateDFSpeciality
//            CreateAccessInGSM  ，CreateAccessInUICC ，CreateOTASepeciality
// FullName:  CreateEFInformation，CreateDFInformation，CreateSpeciality，CreateDFSpeciality
//            CreateAccessInGSM  ，CreateAccessInUICC ，CreateOTASepeciality
// Access:    public 
// Returns:   CMFCPropertyGridProperty*
// Qualifier: See Name
// Parameter: void
//************************************
CMFCPropertyGridProperty* CreateEFInformation(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("EF文件基本信息"));
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("文件结构"), _T(""), _T(""));
	pProp->AddOption(_T("Transparent"));
	pProp->AddOption(_T("Linear Fixed"));
	pProp->AddOption(_T("Cycle"));
	pProp->AddOption(_T("TLink"));
	pProp->AddOption(_T("LLink"));
	pProp->AddOption(_T("CLink"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("文件大小"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("记录条数"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("记录长度"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("短文件标识（SFI）"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("链接目标"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);





	return pGroup1;
}


// 创建DF信息
CMFCPropertyGridProperty* CreateDFInformation(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("MF/DF/ADF文件基本信息"));
	CMFCPropertyGridProperty* pProp;
	pProp = new CMFCPropertyGridProperty(_T("CHV数量"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("EFs数量"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("DFs数量"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	return pGroup1;
}


CMFCPropertyGridProperty* CreateSpeciality(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("EF文件特性"));
	CMFCPropertyGridProperty* pProp;
	pProp = new CMFCPropertyGridProperty(_T("支持Increase"), (_variant_t)false , _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("支持RFM Delete Resize"), (_variant_t)false, _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("支持多通道共享"),(_variant_t)false, _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("支持无效时读写"),(_variant_t)false, _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("支持高频率更新保护"),(_variant_t)false, _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("文件当前有效"), (_variant_t)false, _T(""));
	pGroup1->AddSubItem(pProp);

	return pGroup1;
}

CMFCPropertyGridProperty* CreateDFSpeciality(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("DF文件特性"));
	CMFCPropertyGridProperty* pProp;
	//pProp = new CMFCPropertyGridProperty(_T("支持Increase"), (_variant_t)false , _T(""));
	//pGroup1->AddSubItem(pProp);
	//pProp = new CMFCPropertyGridProperty(_T("支持RFM Delete Resize"), (_variant_t)false, _T(""));
	//pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("支持多通道共享"),(_variant_t)false, _T(""));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("AID"),_T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	//pProp = new CMFCPropertyGridProperty(_T("支持无效时读写"),(_variant_t)false, _T(""));
	//pGroup1->AddSubItem(pProp);
	//pProp = new CMFCPropertyGridProperty(_T("支持高频率更新保护"),(_variant_t)false, _T(""));
	//pGroup1->AddSubItem(pProp);
	//pProp = new CMFCPropertyGridProperty(_T("文件当前有效"), (_variant_t)false, _T(""));
	//pGroup1->AddSubItem(pProp);

	return pGroup1;
}
CMFCPropertyGridProperty* CreateAccessInGSM(void)
{

	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("GSM文件权限"));
	CMFCPropertyGridProperty* pProp;
	pProp = new CMFCPropertyGridProperty(_T("Read"), _T(""), _T(""));
	pProp->AddOption(_T("Alway"));
	pProp->AddOption(_T("CHV1"));
	pProp->AddOption(_T("CHV2"));
	pProp->AddOption(_T("CHV4"));
	pProp->AddOption(_T("NEVER"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Update"), _T(""), _T(""));
	pProp->AddOption(_T("Alway"));
	pProp->AddOption(_T("CHV1"));
	pProp->AddOption(_T("CHV2"));
	pProp->AddOption(_T("CHV4"));;
	pProp->AddOption(_T("NEVER"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Increase"), _T(""), _T(""));
	pProp->AddOption(_T("Alway"));
	pProp->AddOption(_T("CHV1"));
	pProp->AddOption(_T("CHV2"));
	pProp->AddOption(_T("CHV4"));
	pProp->AddOption(_T("NEVER"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("RFM"), _T(""), _T(""));
	pProp->AddOption(_T("Alway"));
	//pProp->AddOption(_T("CHV1"));
	//pProp->AddOption(_T("CHV2"));
	//pProp->AddOption(_T("CHV3"));
	//pProp->AddOption(_T("CHV4"));
	//pProp->AddOption(_T("CHV5"));
	pProp->AddOption(_T("NEVER"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Rehabilitate"), _T(""), _T(""));
	pProp->AddOption(_T("Alway"));
	pProp->AddOption(_T("CHV1"));
	pProp->AddOption(_T("CHV2"));
	pProp->AddOption(_T("CHV4"));
	pProp->AddOption(_T("NEVER"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Invalidate"), _T(""), _T(""));
	pProp->AddOption(_T("Alway"));
	pProp->AddOption(_T("CHV1"));
	pProp->AddOption(_T("CHV2"));
	pProp->AddOption(_T("CHV4"));
	pProp->AddOption(_T("NEVER"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("AC 记录号"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);

	return pGroup1;
}

CMFCPropertyGridProperty* CreateAccessInUICC(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("UICC文件权限"));
	CMFCPropertyGridProperty* pProp;
	pProp = new CMFCPropertyGridProperty(_T("Read"), _T(""), _T(""));
	pProp->AddOption(_T("ALWAYS"));
	pProp->AddOption(_T("PIN 01"));
	pProp->AddOption(_T("PIN 81"));
	pProp->AddOption(_T("PIN 0A"));
	pProp->AddOption(_T("NEVER"));
	pProp->AddOption(_T("ABSENCE"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Update"), _T(""), _T(""));
	pProp->AddOption(_T("ALWAYS"));
	pProp->AddOption(_T("PIN 01"));
	pProp->AddOption(_T("PIN 81"));
	pProp->AddOption(_T("PIN 0A"));
	pProp->AddOption(_T("NEVER"));
	pProp->AddOption(_T("ABSENCE"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Increase"), _T(""), _T(""));
	pProp->AddOption(_T("ALWAYS"));
	pProp->AddOption(_T("PIN 01"));
	pProp->AddOption(_T("PIN 81"));
	pProp->AddOption(_T("PIN 0A"));
	pProp->AddOption(_T("NEVER"));
	pProp->AddOption(_T("ABSENCE"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Active"), _T(""), _T(""));
	pProp->AddOption(_T("ALWAYS"));
	pProp->AddOption(_T("PIN 01"));
	pProp->AddOption(_T("PIN 81"));
	pProp->AddOption(_T("PIN 0A"));
	pProp->AddOption(_T("NEVER"));
	pProp->AddOption(_T("ABSENCE"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Deactive"), _T(""), _T(""));
	pProp->AddOption(_T("ALWAYS"));
	pProp->AddOption(_T("PIN 01"));
	pProp->AddOption(_T("PIN 81"));
	pProp->AddOption(_T("PIN 0A"));
	pProp->AddOption(_T("NEVER"));
	pProp->AddOption(_T("ABSENCE"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ARR 文件"), _T(""), _T(""));
	pProp->AddOption(_T("2F06"));
	pProp->AddOption(_T("6F06"));
	pProp->AddOption(_T("4F06"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ARR 记录条数"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);

	return pGroup1;
}

CMFCPropertyGridProperty* CreateOTASepeciality(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("OTA应用属性"));
	CMFCPropertyGridProperty* pProp;
	pProp = new CMFCPropertyGridProperty(_T("应用类别"), _T(""), _T(""));
	pProp->AddOption(_T(""));
	pProp->AddOption(_T("OTA应用"));
	pProp->AddOption(_T("固定应用"));
	pGroup1->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("应用AID"), _T(""), _T(""));
	pGroup1->AddSubItem(pProp);
	return pGroup1;
}



//************************************
// Method:    CreateKeyInformation，CreateReserveInformation，CreateUsedInformation
// FullName:  CreateKeyInformation，CreateReserveInformation，CreateUsedInformation
// Access:    public 
// Returns:   CMFCPropertyGridProperty*
// Qualifier: See Name
// Parameter: void
//************************************
CMFCPropertyGridProperty* CreateKeyInformation(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("Key"));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("PIN 01"), _T(""), _T("")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("PIN 02"), _T(""), _T("")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("PIN 0A"), _T(""), _T("")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("PUK 01"), _T(""), _T("")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("PUK 02"), _T(""), _T("")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("PS    "), _T(""), _T("")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("KI    "), _T(""), _T("")));

	return pGroup1;
}
CMFCPropertyGridProperty* CreateReserveInformation(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("预留"));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("文件数量"), Int2_variant_t(VT_I4,0), _T("预留创建的文件个数(10进制)")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("FLASH"), Int2_variant_t(VT_I4,0), _T("预留创建的FLASH(10进制)")));

	return pGroup1;
}
CMFCPropertyGridProperty* CreateUsedInformation(void)
{
	CMFCPropertyGridProperty* pGroup1  = new CMFCPropertyGridProperty(_T("已经使用"));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("文件数量"), Int2_variant_t(VT_I4,0), _T("已经使用的文件个数(包含预留)(10进制)")));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("FLASH"), Int2_variant_t(VT_I4,0), _T("已经使用的FLASH(包含预留)(10进制)")));

	pGroup1->GetSubItem(0)->AllowEdit(FALSE);
	pGroup1->GetSubItem(1)->AllowEdit(FALSE);
	return pGroup1;
}


//************************************
// Method:    ExchangeRFUID2PropertiesID
// FullName:  ExchangeRFUID2PropertiesID
// Access:    
// Returns:   int , DEF_PARAMETER_KEY_PIN01 - DEF_PARAMETER_USED_SPACE
// Qualifier:
// Parameter: int iRFUID ,0 - 10
//************************************
int ExchangeRFUID2PropertiesID(int iRFUID)
{
	int iID;

	switch(iRFUID)
	{
	case 0:iID =DEF_PARAMETER_KEY_PIN01;break;
	case 1:iID =DEF_PARAMETER_KEY_PIN02;break;
	case 2:iID =DEF_PARAMETER_KEY_PIN0A;break;
	case 3:iID =DEF_PARAMETER_KEY_PS;break;
	case 4:iID =DEF_PARAMETER_KEY_PUK01;break;
	case 5:iID =DEF_PARAMETER_KEY_PUK02;break;
	case 6:iID =DEF_PARAMETER_KEY_KI;break;
	case 7:iID =DEF_PARAMETER_RESEVER_FILENUMBER;break;
	case 8:iID =DEF_PARAMETER_RESEVER_SPACE;break;
	case 9:iID =DEF_PARAMETER_USED_FILENUMBER;break;
	case 10:iID =DEF_PARAMETER_USED_SPACE;break;
	default:iID = NULL;
	}
	return iID;

}


int ExchangePropertiesID2RFUID(int iPropertiesID)
{
	int iID;

	switch (iPropertiesID)
	{
	case DEF_PARAMETER_KEY_PIN01:iID = 0; break;
	case DEF_PARAMETER_KEY_PIN02:iID = 1; break;
	case DEF_PARAMETER_KEY_PIN0A :iID = 2; break;
	case DEF_PARAMETER_KEY_PS:iID = 3; break;
	case DEF_PARAMETER_KEY_PUK01:iID = 4; break;
	case DEF_PARAMETER_KEY_PUK02:iID = 5; break;
	case DEF_PARAMETER_KEY_KI:iID = 6; break;
	case DEF_PARAMETER_RESEVER_FILENUMBER:iID = 7; break;
	case DEF_PARAMETER_RESEVER_SPACE:iID = 8; break;
	case DEF_PARAMETER_USED_FILENUMBER:iID = 9; break;
	case DEF_PARAMETER_USED_SPACE:iID = 10; break;
	default:iID = NULL;
	}
	return iID;

}

//************************************
// Method:    ExchangeFileListID2PropertiesID
// FullName:  ExchangeFileListID2PropertiesID
// Access:    public 
// Returns:   int ,DEF_PROPERTIES_FILEHANDLE - DEF_PROPERTIES_CONTENT
// Qualifier:
// Parameter: int iFileListID,DEF_FILELIST_HANDLE_COLUMN-DEF_FILELIST_CONTENT_COLUMN
//************************************
int ExchangeFileListID2PropertiesID( int iFileListID )
{
	int iID = 0;
	switch(iFileListID)
	{
	case  DEF_FILELIST_HANDLE_COLUMN : iID = DEF_PROPERTIES_FILEHANDLE;break; 
	case  DEF_FILELIST_FID_COLUMN : iID = DEF_PROPERTIES_FILEID  ;break; 
	case  DEF_FILELIST_NAME_COLUMN : iID = DEF_PROPERTIES_FILENAME  ;break; 
	case  DEF_FILELIST_ROUTE_COLUMN : iID = DEF_PROPERTIES_ROUTE  ;break; 
	case  DEF_FILELIST_TYPE_COLUMN : iID = DEF_PROPERTIES_TYPE;break; 
	case  DEF_FILELIST_STRUCTURE_COLUMN : iID = DEF_PROPERTIES_FILESTRUCTURE    ;break; 
	case  DEF_FILELIST_STATUS_COLUMN : iID = DEF_PROPERTIES_SPECIALITY  ;break; 
	case  DEF_FILELIST_SIZE_COLUMN : iID = DEF_PROPERTIES_SIZE  ;break; 
	case  DEF_FILELIST_RECORD_NUMBER_COLUMN : iID = DEF_PROPERTIES_RECORDNUMBER;break; 
	case  DEF_FILELIST_RECORD_LENGTH_COLUMN : iID = DEF_PROPERTIES_RECORDLENGTH  ;break; 
	case  DEF_FILELIST_2GREAD_COLUMN : iID = DEF_PROPERTIES_2GAC_READ       ;break; 
	case  DEF_FILELIST_2GUPDATE_COLUMN : iID = DEF_PROPERTIES_2GAC_UPDATE     ;break; 
	case  DEF_FILELIST_2GINCREASE_COLUMN : iID = DEF_PROPERTIES_2GAC_INCREASE ;break; 
	case  DEF_FILELIST_2GRFM_COLUMN : iID = DEF_PROPERTIES_2GAC_RFM        ;break; 
	case  DEF_FILELIST_2GREHAB_COLUMN : iID = DEF_PROPERTIES_2GAC_REHA  ;break; 
	case  DEF_FILELIST_2GINVAL_COLUMN : iID = DEF_PROPERTIES_2GAC_INVA  ;break;
	case  DEF_FILELIST_2GACC_COLUMN : iID = DEF_PROPERTIES_2GAC_2GACRECORD     ;break; 
	case  DEF_FILELIST_ADDRESS_COLUMN : iID = -2                                 ;break; 
	case  DEF_FILELIST_3GMODE_COLUMN : iID = DEF_PROPERTIES_FILEMODE            ;break; 
	case  DEF_FILELIST_3GREAD_COLUMN: iID = DEF_PROPERTIES_3GAC_READ            ;break; 
	case  DEF_FILELIST_3GUPDATE_COLUMN: iID = DEF_PROPERTIES_3GAC_UPDATE;break; 
	case  DEF_FILELIST_3GINCREASE_COLUMN: iID = DEF_PROPERTIES_3GAC_INCREASE     ;break; 
	case  DEF_FILELIST_3GREHAB_COLUMN: iID = DEF_PROPERTIES_3GAC_ACTIVE   ;break; 
	case  DEF_FILELIST_3GINVAL_COLUMN  : iID = DEF_PROPERTIES_3GAC_DEACTIVE   ;break; 
	case  DEF_FILELIST_3GACC_COLUMN: iID = DEF_PROPERTIES_3GAC_ARRRECORD  ;break; 
	case  DEF_FILELIST_3GARRFile_COLUMN: iID = DEF_PROPERTIES_3GAC_ARRFILE     ;break; 
	case  DEF_FILELIST_3GSFI_COLUMN         : iID = DEF_PROPERTIES_SFI             ;break; 
	case  DEF_FILELIST_CHVNUMBER_COLUMN     : iID = DEF_PROPERTIES_CHVNUMBER   ;break; 
	case  DEF_FILELIST_EFNUMBER_COLUMN      : iID = DEF_PROPERTIES_EFNUMBER  ;break; 
		//case  29 : iID =     ;break; 
	case  DEF_FILELIST_FILECHARDF_COLUMN    : iID = DEF_PROPERTIES_DF_SPECIALITY_MULCHANNEL;break; 
	case  DEF_FILELIST_DFNUMBER_COLUMN      : iID = DEF_PROPERTIES_DFNUMBER  ;break;
	case  DEF_FILELIST_AID_COLUMN           : iID = DEF_PROPERTIES_DF_SPECIALITY_AID;break; 
	case  DEF_FILELIST_LINKFILE_COLUMN      : iID = DEF_PROPERTIES_FILELINK;break; 
		//case  34: iID = DEF_PROPERTIES_FILEID  ;break; 
	case  DEF_FILELIST_FRU3_COLUMN : iID = DEF_PROPERTIES_OTA_SPECIALITY_TYPE  ;break; 
	case  DEF_FILELIST_FRU4_COLUMN : iID = DEF_PROPERTIES_OTA_SPECIALITY_AID  ;break; 
	case  DEF_FILELIST_CONTENT_COLUMN : iID = DEF_PROPERTIES_CONTENT;break; 
	default:iID = 0;break;
	}
	return iID;
}
//************************************
// Method:    Exchange2PropertyData
// FullName:  Exchange2PropertyData
// Access:    public 
// Returns:   CString
// Qualifier: 把ACCESS 中数据转换成Property的格式，
//            包含 DEF_PROPERTIES_FILEMODE ，
//            DEF_PROPERTIES_OTA_SPECIALITY_AID，
//            DEF_PROPERTIES_OTA_SPECIALITY_TYPE
// Parameter: int iIDInProperties
// Parameter: CString csDataInMDB
//************************************
CString Exchange2PropertyData(int iIDInProperties,CString csDataInMDB)
{
	CString csDataInProperties;
	switch (iIDInProperties)
	{
	case DEF_PROPERTIES_FILEMODE : 
		if (csDataInMDB.Compare(_T("0")) == 0)
			csDataInProperties = _T("GSM 模式");
		else
			csDataInProperties = _T("UICC 模式");
		break;
	case DEF_PROPERTIES_OTA_SPECIALITY_AID: 
	case DEF_PROPERTIES_OTA_SPECIALITY_TYPE: 	 csDataInProperties = csDataInMDB;	
	default: csDataInProperties = csDataInMDB;
	}


	return csDataInProperties;


}



//************************************
// Method:    GetPropertyByID
// FullName:  GetPropertyByID
// Access:    public 
// Returns:   CMFCPropertyGridProperty*
// Qualifier: 通过 Property ID 获取句柄
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: int iIDInProperties
//************************************
CMFCPropertyGridProperty* GetPropertyByID(CMFCPropertyGridCtrl* cProperty,int iIDInProperties)
{
	CMFCPropertyGridProperty* pProp = NULL;

	if (iIDInProperties > 0)
	{

		try
		{
			pProp =  cProperty->GetProperty((iIDInProperties%0x10)-1);
			while ((iIDInProperties/0x10)!=0)
			{
				iIDInProperties = iIDInProperties/0x10;
				pProp = pProp->GetSubItem((iIDInProperties%0x10)-1);
			}
		}
		catch (...) 
		{return NULL;}
	}
	return pProp;
}

//************************************
// Method:    _InputStatuse
// FullName:  _InputStatuse
// Access:    public 
// Returns:   int
// Qualifier: 把卡片文件属性输入到 Property
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: _variant_t varStatuse
// Parameter: BOOL bIsEF
//************************************
int _InputStatuse(CMFCPropertyGridCtrl* cProperty,_variant_t varStatuse,BOOL bIsEF)
{

	int iStatus= _variant_t2Int(varStatuse);
	bool bStatuse;
	CMFCPropertyGridProperty * pGroup;


	if (bIsEF)
	{
		pGroup = GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY_INVALIDATION);
		bStatuse = iStatus&true;
		pGroup->ResetOriginalValue();
		pGroup->SetOriginalValue((_variant_t)bStatuse);
		pGroup->SetValue((_variant_t)bStatuse);


		pGroup = GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY_RFU1);
		bStatuse = (iStatus>>2)&true;
		pGroup->ResetOriginalValue();
		pGroup->SetOriginalValue((_variant_t)bStatuse);
		pGroup->SetValue((_variant_t)bStatuse);


		pGroup = GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY_RFU2);
		bStatuse = (iStatus>>3)&true;
		pGroup->ResetOriginalValue();
		pGroup->SetOriginalValue((_variant_t)bStatuse);
		pGroup->SetValue((_variant_t)bStatuse);


		pGroup = GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY_MULCHANNEL);
		bStatuse = (iStatus>>5)&true;
		pGroup->ResetOriginalValue();
		pGroup->SetOriginalValue((_variant_t)bStatuse);
		pGroup->SetValue((_variant_t)bStatuse);


		pGroup = GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY_RFMDELRES);
		bStatuse = (iStatus>>6)&true;
		pGroup->ResetOriginalValue();
		pGroup->SetOriginalValue((_variant_t)bStatuse);
		pGroup->SetValue((_variant_t)bStatuse);



		pGroup = GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY_INCREASE);
		bStatuse = (iStatus>>7)&true;
		pGroup->ResetOriginalValue();
		pGroup->SetOriginalValue((_variant_t)bStatuse);
		pGroup->SetValue((_variant_t)bStatuse);
	}
	else
	{

		pGroup = GetPropertyByID(cProperty,DEF_PROPERTIES_DF_SPECIALITY_MULCHANNEL);
		bStatuse = (iStatus>>5)&true;
		pGroup->ResetOriginalValue();
		pGroup->SetOriginalValue((_variant_t)bStatuse);
		pGroup->SetValue((_variant_t)bStatuse);

	}

	return TRUE;
}

//************************************
// Method:    SetPropertyValue
// FullName:  SetPropertyValue
// Access:    public 
// Returns:   int
// Qualifier: SeeName
// Parameter: CMFCPropertyGridCtrl * cProperty ，控件
// Parameter: int iIDInProperties              ，ID
// Parameter: _variant_t varInformation        ，数据
// Parameter: BOOL bExChange                   ，是否转换
// Parameter: BOOL bOriginal                   ，是否当原始数据写入
//************************************
int SetPropertyValue( CMFCPropertyGridCtrl* cProperty, int iIDInProperties, _variant_t varInformation, BOOL bExChange ,BOOL bOriginal) 
{
	CMFCPropertyGridProperty* pProp  = GetPropertyByID(cProperty,iIDInProperties);


	COleVariant _Infor;
	if (pProp!=NULL)
	{

		_Infor = pProp->GetValue();


		switch (_Infor.vt)
		{
		case VT_BSTR:
			if (bExChange)
				_Infor = Exchange2PropertyData(iIDInProperties, _variant_t2CString(varInformation));
			else
				_Infor = (CString)varInformation;
			break;
		case VT_I4:_Infor = (long)varInformation; break;
		case VT_I2:_Infor = (short)varInformation; break;
		default:break;
		}


			

		pProp->SetValue(_Infor);
		if (bOriginal)
		{
			pProp->ResetOriginalValue();
			pProp->SetOriginalValue(_Infor);
		}
		else
		{
			if (_variant_t2CString(varInformation).Compare(_variant_t2CString(pProp->GetOriginalValue())) == 0)
			{
				pProp->ResetOriginalValue();
				pProp->SetOriginalValue(_Infor);
			}
		}


		pProp->SetValue(_Infor);




	}
	else
		return FALSE;

	//cProperty->RedrawWindow();

	//cProperty->MarkModifiedProperties();

	return TRUE;
}

//************************************
// Method:    _InputInforamtion
// FullName:  _InputInforamtion
// Access:    public 
// Returns:   int
// Qualifier: 输入卡片文件数据
// Parameter: CMFCPropertyGridCtrl * cProperty，控件
// Parameter: int iIDInProperties              ，ID
// Parameter: _variant_t varInformation
//************************************
int _InputInforamtion(CMFCPropertyGridCtrl* cProperty,int iIDInProperties, _variant_t varInformation)
{

	if (varInformation.vt == VT_EMPTY)
		return FALSE;

	if (iIDInProperties == DEF_PROPERTIES_SPECIALITY)
		return _InputStatuse(cProperty,varInformation);
	if (iIDInProperties == DEF_PROPERTIES_DF_SPECIALITY_MULCHANNEL)
		return _InputStatuse(cProperty,varInformation,FALSE);

	if (!SetPropertyValue(cProperty, iIDInProperties, varInformation))
		return FALSE;
	//UpdateProperty(cProperty, iIDInProperties);
	return TRUE;
}

//************************************
// Method:    _GetPropertyValue
// FullName:  _GetPropertyValue
// Access:    public 
// Returns:   COleVariant
// Qualifier:
// Parameter: CMFCPropertyGridCtrl * cProperty ，控件
// Parameter: int iIDInProperties              ，ID
//************************************
COleVariant _GetPropertyValue(CMFCPropertyGridCtrl* cProperty,int iIDInProperties)
{
	COleVariant varRet;
	CMFCPropertyGridProperty* pProp  = GetPropertyByID(cProperty,iIDInProperties);
	if ((pProp!=NULL)&&(pProp->IsVisible()))
		varRet = pProp->GetValue();
	else 
		varRet.vt = VT_EMPTY;
	return varRet;
}

//************************************
// Method:    _GetPropertySpecialityValue
// FullName:  _GetPropertySpecialityValue
// Access:    public 
// Returns:   COleVariant
// Qualifier: See Name,此处为多个参数合并成一个数据
// Parameter: CMFCPropertyGridCtrl * cProperty
//************************************
COleVariant _GetPropertySpecialityValue(CMFCPropertyGridCtrl* cProperty)
{
	CString csRet = _T("00");
	BOOL bRet;
	COleVariant varRet;
	int iRet  = 0 ;
	varRet = _GetPropertyValue(cProperty,DEF_PROPERTIES_SPECIALITY_INCREASE);

	if (__variant_t2BOOL(varRet,&bRet))
		iRet -=  bRet*0x80;
	else
		TRACE("格式错误");
	varRet = _GetPropertyValue(cProperty,DEF_PROPERTIES_SPECIALITY_RFMDELRES);
	if (__variant_t2BOOL(varRet,&bRet))
		iRet -=  bRet*0x40;
	else
		TRACE("格式错误");
	varRet = _GetPropertyValue(cProperty,DEF_PROPERTIES_SPECIALITY_MULCHANNEL);
	if (__variant_t2BOOL(varRet,&bRet))
		iRet -=  bRet*0x20;
	else
		TRACE("格式错误");

	varRet = _GetPropertyValue(cProperty,DEF_PROPERTIES_SPECIALITY_RFU2);
	if (__variant_t2BOOL(varRet,&bRet))
		iRet -=  bRet*0x08;
	else
		TRACE("格式错误");

	varRet = _GetPropertyValue(cProperty,DEF_PROPERTIES_SPECIALITY_RFU1);
	if (__variant_t2BOOL(varRet,&bRet))
		iRet -=  bRet*0x04;
	else
		TRACE("格式错误");
	varRet = _GetPropertyValue(cProperty,DEF_PROPERTIES_SPECIALITY_INVALIDATION);
	if (__variant_t2BOOL(varRet,&bRet))
		iRet -= bRet*0x01;
	else
		TRACE("格式错误");

	return (_variant_t)iRet;
}


//************************************
// Method:    _GetPropertyDFSpecialityValue
// FullName:  _GetPropertyDFSpecialityValue
// Access:    public 
// Returns:   COleVariant
// Qualifier: See Name,此处为多个参数合并成一个数据
// Parameter: CMFCPropertyGridCtrl * cProperty
//************************************
COleVariant _GetPropertyDFSpecialityValue(CMFCPropertyGridCtrl* cProperty)
{
	BOOL bRet;
	int iRet = 0x13;
	COleVariant varRet = _GetPropertyValue(cProperty,DEF_PROPERTIES_DF_SPECIALITY_MULCHANNEL);
	if (!__variant_t2BOOL(varRet,&bRet))
		TRACE("格式错误");

	iRet = 0x13-bRet*0x20;
	return (_variant_t)iRet;

}

//************************************
// Method:    GetPropertyValue
// FullName:  GetPropertyValue
// Access:    public 
// Returns:   _variant_t
// Qualifier: 将_GetPropertyValue，_GetPropertySpecialityValue，_GetPropertyDFSpecialityValue 合并
//            根据不同ID，进行分辨使用相关方式，来获取参数
// Parameter: CMFCPropertyGridCtrl * cProperty ，控件
// Parameter: int iIDInProperties              ，ID
//************************************
_variant_t GetPropertyValue(CMFCPropertyGridCtrl* cProperty,int iIDInProperties)
{
	_variant_t _sRet;
	_sRet.vt = VT_EMPTY;
	switch(iIDInProperties)
	{
	case DEF_PROPERTIES_SPECIALITY              : _sRet = _GetPropertySpecialityValue(cProperty);      break;
	case DEF_PROPERTIES_DF_SPECIALITY_MULCHANNEL: _sRet = _GetPropertyDFSpecialityValue(cProperty);    break;
	default:                                      _sRet = _GetPropertyValue(cProperty,iIDInProperties);
		                                          _sRet = Exchange2AccessData(iIDInProperties,_sRet);break;
	}
	return _sRet;
}


//************************************
// Method:    _GetACFromACData
// FullName:  _GetACFromACData
// Access:    public 
// Returns:   CString
// Qualifier: 转换2G AC 
// Parameter: int iCommand
// Parameter: CString csACData
//************************************
CString _GetACFromACData(int iCommand,CString csACData)
{
	CString csAC;
	csAC = csACData.Mid(iCommand-1,1);
	switch(_CString2Int(csAC))
	{
	case 0  :csAC = _T("Alway");break;
	case 0xF:csAC = _T("NEVER");break;
	default: csAC = _T("CHV")+csAC ; break;
	}
	return csAC;
}

//************************************
// Method:    Exchange2AccessData
// FullName:  Exchange2AccessData
// Access:    public 
// Returns:   _variant_t
// Qualifier: 把Properties数据专程ACCESS中数据
// Parameter: int iIDInProperties
// Parameter: _variant_t varInProperties
//************************************
_variant_t Exchange2AccessData(int iIDInProperties, _variant_t varInProperties)
{

	_variant_t varInAccess;
	switch (iIDInProperties)
	{
	case DEF_PROPERTIES_FILEMODE : 
		if (((CString) varInProperties).Compare(_T("GSM 模式")) == 0)
			varInAccess = 0;
		if (((CString) varInProperties).Compare(_T("UICC 模式")) == 0)
			varInAccess = 1;
		break;
	case DEF_PROPERTIES_TYPE:
	case DEF_PROPERTIES_FILESTRUCTURE:
	default: varInAccess = varInProperties;
	}
	return varInAccess;
}




//************************************
// Method:    Exchange2BINData
// FullName:  Exchange2BINData
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: int iIDInProperties
// Parameter: _variant_t _Input
//************************************
int Exchange2BINData(int iIDInProperties,_variant_t _Input)
{
	int iRet= -1; ;
	switch (iIDInProperties)
	{
	case DEF_PROPERTIES_FILESTRUCTURE:
		iRet = _ExChangeFileStructure(_Input);break;
	case DEF_PROPERTIES_TYPE:
		iRet = _ExChangeFileType(_Input);break;
	case DEF_PROPERTIES_FILEMODE:
		iRet = _Input;break;
	default:break;
	}
	return iRet;

}

//************************************
// Method:    GetPropertyIntByID
// FullName:  GetPropertyIntByID
// Access:    public 
// Returns:   int
// Qualifier: See Name
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: int iIDInProperties
//************************************
int GetPropertyIntByID(CMFCPropertyGridCtrl* cProperty,int iIDInProperties)
{
	return Exchange2BINData(iIDInProperties,GetPropertyValue(cProperty,iIDInProperties));
}
//************************************
// Method:    UpdatePropertyStatuse
// FullName:  UpdatePropertyStatuse
// Access:    public 
// Returns:   void
// Qualifier: 根据当前Property 数据，显示有用参数，
//            关闭无用数据
// Parameter: CMFCPropertyGridCtrl * cProperty
//************************************
void UpdatePropertyStatuse(CMFCPropertyGridCtrl* cProperty)
{
	UpdateProperty3GStatuse(cProperty);
	UpdatePropertyLinkStatuse(cProperty);
	UpdatePropertyFileStatuse(cProperty);
	UpdatePropertyOTA(cProperty);

}

//************************************
// Method:    UpdateProperty3GStatuse
// FullName:  UpdateProperty3GStatuse
// Access:    public 
// Returns:   void
// Qualifier: 通过文件模式，关闭或打开 3G参数
// Parameter: CMFCPropertyGridCtrl * cProperty
//************************************
void UpdateProperty3GStatuse(CMFCPropertyGridCtrl* cProperty)
{

	if (GetPropertyIntByID(cProperty,DEF_PROPERTIES_FILEMODE) == 1)
	{
		GetPropertyByID(cProperty,DEF_PROPERTIES_3GAC)->Show(TRUE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_SFI)->Show(TRUE);
	}
	else 
	{
		GetPropertyByID(cProperty,DEF_PROPERTIES_3GAC)->Show(FALSE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_SFI)->Show(FALSE);
	}

}
//************************************
// Method:    UpdatePropertyLinkStatuse
// FullName:  UpdatePropertyLinkStatuse
// Access:    public 
// Returns:   void
// Qualifier:通过文件类别，关闭或打开LINK参数
// Parameter: CMFCPropertyGridCtrl * cProperty
//************************************
void UpdatePropertyLinkStatuse(CMFCPropertyGridCtrl* cProperty)
{
	if ((GetPropertyIntByID(cProperty,DEF_PROPERTIES_FILESTRUCTURE)&0x4) != 0x4)
		GetPropertyByID(cProperty,DEF_PROPERTIES_FILELINK)->Show(FALSE);
	else
		GetPropertyByID(cProperty,DEF_PROPERTIES_FILELINK)->Show(TRUE);
}

//************************************
// Method:    UpdatePropertyFileStatuse
// FullName:  UpdatePropertyFileStatuse
// Access:    public 
// Returns:   void
// Qualifier: 通过文件类别，关闭或打开文件参数
// Parameter: CMFCPropertyGridCtrl * cProperty
//************************************
void UpdatePropertyFileStatuse(CMFCPropertyGridCtrl* cProperty)
{
	int iType      = GetPropertyIntByID(cProperty,DEF_PROPERTIES_TYPE);
	int iStructure =GetPropertyIntByID(cProperty,DEF_PROPERTIES_FILESTRUCTURE);

	switch(iType)
	{
	case DEF_FILETYPE_ADF:
		GetPropertyByID(cProperty,DEF_PROPERTIES_DF_SPECIALITY_AID)->Show(TRUE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_DFINFO)->Show(TRUE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_EFINFO)->Show(FALSE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_2GAC)->Show(FALSE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY)->Show(FALSE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_DF_SPECIALITY)->Show(TRUE);
		break;
	case DEF_FILETYPE_MF:
	case DEF_FILETYPE_DF:
		GetPropertyByID(cProperty,DEF_PROPERTIES_DFINFO)->Show(TRUE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_EFINFO)->Show(FALSE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_2GAC)->Show(FALSE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY)->Show(FALSE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_DF_SPECIALITY)->Show(TRUE);
		GetPropertyByID(cProperty,DEF_PROPERTIES_DF_SPECIALITY_AID)->Show(FALSE);
		break;
	case DEF_FILETYPE_EF:

		switch(iStructure)
		{
		case DEF_FILESTRUCTURE_CLINK:
		case DEF_FILESTRUCTURE_LLINK:
			GetPropertyByID(cProperty,DEF_PROPERTIES_FILELINK)->Show(TRUE);
		case DEF_FILESTRUCTURE_CYCLIC:
		case DEF_FILESTRUCTURE_LINER:
			GetPropertyByID(cProperty,DEF_PROPERTIES_DFINFO)->Show(FALSE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_DF_SPECIALITY)->Show(FALSE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_EFINFO)->Show(TRUE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_RECORDLENGTH)->Show(TRUE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_RECORDNUMBER)->Show(TRUE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY)->Show(TRUE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_2GAC)->Show(TRUE);break;

		case DEF_FILESTRUCTURE_TLINK:
			GetPropertyByID(cProperty,DEF_PROPERTIES_FILELINK)->Show(TRUE);
		case DEF_FILESTRUCTURE_TRANSPARENT:
			GetPropertyByID(cProperty,DEF_PROPERTIES_DFINFO)->Show(FALSE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_DF_SPECIALITY)->Show(FALSE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_EFINFO)->Show(TRUE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_RECORDLENGTH)->Show(FALSE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_RECORDNUMBER)->Show(FALSE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_SPECIALITY)->Show(TRUE);
			GetPropertyByID(cProperty,DEF_PROPERTIES_2GAC)->Show(TRUE);
			break;
		}
		break;
	default:break;
	}




}



//************************************
// Method:    _GetPropertyID
// FullName:  _GetPropertyID
// Access:    public 
// Returns:   int
// Qualifier: 通过句柄获取 ID
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: CMFCPropertyGridProperty * cProp
//************************************
int _GetPropertyID(CMFCPropertyGridCtrl*cProperty,CMFCPropertyGridProperty*cProp)
{

	return __GetPropertyID(cProperty,cProp,iPropertiesArray,DEF_PROPERTIES_NUMBER);
}

//************************************
// Method:    __GetPropertyID
// FullName:  __GetPropertyID
// Access:    public 
// Returns:   int
// Qualifier: 通过句柄获取 ID
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: CMFCPropertyGridProperty * cProp
// Parameter: const int * iIDArray ，现有ID列表
// Parameter: int iNumber          ，iIDArray总数
//************************************
int __GetPropertyID(CMFCPropertyGridCtrl*cProperty,CMFCPropertyGridProperty*cProp,const int * iIDArray,int iNumber)
{
	for (int i = 0 ; i < iNumber ; i ++)
	{
		if (GetPropertyByID(cProperty,iIDArray[i]) == cProp)
			return iIDArray[i];	
	}
	return NULL;
}


int UpdateProperty3GARR(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile )
{

	CMFCPropertyGridProperty* pProp;
	CStringArray csRecords;
	CString csARRFile,csARRRoute,csARR,csAC;
	int iCounter = 0;
	int iRecord  = 0;
	if (!__variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_ROUTE),csARRRoute))
		TRACE(_T("格式错误"));
	if (!__variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_ARRFILE),csARRFile))
		TRACE(_T("格式错误"));
	if (!__variant_t2int(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_ARRRECORD),&iRecord))
		TRACE(_T("格式错误"));



	if (!csARRFile.IsEmpty()&
		pcFile->RetrieveARRFile(csARRFile, csARRRoute, &iCounter,&csRecords))
	{
		pProp = GetPropertyByID(cProperty, DEF_PROPERTIES_3GAC_ARRRECORD);
		pProp->RemoveAllOptions();
		pProp->AddOption("0");
		for (int i = 1; i <= iCounter; i++)
			pProp->AddOption(Int2CStringDecimal(i));
	}
	else
		return DEF_NOT_FIND_ARR_FILE;
		

	if ((iRecord <= iCounter)&&(iRecord>0))
	{
		csARR = csRecords.GetAt(iRecord-1);
		csAC.Empty();
		_GetACFromARR(0x01,csARR,csAC,&iRecord);
		SetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_READ,CString2_variant_t(VT_BSTR,csAC),TRUE,FALSE);
		csAC.Empty();
		_GetACFromARR(0x02,csARR,csAC,&iRecord);
		SetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_UPDATE,CString2_variant_t(VT_BSTR,csAC),TRUE,FALSE);;
		csAC.Empty();
		_GetACFromARR(0x32,csARR,csAC,&iRecord,true);
		SetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_INCREASE,CString2_variant_t(VT_BSTR,csAC),TRUE,FALSE);
		csAC.Empty();
		_GetACFromARR(0x10,csARR,csAC,&iRecord);
		SetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_ACTIVE,CString2_variant_t(VT_BSTR,csAC),TRUE,FALSE);
		csAC.Empty();
		_GetACFromARR(0x08,csARR,csAC,&iRecord);
		SetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_DEACTIVE,CString2_variant_t(VT_BSTR,csAC),TRUE,FALSE);
		return TRUE;
	}

	SetPropertyValue(cProperty, DEF_PROPERTIES_3GAC_READ, _T(""), TRUE, FALSE);
	SetPropertyValue(cProperty, DEF_PROPERTIES_3GAC_UPDATE, _T(""), TRUE, FALSE);
	SetPropertyValue(cProperty, DEF_PROPERTIES_3GAC_INCREASE, _T(""), TRUE, FALSE);
	SetPropertyValue(cProperty, DEF_PROPERTIES_3GAC_ACTIVE, _T(""), TRUE, FALSE);
	SetPropertyValue(cProperty, DEF_PROPERTIES_3GAC_DEACTIVE, _T(""), TRUE, FALSE);

	return FALSE;
}

int UpdateProperty3GARRRecord(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile )
{
	CString csARRFile,csARRRoute;
	if (!__variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_ROUTE),csARRRoute))
		TRACE(_T("格式错误"));
	if (!__variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_ARRFILE),csARRFile))
		TRACE(_T("格式错误"));

	CStringArray csARRArr;
	CString csARR,csAC,csEx;
	int iOther;
	int iCounter = 0;

	if (!pcFile->RetrieveARRFile(csARRFile, csARRRoute, &iCounter, &csARRArr))
		return DEF_NOT_FIND_ARR_FILE;

	if (csARRArr.GetCount()<1)
		return FALSE;

	for (int i = 0 ; i < iCounter; i++)
	{
	
		csARR = csARRArr.GetAt(i);
			
		csEx = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_READ));
		csAC.Empty();
		_GetACFromARR(0x01,csARR,csAC,&iOther);
		if (csEx.Compare(csAC) != 0 )
			continue;

		csEx = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_UPDATE));
		csAC.Empty();
		_GetACFromARR(0x02,csARR,csAC,&iOther);
		if (csEx.Compare(csAC) != 0 )
			continue;

		csEx = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_INCREASE));
		csAC.Empty();
		_GetACFromARR(0x32,csARR,csAC,&iOther,true);
		if (csEx.Compare(csAC) != 0 )
			continue;

		csEx = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_ACTIVE));
		csAC.Empty();
		_GetACFromARR(0x10,csARR,csAC,&iOther);
		if (csEx.Compare(csAC) != 0 )
			continue;

		csEx = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_DEACTIVE));
		csAC.Empty();
		_GetACFromARR(0x8,csARR,csAC,&iOther);
		if (csEx.Compare(csAC) != 0 )
			continue;

		SetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_ARRRECORD,(_bstr_t) Int2CStringDecimal(i+1),TRUE,FALSE);
		return TRUE;

	}

	SetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_ARRRECORD,_T("-1"),TRUE,FALSE);

	return FALSE;

}

int UpdateProperty3GARRRecordList(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile)
{
	CString csARRFile,csARRRoute;
	CMFCPropertyGridProperty* pProp;

	int iCounter;

	if (!pcFile->RetrieveARRFile(csARRFile, csARRRoute, &iCounter))
		return DEF_NOT_FIND_ARR_FILE;

	if (!__variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_ROUTE),csARRRoute))
		TRACE(_T("格式错误"));
	if (!__variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_3GAC_ARRFILE),csARRFile))
		TRACE(_T("格式错误"));

	

	pProp =GetPropertyByID(cProperty,DEF_PROPERTIES_3GAC_ARRRECORD);
	pProp->RemoveAllOptions();
	pProp->AddOption("0");
	for (int i = 1 ; i <= iCounter ; i ++)
	{
		pProp->AddOption(Int2CStringDecimal(i));
	}

	return TRUE;
}

int UpdatePropertyFileRoutes(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile)
{
	CMFCPropertyGridProperty* pProp;
	CStringArray csRoutsList;
	int iSum;

	iSum = pcFile->GetDFList(csRoutsList, TRUE, TRUE);

	pProp =GetPropertyByID(cProperty,DEF_PROPERTIES_ROUTE);

	pProp->RemoveAllOptions();

	for (int i = 0 ; i < iSum ; i++)
		pProp->AddOption(csRoutsList.GetAt(i));

	return iSum;
}

int UpdateProperty2GAC(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile)
{
	CString csAC, csACData;
	int iRecord, iCounter;
	if (!__variant_t2int(GetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_2GACRECORD),&iRecord))
		TRACE(_T("格式错误"));


	CStringArray csACRecords;

	if (!pcFile->GetRecordFileData(_T("3F000022"), &iCounter, NULL, &csACRecords))
		return DEF_NOT_FIND_ARR_FILE;

	CMFCPropertyGridProperty* pProp =GetPropertyByID(cProperty,DEF_PROPERTIES_2GAC_2GACRECORD);
	pProp->RemoveAllOptions();
	pProp->AddOption("0");
	for (int i = 1 ; i <= iCounter ; i ++)
		pProp->AddOption(Int2CStringDecimal(i));

	if(iRecord>0)
	{
	
		if (iRecord<=iCounter)
		{
			csACData = csACRecords.GetAt(iRecord-1);
			csAC  =  _GetACFromACData(1,csACData);
			SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_READ,CString2_variant_t (VT_BSTR,csAC),TRUE,FALSE);

			csAC  =  _GetACFromACData(2,csACData);
			SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_UPDATE,CString2_variant_t (VT_BSTR,csAC),TRUE,FALSE);

			csAC  =  _GetACFromACData(3,csACData);
			SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_INCREASE,CString2_variant_t (VT_BSTR,csAC),TRUE,FALSE);

			csAC  =  _GetACFromACData(4,csACData);
			SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_RFM,CString2_variant_t (VT_BSTR,csAC),TRUE,FALSE);

			csAC  =  _GetACFromACData(5,csACData);
			SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_REHA,CString2_variant_t (VT_BSTR,csAC),TRUE,FALSE);

			csAC  =  _GetACFromACData(6,csACData);
			SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_INVA,CString2_variant_t (VT_BSTR,csAC),TRUE,FALSE);

			
		}
		else
			SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_2GACRECORD,_T("-1"),TRUE,FALSE);

		return TRUE;
	}
	else
	{

		SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_READ,_T(""),TRUE,FALSE);
		SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_UPDATE,_T(""),TRUE,FALSE);
		SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_INCREASE,_T(""),TRUE,FALSE);
		SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_RFM,_T(""),TRUE,FALSE);
		SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_REHA,_T(""),TRUE,FALSE);
		SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_INVA,_T(""),TRUE,FALSE);

		return FALSE;
	}

	
}


int UpdateProperty2GACList(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile)
{
	int iCounter;
	if (!pcFile->GetRecordFileData(_T("3F000022"),&iCounter))
		return DEF_NOT_FIND_ARR_FILE;

	CMFCPropertyGridProperty* pProp = GetPropertyByID(cProperty, DEF_PROPERTIES_2GAC_2GACRECORD);
	pProp->RemoveAllOptions();
	pProp->AddOption("0");
	for (int i = 1; i <= iCounter; i++)
	{
		pProp->AddOption(Int2CStringDecimal(i));
	}
	return TRUE;

}

int UpdateProperty2GACRecord(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile)
{

	CString csACData,csAC,csEx;
	CString csRead, csUpdate,csIncreat,csRFM,csInval,csReha;

	csRead    = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_READ));
	csUpdate  = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_UPDATE));
	csIncreat = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_INCREASE));
	csRFM     = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_RFM));
	csInval   = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_INVA));
	csReha    = _variant_t2CString(GetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_REHA));

	int iCounter;
	CStringArray csACRecords;
	if (!pcFile->GetRecordFileData(_T("3F000022"), &iCounter, NULL, &csACRecords))
		return DEF_NOT_FIND_ARR_FILE;


	for (int i = 1 ; i <= iCounter; i++)
	{

		csACData = csACRecords.GetAt(i-1);

		csAC = _GetACFromACData(1,csACData);
		if (csRead.Compare(csAC) != 0 )
			continue;

		csAC = _GetACFromACData(2,csACData);
		if (csUpdate.Compare(csAC) != 0 )
			continue;
		csAC = _GetACFromACData(3,csACData);
		if (csIncreat.Compare(csAC) != 0 )
			continue;
		csAC = _GetACFromACData(4,csACData);
		if (csRFM.Compare(csAC) != 0 )
			continue;

		csAC = _GetACFromACData(5,csACData);
		if (csReha.Compare(csAC) != 0 )
			continue;


		csAC = _GetACFromACData(6,csACData);
		if (csInval.Compare(csAC) != 0 )
			continue;

		SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_2GACRECORD,(_bstr_t) Int2CStringDecimal(i),TRUE,FALSE);
		return TRUE;


	}

	SetPropertyValue(cProperty,DEF_PROPERTIES_2GAC_2GACRECORD,_T("-1"),TRUE,FALSE);
	return FALSE;
}

void UpdatePropertyOTA(CMFCPropertyGridCtrl* cProperty)
{
	if (_variant_t2CString(GetPropertyValue(cProperty, DEF_PROPERTIES_OTA_SPECIALITY_TYPE)).IsEmpty())
		GetPropertyByID(cProperty,DEF_PROPERTIES_OTA_SPECIALITY_AID)->Show(FALSE);
	else
		GetPropertyByID(cProperty,DEF_PROPERTIES_OTA_SPECIALITY_AID)->Show(TRUE);
}


void _UpdateProperty( CMFCPropertyGridCtrl* cProperty, int iIDInProperties, ProfileClass* pcFile)
{
	int iRet; 


	switch(iIDInProperties)
	{
	case DEF_PROPERTIES_3GAC_ARRFILE   :iRet=UpdateProperty3GARRRecordList(cProperty, pcFile);
	case DEF_PROPERTIES_3GAC_ARRRECORD :iRet = UpdateProperty3GARR(cProperty, pcFile); break;
	case DEF_PROPERTIES_2GAC_2GACRECORD:iRet = UpdateProperty2GAC(cProperty, pcFile); UpdateProperty2GACList(cProperty, pcFile); break;

	case DEF_PROPERTIES_TYPE           :iRet = UpdatePropertyFileRoutes(cProperty, pcFile);
	case DEF_PROPERTIES_FILESTRUCTURE  : UpdatePropertyStatuse(cProperty); break;
	case DEF_PROPERTIES_FILEMODE       : UpdateProperty3GStatuse(cProperty); break;

	case DEF_PROPERTIES_2GAC_READ:
	case DEF_PROPERTIES_2GAC_UPDATE:
	case DEF_PROPERTIES_2GAC_INCREASE:
	case DEF_PROPERTIES_2GAC_RFM:
	case DEF_PROPERTIES_2GAC_INVA:
	case DEF_PROPERTIES_2GAC_REHA:     iRet = UpdateProperty2GACRecord(cProperty, pcFile); break;

	case DEF_PROPERTIES_3GAC_READ:
	case DEF_PROPERTIES_3GAC_UPDATE:
	case DEF_PROPERTIES_3GAC_INCREASE:
	case DEF_PROPERTIES_3GAC_ACTIVE:
	case DEF_PROPERTIES_3GAC_DEACTIVE:iRet = UpdateProperty3GARRRecord(cProperty, pcFile); break;
	case DEF_PROPERTIES_OTA_SPECIALITY_TYPE:UpdatePropertyOTA(cProperty);break;
	default: break;
	//case DEF_PROPERTIES_ROUTE: UpdatePropertyFileRoutes(cProperty); break;
	//case DEF_PROPERTIES_DF_SPECIALITY_AID: UpdatePropertyAID(cProperty); break;
	}


}

//************************************
// Method:    UpdatePropertyOption
// FullName:  UpdatePropertyOption
// Access:    public 
// Returns:   void
// Qualifier: 增加Property 的选项
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: CDocument * pFrm
//************************************
void UpdatePropertyOption(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile)
{
	
	//UpdateProperty3GARRRecordList(cProperty,pcFile);
	UpdateProperty3GARR(cProperty, pcFile);
	//UpdateProperty2GAC(cProperty,pcFile);
	UpdateProperty2GACList(cProperty, pcFile);
//	UpdatePropertyAID(cProperty, pcFile);
	UpdatePropertyFileRoutes(cProperty, pcFile);


}

////************************************
//// Method:    UpdatePropertyAID
//// FullName:  UpdatePropertyAID
//// Access:    public 
//// Returns:   void
//// Qualifier: 增加AID 的选项
//// Parameter: CMFCPropertyGridCtrl * cProperty
//// Parameter: CDocument * pFrm
////************************************
//void UpdatePropertyAID(CMFCPropertyGridCtrl* cProperty,  CWnd* pFrm)
//{
//
//	_variant_t varAID = GetPropertyValue(cProperty,DEF_PROPERTIES_DF_SPECIALITY_AID);
//
//	if ((varAID.vt == VT_BSTR)&&(!((CString)varAID).IsEmpty()))
//	{
//
//		CStringArray csAIDArr ;
//		int iAIDCouter = ((CMainFrame*)pFrm)->GetAIDFromFile(csAIDArr);
//		int iNumber    = _variant_t2Int(varAID);
//
//		CMFCPropertyGridProperty* pProp =GetPropertyByID(cProperty,DEF_PROPERTIES_DF_SPECIALITY_AID);
//		pProp->RemoveAllOptions();
//		pProp->AddOption("0");
//		for (int i = 0 ; i < iAIDCouter ; i ++)
//			pProp->AddOption(  (csAIDArr.GetAt(i).Mid(2, _CString2Int(csAIDArr.GetAt(i).Left(2))*2)));
//
//		if (iNumber > 0)
//			pProp->SetValue((_variant_t)(csAIDArr.GetAt(iNumber).Mid(2, _CString2Int(csAIDArr.GetAt(iNumber).Left(2))*2)));
//		
//	}
//}







int AppendOneNew2GAC(CString csPath, int iRead, int iUpdate, int iIncreat, int iRFM, int iReha, int iInval, ProfileClass* pcFile)
{

	UINT    uiList[3] = { DEF_FILELIST_RECORD_NUMBER_COLUMN,DEF_FILELIST_SIZE_COLUMN,DEF_FILELIST_CONTENT_COLUMN };
	CString csACData;
	PCFS _ACFile;
	if (MessageBox(NULL, _T("是否需要生成一条匹配的AC记录写入 3F000022 文件"), _T("Warning"), MB_YESNO) == IDNO)
		return FALSE;

	if (pcFile->GetFileFromDoc(csPath, &_ACFile) < 0)
		return -1;

	csACData = Generate2GACRecord(iRead, iUpdate, iIncreat, iRFM, iReha, iInval);

	if (csACData.IsEmpty())
		return -2;

	_ACFile.stContent = _ACFile.stContent + _T("\r\n") + csACData;

	_ACFile.uiRECORDNUMBER = _ACFile.uiRECORDNUMBER + 1;

	_ACFile.CorrectPCFS();

	if (pcFile->SetFileSubDataListToDoc(_ACFile.uiHANDLE, _ACFile, 3, uiList)!=0)
		return -11;

	return _ACFile.uiRECORDNUMBER;

}


int AppendOneNew3GARR(CString csPath,int iRead,int iUpdate,int iIncreat,int iReha,int iInval, ProfileClass* pcFile)
{
	UINT    uiList[3] = { DEF_FILELIST_RECORD_NUMBER_COLUMN,DEF_FILELIST_SIZE_COLUMN,DEF_FILELIST_CONTENT_COLUMN };
	CString csARRData;
	PCFS    _ARRFile;
	if (MessageBox(NULL,_T("是否需要生成一条匹配的ARR记录写入")+csPath+_T("文件"),_T("Warning"),MB_YESNO) == IDNO)
		return FALSE;

	csARRData = Generate3GARRRecord(iRead,iUpdate,iIncreat,iReha,iInval);

	if (pcFile->GetFileFromDoc(csPath, &_ARRFile) < 0)
		return -1;

	csARRData = Generate3GARRRecord(iRead, iUpdate, iIncreat, iReha, iInval);

	if (csARRData.IsEmpty())
		return -2;

	csARRData = csARRData+RepeatCString("F", _ARRFile.uiRECORDLENGTH*2);
	csARRData = csARRData.Left(_ARRFile.uiRECORDLENGTH *2);

	_ARRFile.stContent = _ARRFile.stContent + _T("\r\n") + csARRData;

	_ARRFile.uiRECORDNUMBER = _ARRFile.uiRECORDNUMBER + 1;

	_ARRFile.CorrectPCFS();

	if (pcFile->SetFileSubDataListToDoc(_ARRFile.uiHANDLE, _ARRFile, 3, uiList) != 0)
		return -11;

	return _ARRFile.uiRECORDNUMBER;
}

