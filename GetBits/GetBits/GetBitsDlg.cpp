
// GetBitsDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "GetBits.h"
#include "GetBitsDlg.h"
#include "afxdialogex.h"
#include "..\..\AllUsedSource\Basconv.h"
#include "lusb0_usb.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL BERROR ;


#define MY_VID            0x1483
#define MY_PID            0x5751

// Device configuration and interface id.
#define MY_CONFIG 1
#define MY_INTF 0

// Device endpoint(s)
#define EP_IN 0x81
#define EP_OUT 0x01

// Device of bytes to transfer.
#define BUF_SIZE 512

CWinThread* pThread;
usb_dev_handle *dev = NULL; /* the device handle */
char _send[BUF_SIZE + 4];
char _recv[BUF_SIZE + 4];
int  connectState = 0;
int  threadState  = 1;

usb_dev_handle *open_dev(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;

	for (bus = usb_get_busses(); bus; bus = bus->next)
	{
		for (dev = bus->devices; dev; dev = dev->next)
		{
			if (dev->descriptor.idVendor == MY_VID
				&& dev->descriptor.idProduct == MY_PID)
			{
				return usb_open(dev);
			}
		}
	}
	return NULL;
}

UINT GetUSBData(LPVOID pParam);

UINT ExpUSBData(LPVOID pParam);

BOOL Port_Open(HANDLE *hCmPort, int iComPort, int iBaudrate, int iParityFlag)

{


	DCB dcb;

	COMMTIMEOUTS timeouts;

	CString csComPort;

	csComPort.Format(_T("\\\\.\\COM%d"), iComPort);



	*hCmPort = CreateFile(csComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (*hCmPort == INVALID_HANDLE_VALUE)

		return FALSE;



	GetCommTimeouts(hCmPort, &timeouts);



	timeouts.ReadIntervalTimeout = MAXWORD;

	//timeouts.ReadIntervalTimeout = 1000;

	timeouts.ReadTotalTimeoutMultiplier = 0;

	timeouts.ReadTotalTimeoutConstant = 0;//100;

	timeouts.WriteTotalTimeoutMultiplier = 100;

	timeouts.WriteTotalTimeoutConstant = 2000;





	if (!SetCommTimeouts(*hCmPort, &timeouts))

		return FALSE;



	if (!GetCommState(*hCmPort, &dcb))

		return FALSE;



	dcb.BaudRate = iBaudrate;

	dcb.ByteSize = 8;

	if (iParityFlag != 0)

	{

		dcb.fParity = TRUE;

		if (iParityFlag == 2) dcb.Parity = EVENPARITY; else dcb.Parity = ODDPARITY;

	}

	else dcb.fParity = false;



	dcb.StopBits = ONESTOPBIT;



	//此处CH340 一定需要先关闭再开

	if (dcb.fDtrControl)

	{

		dcb.fDtrControl = FALSE;

		if (!SetCommState(*hCmPort, &dcb))

			return FALSE;

	}



	dcb.fDtrControl = TRUE;



	if (!SetCommState(*hCmPort, &dcb))

		return FALSE;

	return TRUE;



}

BOOL Port_Close(HANDLE hCmPort)
{
	if (hCmPort == NULL)
		return FALSE;

	if (!CloseHandle(hCmPort))
		return FALSE;

	hCmPort = NULL;

	return TRUE;
}

BOOL ReadCharsFromPort(HANDLE hCmPort, BYTE *cResp,UINT* uiRespLen ,UINT uiMaxLen = 0xFF)
{
	DWORD nRecv;
	nRecv = uiMaxLen;

	if (!ReadFile(hCmPort, cResp, uiMaxLen, &nRecv, NULL))
	{
		//read error
		PurgeComm(hCmPort, PURGE_RXCLEAR);
		return FALSE;
	}

	*uiRespLen = nRecv;

	return TRUE;
}

BOOL SendCharsToPort(HANDLE hCmPort, BYTE *cSend, UINT uiSendLen)
{
	DWORD nSent;
	if (WriteFile(hCmPort, cSend, uiSendLen, &nSent, NULL) == 0)
	{
		PurgeComm(hCmPort, PURGE_TXCLEAR);
		return FALSE;	//error
	}
	return TRUE;
}

#define  Def_Bit_NULL -1 //数据为空的情况
#define  Def_Bit_S  0 //开始位
#define  Def_Bit_DS 1 //数据位 
#define  Def_Bit_DE 8 //数据位 
#define  Def_Bit_F  9 //校验位
#define  Def_Bit_E 11 //结束保护位


bool bBitt[11];  //一个正常数据有10位

int iPrescale  = 372; //分频
int iDeviation = 4;  //误差值
int iCheckiDeviation = 7 ; //校验位的误差值；
int iStopDeviation   = iPrescale/5;

BYTE bPreBit[10];
BYTE bPreBit0 = 0;//前面一个的bit数据

LONG64 BitData2CLKNumber(BYTE* bBit, UINT bBitLen)
{
	LONG64 lCLKNumber = 0;
	for (UINT i = 0 ; i< bBitLen;i++)
		lCLKNumber += lCLKNumber * 0xFF + bBit[i];

	return lCLKNumber;
}

void CLKNumber2BitData(LONG64 lCLKNumber,BYTE* bBit,UINT bBitLen)
{
	LONG64 lTemp = lCLKNumber;

	for(UINT i = bBitLen ; i>0;i--)
	{
		bBit[i-1] = lTemp%0x100;
		lTemp     = lTemp/0x100;
	}

}


//12位Bit转换成Byte数据 USART模式，此处忽略 最后一位的保护为，因为保护位长度不固定

//************************************
// Method:    _Bits2Byte
// FullName:  _Bits2Byte
// Access:    public 
// Returns:   int 
//-1 ： bBits长度不够;
//-2 : 起始位为 高电平
//-3 : 保护位为 低电平
//-4 : 校验为 错误
// Qualifier:
// Parameter: bool * bBits
// Parameter: UINT bBitLen
// Parameter: BYTE * bByte
//************************************
#define  _Bits_Success 1

int _Bits2Byte(bool *bBits, UINT bBitLen, BYTE* bByte)
{

	*bByte = 0;
	//位数不够直接返回失败
	if (bBitLen<11)
		return -1;

	if (bBits[0] != false)
		return  -2;

	if (bBits[10] == false)
		return  -3;

	//if (bBits[9] == false)
	//	return  -4;

	for (int i = 0; i < 8; i++)
	{
		if (bBitt[i + 1] == 1)
			*bByte |= (1 << (i));
	}

	return _Bits_Success;

}
#define  _CLK_Wait    2
#define  _CLK_Success 1
#define  _CLK_NoData  0
#define  _CLK_Error  -1


int iBitStatue = Def_Bit_S;



int _CalculateDeviation(LONG64 lClkNumber,int iCurPrescale)
{
	LONG64 iCurrentDeviation = 0;
	LONG64 iBisNumber        = 0;

	iBisNumber = lClkNumber/iCurPrescale;

	//当数据大于半数时则表示应该Bits +1
	if ((lClkNumber%iCurPrescale) >= iCurPrescale/2)
	{
		iBisNumber = iBisNumber + 1;
	}

	if (iBisNumber>10)
		iCurrentDeviation = iDeviation;
	else
		iCurrentDeviation = iBisNumber+1;

	return iCurrentDeviation;
}


//************************************
// Method:    CLKData2Byte
// FullName:  CLKData2Byte
// Access:    public 
// Returns:   int 
// Qualifier:
// Parameter: BYTE * bBit
// Parameter: UINT bBitLen
// Parameter: BYTE * bByte
//#define  _CLK_Wait    2
//#define  _CLK_Success 1
//#define  _CLK_NoData  0
//#define  _CLK_Error  -1
//************************************
int CLKData2Byte(BYTE* bBit, UINT bBitLen, BYTE *bByte)
{

	int iCorrect = 0;
	int iRet  = _CLK_NoData; //表示当前存储了多少为在bit内 
	int bBitNumber = 0 ;   // 当下可以计算出的bit
	LONG64 lCurPre = 0;
	int iii;
	int iBitsDeviation = iDeviation;

	if (iBitStatue != Def_Bit_NULL)
	{
		lCurPre = BitData2CLKNumber(bBit + 1, bBitLen - 1) - BitData2CLKNumber(bPreBit+1, bBitLen - 1);



		//if ((lCurPre/iPrescale)<8)
		//{
		//	iDeviation = iPrescale/10;
		//}
		//else
		//	iDeviation = 2;


		iCheckiDeviation = iPrescale/2;

		iBitsDeviation   = _CalculateDeviation(lCurPre,iPrescale);
		

		

		//求出为几个bit 数据
		if (((lCurPre+ iBitsDeviation) % iPrescale )<= 2* iBitsDeviation)
		{
			bBitNumber = ((lCurPre + iBitsDeviation)/iPrescale);



			for ( iii = 0; iii < bBitNumber; iii++)
			{

				if (iBitStatue < 11)
				{
					if ((lCurPre%iPrescale) != 0)
					{
						//如果存在误差,此处需要修正,此处为带符号的
						iCorrect = ((int)(bBitNumber*iPrescale) - (int) lCurPre);
					}

					bBitt[iBitStatue] = (bPreBit[0]&1);

					iBitStatue += 1;

					iRet = _CLK_Wait;
				}
				else
				{
					goto __Bits2Byte;

				}
			}

		}
		//此处为校验位出错的问题
		//else if (((lCurPre+iCheckiDeviation)/iPrescale) == (9-iBitStatue))
		//{
		//	for (int iii = 0; iii < (9 - iBitStatue); iii++)
		//		bBitt[iBitStatue + iii] = (bPreBit[0] & 1);

		//}
		//此处为停止位长度不固定

		else if (((lCurPre+iStopDeviation)/iPrescale) >= (11- iBitStatue))
		{
			
			for (int iii = 0; iii < (11 - iBitStatue); iii++)
				bBitt[iBitStatue + iii] = (bPreBit[0] & 1);

__Bits2Byte:
			if (_Bits2Byte(bBitt, 11, bByte)== _Bits_Success)
			{
				iRet = _CLK_Success;
			}
			else
			{//此处需要另外添加代码，解析出错时候的问题

				iRet = _CLK_Error;
			}
			iBitStatue = Def_Bit_S;
			ZeroMemory(bBitt, sizeof(bBitt));
	
			
		}
	}
	/*else
	iBitStatue = Def_Bit_E;*/

	
	
	
	memcpy(bPreBit, bBit, 9);

	//if (iCorrect !=0)
	//{
	//	lCurPre = BitData2CLKNumber(bPreBit + 1,8);

	//	lCurPre = lCurPre+ iCorrect;

	//	CLKNumber2BitData(lCurPre,bPreBit+1,8);
	//}
	
	return iRet;

}

int iPreCorrent = 0;
int CLKDiffData2Byte(BYTE* bBit,UINT bBitLen, BYTE *bByte)
{
	int iRet           = _CLK_NoData; //表示当前存储了多少为在bit内 
	int bBitNumber     = 0 ;   // 当下可以计算出的bit
	int iBitsDeviation = iDeviation;
	int iii;
	LONG64 lDiff = 0 ; 

	for (UINT i = 1 ;i < bBitLen ;i++ )
		lDiff = lDiff*0x100 + bBit[i];

	//此处判断是否需要修正之前的数据
	//当大于基础误差时候,使用修正.
	if ((lDiff+iDeviation )%iPrescale > 2*iDeviation)
		lDiff += iPreCorrent;

	iPreCorrent = 0;


	//此处为保护位的误差,此处还没有使用
	iCheckiDeviation = iPrescale/2;
	//计算多bits 误差
	iBitsDeviation   = _CalculateDeviation(lDiff,iPrescale);

	if (((lDiff+ iBitsDeviation) % iPrescale )<= 2* iBitsDeviation)
	{
		bBitNumber = ((lDiff + iBitsDeviation)/iPrescale);


		iPreCorrent = lDiff - bBitNumber*iPrescale ;

		for ( iii = 0; iii < bBitNumber; iii++)
		{

			if (iBitStatue < 11)
			{

				bBitt[iBitStatue] = (!(bBit[0]>>6));

				iBitStatue += 1;

				iRet = _CLK_Wait;
			}
			else
			{
				goto __Bits2Byte;

			}
		}

	}



	else if (((lDiff+iStopDeviation)/iPrescale) >= (11- iBitStatue))
	{

		for (int iii = 0; iii < (11 - iBitStatue); iii++)
			bBitt[iBitStatue + iii] = (!(bBit[0]>>6));

__Bits2Byte:
		if (_Bits2Byte(bBitt, 11, bByte)== _Bits_Success)
		{
			iRet = _CLK_Success;
		}
		else
		{//此处需要另外添加代码，解析出错时候的问题

			iRet = _CLK_Error;
		}
		iBitStatue = Def_Bit_S;
		ZeroMemory(bBitt, sizeof(bBitt));


	}
	else
		iBitStatue = Def_Bit_E;

	return iRet;

}



UINT _CalculateBit(LONG64 lInput)
{

	//计算多bits 误差
	int iBitsDeviation   = _CalculateDeviation(lInput,iPrescale);

	int bBitNumber = (int)((lInput + iBitsDeviation)/iPrescale);

	int bCBitNumber = (int)((lInput+iPreCorrent+iBitsDeviation)/iPrescale);

	if (bBitNumber == bCBitNumber)
	{
		iPreCorrent = lInput - bBitNumber*iPrescale ;
		return bBitNumber;
	}
	else
		return 0;




}


int _CLKDiffData2Byte_2(BYTE* bBit,UINT bBitLen, BYTE *bByte)
{
	int iRet           = _CLK_NoData; //表示当前存储了多少为在bit内 
	int bBitNumber     = 0 ;   // 当下可以计算出的bit
	int iBitsDeviation = iDeviation;
	int iii;
	LONG64 lDiff = 0 ; 

	for (UINT i = 1 ;i < bBitLen ;i++ )
		lDiff = lDiff + (bBit[i]<<8*(i-1));


	bBitNumber = lDiff/iPrescale;
	if ((lDiff%iPrescale) > iPrescale/2)
		bBitNumber += 1;

	for ( iii = 0; iii < bBitNumber; iii++)
	{

		if ((iBitStatue == Def_Bit_S )&&
			(!(bBit[0]>>6)))
		{
			iRet       = _CLK_Wait;
			bBitNumber = 0;
			continue;
		}


		if (iBitStatue < 11)
		{
			
			bBitt[iBitStatue] = (( bPreBit0 >>6));
			
			iBitStatue += 1;
			
			iRet = _CLK_Wait;
		}
		else
		{
			if (_Bits2Byte(bBitt, 11, bByte)== _Bits_Success)
				iRet = _CLK_Success;
			else
			//此处需要另外添加代码，解析出错时候的问题
				iRet = _CLK_Error;

			iBitStatue = Def_Bit_S;
			ZeroMemory(bBitt, sizeof(bBitt));

			bBitNumber = 0;
			continue;

		}

	}


	bPreBit0 = bBit[0];
__End:	return iRet;

}



CString CLKDiffText(BYTE* bBit,UINT bBitLen)
{
	CString csRes;
	CString csCor;

	long lDiff = 0 ; 
	int iCorrent;
	int iNumber ;

	csRes.Empty();

	for (UINT i = 1 ;i < bBitLen ;i++ )
		lDiff = lDiff*0x100 + bBit[i];

	if (((lDiff+iDeviation )%iPrescale > 2*iDeviation)&&
		((lDiff+iDeviation)/iPrescale)<8)
	{
		lDiff += iPreCorrent;
		csCor.Format(" Corrent :%d",iPreCorrent);
	}
	else
		csCor.Empty();


	iCorrent = lDiff%iPrescale;
	iNumber  = lDiff/iPrescale;



	if (iNumber < 12)
	{
		if (iCorrent> (iPrescale/2))
		{
			iCorrent = iCorrent - iPrescale;
			iNumber +=1;
		}


		//iPreCorrent = iCorrent;
		csRes.Format("ETU:%d Diff:%d",iNumber,iCorrent);
	}
	//else
	//	csRes.Format("CLK:%l",lDiff);


	

	return csRes+csCor;


}

#define  _BYTE_Wait    2
#define  _BYTE_Success 1
#define  _BYTE_NoData  0
#define  _BYTE_Error  -1
#define  _TPDU_NULL  0x00
#define  _TPDU_ATR   0x10
#define  _TPDU_PPS   0x20
#define  _TPDU_TPDU  0x30

#define  _Max_TPDU_Len 0x200
BYTE bPreByte[_Max_TPDU_Len];
UINT iByteLen = 0;
int  iTPDUStatue = _TPDU_NULL;

bool _ATRIsComplete(BYTE* bATR,UINT iATRLen)
{
	UINT iHisLen;
	//必然要有TS T0
	if (iATRLen < 2)
		return false;
	iHisLen = bATR[1] & 0xF;

	if (iATRLen < 2 + iHisLen)
		return false;

	//此处检查是否 Txi 是否进行完毕
	int iCheckSum = 0;
	int iLevel    = 0;
	UINT iOffset   = 2;
	BYTE TDi;

	do 
	{
		//前一个TDi
		TDi = bATR[iOffset-1];
		//如果出现F协议，则表示需要校验位
		if ((iLevel != 0)&&
			((TDi&0xF) == 0xF))
			iCheckSum = 1;


		if (TDi&0x10)
			iOffset += 1;
		if (TDi & 0x20)
			iOffset += 1;
		if (TDi & 0x40)
			iOffset += 1;
		if (TDi & 0x80)
			iOffset += 1;
		if (iOffset>iATRLen)
			return false;
		iLevel++;
	} while (TDi&0x80);

	if ((iOffset+iHisLen+iCheckSum) == iATRLen)
		return true;



	return false;
}

//************************************
// Method:    _PPSIsComplete
// FullName:  _PPSIsComplete
// Access:    public 
// Returns:   bool
// Qualifier: 
// Parameter: BYTE * bPPS
// Parameter: UINT iPPSLen
// 当前只判断了T0 协议
//************************************
bool _PPSIsComplete(BYTE* bPPS, UINT iPPSLen)
{
	if (iPPSLen<7)
		return false;

	if ((bPPS[6] == 0)&&
		(iPPSLen == 7))
		return true;

	if (iPPSLen == 8)
		return true;

	return false;
}

//************************************
// Method:    _PPSIsSuccess
// FullName:  _PPSIsSuccess
// Access:    public 
// Returns:   int ,Prescale
// Qualifier:
// Parameter: BYTE * bPPS
// Parameter: UINT iPPSLen
// Parameter: CString & csSend
// Parameter: CString & csResponse
//************************************
int _PPSIsSuccess(BYTE* bPPS, UINT iPPSLen,
	CString &csSend,CString& csResponse)
{
	int iCurPrescale = 372;
	_UcHex2CString(bPPS, 4, csSend);
	_UcHex2CString(bPPS+4, iPPSLen-4, csResponse);

	if (csSend.Compare(csResponse)==0)
	{
		switch (bPPS[2])
		{
		case 0x94:iCurPrescale = 64; break;
		case 0x95:iCurPrescale = 32; break;
		case 0x96:iCurPrescale = 16; break;
		case 0x97:iCurPrescale = 8; break;
		case 0x18:iCurPrescale = 31; break;
		case 0x11:iCurPrescale = 372; break;
		case 0x10:iCurPrescale = 372; break;
		default:iCurPrescale = 372; break;
		}
		
	}

	return iCurPrescale;
}

bool _TPDUIsComplete(BYTE* bTPDU, UINT iTPDULen,
	CString&csSend,
	CString&csProduce, CString&csACK,
	CString&csData, CString&csLProduce,CString&csSW)
{
	bool bRet = false;
	int iDataLen;
	if (iTPDULen < 7)
		return false;

	//此处判断是否有过程字符
	UINT iProduceNumber = 0;
	UINT iLProduceNumber = 0;
	for (UINT i = 5; i< iTPDULen;i++)
	{
		if (bTPDU[i] == 0x60)
			iProduceNumber++;
		else
			break;
	
	}
	if (iTPDULen < (iProduceNumber+7))
		return false;



	//判断是否有应答
	//不等于Ins 
	// case 1
	if (bTPDU[5+iProduceNumber] != bTPDU[1])
	{
		if (iTPDULen == (iProduceNumber + 7))
		{
			_UcHex2CString(bTPDU, 5, csSend);
			_UcHex2CString(bTPDU+5, iProduceNumber, csProduce);
			csACK.Empty();
			csData.Empty();
			_UcHex2CString(bTPDU + 5+ iProduceNumber, 2, csSW);
			bRet = true;
		}

	}
	//case 2
	else
	{

		if (bTPDU[4] == 0)
			iDataLen = 0x100;
		else
			iDataLen = bTPDU[4];


		for (UINT i =  (6 + iProduceNumber + iDataLen); i< iTPDULen;i++)
		{
			if (bTPDU[i] == 0x60)
				iLProduceNumber++;
			else
				break;

		}

		if ((iDataLen + 8 + iProduceNumber+iLProduceNumber) == iTPDULen)
		{
			_UcHex2CString(bTPDU , 5, csSend);
			_UcHex2CString(bTPDU + 5, iProduceNumber, csProduce);
			_UcHex2CString(bTPDU + 5 + iProduceNumber, 1, csACK);
			_UcHex2CString(bTPDU + 5 + iProduceNumber + 1,  iDataLen, csData);
			_UcHex2CString(bTPDU + 5 + iProduceNumber + 1 + iDataLen,  iLProduceNumber, csLProduce);
			_UcHex2CString(bTPDU + 5 + iProduceNumber + 1 + iDataLen + iLProduceNumber, 2, csSW);
			bRet = true;

		}



	}






	return bRet;
}
//************************************
// Method:    ByteData2TPDU
// FullName:  ByteData2TPDU
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: BYTE bByte
// Parameter: BYTE * bTPDU
// Parameter: UINT * uiTPDULen
//************************************
int ByteData2TPDU(BYTE bByte, BYTE *bTPDU,UINT * uiTPDULen,
	              CString&csSend,
	              CString&csProduce,CString&csACK,
	              CString&csData,CString&csLProduce,CString&csSW)
{
	int iRet = _BYTE_Wait;


	bPreByte[iByteLen] = bByte;
	iByteLen += 1;

	switch (iTPDUStatue)
	{
	case _TPDU_NULL:iTPDUStatue = _TPDU_ATR; break;
	case _TPDU_ATR :
		if (_ATRIsComplete(bPreByte, iByteLen))
		{
			iRet = _BYTE_Success;
			_UcHex2CString(bPreByte, iByteLen, csData);

			
			iTPDUStatue = _TPDU_PPS;
		}
		break;
	case _TPDU_PPS:

		if (bPreByte[0] != 0xFF)
			iTPDUStatue = _TPDU_TPDU;

		if (_PPSIsComplete(bPreByte, iByteLen))
		{
			iRet = _BYTE_Success;

			iPrescale = _PPSIsSuccess(bPreByte, iByteLen, csSend, csData);

		
		}
		break;
	case _TPDU_TPDU:

		if (_TPDUIsComplete(bPreByte, iByteLen, csSend,csProduce,csACK,csData,csLProduce,csSW))
		{
			iRet = _BYTE_Success;

		
		}

		break;

	default:
		break;
	}


	if (iRet == _BYTE_Success)
	{
		*uiTPDULen = iByteLen;
		memcpy(bTPDU, bPreByte, iByteLen);
		ZeroMemory(bPreByte, sizeof(bPreByte));
		iByteLen = 0;
	}
	return iRet | iTPDUStatue;


}
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


// CGetBitsDlg 对话框



CGetBitsDlg::CGetBitsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GETBITS_DIALOG, pParent)
	, m_Value_EDIT(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetBitsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ComPort_Combo, m_pComPort);
	DDX_Control(pDX, IDC_LIST2, m_BitsList);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);
	DDX_Control(pDX, IDC_BYTE_EDIT, m_BYTE_EDIT);
	DDX_Text(pDX, IDC_BYTE_EDIT, m_Value_EDIT);
	DDX_Control(pDX, IDC_LIST1, m_TPDU);
}

BEGIN_MESSAGE_MAP(CGetBitsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ComConnect_Button, &CGetBitsDlg::OnBnClickedComconnectButton)
	ON_WM_SIZE()
	ON_LBN_DBLCLK(IDC_LIST2, &CGetBitsDlg::OnLbnDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON1, &CGetBitsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGetBitsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CGetBitsDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CGetBitsDlg 消息处理程序

BOOL CGetBitsDlg::OnInitDialog()
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

	//InitCombo();

	threadState = 1;
	AfxBeginThread(GetUSBData, _T("thread")); 
	AfxBeginThread(ExpUSBData, _T("ExpData")); 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGetBitsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGetBitsDlg::OnPaint()
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
HCURSOR CGetBitsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CGetBitsDlg::InitCombo()
{

	HANDLE hHandle;

	m_pComPort.ResetContent();

	for (int i = 1; i<100; i++)
	{
		if (Port_Open(&hHandle, i, 256000, 02))
		{
			Port_Close(hHandle);

			m_pComPort.AddString(_T("Com")+Int2CStringDecimal(i));
		}
	}

	if (m_pComPort.GetCount()>0)
	{
		m_pComPort.SetCurSel(0);
	}
	bConnect = false;

	return m_pComPort.GetCount();
}


// 从 COM口  读取 bit 信息 ，包含CLK，高低电平， 以及 Pin口
// X1 X2  XXXXXXXXXXXXXXXX
// 
bool CGetBitsDlg::ReadBitData(BYTE* BitData,UINT* iBitLen,UINT iMaxBitLen)
{
	
	ZeroMemory(BitData, iMaxBitLen);

	UINT uiRespLen = 0;
	UINT uiTempLen = 0;

	do 
	{
		if (!ReadCharsFromPort(hSTMUSART1, BitData, &uiTempLen, 9 - uiRespLen))
			//return false;

		uiRespLen += uiTempLen;

	} while (uiRespLen < 9);





	

//	_UcHex2CString(BitData, 9, csBitData);

	return true;
}

#define  _Display_Bit_CLK 1
#define  _Display_Byte    1
#define  _Display_TPDU    1
#define  _Display_APDU    1
int CGetBitsDlg::DisplayBit()
{
	//OnComm();
	BYTE BitData[9];
	UINT iBitLen = 9;

	if (!ReadBitData(BitData, &iBitLen, 9))
		return false;
#if _Display_Bit_CLK
	CString csBitData;
	_UcHex2CString(BitData, 9, csBitData);
	m_BitsList.SetCurSel(-1);
	m_BitsList.AddString(csBitData);
	//此处显示到对应控件
#endif


#if _Display_Byte

	BYTE bByte;

	if (CLKData2Byte(BitData, 9, &bByte) == _CLK_Success)
	{
		//此处显示到对应控件
#if _Display_TPDU
		BYTE bTPDU[260];
		UINT iTPDULen = 0;
		CString csSend, csProduce, csACK, csData,csLPro ,csSW;

		if ((ByteData2TPDU(bByte, bTPDU, &iTPDULen, csSend , csProduce, csACK, csData,csLPro ,csSW) & 0xF) == _BYTE_Success)
		{


		}
#endif

	}
#endif

	// TODO: 在此处添加实现代码.
	return 0;
}


void CGetBitsDlg::OnBnClickedComconnectButton()
{
//	CString csCom;
//	int iSel = m_pComPort.GetCurSel();
//	m_pComPort.GetLBText(iSel,csCom);
//	csCom = csCom.Mid(3);
//
//
//	//	
//	int iCom =_ttoi(csCom);
//
//	if (bConnect)
//	{
//		m_ctrlComm.put_PortOpen(FALSE);//打开串口
//		bConnect = false;
//		SetDlgItemText(IDC_ComConnect_Button,_T("Connect"));
//		return;
//	}
//
// 	m_ctrlComm.put_CommPort(iCom);//选择串口号(这里因为列表框的索引号是从0开始，所以(nSel+1)对应的才是我们所选的串口号)
//	
//	m_ctrlComm.put_RThreshold(1);//收到两个字节引发OnComm事件	
//	m_ctrlComm.put_InputMode(1);//输入模式选为二进制	
//
//	m_ctrlComm.put_Settings(_T("864000,n,8,1"));//设置串1口参数，波特率，无奇偶校验，位停止位，位数据位
//
//	//m_ctrlComm.put_DTREnable(TRUE);
//
//	//m_ctrlComm.put_RTSEnable(TRUE);
//	//
//	//m_ctrlComm.put_PortOpen(TRUE);//打开串口
//
//
//	//if (m_ctrlComm.get_DTREnable())
//	//	m_ctrlComm.put_DTREnable(FALSE);
//
//	//if (m_ctrlComm.get_RTSEnable())
//	//	m_ctrlComm.put_RTSEnable(FALSE);
//
//	m_ctrlComm.put_DTREnable(FALSE);
//
//	m_ctrlComm.put_RTSEnable(FALSE);
//
//	//m_ctrlComm.put_PortOpen(FALSE);//打开串口
//
//
////	m_ctrlComm.put_PortOpen(TRUE);//打开串口
//	
//
//	m_ctrlComm.put_PortOpen(TRUE);//打开串口
//	m_ctrlComm.put_InputLen(0); //设置当前接收区数据长度为0 
//	m_ctrlComm.get_Input();//先预读缓冲区以清除残留数据 
//	bConnect = true;
//
//	SetDlgItemText(IDC_ComConnect_Button,_T("Disconnect"));

	if(connectState == 0)
	{
		usb_init(); /* initialize the library */
		usb_find_busses(); /* find all busses */
		usb_find_devices(); /* find all connected devices */

		if (!(dev = open_dev()))
		{
			AfxMessageBox("connect fail!");
			return;
		}

		if (usb_set_configuration(dev, MY_CONFIG) < 0)
		{
			usb_close(dev);
			AfxMessageBox("set config fail!");
			return;
		}

		if (usb_claim_interface(dev, 0) < 0)
		{
			usb_close(dev);
			AfxMessageBox("set interface fail!");
			return;
		}

		connectState = 1;
		GetDlgItem(IDC_ComConnect_Button)->SetWindowText("Disconnect");
	}
	else
	{
		if (dev)
		{
			usb_release_interface(dev, 0);
			usb_close(dev);
			dev = NULL;
		}

		connectState = 0;
		GetDlgItem(IDC_ComConnect_Button)->SetWindowText("Connect");
	}
	bbitsLen = 0;
	bdiflen  = 0;

}


UINT GetUSBData(LPVOID pParam) 
{ 
	int ret;
	CString str;

	char send[2] = {0x90,0x00};
	char GetSize[5]={0xF0,0xA0,0x00,0x00,0x08};
	char GetBits[5]={0xF0,0xB0,0x00,0x00,0x40};
	unsigned char Size[8];
	int iSize    = 0;
	int iGetSize = 0;
	int iCurrent = 0;
	int iGetOff  = 0;
	while(threadState)
	{
		if(dev)
		{
			ret = usb_bulk_write(dev, EP_OUT, GetSize,5, 5000);

			ret = usb_bulk_read(dev, EP_IN, (char *)Size,8, 5000);

			/*		if (ret < 0)
			{
			AfxMessageBox("接受失败!没有数据!");
			::Sleep(10000);
			}*/

			iSize    = Size[0]*0x1000000 + Size[1]*0x10000+ Size[2]*0x100+Size[3];
			iGetSize = Size[4]*0x1000000 + Size[5]*0x10000+ Size[6]*0x100+Size[7];
			if (iSize <= iGetSize)
			{::Sleep(1000);
			
			 continue;
			}

			//while(iSize > iGetSize)
			//{
				iCurrent = iSize - iGetSize;
				if (iCurrent > 0x100 )
					iCurrent = 0x100;

				GetBits[4] = iCurrent&0xFF;
				ret = usb_bulk_write(dev, EP_OUT, GetBits,5, 5000);
				iGetOff = 0;

				while(iCurrent > iGetOff)
				{
					ret = usb_bulk_read(dev, EP_IN, _recv+iGetOff,512, 50000);
					if (ret > 0)
						iGetOff += ret;
				}

				CGetBitsDlg *pDialog = (CGetBitsDlg *)AfxGetApp()->GetMainWnd();
				pDialog->DisplayBitsData((BYTE*)_recv,iCurrent);
				



				

				//if(ret ==  iCurrent)
				//{

				//	iGetSize+= iCurrent;
				//	CGetBitsDlg *pDialog = (CGetBitsDlg *)AfxGetApp()->GetMainWnd();
				//	pDialog->DisplayBitsData((BYTE*)_recv,ret);
				//}
				//else{			
				//	ret = usb_bulk_read(dev, EP_IN, _recv,iCurrent, 5000);
				//}


			//}

			

			/*
			ret = usb_bulk_read(dev, EP_IN, _recv,BUF_SIZE, 5000);
			if(ret > 0)
			{
				CGetBitsDlg *pDialog = (CGetBitsDlg *)AfxGetApp()->GetMainWnd();
				pDialog->DisplayBitsData((BYTE*)_recv,ret);

				//ret = usb_bulk_write(dev, EP_OUT, send, 2, 5000);

				//if (ret<=0)
				//{
				//	AfxMessageBox("发送失败!");
				//}
			
				
			}*/
		}

		::Sleep(1);
	}

	return  0 ; 
} 

ULONG64 iIndex;

UINT ExpUSBData(LPVOID pParam) 

{

	//iIndex = 0;

	//CGetBitsDlg *pDialog = (CGetBitsDlg *)AfxGetApp()->GetMainWnd();

	//while(threadState)
	//{
	//	if (pDialog->_DisplayBitsData_Exp() == 0)
	//		::Sleep(10);

	//}
		return  0 ; 

}





void CGetBitsDlg::NewGetDataThread()
{
	DWORD code;

	//if (!ConfirmCardStatue())
	//	return;


	GetExitCodeThread(hThread,&code);      

	if (code != STILL_ACTIVE)
	{
		hThread  = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) GetDataThread,NULL, 0, NULL);
	}
	else
		ResumeThread(hThread);

	//iThread = Def_Thread_GetFile;
	return ;
}

int CGetBitsDlg::GetDataThread(void)
{


	CGetBitsDlg *NewApp; 
	NewApp = (CGetBitsDlg *)AfxGetApp()->GetMainWnd();


	NewApp->GetBitsData();


	return 1;

}

int CGetBitsDlg::GetBitsData()
{

	ULONG64 ulSector      = 0;
	ULONG32 ulReadSector      = 0;
	UINT    uiRam         = 0;

	ULONG64 ul64Bytes     = 0;

	ULONG64 ul64ReadBytes = 0;


	BYTE ucCommand[600];
	UINT uiLen = 0 ;
	BERROR = 0;

	while(!BERROR)
	{
	if (GetSectorCount(&ulSector,&uiRam) == 0x9000)
	{
		if (GetBitsBuf(ul64ReadBytes%512,(ULONG32)(ul64ReadBytes/512),ucCommand, &uiLen)==0x9000)
		{
			DisplayBitsData(ucCommand,uiLen);

			ulReadSector+=1;
			ul64ReadBytes+=uiLen;
		}

	}

	//	ul64Bytes = ulSector*512 + uiRam;


		//while(ul64ReadBytes<ul64Bytes)
		//{

			//else
			//	goto _GetSectorCount;

	//	}
//_Error_: Sleep(1);

	}
	return 1;
}



BEGIN_EVENTSINK_MAP(CGetBitsDlg, CDialogEx)
//	ON_EVENT(CGetBitsDlg, IDC_MSCOMM1, 1, CGetBitsDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()



void CGetBitsDlg::OnCommMscomm1()
{
	VARIANT variant_inp;   //Variant 是一种特殊的数据类型，除了定长String数据及用户定义类型外，可以包含任何种类的数据。
	COleVariant safearray_inp;     
	LONG len,k,ik;    
	BYTE rxdata[2048]; //设置BYTE数组 An 8-bit integer that is not signed.     
	CString strtemp;     
	CString m_strRXData;

	CByteArray byt;
	CString csOutput;

	if(m_ctrlComm.get_CommEvent() == 2) //事件值为2表示接收缓冲区内有字符     
	{            
		////////以下你可以根据自己的通信协议加入处理代码 

		//Sleep( 100);
		safearray_inp=m_ctrlComm.get_Input();		//读缓冲区  

		safearray_inp.ChangeType (VT_ARRAY |VT_UI1);

		BYTE HUGEP *pbstr;
		HRESULT hr;
		hr=SafeArrayAccessData (safearray_inp.parray,(void HUGEP*FAR*)&pbstr);//获取安全数组指针
		if (FAILED (hr))
			AfxMessageBox("获取数组指针失败！");

		long num=0;
		hr=SafeArrayGetUBound (safearray_inp.parray,1,&num);

		if (FAILED (hr))
			AfxMessageBox("获取数组上界失败！");

		num = num+1;

		//_UcHex2CString(pbstr,num,csOutput);
		//csOutput.Format("%d",num);
		//m_BitsList .AddString(_T("######")+csOutput+_T("######"));

		_UcHex2CString(pbstr,num,csOutput);

		BYTE bBYTE;

		for (k = 0 ; k< num; k++)
		{

			bbits[bbitsLen] =  pbstr[k];

			if (bbitsLen == 0)
				bdiflen = bbits[0]&0x7;

			bbitsLen =bbitsLen+1;

			if (bbitsLen  == bdiflen*2)
			{
				_UcHex2CString(bbits,bbitsLen,csOutput);
				m_BitsList .AddString(csOutput);
			//	m_BitsList .AddString(CLKDiffText(bbits,bbitsLen));

				//此处为Reset
				if ((bbits[0]&0x30) != 0x30) 
				{
					iPrescale = 372;
					iBitStatue = Def_Bit_S;
					iTPDUStatue = _TPDU_NULL;
					iByteLen = 0;
				}
				else if (CLKDiffData2Byte(bbits,bbitsLen,&bBYTE) == _BYTE_Success)
				{
					//m_BitsList .AddString(_T("----")+Int2CString(bBYTE,2));

					CString csEdit;
					//csEdit.Empty();
					//GetDlgItemTextA(IDC_BYTE_EDIT,csEdit);
					//csEdit = csEdit+Int2CString(bBYTE,2);
					//SetDlgItemText(IDC_BYTE_EDIT,csEdit);


					BYTE bBYTES[300];
					UINT iLen = 0;
					CString csSend,csPro,csACK,csData,csLPro,csSW;
					int iRet = ByteData2TPDU(bBYTE,bBYTES,&iLen ,csSend,csPro,csACK,csData,csLPro,csSW);
					if ((iRet&0xF) == _BYTE_Success)
					{
						CString csTPDU;
						_UcHex2CString(bBYTES,iLen,csTPDU);
						m_TPDU.AddString(csTPDU);

				
	
					}

				}
	
				bbitsLen = 0;
				bdiflen  = 0;

			}
		}
		SafeArrayUnaccessData (safearray_inp.parray);
	}     
}


void CGetBitsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	
	
	
	// TODO: 在此处添加消息处理程序代码
}


void CGetBitsDlg::OnLbnDblclkList2()
{

	int iIndex,iLen;
	CString csCopyData;
	CString csTemp;

	//iIndex =m_BitsList. GetCurSel();
	//if (iIndex<1)
	//	return;

	//int nCount = m_BitsList.GetSelCount();
	//CArray<int,int> aryListBoxSel;

	//aryListBoxSel.SetSize(nCount);
	//m_BitsList.GetSelItems(nCount, aryListBoxSel.GetData()); 
	//AFXDUMP(aryListBoxSel);

	////int iSum = GetSelCount();
	//csCopyData.Empty();
	//for (int i = 0 ; i < nCount ; i++)
	//{
	//	csTemp.Empty();
	//	iLen   = m_BitsList.GetTextLen(aryListBoxSel.GetAt(i));
	//	m_BitsList.GetText(aryListBoxSel.GetAt(i),csTemp.GetBuffer(iLen));
	//	csTemp.ReleaseBuffer();
	//	csTemp += _T("\r\n");
	//	csCopyData += csTemp;
	//}

	int iCount = m_BitsList.GetCount();

	for (int i = 0 ; i< iCount; i++)
	{

		iLen   = m_BitsList.GetTextLen(i);
		m_BitsList.GetText(i,csTemp.GetBuffer(iLen));
		csTemp.ReleaseBuffer();
		csTemp.ReleaseBuffer();
		csTemp += _T("\r\n");
		csCopyData += csTemp;

	}


	if(OpenClipboard())
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, csCopyData.GetLength()+1); 
		buffer = (char*)GlobalLock(clipbuffer); 
		memcpy(buffer, csCopyData,csCopyData.GetLength()); 
		GlobalUnlock(clipbuffer); 
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();

	}
	else
		MessageBox(_T("复制失败"));
//	m_BitsList.ResetContent();
}


void CGetBitsDlg::OnBnClickedButton1()
{
	m_BitsList.ResetContent();
	m_TPDU.ResetContent();
	SetDlgItemText(IDC_BYTE_EDIT,_T(""));
}

void CGetBitsDlg::OnBnClickedButton2()
{
	//iPrescale = 372;
	iBitStatue = Def_Bit_S;
	iTPDUStatue = _TPDU_NULL;
	iByteLen = 0;


	char send[2] = {0x90,0x90};

	usb_bulk_write(dev, EP_OUT, send, 2, 5000);
	threadState = 1;
	AfxBeginThread(GetUSBData, _T("ReadUSB")); 

}

void CGetBitsDlg::OnBnClickedButton3()
{
	//BYTE ucCommand[1024];
	//UINT uiLen = 0 ;
	//GetBitsBuf(01,ucCommand,&uiLen);
	//CString csOutput;
	//_UcHex2CString(ucCommand,(int)uiLen,csOutput);
	//DisplayBitsData(ucCommand,uiLen);
	NewGetDataThread();
}




// 获取设备信息
int CGetBitsDlg::GetInformation(BYTE* ucInfo, UINT* uiInfoLen)
{
	BYTE ucCommand[5]  = {0xFE,0x02,0x00,0x00,0x08};
	SendBuf2Comm (ucCommand,5);
	Sleep(5);

	if (!ReceiveBuf4Comm(ucInfo,uiInfoLen))
		return -1;
	* uiInfoLen = *uiInfoLen-2;
	return (ucInfo[*uiInfoLen-2]<<8) + ucInfo[*uiInfoLen-1];
}

// 查询是否有数据需要被传出
int CGetBitsDlg::GetSectorCount( ULONG64* ulSector,UINT* uiRamLen )
{
	int iRet ;
	BYTE ucBuf[20];
	BYTE ucCommand[5]  = {0xFE,0xC0,0x00,0x00,0x00};
	UINT uiBufLen;
	SendBuf2Comm (ucCommand,5);
	Sleep(20);

	if(!ReceiveBuf4Comm(ucBuf,&uiBufLen))
		return -1;
	*ulSector = 0;
	*uiRamLen = 0;
	iRet = (ucBuf[uiBufLen-2]<<8) + ucBuf[uiBufLen-1];

	if (iRet == 0x9000)
	{
		for (int i = 0 ; i < 4 ; i++)
			*ulSector = (*ulSector<<(8*i)) + ucBuf[i+4];

		for (int i = 0 ; i < 2 ; i++)
			*uiRamLen = (*uiRamLen<<(8*i)) + ucBuf[i+8];
	}
	return iRet;
}


int CGetBitsDlg::SendBuf2Comm(BYTE* ucBuf , UINT ucBufLen)
{

	CByteArray byt;
	byt.RemoveAll();
	byt.SetSize(ucBufLen);
	for (UINT i = 0 ; i< ucBufLen ; i++)
		byt.SetAt(i,ucBuf[i]);

	m_ctrlComm.put_Output((COleVariant)byt);

	return true;
}


int CGetBitsDlg::ReceiveBuf4Comm(BYTE* ucBuf , UINT* ucBufLen)
{
	VARIANT variant_inp;   
	COleVariant safearray_inp;     

	safearray_inp=m_ctrlComm.get_Input();		//读缓冲区  

	safearray_inp.ChangeType (VT_ARRAY |VT_UI1);

	BYTE HUGEP *pbstr;
	HRESULT hr;
	hr=SafeArrayAccessData (safearray_inp.parray,(void HUGEP*FAR*)&pbstr);//获取安全数组指针
	if (FAILED (hr))
		AfxMessageBox("获取数组指针失败！");

	long num=0;

	hr=SafeArrayGetUBound (safearray_inp.parray,1,&num);

	if (FAILED (hr))
	{
		AfxMessageBox("获取数组上界失败！");
		return 0;
	}

	*ucBufLen = num+1;

	memcpy(ucBuf,pbstr, num+1);
	SafeArrayUnaccessData (safearray_inp.parray);
	return 1;
}


int CGetBitsDlg::GetBitsBuf(UINT uiOffset,ULONG32 ulSector,BYTE* bBits, UINT* uiBitsLen)
{
	int iRet ;
	BYTE ucCommand[13]  = {0xFE,0xB0,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	ucCommand[2] = (uiOffset>>8)&0xFF;
	ucCommand[3] = uiOffset&0xFF;


	if (uLong2Byte(ulSector,ucCommand+9) != 4)
		ZeroMemory(ucCommand+9,4);


	SendBuf2Comm (ucCommand,13);
	Sleep(50);
	if (!ReceiveBuf4Comm(bBits,uiBitsLen))
		return -1;
		
	iRet = (bBits[*uiBitsLen-2]<<8) + bBits[*uiBitsLen-1];
	* uiBitsLen = * uiBitsLen-2;
	return iRet;

}






int CGetBitsDlg::DisplayBitsData(BYTE* ucBits, UINT BitsLen)
{
	CString csOutput;
	BYTE bBYTE;

	for (UINT k = 0 ; k< BitsLen; k++)
	{

		bbits[bbitsLen] =  ucBits[k];

		if (bbitsLen == 0)
			bdiflen = (bbits[0]&0x7)+1;
		bbitsLen = bbitsLen+1;

		if (bbitsLen  == bdiflen)
		{

			//if ((bbits[1] != 0xFF)||(bbits[2] != 0xFF))
			//{
				_UcHex2CString(bbits,bbitsLen,csOutput);
				m_BitsList .AddString(csOutput);
				m_BitsList.SetCaretIndex(m_BitsList.GetCount());
			//}

			//m_BitsList .AddString(CLKDiffText(bbits,bbitsLen));


			if (_CLKDiffData2Byte_2(bbits,bbitsLen,&bBYTE) == _BYTE_Success)
			{
				m_BitsList .AddString(_T("--")+Int2CString(bBYTE,2));
				m_BitsList.SetCaretIndex(m_BitsList.GetCount());

				//m_BYTE_EDIT.SetSel(-1,-1);
				//m_BYTE_EDIT.ReplaceSel(Int2CString(bBYTE,2));

				BYTE bBYTES[300];
				UINT iLen = 0;
				CString csSend,csPro,csACK,csData,csLPro,csSW;
				int iRet = ByteData2TPDU(bBYTE,bBYTES,&iLen ,csSend,csPro,csACK,csData,csLPro,csSW);
				if ((iRet&0xF) == _BYTE_Success)
				{
					CString csTPDU;
					_UcHex2CString(bBYTES,iLen,csTPDU);
					m_TPDU.AddString(csTPDU);
					m_TPDU.SetCaretIndex(m_TPDU.GetCount());
					if (!csSW.IsEmpty())
					{
						if (((_CString2Int(csSW)&0xF000)!=0x9000)&&
							((_CString2Int(csSW)&0xF000)!=0x6000))
						{

							BERROR = 1;
							m_BitsList .AddString(_T("--ERROR--"));
							threadState = 0;
							//OnLbnDblclkList2();
							//AfxMessageBox("出错误了");


						}
					}
				}

			}


			//此处为Reset
			if ((bbits[0]&0x30) != 0x30) 
			{
				iPrescale = 372;
				iBitStatue = Def_Bit_S;
				iTPDUStatue = _TPDU_NULL;
				iByteLen = 0;
			}
			bbitsLen = 0;
			bdiflen  = 0;

		}
		else if (bdiflen == 0)
		{

			bbitsLen = 0;
			bdiflen  = 0;
		}

	}
	return 0;
}


int CGetBitsDlg::_DisplayBitsData(BYTE* ucBits, UINT BitsLen)
{
	CString csOutput;
	BYTE bBYTE;

	for (UINT k = 0 ; k< BitsLen; k++)
	{

		bbits[bbitsLen] =  ucBits[k];

		if (bbitsLen == 0)
			bdiflen = (bbits[0]&0x7)+1;
		bbitsLen = bbitsLen+1;

		if (bbitsLen  == bdiflen)
		{
			_UcHex2CString(bbits,bbitsLen,csOutput);
			m_BitsList .AddString(csOutput);
			if ((bbits[0]&0x30) != 0x30) 
			{
				iPrescale = 372;
				iBitStatue = Def_Bit_S;
				iTPDUStatue = _TPDU_NULL;
				iByteLen = 0;
			}
			bbitsLen = 0;
			bdiflen  = 0;

		}
		else if (bdiflen == 0)
		{

			bbitsLen = 0;
			bdiflen  = 0;
		}

	}
	return 0;
}





int CGetBitsDlg::_DisplayBitsData_Exp()
{
	CString csTemp;
	BYTE bits[20];
	int ibitsLen;
	BYTE bBYTE;

	if (m_BitsList.GetCount()<=iIndex)
		return 0;


	m_BitsList.GetText(iIndex,csTemp);

	ibitsLen = _CString2UcHex(csTemp,bits);
	
	if (_CLKDiffData2Byte_2(bits,ibitsLen,&bBYTE) == _BYTE_Success)
	{

		BYTE bBYTES[300];
		UINT iLen = 0;
		CString csSend,csPro,csACK,csData,csLPro,csSW;
		int iRet = ByteData2TPDU(bBYTE,bBYTES,&iLen ,csSend,csPro,csACK,csData,csLPro,csSW);
		if ((iRet&0xF) == _BYTE_Success)
		{
			CString csTPDU;
			_UcHex2CString(bBYTES,iLen,csTPDU);
			m_TPDU.AddString(csTPDU);
			m_TPDU.SetCaretIndex(m_TPDU.GetCount());
			if (!csSW.IsEmpty())
			{
				if (((_CString2Int(csSW)&0xF000)!=0x9000)&&
					((_CString2Int(csSW)&0xF000)!=0x6000))
				{

					BERROR = 1;
					//m_BitsList .AddString(_T("--ERROR--"));
					//threadState = 0;
					//OnLbnDblclkList2();
					//AfxMessageBox("出错误了");


				}
			}
		}

	}


	//此处为Reset
	if ((bbits[0]&0x30) != 0x30) 
	{
		iPrescale = 372;
		iBitStatue = Def_Bit_S;
		iTPDUStatue = _TPDU_NULL;
		iByteLen = 0;
	}

	iIndex+=1;
	return 1;
}
