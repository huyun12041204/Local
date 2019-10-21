// InputDataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RunLog.h"
#include "InputDataDlg.h"
#include "RunLogDlg.h"


// CInputDataDlg �Ի���

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


// CInputDataDlg ��Ϣ�������

void CInputDataDlg::InitInputDlg(void)
{
	LONG lStyle;
	lStyle = GetWindowLong(m_ParaList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_ParaList.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_ParaList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
//	dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�

	ListView_SetExtendedListViewStyle(::GetDlgItem 
		(m_hWnd,IDC_LIST3),LVS_EX_FULLROWSELECT | 
		LVS_EX_GRIDLINES); 

    m_edit.ShowWindow(SW_HIDE);//���ر༭��
	m_ParaList.SetExtendedStyle(dwStyle); //������չ���


	m_ParaList.InsertColumn( 0, "Name", LVCFMT_LEFT, 80);
	m_ParaList.InsertColumn( 1, "Data", LVCFMT_LEFT, 300);//������

}

int CInputDataDlg::AddPara2List( CString csName, CString csData)
{

	BOOL bRet;
	int iRow;
    iRow = m_ParaList.InsertItem (0,csName);//������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CInputDataDlg::OnNMDblclkList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	CString csTemp;
	if(pNMListView->iItem!=-1)
	{
		m_row=pNMListView->iItem;//m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
		m_column=pNMListView->iSubItem;//m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������
		m_ParaList.GetSubItemRect(m_row,m_column,LVIR_LABEL,rc);//ȡ������ľ���
		rc.left+=20;
		rc.top+=18;
		rc.right+=16;
		rc.bottom+=20;
		csTemp = m_ParaList.GetItemText(m_row,m_column);
		m_ParaList.SetItemText(m_row,m_column,"");
		m_edit.SetWindowText(csTemp);//�������������ʾ���༭����

		m_edit.ShowWindow(SW_SHOW);//��ʾ�༭��
		m_edit.MoveWindow(&rc);//���༭���ƶ����������棬������������
		m_edit.SetFocus();//ʹ�༭��ȡ�ý���
		m_edit.CreateSolidCaret(1,rc.Height()-5);//����һ�����
		m_edit.ShowCaret();//��ʾ���
		m_edit.SetSel(-1);//ʹ����Ƶ������
	}
	*pResult = 0;
}



void CInputDataDlg::OnEnKillfocusEdit1()
{
		CString str;
	m_edit.GetWindowText(str);//ȡ�ñ༭�������
	m_ParaList.SetItemText(m_row,m_column,str);//�������ݸ��µ�CListCtrl��
	m_edit.ShowWindow(SW_HIDE);//���ر༭��
}
