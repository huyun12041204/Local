#pragma once
#include "afxwin.h"


// CManageSSDDlg �Ի���

class CManageSSDDlg : public CDialog
{
	DECLARE_DYNAMIC(CManageSSDDlg)

public:
	CManageSSDDlg(CWnd* pParent = NULL);   // ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ManageSSDDlg };

protected:
	int iMod;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCalculatorButton();
public:
	int InitManageSSDDlg(int iDF);

public:
	afx_msg void OnBnClickedGetdataButton();
public:
	CEdit m_Edit;
public:
	afx_msg void OnBnClickedRuncommandButton();
public:
	int Print2Edit(CString csData);
public:
	CString CalComfirmSSD(void);
};
