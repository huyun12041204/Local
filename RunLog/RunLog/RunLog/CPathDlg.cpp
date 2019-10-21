// CPathDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "RunLog.h"
#include "CPathDlg.h"
#include "RunLogDlg.h"
#include "afxdialogex.h"


// CPathDlg 对话框

IMPLEMENT_DYNAMIC(CPathDlg, CDialogEx)

CPathDlg::CPathDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PathDlg, pParent)
{



}

CPathDlg::~CPathDlg()
{
}

void CPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_pEditBrowse);
}


BEGIN_MESSAGE_MAP(CPathDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CPathDlg::OnEnChangeMfceditbrowse1)
	ON_BN_CLICKED(IDOK, &CPathDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPathDlg 消息处理程序


void CPathDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CPathDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	extern HWND h_SafeHand;
	CRunLogDlg* TempApp;
	TempApp = (CRunLogDlg*)(CWnd::FromHandle(h_SafeHand));

	CString csPath;

	GetDlgItemText(IDC_MFCEDITBROWSE1, csPath);
	
	TempApp->SetLogPath(csPath);

	CDialogEx::OnOK();
}

void CPathDlg::SetLogPath(CString csPath)
{
	SetDlgItemText(IDC_MFCEDITBROWSE1, csPath);
}