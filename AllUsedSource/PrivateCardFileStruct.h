

#include "ProfileClass.h"

#define  DEF_FILELIST_HANDLE_COLUMN     0
#define  DEF_FILELIST_FID_COLUMN        1
#define  DEF_FILELIST_NAME_COLUMN       2
#define  DEF_FILELIST_ROUTE_COLUMN      3
#define  DEF_FILELIST_TYPE_COLUMN       4
//#define  DEF_FILELIST_TYPE_COLUMN     3

#define  DEF_FILELIST_STRUCTURE_COLUMN           5
#define  DEF_FILELIST_STATUS_COLUMN              6
#define  DEF_FILELIST_SIZE_COLUMN                7
#define  DEF_FILELIST_RECORD_NUMBER_COLUMN       8
#define  DEF_FILELIST_RECORD_LENGTH_COLUMN       9

#define  DEF_FILELIST_2GREAD_COLUMN       10
#define  DEF_FILELIST_2GUPDATE_COLUMN     11
#define  DEF_FILELIST_2GINCREASE_COLUMN   12
#define  DEF_FILELIST_2GRFM_COLUMN        13
#define  DEF_FILELIST_2GREHAB_COLUMN      14
#define  DEF_FILELIST_2GINVAL_COLUMN      15
#define  DEF_FILELIST_2GACC_COLUMN        16

#define  DEF_FILELIST_ADDRESS_COLUMN      17

#define  DEF_FILELIST_3GMODE_COLUMN       18

#define  DEF_FILELIST_3GREAD_COLUMN       19
#define  DEF_FILELIST_3GUPDATE_COLUMN     20
#define  DEF_FILELIST_3GINCREASE_COLUMN   21
#define  DEF_FILELIST_3GREHAB_COLUMN      22
#define  DEF_FILELIST_3GINVAL_COLUMN      23

#define  DEF_FILELIST_3GACC_COLUMN        24
#define  DEF_FILELIST_3GARRFile_COLUMN    25
#define  DEF_FILELIST_3GSFI_COLUMN        26

#define  DEF_FILELIST_CHVNUMBER_COLUMN    27
#define  DEF_FILELIST_EFNUMBER_COLUMN     28

//*************
#define  DEF_FILELIST_CHVKEYDF_COLUMN     29
#define  DEF_FILELIST_FILECHARDF_COLUMN   30
#define  DEF_FILELIST_DFNUMBER_COLUMN     31
//*************
#define  DEF_FILELIST_AID_COLUMN          32

#define  DEF_FILELIST_LINKFILE_COLUMN     33

#define  DEF_FILELIST_FRU1_COLUMN     34
#define  DEF_FILELIST_FRU2_COLUMN     35
#define  DEF_FILELIST_FRU3_COLUMN     36
#define  DEF_FILELIST_FRU4_COLUMN     37

#define  DEF_FILELIST_CONTENT_COLUMN     38



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



#define  DEF_ARRLEVEL_2F06            0
#define  DEF_ARRLEVEL_6F06            1
#define  DEF_ARRLEVEL_4F06            2


#define  DEF_RFU_CHV1      0
#define  DEF_RFU_CHV2      1
#define  DEF_RFU_ADM1      2
#define  DEF_RFU_ADM2      3
#define  DEF_RFU_UNCHV1    4
#define  DEF_RFU_UNCHV2    5
#define  DEF_RFU_KI        6
#define  DEF_RFU_RFU_FILE  7
#define  DEF_RFU_RFU_SPACE 8
#define  DEF_RFU_RFU_FRU1  9
#define  DEF_RFU_RFU_FRU2  10

#define DEF_STK_NONE       0
#define DEF_STK_FIX_APP    1
#define DEF_STK_OTA_APP    2

#define  DEF_FILETYPE_MF  0x10
#define  DEF_FILETYPE_DF  0x20
#define  DEF_FILETYPE_ADF 0x30
#define  DEF_FILETYPE_EF  0x40

#define  DEF_FILESTRUCTURE_TRANSPARENT 0
#define  DEF_FILESTRUCTURE_LINER       1
#define  DEF_FILESTRUCTURE_CYCLIC      3


#define  DEF_FILESTRUCTURE_TLINK     4
#define  DEF_FILESTRUCTURE_LLINK     5
#define  DEF_FILESTRUCTURE_CLINK     7


#define DEF_APPLICAITON_TYPE_NOT    0
#define DEF_APPLICAITON_TYPE_FIXED  1
#define DEF_APPLICAITON_TYPE_OTA    2



#ifndef DEF_PCFS

#define DEF_PCFS 

#define  DEF_EF_Properties  33



int _ExChangeFileStructure(_variant_t cTemp);
int _ExChangeFileType(_variant_t cTemp);
_variant_t _TranslateFileStructure(int iCode);
_variant_t _TranslateFileType(int iCode);
//************************************
// Method:    _Translate2GAC
// FullName:  _Translate2GAC
// Access:    public 
// Returns:   CString
// Qualifier:
// Parameter: char cAC
// Parameter: BOOL bFormat 此变量用来补齐 6字节，如果不足，补空格
//************************************
CString _Translate2GAC(char cAC, BOOL bFormat = FALSE);
CString _Translate3GAC(char cAC);

char _Translate2GACText(CString csText);
char _Translate3GACText(CString csText);
int _TranslateARRFileLevel(CString csFID);
CString _TranslateARRFile(int iLevel);

UINT _TanslateApplicationType(CString csText);
CString _TanslateSTKAPPCode(UINT uiCode);

CString Generate3GARRRecord(int iRead, int iUpdate, int iIncreate, int iInval, int iReha);

CString Generate2GACRecord(int iRead, int iUpdate, int iIncreate, int iRFM, int iInval, int iReha, BOOL bComment = FALSE);

const int EFProperties[33] = {DEF_FILELIST_HANDLE_COLUMN,DEF_FILELIST_FID_COLUMN,DEF_FILELIST_NAME_COLUMN,DEF_FILELIST_ROUTE_COLUMN,DEF_FILELIST_TYPE_COLUMN,
	DEF_FILELIST_STRUCTURE_COLUMN,DEF_FILELIST_STATUS_COLUMN,DEF_FILELIST_SIZE_COLUMN,DEF_FILELIST_RECORD_NUMBER_COLUMN,DEF_FILELIST_RECORD_LENGTH_COLUMN,
	DEF_FILELIST_2GREAD_COLUMN,DEF_FILELIST_2GUPDATE_COLUMN,DEF_FILELIST_2GINCREASE_COLUMN, 
	DEF_FILELIST_2GRFM_COLUMN,DEF_FILELIST_2GREHAB_COLUMN,DEF_FILELIST_2GINVAL_COLUMN,DEF_FILELIST_2GACC_COLUMN,
	DEF_FILELIST_ADDRESS_COLUMN,DEF_FILELIST_3GMODE_COLUMN,
	DEF_FILELIST_3GREAD_COLUMN,DEF_FILELIST_3GUPDATE_COLUMN,DEF_FILELIST_3GINCREASE_COLUMN,DEF_FILELIST_3GREHAB_COLUMN,DEF_FILELIST_3GINVAL_COLUMN,
	DEF_FILELIST_3GACC_COLUMN,DEF_FILELIST_3GARRFile_COLUMN,
	DEF_FILELIST_3GSFI_COLUMN,DEF_FILELIST_LINKFILE_COLUMN,
	DEF_FILELIST_FRU1_COLUMN,DEF_FILELIST_FRU2_COLUMN,DEF_FILELIST_FRU3_COLUMN,DEF_FILELIST_FRU4_COLUMN,
	DEF_FILELIST_CONTENT_COLUMN};
#define  DEF_DF_Properties  20
const int DFProperties[20] = {DEF_FILELIST_HANDLE_COLUMN,DEF_FILELIST_FID_COLUMN,DEF_FILELIST_NAME_COLUMN,DEF_FILELIST_ROUTE_COLUMN,DEF_FILELIST_TYPE_COLUMN,
	DEF_FILELIST_STRUCTURE_COLUMN,DEF_FILELIST_STATUS_COLUMN,
	DEF_FILELIST_3GMODE_COLUMN,
	DEF_FILELIST_3GREAD_COLUMN,DEF_FILELIST_3GUPDATE_COLUMN,DEF_FILELIST_3GINCREASE_COLUMN,DEF_FILELIST_3GREHAB_COLUMN,DEF_FILELIST_3GINVAL_COLUMN,
	DEF_FILELIST_3GACC_COLUMN,DEF_FILELIST_3GARRFile_COLUMN,
	DEF_FILELIST_FRU1_COLUMN,DEF_FILELIST_FRU2_COLUMN,DEF_FILELIST_FRU3_COLUMN,DEF_FILELIST_FRU4_COLUMN,
	DEF_FILELIST_CONTENT_COLUMN};

typedef struct PrivateCardFileStruct
{
	CString stFILENAME;
	CString stROUTE;
	CString stLINKROUTE;
	CString stContent;


	WORD uiHANDLE;
	WORD uiFID;
	BYTE uiTYPE;
	BYTE uiSTRUCT;
	BYTE uiEFSTATUS;
	UINT uiSIZE;
	BYTE uiRECORDNUMBER;
	BYTE uiRECORDLENGTH;
	ULONG ulADDRESS;

	bool  b3G;
	char  i2GAC;
	char  i3GAC;
	BYTE ui3GARRLEVEL;
	BYTE ui3GSFI;

	BYTE uiCHVNUMBER;
	BYTE uiEFNUMBER;
	BYTE uiDFNUMBER;
	BYTE uiDFSTATUS;
	BYTE uiAIDNUMBER;
	WORD uiROUTE;
	WORD uiLINKROUTE;

	WORD FRU1;
	WORD FRU2;
	BYTE FRU3;
	ULONG FRU4;

	char c2GREAD;
	char c2GUPDATE;
	char c2GINCREATE;
	char c2GRFM;
	char c2GREHAB;
	char c2GINVAL;

	char c3GREAD;
	char c3GUPDATE;
	char c3GINCREATE;
	char c3GDEACTIVE;
	char c3GACTIVE;


	_variant_t GetPrivateCardFileStructData(int iCode,BOOL* bUsed = NULL);
	//************************************
	// Method:    SetPrivateCardFileStructData
	// FullName:  PrivateCardFileStruct::SetPrivateCardFileStructData
	// Access:    public 
	// Returns:   int 
	// Qualifier: 合理参数 返回 TRUE; 不合理参数返回 FALSE;
	// Parameter: int iCode
	// Parameter: _variant_t _Input
	//************************************
	int SetPrivateCardFileStructData(int iCode,_variant_t _Input);

	int CreateTestData(int iHandle);
	int CreateBinaryFile(int iHandle);
	int CreateLinfixFile( int iHandle );
	int CreateCyclicFile( int iHandle );
	int EmptyFile();
	int IsEmpty();
	int CreateMF ( int iHandle );
	int CreateDF ( int iHandle );
	int CreateADF( int iHandle );
	int ComparePCFS(PrivateCardFileStruct _file);
	int IsModified(PrivateCardFileStruct _file,int* iModifiedID = NULL);
	int CorrectPCFS(BOOL b3GMode = TRUE,BOOL bContent= TRUE,BOOL bType   = TRUE,
		            BOOL bStruct = TRUE,BOOL bRecord = TRUE);
	int GetErrorNumber(int i2GACMax, int i3GACMax, int iAIDMax,
		               CStringArray& csErrorInformation,    
		               const CStringArray& csRouteArray);
	int CheckParameter(CStringArray& csErrorInformation);
	BOOL IsLinkFile();
	PrivateCardFileStruct &operator=(const PrivateCardFileStruct &other)
	{
		this->stFILENAME   = other.stFILENAME;
		this->stROUTE      = other.stROUTE;
		this->stLINKROUTE  = other.stLINKROUTE;
		this->stContent    = other.stContent ;


		this->uiHANDLE       = other.uiHANDLE  ;
		this->uiFID          = other.uiFID  ;
		this->uiTYPE         = other.uiTYPE  ;
		this->uiSTRUCT       = other.uiSTRUCT  ;
		this->uiEFSTATUS     = other.uiEFSTATUS  ;
		this->uiSIZE         = other.uiSIZE  ;
		this->uiRECORDNUMBER = other.uiRECORDNUMBER  ;
		this->uiRECORDLENGTH = other.uiRECORDLENGTH  ;
		this->ulADDRESS      = other.ulADDRESS  ;

		this->b3G         = other.b3G  ;
		this->i2GAC       = other.i2GAC  ;
		this->i3GAC       = other.i3GAC  ;
		this->ui3GARRLEVEL = other.ui3GARRLEVEL  ;
		this->ui3GSFI      = other.ui3GSFI  ;

		this->uiCHVNUMBER = other.uiCHVNUMBER  ;
		this->uiEFNUMBER  = other.uiEFNUMBER  ;
		this->uiDFNUMBER  = other.uiDFNUMBER  ;
		this->uiDFSTATUS  = other.uiDFSTATUS  ;
		this->uiAIDNUMBER = other.uiAIDNUMBER  ;
		this->uiROUTE     = other.uiROUTE  ;
		this->uiLINKROUTE = other.uiLINKROUTE  ;

		this->FRU1 = other.FRU1  ;
		this->FRU2 = other.FRU2  ;
		this->FRU3 = other.FRU3  ;
		this->FRU4 = other.FRU4  ;

		this->c2GREAD     = other.c2GREAD  ;
		this->c2GUPDATE   = other.c2GUPDATE  ;
		this->c2GINCREATE = other.c2GINCREATE  ;
		this->c2GRFM      = other.c2GRFM  ;
		this->c2GREHAB    = other.c2GREHAB  ;
		this->c2GINVAL    = other.c2GINVAL  ;

		this->c3GREAD     = other.c3GREAD  ;
		this->c3GUPDATE   = other.c3GUPDATE  ;
		this->c3GINCREATE = other.c3GINCREATE  ;
		this->c3GDEACTIVE = other.c3GDEACTIVE  ;
		this->c3GACTIVE   = other.c3GACTIVE  ;

		return *this;
	}
}PCFS;
#endif // _DEBUG