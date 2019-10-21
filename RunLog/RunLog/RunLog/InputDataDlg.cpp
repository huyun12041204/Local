// InputDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RunLog.h"
#include "InputDataDlg.h"
#include "RunLogDlg.h"


// CInputDataDlg 对话框

IMPLEMENT_DYNAMIC(CInputDataDlg, CDialog)

CInputDataDlg::CInputDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDataDlg::IDD, pParent)
{

}


void CInputDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_ParaList);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CInputDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInputDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInputDataDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, &CInputDataDlg::OnNMDblclkList3)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CInputDataDlg::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CInputDataDlg 消息处理程序

void CInputDataDlg::InitInputDlg(void)
{
	LONG lStyle;
	lStyle = GetWindowLong(m_ParaList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_ParaList.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_ParaList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
//	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件

	ListView_SetExtendedListViewStyle(::GetDlgItem 
		(m_hWnd,IDC_LIST3),LVS_EX_FULLROWSELECT | 
		LVS_EX_GRIDLINES); 

    m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
	m_ParaList.SetExtendedStyle(dwStyle); //设置扩展风格


	m_ParaList.InsertColumn( 0, "Name", LVCFMT_LEFT, 80);
	m_ParaList.InsertColumn( 1, "Data", LVCFMT_LEFT, 300);//插入列

}

int CInputDataDlg::AddPara2List( CString csName, CString csData)
{

	BOOL bRet;
	int iRow;
    iRow = m_ParaList.InsertItem (0,csName);//插入行

    bRet = m_ParaList.SetItemText(iRow,1,csData);

    return iRow;
}

void CInputDataDlg::OnBnClickedOk()
{
	extern HWND h_SafeHand;
	CRunLogDlg *TempApp;
	CString csTempData,csTempName;
	int iSum;
	TempApp    =  (CRunLogDlg*)(CWnd::FromHandle(h_SafeHand));

	iSum = m_ParaList.GetItemCount();

	for (int i = 0 ; i< iSum ; i++)
	{
		csTempName = m_ParaList.GetItemText(i,0);
		csTempData = m_ParaList.GetItemText(i,1);
		csTempData = DeleteEnterSpace(csTempData);
		if (csTempName == _T("AKey"))
		{
			if (csTempData.GetLength()==16)
				TempApp ->SetAkeyBuffer(csTempData);
		}
		if (csTempName == _T("KIc"))
		{
			if (csTempData.GetLength()==32)
				TempApp ->SetKIcBuffer(csTempData);

		}
		if (csTempName == _T("KID"))
		{
			if (csTempData.GetLength()==32)
				TempApp ->SetKIDBuffer(csTempData);

		}
	}
	OnOK();
}

void CInputDataDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CInputDataDlg::OnNMDblclkList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	CString csTemp;
	if(pNMListView->iItem!=-1)
	{
		m_row=pNMListView->iItem;//m_row为被选中行的行序号（int类型成员变量）
		m_column=pNMListView->iSubItem;//m_column为被选中行的列序号（int类型成员变量）
		m_ParaList.GetSubItemRect(m_row,m_column,LVIR_LABEL,rc);//取得子项的矩形
		rc.left+=20;
		rc.top+=18;
		rc.right+=16;
		rc.bottom+=20;
		csTemp = m_ParaList.GetItemText(m_row,m_column);
		m_ParaList.SetItemText(m_row,m_column,"");
		m_edit.SetWindowText(csTemp);//将子项的内容显示到编辑框中

		m_edit.ShowWindow(SW_SHOW);//显示编辑框
		m_edit.MoveWindow(&rc);//将编辑框移动到子项上面，覆盖在子项上
		m_edit.SetFocus();//使编辑框取得焦点
		m_edit.CreateSolidCaret(1,rc.Height()-5);//创建一个光标
		m_edit.ShowCaret();//显示光标
		m_edit.SetSel(-1);//使光标移到最后面
	}
	*pResult = 0;
}



void CInputDataDlg::OnEnKillfocusEdit1()
{
		CString str;
	m_edit.GetWindowText(str);//取得编辑框的内容
	m_ParaList.SetItemText(m_row,m_column,str);//将该内容更新到CListCtrl中
	m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
}
