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

// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "FilesListView.h"
#include "FilesTreeView.h"
#include "OutputWnd.h"
#include "ReferenceWnd.h"
#include "PropertiesWnd.h"
#include "PrivateCardFileStruct.h"
#include "ProfileClass.h"
#include "ProfileExplorer.h"


#define DEF_STATUEBAR_WORK           1
#define DEF_STATUEBAR_SUBWORK        2
#define DEF_STATUEBAR_RESERVE        3
#define DEF_STATUEBAR_FILE           4

#define DEF_StatueBar_SUBWORK        2

#define  DEF_Mutil_Thread         1


#define Def_Thread_Free           0
#define Def_Thread_Compile        10
#define Def_Thread_Built          20
#define Def_Thread_Check          30
#define Def_Thread_InsertProfile  40 

#define  DEF_Event_Process         1

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员

	HACCEL                      m_hAccel;
	CMFCRibbonBar               m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages           m_PanelImages;
	CMFCRibbonStatusBar         m_wndStatusBar;
	CMFCRibbonStatusBarPane*      m_ElementWork;
	CMFCRibbonStatusBarPane*      m_ElementSubWork;
	CMFCRibbonStatusBarPane*      m_ElementReserve;
	CMFCRibbonStatusBarPane*      m_ElementFile;
//	CMFCRibbonBaseElement*      m_ElementThr;

	CFilesListView              m_wndFilesListView;
	CFilesTreeView              m_wndFilesTreeView;
	COutputWnd                  m_wndOutput;
	CReferenceWnd               m_wndReference;
	CPropertiesWnd              m_wndProperties;
	CFindFileDialog             m_wndFind;


	BOOL b_mFixedARR;
	BOOL b_mFixedAC;

	BOOL b_bRebuiltARR;
	BOOL b_bRebuiltAC;
	BOOL b_bRebuiltOTA;
	BOOL b_bRebuiltAddress;


	BOOL b_cAddress;
	BOOL b_cHandle;
	BOOL b_cStructure;
	BOOL b_cSFI;
	BOOL b_cLinks;
	BOOL b_cCharacteristic;

	BOOL b_cDIR;
	BOOL b_cARR;
	BOOL b_cPBR;
	BOOL b_cSST;
	BOOL b_cUST;
	BOOL b_cCST;
	BOOL b_cPRL;

	BOOL b_c19001F01;
	BOOL b_c19001F04;
	BOOL b_c19001F10;
	BOOL b_c19001F11;

	BOOL b_c0002;
	BOOL b_c0018;
	BOOL b_c0019;
	BOOL b_c001A;
	BOOL b_c0022;
	BOOL b_c0040;

	BOOL b_vFileList;
	BOOL b_vFileTree;
	BOOL b_vOutput;
	BOOL b_vReference;
	BOOL b_vProperty;


	HANDLE hWorkThread;
	HANDLE hStatusThread;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

	afx_msg void OnViewShow(UINT iID);
	afx_msg void OnUpdateViewShow(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange        (UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()



	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	BOOL CreateStatueBar();
	void SetStatueBarWindowIcons(BOOL bHiColorIcons);

public:
	void LoadSetting(int iID);
	void SaveSetting(int iID);
	void OpenReference(CString csPath);
	//************************************
	// Method:    InsertProfile
	// FullName:  CMainFrame::InsertProfile
	// Access:    protected 
	// Returns:   int TRUE 成功，FALSE 失败 
	// Qualifier:
	// Parameter: CString csProfile
	//************************************
	int InsertProfile(BOOL bCurrent = TRUE);
	int InsertFRU(long lSubFile, long lFlashContent);

	int InitParameter();
	afx_msg void OnNewProject();
	afx_msg void OnNew(UINT iID);
	afx_msg void OnNewEf();
	afx_msg void OnNewDf();
	afx_msg void OnNewModule();
	afx_msg void OnModuleSave();
	afx_msg void _OnModuleSave(UINT iID);
	afx_msg void OnUpdateModuleSave(CCmdUI *pCmdUI);
	afx_msg void OnModuleFetch();
	afx_msg void _OnModuleFetch(UINT iID);
	afx_msg	void OnUpdateModuleFetch(CCmdUI *pCmdUI);
	afx_msg	void _OnUpdateModuleFetch(CCmdUI *pCmdUI);
	afx_msg void OnDeleteFile();
	afx_msg void OnFindFind();
	afx_msg void OnFindRepeat();
	afx_msg void OnModifyComfirm();



	afx_msg void OnCheckSetting(UINT iID);
	afx_msg void OnUpdateCheckSetting(CCmdUI* pCmdUI);


	afx_msg void OnImportFiles();

	BOOL __ExportFiles(const CStringArray& csPath, CString csProfile);
	void _ExportFiles(const CStringArray& csPath);

	afx_msg void OnExportFiles();


	int CompileCompile();
	afx_msg void OnCompileCompile();

	void CompileBuilt();
	afx_msg void OnCompileBuilt(UINT iID);


	int CheckCheck();
	afx_msg void OnCheckCheck();


	CDockablePane* GetDockablePane(int iID);
	int CreateWriteFile(CString csTitle, CString &csReuslt);



	static void InsertProfileThread();
	void NewInsertProfileThread();


	static void StatusBarThread();
	void NewStatusThread();



	void NewCompileThread();
	static void CompileThread();



	void NewBuiltThread(UINT iID);
	static void BuiltThreadOnly();
	static void BuiltThread();
	static void BuiltThreadAPDU();


	void NewCheckThread();
	static void CheckThread();

	void _Display2Output(CString csMessage, int iTag = 1);
	void _Display2StatusBar(CString csText, int iIndex = DEF_STATUEBAR_SUBWORK);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnShowMDITabContextMenu(CPoint point, DWORD dwAllowedItems, BOOL bTabDrop);
	afx_msg void OnClostAll();
	afx_msg void OnCloseCurrent();
	afx_msg void OnCloseOther();
};


