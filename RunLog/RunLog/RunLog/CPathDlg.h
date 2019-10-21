#pragma once


// CPathDlg 对话框

class CPathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPathDlg)

public:
	CPathDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPathDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PathDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeMfceditbrowse1();
	// //LOG目录
	CMFCEditBrowseCtrl m_pEditBrowse;
	afx_msg void OnBnClickedOk();
	void SetLogPath(CString csPath);
};
