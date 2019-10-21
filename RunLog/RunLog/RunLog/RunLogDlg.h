// RunLogDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "InputDataDlg.h"
#include "ManageSSDDlg.h"
#include "CPathDlg.h"
//#include "..\PcscReader\PcscReader.h"
#include "..\..\..\AllUsedSource\PcscReader\PcscReader.h"
#include "CCR5.h"
#include "stm_api.h"
#include "Basconv.h"
#include "Markup.h"
#include "Crypto.h"

#define Def_Image_DIR  00
#define Def_Image_File  01
#define Def_Image_Path  02
#define Def_Image_Right 03
#define Def_Image_Wrong 04
#define Def_Image_Arrow 05



#define  Def_PCSC 01
#define  Def_CCR5 02
#define  Def_XXXX 0x0F


#define _LOG_Complete 0

#define _CTLOG_AS   _LOG_Complete
#define _CTLOG_AH   1
#define _CTLOG_ED   2
#define _CTLOG_ES   3
#define _CTLOG_CONT 0x10


#define  _CT_LOG    00
#define  _CMCC_LOG  02
#define  _Catch_LOG 01
#define  _Convertion_LOG 03
#define  _BJ_LOG    04

#define _BJ_ABORT             00
#define _BJ_APDU_HEAD         01
#define _BJ_APDU_SEND_DATA    02
#define _BJ_APDU_RECIEVE_DATA 03
#define _BJ_APDU_SW           04
#define _BJ_RESET             05
#define _BJ_PPS_SEND          06
#define _BJ_PPS_RECIEVE       07
#define _BJ_CONTINUE_DATA   0x10  




// CRunLogDlg 对话框
class CRunLogDlg : public CDialogEx
{
// 构造
public:
	CRunLogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RUNLOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


protected:
	unsigned char ucRand[4];
	unsigned char ucUIMID[0x8];
	unsigned char ucRandSSD[0x7];
	unsigned char ucAKey[0x8];
	unsigned char ucIMSI_M[0xA];
	byte ucKIc[0x10];
	byte ucKID[0x10];
	int iCurFid;
	int iCurDF;
	int iPreIns;
	int iFetchLen;
	int iStatusLen;
	int iGetResponseLen;
	bool bAdj;
	bool bADJ0x82;
	bool bADJ0x10;
	bool bADJ0x12;
	bool bADJ0x14;
	bool bADJ0xC0;
	bool bADJ0xF2;

	bool bADJRes0x32;
	bool bADJRes0x8A;
	bool bADJRes0x84;

	bool bADJRes0xA4;

	bool bADJSTKStartup;

	bool bInSTK;

	bool bInSTKStartup;

	bool b1Step;


	int  iCheckLevel;
	CTime cTimeStart;

	CString csTestLogPath;



protected:
	HICON m_hIcon;
	CPcscReader PcscReader;//读卡器

	int iConSta;
	int iCurReader;
	int iCheckLeve;

	bool bWrongPause;
	bool bWriteLog;
	bool bWriteTime;

	double dfTim;


	int iFileSum;
	int iFileNum;
	int iLogType;

	//为了ＢＪＬＯＧ设置的
	UINT uiPreRet ;

	CString csCurMDir;
	CString csCurLogFile;
	CString csErrFile;

	CRect m_RectSize;


	HANDLE hThread;

	CCCR5 CCR550;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	CRichEditCtrl m_CTLOG_Edit;
	CComboBox m_PCSCReader_Combo;

	CTreeCtrl m_FileTree;
	CProgressCtrl m_Progress;
	CProgressCtrl m_ProgreeGlob;


	
public:
	afx_msg void OnOpenctlog();
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPcscReader();
	afx_msg void OnBnClickedConnectButton();
	afx_msg void OnEnSetfocusCtlogEdit();
	afx_msg void OnAllList();
	afx_msg void OnOpendir();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void On1Script();
	afx_msg void OnMenustop();
	afx_msg void OnMenuparse();
	afx_msg void OnCcr5Reader();
	afx_msg void OnOtherReader();
	afx_msg void OnWritelog();
	afx_msg void OnWrongpause();
	afx_msg void OnUpdatePcscReader(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCcr5Reader(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOtherReader(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWritelog(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWrongpause(CCmdUI *pCmdUI);
	afx_msg void OnAdj0x82();
	afx_msg void OnUpdatePara0x82(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAdj0x82(CCmdUI *pCmdUI);
	afx_msg void OnPara0x82();
	afx_msg void OnUpdateMenuparse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenustop(CCmdUI *pCmdUI);
	afx_msg void OnUpdate1Script(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAllList(CCmdUI *pCmdUI);
	afx_msg void OnMenurestore();
	afx_msg void OnUpdateMenurestore(CCmdUI *pCmdUI);
	afx_msg void OnAdj0x12();
	afx_msg void OnPara0x12();
	afx_msg void OnUpdateAdj0x12(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePara0x12(CCmdUI *pCmdUI);
	afx_msg void OnUimUpdatessd();
	afx_msg void OnCatchlog();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedRestore();
	afx_msg void OnAdj0xf2();
	afx_msg void OnUpdateAdj0xf2(CCmdUI *pCmdUI);
	afx_msg void OnAdj0xc0();
	afx_msg void OnUpdateAdj0xc0(CCmdUI *pCmdUI);
	afx_msg void OnLeve1();
	afx_msg void OnUpdateLeve1(CCmdUI *pCmdUI);
	afx_msg void OnLeve2();
	afx_msg void OnUpdateLeve2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLeve3(CCmdUI *pCmdUI);
	afx_msg void OnLeve3();
	afx_msg void OnLeve4();
	afx_msg void OnUpdateLeve4(CCmdUI *pCmdUI);
	afx_msg void OnLeve5();
	afx_msg void OnUpdateLeve5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpenctlog(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpendir(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCatchlog(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedOnestep();
	afx_msg void OnAdjRes0x84();
	afx_msg void OnUpdateAdjRes0x84(CCmdUI *pCmdUI);
	afx_msg void OnAdjRes0x8a();
	afx_msg void OnUpdateAdjRes0x8a(CCmdUI *pCmdUI);
	afx_msg void OnWriteruntime();
	afx_msg void OnUpdateWriteruntime(CCmdUI *pCmdUI);
	afx_msg void OnAdjRes0xa4();
	afx_msg void OnUpdateAdjRes0xa4(CCmdUI *pCmdUI);
	afx_msg void OnAdjRes0x32();
	afx_msg void OnUpdateAdjRes0x32(CCmdUI *pCmdUI);




	afx_msg void OnAdj0x10();
	afx_msg void OnUpdateAdj0x10(CCmdUI *pCmdUI);
	afx_msg void OnAdj0x14();
	afx_msg void OnUpdateAdj0x14(CCmdUI *pCmdUI);

	afx_msg void OnStkStartup();
	afx_msg void OnUpdateStkStartup(CCmdUI *pCmdUI);






	afx_msg void OnAbout();
	afx_msg void OnOpencmcclog();
	afx_msg void OnOpencmcclist();
	afx_msg void OnConvertionlog();

	afx_msg void OnUpdateOpencmcclog(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpencmcclist(CCmdUI *pCmdUI);

	afx_msg void OnCsimManagessd();

	afx_msg void OnStnClickedCurrentfile();
	afx_msg void OnBnClickedStartcase();
	afx_msg void OnDefaultsetting();



	void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);


protected:

	virtual void OnCancel();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:

	int Init_Control(void);
	int Init_File_Tree(void);
	int Init_PCSCReader_Combo(void);
	int Init_CCR5_Combo(void);
	int Init_Stm_Combo(void);

	int CompareResponse(CString csData);
	int CompareExpectSW(CString csSW);
	int CompareExpectResponse(CString csResponse);
	int CompareData(CString csData1, CString csData2);
	int CompareFCP(CString csExFCP,CString csFCP);
	int CompareRangSW(CString csSW, CString csExpectSW);
	int CompareListSW(CString csSW,CString csExpectSW);

	int Print2Edit(CString csData);

	int TransationData(CString csData);

	int TransationCatchLogFileData(CString csData);

	int TransationLogFileData(CString csData);

	int TransationCMCCLogFileData(CString csData);

	int TransationConvertionLogFileData(CString csData);


	int TransationBJLogFileData(CString csData, UINT uiPreType);

	int GetLogFileList(CString csDIR,HTREEITEM hDIRTreeItem = TVI_ROOT);

	int CurrentStatus(void);


	void _RemoveAllEdit();
	void RunLOGofEdit(HTREEITEM hCurrentItem = NULL);

	void ContrleBusy(BOOL bStatus);

	void SetItemCheck(HTREEITEM hItem, BOOL bCheck);
	void SetChildItemCheck(HTREEITEM hItem, BOOL bCheck);
	void SetParentCheck(HTREEITEM hItem, BOOL bCheck);
	void SetItemFileData2Edit(HTREEITEM hItem);

	void SetCTLog2Edit(CString csFilePath);
    void SetCMCCLog2Edit(CString csFilePath);
	void Addcmcclist(CString csListFilePath,CString csListDir);

    void Put1File2Edit(CString csFileName,CString csFilePath);

	static int ThreadRun(void);
	static int ThreadAllRun(void);

protected:

	CString GetDIR(void);
	CString GenerateRunLogPath(CString csDir,HTREEITEM hCurrentItem);
	CString GenerateRunErrPath(CString csDir);
	BOOL CreateLogFile(CString csFile);

	BOOL CreateErrFile(CString csFile);



	void TRSPCommand(CString csData);
	void PrintLog2File(CString csPrint);
	void PrintErr2File(CString csPrint);
	void PrintCurrentTransation(void);
	void PriteCurrentTime(void);
	void SetStatus(CString csATR);


protected:

	int Init_Progress(int iRange = 1000);
	int Init_ProgressGlob(int iRange);

	void SetProgress(int iCurrent);
	void SetProgressGlob(int iCurrent);

	int StatisticFileNumber(void);
	int StatisticFileNumberCurrentDIR(HTREEITEM hCurrentItem,int iNumber);

	
protected:
	int PostCurrentCommand(void);
	int PretreatCurrentCommand(void);

protected:

	BOOL GetSetting(CString csFile);
	BOOL TransSetting(CString csData);
	BOOL SaveSetting(CString csFile);
	BOOL RebornSetting(CString csData,CString* csNewInit);



protected:
	
	int Connect_PSCS(CString *csATR);
	int Connect_CCR5(CString *csCardATR);
	int Send_CCR5(unsigned char *ucSend,int iSend,unsigned char *ucResp,int *iResLen);
	int Connect_Stm(CString *csATR);
	int Send_Stm(unsigned char *ucSend,int iSend,unsigned char *ucResp,int *iResLen);

public:

	void RunAllList(void);
	void RunCurrentTree(HTREEITEM hCurrentItem);

	int  Run1Script(void);

	bool CheckPrograssStatus(HANDLE hThreadHand);

	CString Run1LineScript(CString csData);

public:
	int SetAkeyBuffer(CString csAkey);
	int SetKIcBuffer(CString csKIc);
	int SetKIDBuffer(CString csKID);
	CString CalaConfirmSSDData(void);
	CString GetProtectedData(int iType);

public:
	void SetLogPath(CString csPath);




public:
	afx_msg void OnUpdateConvertionlog(CCmdUI *pCmdUI);
	afx_msg void OnBjLog();
	afx_msg void OnPathSettingButton();
};