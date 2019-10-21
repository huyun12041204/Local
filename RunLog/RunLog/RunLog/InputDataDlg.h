#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CInputDataDlg �Ի���

class CInputDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDataDlg)

public:
	CInputDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
// �Ի�������
	enum { IDD = IDD_InputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ParaList;
public:
	void InitInputDlg(void);
public:
	int AddPara2List(CString csName, CString csData);
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnNMDblclkList3(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CEdit m_edit;
	int m_row;
	int m_column;

public:
	afx_msg void OnEnKillfocusEdit1();
};
