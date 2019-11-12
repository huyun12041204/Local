
// ShuiWu_PenDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ShuiWu_Pen.h"
#include "ShuiWu_PenDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CShuiWuPenDlg 对话框



CShuiWuPenDlg::CShuiWuPenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHUIWU_PEN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShuiWuPenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_READER_COMBOM, m_pReader);
	DDX_Control(pDX, IDC_EDIT5, m_pEdit);
	DDX_Control(pDX, IDC_3MNC_CHECK, m_p3MNC);
}

BEGIN_MESSAGE_MAP(CShuiWuPenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PESON_BUTTON, &CShuiWuPenDlg::OnBnClickedPesonButton)
	ON_BN_CLICKED(IDC_VERIFY_BUTTON, &CShuiWuPenDlg::OnBnClickedVerifyButton)
END_MESSAGE_MAP()


// CShuiWuPenDlg 消息处理程序

BOOL CShuiWuPenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE);

	//ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	InitCombo();
	InitEdit();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CShuiWuPenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CShuiWuPenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CShuiWuPenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShuiWuPenDlg::InitCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	
	int iSum;
	LPTSTR    szReaderName[10];
    iSum = PcscReader.GetContext();
	iSum = PcscReader.GetPcsclist(szReaderName);
	if (iSum < 1)
		return ;

	for (int i = 0; i < iSum; i++)
		m_pReader.AddString(szReaderName[i]);

	m_pReader.SetCurSel(0);
}

void CShuiWuPenDlg::InitEdit()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemText(IDC_ICCID_EDIT, "98680000000000000000");
	SetDlgItemText(IDC_IMSI_EDIT, "084900000000000000");
	SetDlgItemText(IDC_KI_EDIT, "10000000000000000000000000000001");
	SetDlgItemText(IDC_OPC_EDIT, "20000000000000000000000000000002");
}

int CShuiWuPenDlg::ConnectReader_PCSC(CString csName)
{
	int iRet;
	LPTSTR  szReaderName[10];
	iRet = PcscReader.GetContext();
	iRet = PcscReader.GetPcsclist(szReaderName);
	iRet = PcscReader.ConnectReader((LPTSTR)(LPCTSTR)csName);

	return iRet;
}
void CShuiWuPenDlg::PrintText(CString csText)
{
	CString csPre;
	
	GetDlgItemText(IDC_EDIT5, csPre);
	SetDlgItemText(IDC_EDIT5, csPre + csText + "\r\n");

}

BOOL CShuiWuPenDlg::CommandIsSuccess()
{
	BYTE SW[2];
	PcscReader.GetLastSW(SW);

	if ((SW[0] == 0x90) &&(SW[1] == 0x00))
	{
		return TRUE;
	}
	else if (SW[0] == 0x61)
		return TRUE;
	else if (SW[0] == 0x9F)
		return TRUE;
	else if (SW[0] == 0x91)
		return TRUE;

	return FALSE;
}

BOOL CShuiWuPenDlg::SendCommand(CString csSend, CString&csResp,CString& csSW)
{
	BYTE SEND[300];
	BYTE RESP[255];
	BYTE SW[2];


	_DeleteEnterSpace(csSend);
	int iRESP = 0;
	int iSEND = _CString2UcHex(csSend, SEND);

	PcscReader.SendCommand(SEND, iSEND, RESP, &iRESP);

	iRESP = PcscReader.GetLastResponse(RESP);

	_UcHex2CString(RESP, iRESP, csResp);

	PcscReader.GetLastSW(SW);

	_UcHex2CString(SW,2, csSW);

	return CommandIsSuccess();


}

void CShuiWuPenDlg::OnBnClickedPesonButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_pReader.GetCurSel();
	CString csReader;
	GetDlgItemText(IDC_READER_COMBOM, csReader);
	SetDlgItemText(IDC_EDIT5, "");

	if (ConnectReader_PCSC(csReader) != 0)
	{
		MessageBox("连接读卡器失败!");
		return;
	}
	else
		PrintText("连接读卡器成功!");

	BYTE ucATR[50];
	int iATR = PcscReader.GetCardATR(ucATR);

	if (iATR<=2)
	{
		MessageBox("连接卡片失败!");
		return;
	}
	else
		PrintText("连接卡片成功!");

	BYTE VerifyPS[20] = { 0x00,0x17,0x00,0x01,0x08,0x36 ,0x35,0x36 ,0x33 ,0x30 ,0x39 ,0x36 ,0x39 };
	BYTE RES[20];
	int iRespLen = 0;
	int iRet = PcscReader.SendCommand(VerifyPS,13, RES,&iRespLen);

	if (!CommandIsSuccess())
	{
		MessageBox("安全认证失败!");
		return;
	}
	else
		PrintText("安全认证成功!");
	CString csRESP, csSW;

	 if (!SendCommand(_T("00A4000C023F00"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 01!");
		 return;
	 }


	 if (!SendCommand(_T("00A4000C022FE2"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 02!");
		 return;
	 }

	 if (!SendCommand(_T("00B000000A"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 03!");
		 return;
	 }
	 //if (csRESP.Compare("FFFFFFFFFFFFFFFFFFFF")!=0)
	 //{
		// MessageBox("卡片已经个人化!");
		// return;
	 //}
	 CString csICCID;

	 GetDlgItemText(IDC_ICCID_EDIT, csICCID);

	 _DeleteEnterSpace(csICCID);
	 if (csICCID.GetLength() != 20)
	 {
		 MessageBox("ICCID 长度不正确!");
		 return;
	 }


	 if (!SendCommand(_T("00D600000A")+csICCID, csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 04!");
		 return;
	 }
	 else
		 PrintText("写入卡片" + csICCID + _T("的 ICCID 成功!"));


	 if (!SendCommand(_T("00A4000C027F20"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 11!");
		 return;
	 }


	 if (!SendCommand(_T("00A4000C026F07"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 12!");
		 return;
	 }


	 CString csIMSI;
	 GetDlgItemText(IDC_IMSI_EDIT, csIMSI);
	 _DeleteEnterSpace(csIMSI);
	 if (csIMSI.GetLength() != 18)
	 {
		 MessageBox("IMSI 长度不正确!");
		 return;
	 }
	 if (!SendCommand(_T("00D6000009") + csIMSI, csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 13!");
		 return;
	 }
	 else
		 PrintText("写入卡片" + csICCID + _T("的 GSM/IMSI 成功!"));


	 if (!SendCommand(_T("00A4000C027FF0"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 14!");
		 return;
	 }


	 if (!SendCommand(_T("00A4000C026F07"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 15!");
		 return;
	 }


	 //CString csIMSI;
	 //GetDlgItemText(IDC_IMSI_EDIT, csIMSI);
	 //_DeleteEnterSpace(csIMSI);
	 if (csIMSI.GetLength() != 18)
	 {
		 MessageBox("IMSI 长度不正确!");
		 return;
	 }
	 if (!SendCommand(_T("00D6000009") + csIMSI, csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 16!");
		 return;
	 }
	 else
		 PrintText("写入卡片" + csICCID + _T("的 LTE/IMSI 成功!"));

	 CString csKI;
	 GetDlgItemText(IDC_KI_EDIT, csKI);
	 _DeleteEnterSpace(csKI);
	 if (csKI.GetLength() != 32)
	 {
		 MessageBox("KI 长度不正确!");
		 return;
	 }
	 if (!SendCommand(_T("0015000010") + csKI, csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 21!");
		 return;
	 }
	 else
		 PrintText("写入卡片" + csICCID + _T("的 KI 成功!"));



	 if (!SendCommand(_T("00A4000C020040"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 22!");
		 return;
	 }
	 CString csOPC;
	 GetDlgItemText(IDC_OPC_EDIT, csOPC);
	 _DeleteEnterSpace(csOPC);

	 if (csKI.GetLength() != 32)
	 {
		 MessageBox("OPC 长度不正确!");
		 return;
	 }
	 if (!SendCommand(_T("00D600001B00") + csOPC + _T("40002040600001020408"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 23!");
		 return;
	 }
	 else
		 PrintText("写入卡片" +csICCID + _T("的 OPC 成功!"));
		


	 if (!SendCommand(_T("00A4000C026FAD"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 31!");
		 return;
	 }

	 CString csMNCLEN;

	 if (m_p3MNC.GetCheck())
		 csMNCLEN = _T("03");
	 else
		 csMNCLEN = _T("02");


	 if (!SendCommand(_T("00D6000301") + csMNCLEN, csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 32!");
		 return;
	 }
	 else
	 {
		 PrintText("写入卡片" + csICCID + _T("的 MNC长度 成功!"));
	 }


	 if (!SendCommand(_T("00A4000C026F46"), csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 41!");
		 return;
	 }

	 if (!SendCommand(_T("00D600001100436F6D7541736961FFFFFFFFFFFFFFFF") , csRESP, csSW))
	 {
		 MessageBox("卡片交互失败 42!");
		 return;
	 }
	 else
	 {
		 PrintText("写入卡片" + csICCID + _T("的 SPN 成功!"));
	 }


	
	



}


void CShuiWuPenDlg::OnBnClickedVerifyButton()
{
	// TODO: 在此添加控件通知处理程序代码

	CString csReader;
	GetDlgItemText(IDC_READER_COMBOM, csReader);
	SetDlgItemText(IDC_EDIT5, "");

	if (ConnectReader_PCSC(csReader) != 0)
	{
		MessageBox("连接读卡器失败!");
		return;
	}
	else
		PrintText("连接读卡器成功!");

	BYTE ucATR[50];
	int iATR = PcscReader.GetCardATR(ucATR);

	if (iATR <= 2)
	{
		MessageBox("连接卡片失败!");
		return;
	}
	else
		PrintText("连接卡片成功!");

	BYTE VerifyPS[20] = { 0x00,0x17,0x00,0x01,0x08,0x36 ,0x35,0x36 ,0x33 ,0x30 ,0x39 ,0x36 ,0x39 };
	BYTE RES[20];
	int iRespLen = 0;
	int iRet = PcscReader.SendCommand(VerifyPS, 13, RES, &iRespLen);
	CString csRESP, csSW;
	if (!CommandIsSuccess())
	{
		MessageBox("安全认证失败!");
		return;
	}
	else
		PrintText("安全认证成功!");
	 //开始校验数据 
	PrintText("开始 校验数据----->");

	if (!SendCommand(_T("00A4000C023F00"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 51!");
		return;
	}


	if (!SendCommand(_T("00A4000C022FE2"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 52!");
		return;
	}

	if (!SendCommand(_T("00B000000A"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 53!");
		return;
	}
	else
	{
		PrintText("卡片 ICCID:" + csRESP);

	}

	if (!SendCommand(_T("00A4000C027FF0"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 61");
		return;
	}


	if (!SendCommand(_T("00A4000C026F07"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 62!");
		return;
	}

	if (!SendCommand(_T("00B0000009"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 63!");
		return;
	}
	else
	{
		PrintText("卡片 IMSI:" + csRESP);
	}


	if (!SendCommand(_T("00A4000C026FAD"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 71!");
		return;
	}

	if (!SendCommand(_T("00B0000301"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 72!");
		return;
	}
	else
	{
		PrintText("卡片 MNC 长度:" + csRESP);
	}

	if (!SendCommand(_T("00A4000C026F46"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 71!");
		return;
	}

	if (!SendCommand(_T("00B0000011"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 72!");
		return;
	}
	else
	{
		PrintText("卡片 SPN:" + csRESP);
	}



	CString csKI, csOPC;

	GetDlgItemText(IDC_KI_EDIT, csKI);

	GetDlgItemText(IDC_OPC_EDIT, csOPC);

	if (!SendCommand(_T("00A4000C020040"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 81!");
		return;
	}

	if (!SendCommand(_T("00B0000110"), csRESP, csSW))
	{
		MessageBox("卡片交互失败 82!");
		return;
	}
	else
	{
		csOPC.MakeUpper();
		csRESP.MakeUpper();
		if (csRESP.Compare(csOPC)!=0)
		{
			PrintText("校验卡片 OPC:" + csOPC + _T("  失败!"));
		}
		else
			PrintText("校验卡片 OPC:" + csOPC + _T("  成功!"));
	}


	if (csKI.GetLength()!=32)
	{
		MessageBox("KI 长度不正确!");
		return;
	}

	if (!SendCommand(_T("0019000010")+csKI, csRESP, csSW))
	{
		//MessageBox("卡片交互失败 71!");
		PrintText("校验卡片 KI:" + csKI + _T("  失败!"));
		return;
	}
	else
	{
		if (csSW!= _T("9000"))
		{
			PrintText("校验卡片 KI:" + csKI + _T("  失败!"));
		}
		else
			PrintText("校验卡片 KI:" + csKI + _T("  成功!"));


	}
	





}
