// ManageSSDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RunLog.h"
#include "ManageSSDDlg.h"
#include "RunLogDlg.h"




// CManageSSDDlg 对话框

IMPLEMENT_DYNAMIC(CManageSSDDlg, CDialog)

CManageSSDDlg::CManageSSDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManageSSDDlg::IDD, pParent)
{

}



void CManageSSDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Result_Edit, m_Edit);
}


BEGIN_MESSAGE_MAP(CManageSSDDlg, CDialog)
	ON_BN_CLICKED(IDC_Calculator_Button, &CManageSSDDlg::OnBnClickedCalculatorButton)
	ON_BN_CLICKED(IDC_GetData_Button, &CManageSSDDlg::OnBnClickedGetdataButton)
	ON_BN_CLICKED(IDC_RunCommand_Button, &CManageSSDDlg::OnBnClickedRuncommandButton)
END_MESSAGE_MAP()


// CManageSSDDlg 消息处理程序

void CManageSSDDlg::OnBnClickedCalculatorButton()
{


	SetDlgItemText(IDC_Result_Edit,CalComfirmSSD());
 
	
}

int CManageSSDDlg::InitManageSSDDlg(int iDF)
{
	extern HWND h_SafeHand;
	CRunLogDlg *TempApp;
	CString csAkey;
	TempApp    =  (CRunLogDlg*)(CWnd::FromHandle(h_SafeHand));

	SetDlgItemText(IDC_AKey_Edit,TempApp->GetProtectedData(01));
	SetDlgItemText(IDC_IMSI_M_Edit,TempApp->GetProtectedData(02));
	SetDlgItemText(IDC_UMID_ESN_Edit,TempApp->GetProtectedData(03));
	SetDlgItemText(IDC_RandSSD_Edit,TempApp->GetProtectedData(04));
	SetDlgItemText(IDC_Challage_Edit,TempApp->GetProtectedData(05));
	iMod = iDF;
	return 0;
}


void CManageSSDDlg::OnBnClickedGetdataButton()
{

	extern HWND h_SafeHand;
	CRunLogDlg *TempApp;
	TempApp    =  (CRunLogDlg*)(CWnd::FromHandle(h_SafeHand));

	CString csResult;
	CString csSend;

	SetDlgItemText(IDC_Result_Edit,"");


	if (iMod == 0x7F25)
	{


		csResult = TempApp->Run1LineScript("0012000000");
		Print2Edit("Reset");
		Print2Edit(csResult);

		if (csResult.IsEmpty())
			return;

		csSend  = "A0A40000027F25";
		csResult = TempApp->Run1LineScript(csSend);

		Print2Edit(csSend);
		Print2Edit(csResult);

		if (csResult.IsEmpty())
			return;

		csSend  = "A0A40000026F22";
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		csSend  = "A0B000000A";
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;
		SetDlgItemText(IDC_IMSI_M_Edit,csResult.Left(20));


		csSend  = "A0A40000026F42";
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		csSend  = "A0B0000001";
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

	

		if( (_CString2Int(csResult.Left(02))&0x1) == 0)
		{
			csSend  = "A0A40000026F38";
			csResult = TempApp->Run1LineScript(csSend);
			Print2Edit(csSend);
			Print2Edit(csResult);
			if (csResult.IsEmpty())
				return;

			csSend  = "A0B0000008";
			csResult = TempApp->Run1LineScript(csSend);
			Print2Edit(csSend);
			Print2Edit(csResult);
			if (csResult.IsEmpty())
				return;
		}
		else
		{

			csSend  = "A0A40000026F31";
			csResult = TempApp->Run1LineScript(csSend);
			Print2Edit(csSend);
			Print2Edit(csResult);
			if (csResult.IsEmpty())
				return;

			csSend  = "A0B0000008";
			csResult = TempApp->Run1LineScript(csSend);
			Print2Edit(csSend);
			Print2Edit(csResult);
			if (csResult.IsEmpty())
				return;
		}

		
	}
	else if (iMod == 0x7FF1)
	{

		csResult = TempApp->Run1LineScript("0012000000");
		Print2Edit("Reset");
		Print2Edit(csResult);

		if (csResult.IsEmpty())
			return;

		csSend  = "00A4040C10A0000003431002FF86FF0389FFFFFFFF";
		csResult = TempApp->Run1LineScript(csSend);

		Print2Edit(csSend);
		Print2Edit(csResult);

		if (csResult.IsEmpty())
			return;

		csSend  = "00A4000C026F22";
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		csSend  = "00B000000A";
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;
		SetDlgItemText(IDC_IMSI_M_Edit,csResult.Left(20));


		csSend  = "00A4000C026F42";
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		csSend  = "00B0000001";
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;




		if (	(_CString2Int(csResult.Left(02)) & 1)== 00)
		{
			csSend  = "00A4000C026F38";
			csResult = TempApp->Run1LineScript(csSend);
			Print2Edit(csSend);
			Print2Edit(csResult);
			if (csResult.IsEmpty())
				return;

			csSend  = "00B0000008";
			csResult = TempApp->Run1LineScript(csSend);
			Print2Edit(csSend);
			Print2Edit(csResult);
			if (csResult.IsEmpty())
				return;
		}
		else
		{

			csSend  = "00A4000C026F31";
			csResult = TempApp->Run1LineScript(csSend);
			Print2Edit(csSend);
			Print2Edit(csResult);
			if (csResult.IsEmpty())
				return;

			csSend  = "00B0000008";
			csResult = TempApp->Run1LineScript(csSend);
			Print2Edit(csSend);
			Print2Edit(csResult);
			if (csResult.IsEmpty())
				return;
		}


	}


	SetDlgItemText(IDC_UMID_ESN_Edit,csResult.Left(16));	

}

void CManageSSDDlg::OnBnClickedRuncommandButton()
{
	extern HWND h_SafeHand;
	CRunLogDlg *TempApp;
	TempApp    =  (CRunLogDlg*)(CWnd::FromHandle(h_SafeHand));

	CString csSend;
	CString csResult;
	CString csESN,csRandSSD;
	CString csRand;

	SetDlgItemText(IDC_Result_Edit,"");
	if (iMod == 0x7F25)
	{

		csResult = TempApp->Run1LineScript("0012000000");
		Print2Edit("Reset");
		Print2Edit(csResult);

		csSend   ="A0A40000027F25";

		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		GetDlgItemText(IDC_Challage_Edit,csRand);

		csSend   = "A08A000004"+csRand;
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);

		if (csResult.IsEmpty())
			return;


		csSend   = "A0C0000004";

		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		SetDlgItemText(IDC_Challage_Edit,csResult.Left(0x8));

		GetDlgItemText(IDC_RandSSD_Edit,csRandSSD);

		GetDlgItemText(IDC_UMID_ESN_Edit,csESN);

		csSend   = "A08400000F"+csRandSSD+"00"+csESN.Right(14);
		csResult = TempApp->Run1LineScript(csSend);

		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		csSend   = "A082000003"+CalComfirmSSD();
		csResult = TempApp->Run1LineScript(csSend);

		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;
	}
	else if (iMod == 0x7FF1)
	{

		csResult = TempApp->Run1LineScript("0012000000");
		Print2Edit("Reset");
		Print2Edit(csResult);

		csSend   ="00A4040C10A0000003431002FF86FF0389FFFFFFFF";

		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		GetDlgItemText(IDC_Challage_Edit,csRand);

		csSend   = "808A000004"+csRand;
		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);

		if (csResult.IsEmpty())
			return;


		csSend   = "00C0000004";

		csResult = TempApp->Run1LineScript(csSend);
		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		SetDlgItemText(IDC_Challage_Edit,csResult.Left(0x8));

		GetDlgItemText(IDC_RandSSD_Edit,csRandSSD);

		GetDlgItemText(IDC_UMID_ESN_Edit,csESN);

		csSend   = "808200000F"+csRandSSD+"00"+csESN.Right(14);
		csResult = TempApp->Run1LineScript(csSend);

		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;

		csSend   = "8082000103"+CalComfirmSSD();
		csResult = TempApp->Run1LineScript(csSend);

		Print2Edit(csSend);
		Print2Edit(csResult);
		if (csResult.IsEmpty())
			return;
	}
}

int CManageSSDDlg::Print2Edit(CString csData)
{
	m_Edit.SetSel(-1,-1);
	m_Edit.ReplaceSel(csData+"\r\n");
	m_Edit.SetFocus();
	return 0;
}

CString CManageSSDDlg::CalComfirmSSD(void)
{
	unsigned char ucAKey[0x8];
	unsigned char ucRandSSD[7];
	unsigned char ucIMSI_M[10];
	unsigned char ucRand[4];
	unsigned char ucUIMID[0x8];

	unsigned char ucSSD[16];
	unsigned char ucAuthBS[3];
	unsigned char ucIMSIS1[3];
	unsigned char ucESN[4];
	unsigned char ucTemp;

	CString csTemp;
	int iLen;

	//获取数据

	GetDlgItemText(IDC_AKey_Edit,csTemp);
	iLen = csTemp.GetLength();
	if (iLen == 16)
		_CString2UcHex(csTemp,ucAKey);

	GetDlgItemText(IDC_IMSI_M_Edit,csTemp);
	iLen = csTemp.GetLength();
	if (iLen == 20)
		_CString2UcHex(csTemp,ucIMSI_M);

	GetDlgItemText(IDC_RandSSD_Edit,csTemp);
	iLen = csTemp.GetLength();
	if (iLen == 14)
		_CString2UcHex(csTemp,ucRandSSD);

	GetDlgItemText(IDC_Challage_Edit,csTemp);
	iLen = csTemp.GetLength();
	if (iLen == 0x8)
		_CString2UcHex(csTemp,ucRand);

	GetDlgItemText(IDC_UMID_ESN_Edit,csTemp);
	iLen = csTemp.GetLength();
	if (iLen == 16)
		_CString2UcHex(csTemp,ucUIMID);



	memcpy(ucESN,ucUIMID+1,4);


	ucTemp = ucESN[0];
	ucESN[0] = ucESN[3];
	ucESN[3] = ucTemp;
	ucTemp = ucESN[1];
	ucESN[1] = ucESN[2];
	ucESN[2] = ucTemp;

	ucIMSIS1[0] = ucIMSI_M[5];
	ucIMSIS1[1] = ucIMSI_M[4];
	ucIMSIS1[2] = ucIMSI_M[3];


	InitAKey((char *)ucAKey);
	InitESN((char *) ucESN);
	SSD_Generation(ucRandSSD, (char *)ucSSD, (char *)ucSSD+0x8);

	InitESN((char *) ucESN);
	Auth_Signature(ucRand, ucIMSIS1, ucSSD, 0x00, (char *)ucAuthBS);


	CString csAuth;

	_UcHex2CString(ucAuthBS,3,csAuth);

	return csAuth;
}
