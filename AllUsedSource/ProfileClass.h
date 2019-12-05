#pragma once

#include "PrivateCardFileStruct.h"

#ifndef DEF_ProfileClass
#define DEF_ProfileClass
#define  DEF_PROFILE_SUCCESS      00
#define  DEF_PROFILE_UNNECESSARY  01

#define DEF_RESULT_NORMAL        "0-->"
#define DEF_RESULT_HINT          "1-->"
#define DEF_RESULT_WARN          "2-->"
#define DEF_RESULT_ERROR         "3-->"
#define DEF_RESULT_RIGHT         "4-->"

#define DEF_NORMAL_TAG       00
#define DEF_HINT_TAG         01
#define DEF_WARN_TAG         02
#define DEF_ERROR_TAG        03
#define DEF_RIGHT_TAG        04
class ProfileClass
{

protected:
	ADODB::_ConnectionPtr m_pConnection;
	ADODB::_RecordsetPtr  m_pDocRecordset;
	ADODB::_RecordsetPtr  m_pFRURecordset;
	ADODB::_RecordsetPtr  m_pModRecordset;
	CListBox* m_pMainwnd;
	CListBox* m_pHintwnd;
	CListBox* m_pAllwnd  ;
	CString csProfileaPath;

	CString GenerateARRPath(CString csFID,CString csRoute);

	//************************************
	// Method:    _ConnecteAccess
	// FullName:  ProfileClass::_ConnecteAccess
	// Access  :  protected 
	// Returns :  int 
	//                   TRUE,   成功
	//                   FALSE,  打开文件失败,或者建立ACCESS连接失败。
	//                   -0x11,  打开AllFiles页失败。
	//                   -0x12,  打开FRU     页失败。
	//                   -0x13,  打开MODULE  页失败。
	// Qualifier:
	// Parameter: const CString & csPath 
	//************************************
	int  _ConnecteAccess     (const CString& csPath);
	//************************************
	// Method:    _CreateConnection
	// FullName:  ProfileClass::_CreateConnection
	// Access:    protected 
	// Returns:   BOOL  
	//            TRUE,   成功
	//            FALSE   失败
	// Qualifier:
	// Parameter: const CString & csConnstr
	//************************************
	BOOL _CreateConnection   (const CString &csConnstr);
	//************************************
	// Method:    _CreateRecordset
	// FullName:  ProfileClass::_CreateRecordset
	// Access:    protected 
	// Returns:   BOOL
	//            TRUE,   成功
	//            FALSE   失败
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: CString csFrom
	//************************************
	BOOL _CreateRecordset    (ADODB::_RecordsetPtr& m_pRecord, CString csFrom);
	//************************************
	// Method:    _DisconnectAccess
	// FullName:  ProfileClass::_DisconnectAccess
	// Access:    protected 
	// Returns:   BOOL
	//            TRUE,   成功
	//            FALSE   失败
	// Qualifier:
	//************************************
	BOOL _DisconnectAccess   ();

	//************************************
	// Method:    _DisconnectRecordset
	// FullName:  ProfileClass::_DisconnectRecordset
	// Access:    protected 
	// Returns:   void
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	//************************************
	void _DisconnectRecordset(ADODB::_RecordsetPtr& m_pRecord);


	//************************************
	// Method:    _GetRecordSubData
	// FullName:  ProfileClass::_GetRecordSubData
	// Access:    protected 
	// Returns:   long
	//            -1 : 表示数据格式 不为WChar，int ， 长的二进制数据
	//             0 : 成功 数据为 WChar或者int，可以通过通过查varOutput的格式来判断具体
	//            〉0: 成功 数据为 长的二进制数据，
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: long nItem
	// Parameter: _variant_t & varOutput
	//************************************
	long _GetCurrentRecordSubData (ADODB::_RecordsetPtr& m_pRecord, long nItem, _variant_t& varOutput);
	

	//************************************
	// Method:    _GetRecordSubData
	// FullName:  ProfileClass::_GetRecordSubData
	// Access:    protected 
	// Returns:   long
	//            -1 : 表示数据格式 不为WChar，int ， 长的二进制数据
	//             0 : 成功 数据为 WChar或者int，可以通过通过查varOutput的格式来判断具体
	//            〉0: 成功 数据为 长的二进制数据，
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: long nRecord
	// Parameter: long nItem
	// Parameter: _variant_t & varOutput
	//************************************
	long _GetRecordSubData(ADODB::_RecordsetPtr& m_pRecord, long nRecord, long nItem, _variant_t& varOutput);


public:
	ProfileClass();
	~ProfileClass();

	BOOL IsValid();

	void InstallOutput( CWnd* m_pMain, CWnd* m_pHint, CWnd* m_pAll);

	void OutputToWnd(UINT uiTAG,CString csText);

	void DisplayAccessError(_com_error e);
	//************************************
	// Method:    LoadFile
	// FullName:  ProfileClass::LoadFile
	// Access:    public 
	// Returns:   int TRUE ,成功； 2 已经打开当前文件
	// Qualifier: see ConnectProfile
	// Parameter: const CString csPath
	//************************************
	int LoadFile(const CString csPath);

	//************************************
	// Method:    ConnectProfile
	// FullName:  ProfileClass::ConnectProfile
	// Access:    public 
	// Returns:   int
	// Qualifier: 建立连接，并且打开MDB文件，且同时打开三个分页
	//            返回值见 _ConnecteAccess
	//************************************
	int ConnectProfile();



	//************************************
	// Method:    DisconnectProfile
	// FullName:  ProfileClass::DisconnectProfile
	// Access:    public 
	// Returns:   int
	// Qualifier: 
	//************************************
	int DisconnectProfile();
	//************************************
   // Method:    GetDocRecordCount
   // FullName:  ProfileClass::GetDocRecordCount
   // Access:    public 
   // Returns:   long
   // Qualifier: 返回一共有多少条记录
   //************************************
	long GetDocRecordCount();
	long GetModuleRecordCount();
	long GetModuleValidRecordCount();
	long DeleteModuleInvalidRecord();
	//************************************
	// Method:    GetProfilePath
	// FullName:  ProfileClass::GetProfilePath
	// Access:    public 
	// Returns:   CString
	// Qualifier:
	//************************************
	CString GetProfilePath();
	//************************************
	// Method:    GetFileRecord
	// FullName:  ProfileClass::GetFileRecord
	// Access:    public 
	// Returns:   long 0:没有找到, >0 记录号为结果, -1 获取数据错误
	// Qualifier:
	// Parameter: CString csPath
	//************************************
	long GetFileRecord(CString csPath);


	//************************************
// Method:    _GetFileFromTable
// FullName:  ProfileClass::_GetFileFromTable
// Access:    public 
	// Returns:   int 
	//            成功返回 0
	//            失败返回 〉0 ， 返回数值为出错总数，
	//                     -1  ,  没有找寻到 nRecord相对应的文件
// Qualifier:
// Parameter: ADODB::_RecordsetPtr & m_pRecord
// Parameter: long nRecord
// Parameter: PCFS * pcfsFile
// Parameter: UINT * uiFails
//************************************
	int _GetFileFromTable(ADODB::_RecordsetPtr& m_pRecord, long nRecord, PCFS* pcfsFile, BOOL brRelative = TRUE, UINT *uiFails = NULL /*== NULL*/);


	//************************************
	// Method:    _GetSimpleFileFromTab
	// FullName:  ProfileClass::_GetSimpleFileFromTab
	// Access:    public 
	// Returns:   int 成功 0 ，错误 -1 ，存在错误>0
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: PCFS * pcfsFile
	//************************************
	int _GetSimpleFileFromTab(ADODB::_RecordsetPtr& m_pRecord, PCFS* pcfsFile);

	//************************************
	// Method:    GetSimpleFileFromDocFast
	// FullName:  ProfileClass::GetSimpleFileFromDocFast
	// Access:    public 
	// Returns:   int 成功 0 ，错误 -1 ，存在错误>0
	// Qualifier: 此处存在错误风险，没有做判断直接选择了 MOVENEXT
	// Parameter: bool bFirst
	// Parameter: PCFS * pcfsFile
	//************************************
	int GetSimpleFileFromDocFast(bool bFirst, PCFS* pcfsFile);
	//************************************
	// Method:    GetFileFromDoc
	// FullName:  ProfileClass::GetFileFromDoc
	// Access:    public 
	// Returns:   int 
	//            成功返回 0
	//            失败返回 〉0 ， 返回数值为出错总数，
	//                     -1  ,  没有找寻到 nRecord相对应的文件
	// Qualifier:
	// Parameter: long nRecord
	// Parameter: PCFS * pcfsFile
	// Parameter: UINT * uiList 默认为NULL，出错的SubItem 
	//************************************
	int GetFileFromDoc(long nRecord, PCFS* pcfsFile, BOOL brRelative = TRUE, UINT *uiFails = NULL);

	int GetFileFromModule(long nRecord, PCFS* pcfsFile, BOOL brRelative = TRUE, UINT *uiFails = NULL);
	//************************************
	// Method:    GetFileFromDocByPath
	// FullName:  ProfileClass::GetFileFromDocByPath
	// Access:    public 
	// Returns:   int
	//            成功返回 0
	//            成功但是部分数据有错误返回 〉0 ， 返回数值为出错总数，
	//            失败返回-1   
	// Qualifier:
	// Parameter: CString csPath
	// Parameter: PCFS * pcfsFile
	// Parameter: UINT * uiList
	//************************************
	int GetFileFromDoc(CString csPath, PCFS* pcfsFile, UINT *uiFails = NULL);

	//************************************
	// Method:    GetRecordSubDataFromDoc
	// FullName:  ProfileClass::GetRecordSubDataFromDoc
	// Access:    public 
	// Returns:   long
	//            -1 : 表示数据格式 不为WChar，int ， 长的二进制数据
	//             0 : 成功 数据为 WChar或者int，可以通过通过查varOutput的格式来判断具体
	//            〉0: 成功 数据为 长的二进制数据，
	// Qualifier: 
	// Parameter: long nRecord
	// Parameter: long nItem
	// Parameter: _variant_t & varOutput
	//************************************
	long GetRecordSubDataFromDoc(long nRecord, long nItem, _variant_t& varOutput);



	//************************************
	// Method:    GetRecordFileData
	// FullName:  ProfileClass::GetRecordFileData
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: CString csPath
	// Parameter: int * iRecord
	// Parameter: int * iRecordLength
	// Parameter: CStringArray * csRecords
	//************************************
	BOOL GetRecordFileData(CString csPath, int* iRecord = NULL, int* iRecordLength = NULL, CStringArray* csRecords = NULL);

	//************************************
	// Method:    RetrieveARRFile
	// FullName:  ProfileClass::RetrieveARRFile
	// Access:    public 
	// Returns:   BOOL  
	// Qualifier:
	// Parameter: CString csFID    ARR文件 FID 例如 2F06 6F06
	// Parameter: CString csRoute  当前文件目录  例如 3F00， 3F007F10
	// Parameter: int * iRecord
	// Parameter: CStringArray * csRecords
	//************************************
	BOOL RetrieveARRFile(CString csFID, CString csRoute, int* iRecord = NULL, CStringArray* csRecords = NULL);




	UINT GetDFList(CStringArray& csDF, BOOL bADF = TRUE, BOOL bMF = FALSE);

	//************************************
	// Method:    _SetCurrentRecordSubData
	// FullName:  ProfileClass::_SetCurrentRecordSubData
	// Access:    public 
	// Returns:   int TRUE or FLASE
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: long nItem
	// Parameter: _variant_t & varOutput
	//************************************
	int _SetCurrentRecordSubData(ADODB::_RecordsetPtr& m_pRecord, long nItem, _variant_t varInput);


	//************************************
	// Method:    _SetRecordSubData
	// FullName:  ProfileClass::_SetRecordSubData
	// Access:    public 
	// Returns:   int TRUE or FLASE
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: long nRecord
	// Parameter: long nItem
	// Parameter: _variant_t varOutput
	//************************************
	int _SetRecordSubData(ADODB::_RecordsetPtr& m_pRecord, long nRecord, long nItem, _variant_t varOutput);

	
	//************************************
	// Method:    _SelectRecord
	// FullName:  ProfileClass::_SelectRecord
	// Access:    public 
	// Returns:   BOOL TURE :成功, FALSE:失败
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: long nRecord
	// Parameter: BOOL brRelative FALSE 使用绝对的路径查找
	//************************************
	BOOL _SelectRecord(ADODB::_RecordsetPtr& m_pRecord, long nRecord, BOOL brRelative = TRUE);

	//************************************
	// Method:    _DeleteCurrentRecord
	// FullName:  ProfileClass::_DeleteCurrentRecord
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	//************************************
	BOOL _DeleteCurrentRecord(ADODB::_RecordsetPtr& m_pRecord);
	//************************************
	// Method:    _DeleteRecord
	// FullName:  ProfileClass::_DeleteRecord
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: long lRecord
	//************************************
	BOOL _DeleteRecord(ADODB::_RecordsetPtr& m_pRecord, long lRecord);
	//************************************
	// Method:    DeleteDocRecord
	// FullName:  ProfileClass::DeleteDocRecord
	// Access:    public 
	// Returns:   BOOL
	// Qualifier: 删除并排列，使用完之后最好使用检查一遍HANDLE
	// Parameter: long lRecord
	//************************************
	BOOL DeleteDocRecord(long lRecord);


	//************************************
	// Method:    _SetFileToTable
	// FullName:  ProfileClass::_SetFileToTable
	// Access:    public 
	// Returns:   int  -1  ,  没有找寻到 nRecord相对应的文件
	//                  0  , 成功没有错误
	//                》0  ，成功但是存在 n个错误，错误数为返回的结果，
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	// Parameter: long nRecord
	// Parameter: PCFS pcfsFile
	// Parameter: UINT * uiFails
	//************************************
	int _SetFileToTable(ADODB::_RecordsetPtr& m_pRecord, long nRecord, PCFS pcfsFile, BOOL brRelative = TRUE, UINT *uiFails = NULL/*== NULL*/);
	//************************************
	// Method:    SetFileFromDoc
	// FullName:  ProfileClass::SetFileFromDoc
	// Access:    public 
	// Returns:   int  -1  ,  没有找寻到 nRecord相对应的文件
	//                  0  , 成功没有错误
	//                》0  ，成功但是存在 n个错误，错误数为返回的结果，
	// Qualifier:
	// Parameter: long nRecord
	// Parameter: PCFS pcfsFile
	// Parameter: UINT * uiList
	//************************************
	int SetFileToDoc(long nRecord, PCFS pcfsFile, BOOL brRelative = TRUE, UINT *uiFails= NULL /*== NULL*/);
	int SetFileToModule(long nRecord, PCFS pcfsFile, BOOL brRelative = TRUE, UINT *uiFails = NULL /*== NULL*/);
	//************************************
	// Method:    SetFileSubDataListToDoc
	// FullName:  ProfileClass::SetFileSubDataListToDoc
	// Access:    public 
	// Returns:   int 0 ,成功没有失败选项,>0 存在失败选项
	// Qualifier:
	// Parameter: long nRecord
	// Parameter: PCFS pcfsFile
	// Parameter: UINT iSubDataNumber
	// Parameter: UINT * iSubDataList
	//************************************
	int SetFileSubDataListToDoc(long nRecord, PCFS pcfsFile, UINT iSubDataNumber, UINT * iSubDataList, UINT *uiFails = NULL);


	int _AddNewFileToTable(ADODB::_RecordsetPtr& m_pRecord, PCFS pcfsFile);
	int AddNewFileToDoc(PCFS pcfsFile);
	int AddNewFileToModule(PCFS pcfsFile);
	//************************************
	// Method:    ChangeRecordNumberOnly
	// FullName:  ProfileClass::ChangeRecordNumberOnly
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: long lOriNumber
	// Parameter: long lDesNumber
	//************************************
	int ChangeRecordNumberOnly(long lOriNumber, long lDesNumber);
	//************************************
	// Method:    SetRecordNumberInDoc
	// FullName:  ProfileClass::SetRecordNumberInDoc
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: long lOriNumber
	// Parameter: long lDesNumber
	//************************************
	int SetRecordNumberInDoc(long lOriNumber, long lDesNumber);


	//************************************
	// Method:    _CurrentRecordIsLinkRecord
	// FullName:  ProfileClass::_CurrentRecordIsLinkRecord
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: ADODB::_RecordsetPtr & m_pRecord
	//************************************
	BOOL _CurrentRecordIsLinkRecord(ADODB::_RecordsetPtr& m_pRecord);
	//************************************
	// Method:    RecordIsLinkRecord
	// FullName:  ProfileClass::RecordIsLinkRecord
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: long lRecord
	// Parameter: CString & csPLinkFile
	//************************************
	BOOL RecordIsLinkRecord(long lRecord, CString& csPLinkFile);
	//************************************
	// Method:    RecordIsLinkFile
	// FullName:  ProfileClass::RecordIsLinkFile
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: long lNumber
	// Parameter: long * lPLinkNumber
	//************************************
	BOOL RecordIsLinkRecord(long lRecord, long* lPLinkNumber);
	//************************************
	// Method:    PostFileToChildLinkFile
	// FullName:  ProfileClass::PostFileToChildLinkFile
	// Access:    public 
	// Returns:   int >0 成功了并且返回个数，0 成功，没有修改，〈0 存在失败，失败个数 绝对值
	// Qualifier:
	// Parameter: PCFS pcfsFile
	//************************************
	int  PostFileToChildLinkFile(PCFS pcfsFile);






	//************************************
	// Method:    GetFileBasicInformation
	// FullName:  ProfileClass::GetFileBasicInformation
	// Access:    public 
	// Returns:   BOOL
	// Qualifier: 用于输出的，返回基本信息
	// Parameter: CString & csText
	//************************************
	BOOL GetFileBasicInformation(ADODB::_RecordsetPtr& m_pRecord, CString& csText);
	//************************************
	// Method:    ComputeProfileAC
	// FullName:  ProfileClass::ComputeProfileAC
	// Access:    public 对0022文件进行编译
	// Returns:   BOOL
	// Qualifier:
	// Parameter: BOOL bRebuilt
	// Parameter: CStringArray * csResult
	//************************************
	int ComputeProfileAC();

	//************************************
	// Method:    ReInput3GACText
	// FullName:  ProfileClass::ReInput3GACText
	// Access:    public 
	// Returns:   int 0 全部成功，〉0表示出错的个数
	// Qualifier:
	//************************************
	int ReInput3GACText();



	//************************************
	// Method:    _ReInpur3GACText
	// FullName:  ProfileClass::_ReInpur3GACText
	// Access:    public 
	// Returns:   int
	//-1; 选择记录出错， 
	//- 2获取3G权限数据出错,
	//-3 ARR参数错误，
	//- 10 打开ARR文件错误,
	//-11 ARR记录条数小于 文件指示的记录，
	//-21,再次选择会当前文件失败 
	// Qualifier:
	// Parameter: long i
	//************************************
	int _ReInpur3GACText(long i);

	//************************************
	// Method:    GetARRFileList
	// FullName:  ProfileClass::GetARRFileList
	// Access:    public 
	// Returns:   int 
	// Qualifier: 获取ARR文件列表 返回为 文件个数,如果为0,则表示没有
	// Parameter: CStringArray & csPathArray
	//************************************
	int GetARRFileList(CStringArray& csPathArray);
	//************************************
	// Method:    ComputeARRFile
	// FullName:  ProfileClass::ComputeARRFile
	// Access:    public 
	// Returns:   int TRUE:表示成功; 其他表示错误;
	// Qualifier:
	// Parameter: CString csPath
	// Parameter: CStringArray * csResult
	//************************************
	int ComputeARRFile(CString csPath );
	//************************************
	// Method:    ComputeProfileARR
	// FullName:  ProfileClass::ComputeProfileARR
	// Access:    public  对所有ARR文件进行重新编译
	// Returns:   int  0 :成功, >0 存在错误,错误文件数为返回值
	// Qualifier:
	// Parameter: CStringArray * csResult
	//************************************
	int ComputeProfileARR();
	//************************************
	// Method:    ComputeProfileAddress
	// FullName:  ProfileClass::ComputeProfileAddress
	// Access:    public  对文件地址进行重新编译
	// Returns:   int 成功:0 , ACCESS操作失败：-1， 写入存在错误：〉0 返回为错误个数 
	// Qualifier:
	// Parameter: BOOL bRebuilt
	// Parameter: CStringArray * csResult
	//************************************
	int ComputeProfileNotLinkAddress();

	//************************************
	// Method:    ComputeProfileLinkAddress
	// FullName:  ProfileClass::ComputeProfileLinkAddress
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: CStringArray * csResult
	//************************************
	int ComputeProfileLinkAddress();
	int ComputeProfileOTA();
	int ComputeProfileFixed();

	CString __BuiltFile(int iHandle = 0, int iType = 0x10, int iStructure = 0x0, int iFID = 0x3f00,
		int iLinkHandle = 0, int iSize = 0, int iAddress = 0, int iStatus = 0x33,
		int iACCRecord = 1, int iRecordLength = 0, int iARRRecords = 0, int iARRLevel = 0, int iSFI = 0);
	CString _BuiltMF(int iHandle, int iPINList, int iFID,
		int iStatus, int iARRRecords, int iARRLevel,
		int iPINNumber, int iDFNumber, int iEFNumber);
	CString _BuiltDF(int iHandle, int iPINList, int iFID,
		int iStatus, int iARRRecords, int iARRLevel,
		int iPINNumber, int iDFNumber, int iEFNumber);
	CString _BuiltADF(int iHandle, int iPINList, int iFID,
		int iStatus, int iARRRecords, int iARRLevel,
		int iPINNumber, int iDFNumber, int iEFNumber, int iAIDRecords);
	CString _BuiltEF_Transparent(int iHandle, int iFID, int iSize, int iAddress,
		int iStatus, int iACCRecord, int iARRRecords,
		int iARRLevel, int iSFI);
	CString _BuiltEF_Liner(int iHandle, int iFID, int iSize, int iAddress,
		int iStatus, int iACCRecord, int iRecordLength,
		int iARRRecords, int iARRLevel, int iSFI);
	CString _BuiltEF_Cyclic(int iHandle,
		int iFID, int iSize, int iAddress,
		int iStatus, int iACCRecord, int iRecordLength,
		int iARRRecords, int iARRLevel, int iSFI);

	CString _BuiltEF_LLINK(int iHandle, 
		int iFID, int iSize, int iAddress, 
		int iLinkHandle, int iStatus, int iACCRecord, 
		int iRecordLength, int iARRRecords, int iARRLevel, int iSFI);

	CString _BuiltEF_CLINK(int iHandle, 
		int iFID, int iSize, int iAddress, 
		int iLinkHandle, int iStatus, int iACCRecord, 
		int iRecordLength, int iARRRecords, int iARRLevel, int iSFI);

	CString _BuiltEF_TLINK(int iHandle, 
		int iFID, int iSize, int iAddress, 
		int iLinkHandle, int iStatus, int iACCRecord,
		int iRecordLength, int iARRRecords, int iARRLevel, int iSFI);

	BOOL _BuiltFile(long nRecord, CString&csFileHead, const int iAddOff);

	BOOL BuiltFile(CString&csFileHead, const int iAddOff);

	BOOL __BuiltContent(ADODB::_RecordsetPtr& m_pRecord, CString& csContent);

	BOOL _BuiltContent(long nRecord, CString& csContent);

	BOOL BuiltContent(CString& csContent);

	BOOL BuiltFlashSize(long*  lSize);
	BOOL BuiltFRU(CString& csFRU, int& iAoff);


	BOOL _BuiltFRU(CString &csFRU,
		int iFileNumber, long lSpace,
		CString csPin1, CString csPuk1,
		CString csPin2, CString csPuk2,
		CString csADM, CString csPS,
		CString csKI,
		int iStaPin1 = 0x33, int iStaPuk1 = 0xAA,
		int iStaPin2 = 0x33, int iStaPuk2 = 0xAA,
		int iStaADM = 0xAA, int iStaPS = 0xAA,
		int iEnablePin1 = 2, int iEnablePin2 = 0,
		int iEx1 = 0, int iEx2 = 1);

	int CheckProfileAddress(BOOL bModify);
	int CheckProfileHandle(BOOL bModify );
	int CheckProfileStructure();
	int CheckDFSFI(CString csRoute );
	int CheckProfileSFI();
	int CheckProfileLinks();
	int CheckProfileSingleFile();
	int CheckDIR2F00();
	int CheckARR();

	int CheckTypeFilesIsExisted(CString csRoute, CString csType, UINT iType, UINT uiNumber);
	int CheckPBRRecord(CString csRoute, CString csRecord, UINT uiNumber);
	int CheckPBR(CString csRoute);



	int GetFRUData(int iID, _variant_t& __data);
	int SetFRUData(int iID, _variant_t __data);
};


//int CreateMDBFile(CString csPath);
//int CreateTab(CString csPath, CString csTab);
#endif
