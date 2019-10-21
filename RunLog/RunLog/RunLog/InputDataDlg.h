#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CInputDataDlg 对话框

class CInputDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDataDlg)

public:
	CInputDataDlg(CWnd* pParent = NULL);   // 标准构造函数
// 对话框数据
	enum { IDD = IDD_InputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
