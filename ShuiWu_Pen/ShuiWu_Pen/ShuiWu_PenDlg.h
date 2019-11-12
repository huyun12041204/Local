
// ShuiWu_PenDlg.h: 头文件
//

#pragma once


// CShuiWuPenDlg 对话框
class CShuiWuPenDlg : public CDialogEx
{
// 构造
public:
	CShuiWuPenDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHUIWU_PEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPesonButton();
	CComboBox m_pReader;
	CPcscReader PcscReader;
	void InitCombo();
	void InitEdit();
	int ConnectReader_PCSC(CString csName);
	BOOL CommandIsSuccess();
	BOOL SendCommand(CString csSend, CString& csResp, CString& csSW);
	void PrintText(CString csText);
	CEdit m_pEdit;
	CButton m_p3MNC;
	afx_msg void OnBnClickedVerifyButton();
};
