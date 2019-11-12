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





// ProfileExplorer.h: ProfileExplorer 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "ProfileClass.h"

// CProfileExplorerApp:
// 有关此类的实现，请参阅 ProfileExplorer.cpp
//

class CProfileExplorerApp : public CWinAppEx
{
public:
	CProfileExplorerApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	ProfileClass pcFile;


	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);



	virtual void OnFileNew();
	virtual void OnFileOpen();
	afx_msg void OnAppAbout();

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	DECLARE_MESSAGE_MAP()



};

//************************************
// Method:    IsCardFilesAppPath
// FullName:  IsCardFilesAppPath
// Access:    public 
// Returns:   BOOL TRUE or FALSE
// Qualifier: 通过名字判断，是打开MDB,还是MDB单行
// Parameter: LPCTSTR lpszPathName 打开文件的文件路径
// Parameter: CString * csFilePath 打开文件的文件真实路径
// Parameter: CString * csTitle    打开文件的文件名称
//************************************
BOOL IsCardFilesAppPath(LPCTSTR lpszPathName, CString* csFilePath = NULL, CString* csTitle = NULL);
CString GenerateCardFilesAppPath(CString csProfile, CString csFilePath);
extern CProfileExplorerApp theApp;
extern UINT uiChildViewWide;
extern int iThread;
#pragma once


// CFindFileDialog 对话框

class CFindFileDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFindFileDialog)

public:
	CFindFileDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFindFileDialog();


	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HTREEITEM hItem[100];
	int       iItem[100];
	int       iNumber;
	int       iCurrent;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFindFind();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnBnClickedFindNext();
};