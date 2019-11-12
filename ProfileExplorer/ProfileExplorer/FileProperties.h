
#pragma once
#include "stdafx.h"
#include "ProfileClass.h"

#define DEF_PROPERTIES_NUMBER              45
#define DEF_FRU_NUMBER                      9

#define DEF_PROPERTIES_BASICINFORMATION  0x001
#define DEF_PROPERTIES_FILEHANDLE        0x11
#define DEF_PROPERTIES_FILENAME          0x21
#define DEF_PROPERTIES_FILEID            0x31
#define DEF_PROPERTIES_TYPE              0x41
#define DEF_PROPERTIES_ROUTE             0x51
#define DEF_PROPERTIES_FILEMODE          0x61

#define DEF_PROPERTIES_DFINFO    0x71
#define DEF_PROPERTIES_CHVNUMBER 0x171
#define DEF_PROPERTIES_EFNUMBER  0x271
#define DEF_PROPERTIES_DFNUMBER  0x371


#define DEF_PROPERTIES_EFINFO         0x81
#define DEF_PROPERTIES_FILESTRUCTURE  0x181
#define DEF_PROPERTIES_SIZE           0x281
#define DEF_PROPERTIES_RECORDNUMBER   0x381
#define DEF_PROPERTIES_RECORDLENGTH   0x481
#define DEF_PROPERTIES_SFI            0x581
#define DEF_PROPERTIES_FILELINK       0x681


#define DEF_PROPERTIES_SPECIALITY             0x2
#define DEF_PROPERTIES_SPECIALITY_INCREASE    0x12
#define DEF_PROPERTIES_SPECIALITY_RFMDELRES   0x22
#define DEF_PROPERTIES_SPECIALITY_MULCHANNEL  0x32
#define DEF_PROPERTIES_SPECIALITY_RFU1        0x42
#define DEF_PROPERTIES_SPECIALITY_RFU2        0x52
#define DEF_PROPERTIES_SPECIALITY_INVALIDATION 0x62

#define DEF_PROPERTIES_DF_SPECIALITY       0x3
#define DEF_PROPERTIES_DF_SPECIALITY_MULCHANNEL       0x13
#define DEF_PROPERTIES_DF_SPECIALITY_AID              0x23
//#define DEF_PROPERTIES_DF_AIDRecord        0x33

#define DEF_PROPERTIES_2GAC                0x4
#define DEF_PROPERTIES_2GAC_READ           0x14
#define DEF_PROPERTIES_2GAC_UPDATE         0x24
#define DEF_PROPERTIES_2GAC_INCREASE       0x34
#define DEF_PROPERTIES_2GAC_RFM            0x44
#define DEF_PROPERTIES_2GAC_REHA           0x54
#define DEF_PROPERTIES_2GAC_INVA           0x64
#define DEF_PROPERTIES_2GAC_2GACRECORD     0x74

#define DEF_PROPERTIES_3GAC          0x5
#define DEF_PROPERTIES_3GAC_READ     0x15
#define DEF_PROPERTIES_3GAC_UPDATE   0x25
#define DEF_PROPERTIES_3GAC_INCREASE 0x35
#define DEF_PROPERTIES_3GAC_ACTIVE   0x45
#define DEF_PROPERTIES_3GAC_DEACTIVE 0x55
#define DEF_PROPERTIES_3GAC_ARRFILE   0x65
#define DEF_PROPERTIES_3GAC_ARRRECORD 0x75

#define DEF_PROPERTIES_OTA_SPECIALITY        0x6
#define DEF_PROPERTIES_OTA_SPECIALITY_TYPE  0x16
#define DEF_PROPERTIES_OTA_SPECIALITY_AID   0x26

#define DEF_PROPERTIES_CONTENT         -1


#define  DEF_PARAMETER_KEY         0x01
#define  DEF_PARAMETER_KEY_PIN01   0x11
#define  DEF_PARAMETER_KEY_PIN02   0x21
#define  DEF_PARAMETER_KEY_PIN81   0x21
#define  DEF_PARAMETER_KEY_PIN0A   0x31


#define  DEF_PARAMETER_KEY_PUK01   0x41
#define  DEF_PARAMETER_KEY_PUK02   0x51
#define  DEF_PARAMETER_KEY_PUK81   0x51
#define  DEF_PARAMETER_KEY_PS      0x61
#define  DEF_PARAMETER_KEY_KI      0x71

#define  DEF_PARAMETER_RESEVER             0x02
#define  DEF_PARAMETER_RESEVER_FILENUMBER  0x12
#define  DEF_PARAMETER_RESEVER_SPACE       0x22

#define  DEF_PARAMETER_USED                0x03
#define  DEF_PARAMETER_USED_FILENUMBER     0x13
#define  DEF_PARAMETER_USED_SPACE          0x23

const int iPropertiesArray[DEF_PROPERTIES_NUMBER] = { DEF_PROPERTIES_BASICINFORMATION,DEF_PROPERTIES_FILEHANDLE,
	DEF_PROPERTIES_FILENAME,DEF_PROPERTIES_FILEID,
	DEF_PROPERTIES_TYPE,DEF_PROPERTIES_ROUTE,DEF_PROPERTIES_FILEMODE,
	DEF_PROPERTIES_DFINFO,DEF_PROPERTIES_CHVNUMBER,DEF_PROPERTIES_EFNUMBER,DEF_PROPERTIES_DFNUMBER,
	DEF_PROPERTIES_EFINFO,DEF_PROPERTIES_FILESTRUCTURE,DEF_PROPERTIES_SIZE,
	DEF_PROPERTIES_RECORDNUMBER,DEF_PROPERTIES_RECORDLENGTH,DEF_PROPERTIES_SFI,DEF_PROPERTIES_FILELINK,
	DEF_PROPERTIES_SPECIALITY,DEF_PROPERTIES_SPECIALITY_INCREASE,DEF_PROPERTIES_SPECIALITY_RFMDELRES,
	DEF_PROPERTIES_SPECIALITY_MULCHANNEL,DEF_PROPERTIES_SPECIALITY_RFU1,DEF_PROPERTIES_SPECIALITY_RFU2,
	DEF_PROPERTIES_SPECIALITY_INVALIDATION,
	DEF_PROPERTIES_2GAC,DEF_PROPERTIES_2GAC_READ,DEF_PROPERTIES_2GAC_UPDATE,
	DEF_PROPERTIES_2GAC_INCREASE,DEF_PROPERTIES_2GAC_RFM,DEF_PROPERTIES_2GAC_REHA,
	DEF_PROPERTIES_2GAC_INVA,DEF_PROPERTIES_2GAC_2GACRECORD,
	DEF_PROPERTIES_3GAC,DEF_PROPERTIES_3GAC_READ,DEF_PROPERTIES_3GAC_UPDATE,
	DEF_PROPERTIES_3GAC_INCREASE,DEF_PROPERTIES_3GAC_ACTIVE,DEF_PROPERTIES_3GAC_DEACTIVE,
	DEF_PROPERTIES_3GAC_ARRFILE,DEF_PROPERTIES_3GAC_ARRRECORD,
	DEF_PROPERTIES_DF_SPECIALITY_MULCHANNEL,DEF_PROPERTIES_DF_SPECIALITY_AID,
	DEF_PROPERTIES_OTA_SPECIALITY_AID,DEF_PROPERTIES_OTA_SPECIALITY_TYPE };

const int iFRUArray[DEF_FRU_NUMBER] = { DEF_PARAMETER_KEY_PIN01,DEF_PARAMETER_KEY_PIN02,DEF_PARAMETER_KEY_PIN0A,
	DEF_PARAMETER_KEY_PS,DEF_PARAMETER_KEY_PUK01,DEF_PARAMETER_KEY_PUK02,
	DEF_PARAMETER_KEY_KI,DEF_PARAMETER_RESEVER_FILENUMBER,DEF_PARAMETER_RESEVER_SPACE };


CMFCPropertyGridProperty* CreateBasicInformation();
CMFCPropertyGridProperty* CreateEFInformation(void);
CMFCPropertyGridProperty* CreateDFInformation(void);
CMFCPropertyGridProperty* CreateSpeciality(void);
CMFCPropertyGridProperty* CreateDFSpeciality(void);
CMFCPropertyGridProperty* CreateAccessInGSM(void);
CMFCPropertyGridProperty* CreateAccessInUICC(void);
CMFCPropertyGridProperty* CreateOTASepeciality(void);
CMFCPropertyGridProperty* CreateKeyInformation(void);
CMFCPropertyGridProperty* CreateReserveInformation(void);
CMFCPropertyGridProperty* CreateUsedInformation(void);


CMFCPropertyGridProperty* GetPropertyByID(CMFCPropertyGridCtrl* cProperty,int iIDInProperties);


COleVariant _GetPropertyValue(CMFCPropertyGridCtrl* cProperty,int iIDInProperties);

_variant_t GetPropertyValue(CMFCPropertyGridCtrl* cProperty,int iIDInProperties);
int GetPropertyIntByID(CMFCPropertyGridCtrl* cProperty,int iIDInProperties);

CString Exchange2PropertyData(int iIDInProperties,CString csDataInMDB);
_variant_t Exchange2AccessData(int iIDInProperties, _variant_t varInProperties);
int Exchange2BINData(int iIDInProperties,_variant_t _Input);

void UpdatePropertyStatuse(CMFCPropertyGridCtrl* cProperty);
void UpdateProperty3GStatuse(CMFCPropertyGridCtrl* cProperty);
void UpdatePropertyLinkStatuse(CMFCPropertyGridCtrl* cProperty);
void UpdatePropertyOTA(CMFCPropertyGridCtrl* cProperty);

int _InputStatuse(CMFCPropertyGridCtrl* cProperty, _variant_t varStatuse, BOOL bIsEF = TRUE);
int SetPropertyValue( CMFCPropertyGridCtrl* cProperty, int iIDInProperties, _variant_t varInformation, BOOL bExChange=TRUE ,BOOL bOrigina = TRUE );
int _InputInforamtion(CMFCPropertyGridCtrl* cProperty,int iIDInProperties, _variant_t varInformation);
int ExchangeFileListID2PropertiesID( int iFileListID );
int ExchangePropertiesID2FileListID(int iPropertiesID);
int ExchangeRFUID2PropertiesID(int iRFUID);
int ExchangePropertiesID2RFUID(int iPropertiesID);
//void _UpdateProperty( CMFCPropertyGridCtrl* cProperty, int iIDInProperties,  CWnd* pFrm = NULL);

void _UpdateProperty(CMFCPropertyGridCtrl* cProperty, int iIDInProperties, ProfileClass* pcFile);
void UpdateProperty( CMFCPropertyGridCtrl* cProperty, int iIDInProperties, ProfileClass* pcFile);
void UpdatePropertyFileStatuse(CMFCPropertyGridCtrl* cProperty);


int __GetPropertyID(CMFCPropertyGridCtrl*cProperty, CMFCPropertyGridProperty*cProp, const int * iIDArray, int iNumber);
int  _GetPropertyID(CMFCPropertyGridCtrl*cProperty, CMFCPropertyGridProperty*cProp);





#define  DEF_NOT_FIND_ARR_FILE -1

//************************************
// Method:    UpdateProperty3GARR
// FullName:  UpdateProperty3GARR
// Access:    public 
// Returns:   int 0:失败;1成功; -1 找不到ARR文件
// Qualifier:
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: ProfileClass * pcFile
//************************************
int UpdateProperty3GARR(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile = NULL);

//************************************
// Method:    UpdateProperty3GARRRecord
// FullName:  UpdateProperty3GARRRecord
// Access:    public 
// Returns:   int 0:失败;1成功; -1 找不到ARR文件
// Qualifier:
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: ProfileClass * pcFile
//************************************
int UpdateProperty3GARRRecord(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile = NULL);

//************************************
// Method:    UpdateProperty3GARRRecordList
// FullName:  UpdateProperty3GARRRecordList
// Access:    public 
// Returns:   int 1成功; -1 找不到ARR文件
// Qualifier:
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: ProfileClass * pcFile
//************************************
int UpdateProperty3GARRRecordList(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile = NULL);


//************************************
// Method:    UpdatePropertyFileRoutes
// FullName:  UpdatePropertyFileRoutes
// Access:    public 
// Returns:   int :Routes number
// Qualifier:
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: ProfileClass * pcFile
//************************************
int UpdatePropertyFileRoutes    (CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile);

//************************************
// Method:    UpdateProperty2GAC
// FullName:  UpdateProperty2GAC
// Access:    public 
// Returns:   int 0:失败;1成功; -1 找不到ARR文件
// Qualifier:
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: ProfileClass * pcFile
//************************************
int UpdateProperty2GAC          (CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile);



//************************************
// Method:    UpdateProperty2GACList
// FullName:  UpdateProperty2GACList
// Access:    public 
// Returns:   int  1成功; -1 找不到ARR文件
// Qualifier:
// Parameter: CMFCPropertyGridCtrl * cProperty
// Parameter: ProfileClass * pcFile
//************************************
int UpdateProperty2GACList  (CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile);
int UpdateProperty2GACRecord(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile);


void UpdatePropertyOption(CMFCPropertyGridCtrl* cProperty, ProfileClass* pcFile);




//************************************
// Method:    AppendOneNew2GAC
// FullName:  AppendOneNew2GAC
// Access:    public 
// Returns:   int 成功:记录号 >0; 不生成:FALSE ;没有找到csPath文件:-1; 生成AC失败: -2 ;写入文件存在错误:-11
// Qualifier:
// Parameter: CString csPath
// Parameter: int iRead
// Parameter: int iUpdate
// Parameter: int iIncreat
// Parameter: int iRFM
// Parameter: int iReha
// Parameter: int iInval
// Parameter: ProfileClass * pcFile
//************************************
int AppendOneNew2GAC(CString csPath, int iRead, int iUpdate, int iIncreat, int iRFM, int iReha, int iInval, ProfileClass* pcFile);

//************************************
// Method:    AppendOneNew3GAC
// FullName:  AppendOneNew3GAC
// Access:    public 
// Returns:   int 成功:记录号 >0; 不生成:FALSE ;没有找到csPath文件:-1; 生成ARR失败: -2 ;写入文件存在错误:-11
// Qualifier:
// Parameter: CString csPath
// Parameter: int iRead
// Parameter: int iUpdate
// Parameter: int iIncreat
// Parameter: int iReha
// Parameter: int iInval
// Parameter: ProfileClass * pcFile
//************************************
int AppendOneNew3GARR(CString csPath, int iRead, int iUpdate, int iIncreat, int iReha, int iInval, ProfileClass* pcFile);


