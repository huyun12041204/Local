//#include <vcl.h>
#include "stdafx.h"
#include "CCR5.h"


BYTE ucPrePcb = 0x40;
BYTE ucPreRes  = 0x00;
int iFlag      = 0;


BYTE GetnerateCheckSum(BYTE* ucTempdata,int iDataLen)
{
	BYTE ucCheckSum;
	ucCheckSum = 00;
	for (int i= 0; i< iDataLen ; i++)
	{
		ucCheckSum = ucCheckSum ^ ucTempdata[i];		
	}
	return ucCheckSum;
}



HANDLE Port_Open(int iPortNumber, int iBaudrate, int iParityFlag)
{

	HANDLE m_hCom;
	DCB		dcb;
	COMMTIMEOUTS	timeouts;
	BOOL	Error;

	char gszPort[5];

        gszPort[0]='C';
        gszPort[1]='O';
        gszPort[2]='M';
        gszPort[3]=(char)iPortNumber+0x30;
        gszPort[4]=0;

	m_hCom = CreateFile(gszPort,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
	if (m_hCom == INVALID_HANDLE_VALUE){
		//m_hCom = NULL;
		return NULL;}

	Error = GetCommTimeouts(m_hCom,&timeouts);
	if (!Error)	{
		//m_hCom = NULL;
		return NULL;
	}

	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 1000;//100;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 10;
	Error = SetCommTimeouts(m_hCom,&timeouts);

	if (!Error)	{
		//m_hCom = NULL;
		return NULL;
	}

	Error = GetCommState(m_hCom,&dcb);
	if (!Error)	{

		return NULL;
	}


	dcb.BaudRate = iBaudrate;
	dcb.ByteSize = 8;
	if ( iParityFlag != 0  ) 
	{
		dcb.fParity = TRUE;
		if ( iParityFlag == 2 ) dcb.Parity = EVENPARITY; else dcb.Parity = ODDPARITY;
	}
	else dcb.fParity = false;

	dcb.StopBits = ONESTOPBIT;

	Error = SetCommState(m_hCom,&dcb);
	if (!Error)	{
		//m_hCom = NULL;
		return NULL;
	}
	return m_hCom;
}



int Port_Close(HANDLE m_hCom)
{
	if ( m_hCom==NULL ) return CT_Error;

	if(!CloseHandle(m_hCom)) return CT_Error;
	m_hCom = NULL;
	return CT_OK;
}
int SetBaudRate(HANDLE m_hCom, int iBaudrate)
{
	DCB		dcb;
	COMMTIMEOUTS	timeouts;
	BOOL	Error;
	int iParityFlag = 2;

	Error = GetCommTimeouts(m_hCom,&timeouts);
	if (!Error)	{
		m_hCom = NULL;
		return CT_Error;
	}
	timeouts.ReadIntervalTimeout = 100;// 60;
	timeouts.ReadTotalTimeoutMultiplier = 1;//20;
	timeouts.ReadTotalTimeoutConstant = 1000;//100;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 10;

	Error = SetCommTimeouts(m_hCom,&timeouts);
	if (!Error)	{
		m_hCom = NULL;
		return CT_Error;
	}

	Error = GetCommState(m_hCom,&dcb);
	if (!Error)	{
		m_hCom = NULL;
		return CT_Error;
	}

	dcb.BaudRate = iBaudrate;
	dcb.ByteSize = 8;
	if ( iParityFlag != 0  ) 
	{
		dcb.fParity = TRUE;
		if ( iParityFlag == 2 ) dcb.Parity = EVENPARITY; else dcb.Parity = ODDPARITY;
	}
	else dcb.fParity = FALSE;

	dcb.StopBits = ONESTOPBIT;

	Error = SetCommState(m_hCom,&dcb);
	if (!Error)	{
		m_hCom = NULL;
		return CT_Error;
	}
	return CT_OK;
}



int ReadCharsFromPort(HANDLE hCom, unsigned char *cResp, unsigned int uiRespLen)
{
	DWORD nRecv;
	if ( !ReadFile(hCom, cResp, uiRespLen, &nRecv,NULL )|| nRecv!=uiRespLen)
	{   
		//read error
		PurgeComm(hCom, PURGE_RXCLEAR);
		return CT_Error;
	}
	return CT_OK;
}

int SendCharsToPort(HANDLE hCom, unsigned char *cSend, unsigned int uiSendLen)
{
	DWORD nSent;
	if(WriteFile(hCom, cSend, uiSendLen, &nSent, NULL) == 0)
	{
		PurgeComm(hCom, PURGE_TXCLEAR);
		return CT_Error;	//error
	}
	return CT_OK;
}

//
//int ReceData(HANDLE hTerm, BYTE ucReceDataTag, BYTE ucPcb,BYTE * ucpReceDataBuf, unsigned int * uipReceDataLen)
//{
//
//	BYTE rawResp[0x10];
//	int iLength = 0;
//	int iTimes = 0x00;
//	int iMultiplierTime = 10; 
//	int iCheckSum = CT_CheckSum;
//
//	// 获取 Command Tag
//	while(( ReadCharsFromPort(hTerm, rawResp, 1)!=CT_OK)&(iTimes <5))
//	{
//		Sleep(iMultiplierTime);
//		iTimes++;
//	}
//	if (rawResp[0] != ucReceDataTag)
//		return CT_Wrong_Tag;
//
//	//读取 Procedure byte
//	if (ReadCharsFromPort(hTerm, rawResp, 1)!=CT_OK)
//		return CT_Error;
//
//	if (rawResp[0] != ucPcb)
//		return CT_Wrong_Tag;
//
//	// 获取长度
//
//	if (ReadCharsFromPort(hTerm, rawResp, 1)!=CT_OK)
//		return CT_Error;
//
//	if(rawResp[0] == 0)
//		iLength = 0x100;
//	else
//		iLength = rawResp[0] +iCheckSum ;  //此处注意需要确定是否有校验位
//
//	*uipReceDataLen = iLength;
//
//	if( ReadCharsFromPort(hTerm, ucpReceDataBuf, *uipReceDataLen) != CT_OK)
//	{
//		*uipReceDataLen = 0;
//		return CT_Error;
//	}
//	return CT_OK;
//}


int ReceData(HANDLE hTerm, BYTE ucReceDataTag, BYTE *ucPcb,BYTE * ucpReceDataBuf, unsigned int * uipReceDataLen)
{

	BYTE rawResp[0x10];
	int iLength = 0;
	int iTimes = 0x00;
	int iMultiplierTime = 10; 
	int iCheckSum = CT_CheckSum;

	// 获取 Command Tag
	while(( ReadCharsFromPort(hTerm, rawResp, 1)!=CT_OK)&(iTimes <5))
	{
		Sleep(iMultiplierTime);
		iTimes++;
	}
	if (rawResp[0] != ucReceDataTag)
		return CT_Wrong_Tag;

	//读取 Procedure byte
	if (ReadCharsFromPort(hTerm, rawResp, 1)!=CT_OK)
		return CT_Error;

	*ucPcb = rawResp[0] ;
	//if (rawResp[0] != ucPcb)
	//	return CT_Wrong_Tag;

	// 获取长度

	if (ReadCharsFromPort(hTerm, rawResp, 1)!=CT_OK)
		return CT_Error;


	iLength = rawResp[0] +iCheckSum ;  //此处注意需要确定是否有校验位

	*uipReceDataLen = iLength;

	if( ReadCharsFromPort(hTerm, ucpReceDataBuf, iLength) != CT_OK)
	{
		*uipReceDataLen = 0;
		//return CT_Error;
	}
	return CT_OK;
}




int SendData(HANDLE hTerm, BYTE ucSendDataTag,BYTE ucPcb, BYTE * ucpSendDataBuf, unsigned int uiSendDataLen, int iChecksum = CT_CheckSum)
{
	BYTE  ucTempBuf[0xff],chksum[2];
	ucTempBuf[0] = ucSendDataTag;


	// 分别为Tag Pcb len
	ucTempBuf[0] = ucSendDataTag;
	ucTempBuf[1] = ucPcb;
	ucTempBuf[2] = uiSendDataLen;


	if(SendCharsToPort(hTerm,ucTempBuf,03) != CT_OK)
		return CT_Error;

	//data

	if(SendCharsToPort(hTerm,ucpSendDataBuf,uiSendDataLen) != CT_OK)
		return CT_Error;
	// checksum

	chksum[0] = GetnerateCheckSum(ucTempBuf,3);
	chksum[1] = GetnerateCheckSum(ucpSendDataBuf,uiSendDataLen);
	chksum[0] = GetnerateCheckSum(chksum,2);

	if(SendCharsToPort(hTerm,chksum,01) != CT_OK)
		return CT_Error;

	return CT_OK;

}

//int ExchangeCommand(HANDLE hTerm,BYTE *ucCommand ,unsigned int iCommandLen, BYTE *ucResponse, unsigned int *iResponseLen)
//{
//	int iRet;
//	BYTE ucTag,ucPcb;
//
//	ucTag = 0x12;
//
//	ucPcb = (ucPrePcb&0x40) ? 0xbf&ucPrePcb:0x40|ucPrePcb;  // 此处担心出现其他版本的Pcb
//	ucPrePcb = ucPcb;
//
//	iRet = SendData(hTerm,ucTag,ucPcb,ucCommand,iCommandLen,CT_CheckSum) ;
//	if (iRet != CT_OK )
//		return CT_Error_Send;
//
//	ucTag = 0x21;
//
//	iRet =  ReceData( hTerm, ucTag,ucPcb ,ucResponse, iResponseLen);
//	if (iRet != CT_OK )
//		return CT_Error_Rece;
//	return iRet;
//}


int SavePCB( BYTE ucPcb,BYTE ucResPcb)
{
	ucPrePcb = ucPcb;
	ucPreRes = ucResPcb;

	if ((ucPcb&0x80)!=0x80)
	{
	   iFlag = ~iFlag;
	}

	if (ucPcb == 0xc0)
	{
		ucPrePcb = 0x00;
	}

	if ((ucPreRes&0x0F)!=0x00) 
	{
		return ucPreRes;
	}



	return CT_OK;
}
BYTE GeneratePCB(int iSum,int iNum)
{
	BYTE ucResult;
	if (iSum==iNum) 
	{

	//	ucResult = (~ ucEndPcb)&0x40;

		if (iFlag == 0)
		{
			ucResult = 0x00;
		}
		else
			ucResult = 0x40;



	//	ucResult = (~ucPrePcb)&0x40;
		//if(((ucPreRes&0xBF)==0x00)&&((ucPreRes&0xBF)!=0x00))
		//{
		//	ucResult = (~ucPreRes)&0x40;	
		//}
		//else
		//{
		//	ucResult = (~ucPrePcb)&0x40;	
		//}

	}

	if (iSum>iNum) 
	{


		if (iFlag == 0)
		{
			ucResult = 0x20;
		}
		else
			ucResult = 0x60;


//		if((ucPreRes&0xBF)==0x00)
//		{
////			ucResult = (~ucPreRes)&0x40;	
//			ucResult = (~(ucPreRes&0xD0))&0x60;		
//		}
//		else
//		{
//			ucResult = (~(ucPrePcb&0xD0))&0x60;		
//		}
	
	}
	if (iSum<iNum) 
	{
		 
		ucResult = 0x80;
		if ((ucPreRes&0x40)==0)
		{
			ucResult = ucResult + 0x10;
		}
	}		
	return 	ucResult;

}



//BYTE GeneratePCB(BYTE ucPrPCB)
//{
//
//	BYTE ucCurPCB;
//
//	switch (ucPrPCB)
//	{
//	case 0x00:
//		ucCurPCB = 0x40;
//		break;
//	case 0x40:
//		ucCurPCB = 0x00;
//		break;
//	case 0x20:
//		ucCurPCB = 0x90;
//		break;
//	case 0x90:
//		ucCurPCB = 0x60;
//		break;
//	case 0x60:
//		ucCurPCB = 0x80;
//		break;
//	case 0x80:
//		ucCurPCB = 0x20;
//		break;
//	}
//
//	return ucCurPCB;
//
//}


int ExchangeCommand(HANDLE hTerm,BYTE *ucCommand ,unsigned int iCommandLen, BYTE *ucResponse, unsigned int *iResponseLen) 
{ 
	//int iRet;
	//int iSum,iNum; 
	//int iSendLen,iOffset;
	//BYTE ucTag,ucPcb,ucResPcb; 
	//BYTE ucTemp[10];
	//unsigned int uiTempLen,uiResOffset;  


	//iSum = iCommandLen /0xFE +1;
	//iNum = 0;
	//iOffset  = 0;
	//*iResponseLen = 0; // 由于每条都会带
	//uiResOffset   = 0;

	//do
	//{
	//	ucTag = 0x12; 
	//    iNum  = iNum+1;
	//	ucPcb = GeneratePCB(iSum,iNum);  // 此处担心出现其他版本的Pcb 

	//	if((ucPcb&0x80) == 00)
	//	{
	//		iSendLen = (iCommandLen - iOffset)>0xFE?0xFE:(iCommandLen - iOffset);           	
	//	}
	//	else
	//		iSendLen = 0;

	//	iRet  = SendData(hTerm,ucTag,ucPcb,ucCommand+iOffset,iSendLen,CT_CheckSum) ;

	//	iOffset = iOffset+iSendLen;

	//	if (iRet != CT_OK ) 
	//		return CT_Error_Send;  	

	//	if((ucPcb&0xA0)==0x20) 
	//	{
	//		ucTag = 0x21; 
	//		iRet =  ReceData( hTerm, ucTag,&ucResPcb,ucTemp,&uiTempLen); 
	//		if (iRet != CT_OK ) 
	//			return CT_Error_Rece;	 
	//	}   
	//	//if((ucPcb&0xA0)==0x80) 
	//	else	
	//	{
	//		ucTag     = 0x21; 
	//		uiTempLen = 0x00;

	//		iRet =  ReceData( hTerm, ucTag,&ucResPcb,ucResponse+uiResOffset,&uiTempLen); 

	//		if (iRet != CT_OK ) 
	//			return CT_Error_Rece;	 
	//		*iResponseLen  = uiResOffset + uiTempLen;
	//		uiResOffset    = *iResponseLen -1;
	//	}       

	//	iRet = SavePCB(ucPcb,ucResPcb); 
	//	if (iRet!= CT_OK) 
	//	{
	//		return iRet;

	//	}              

	//}
	//while((ucResPcb&0xB0)!=0x00);

	//return iRet;

	int iRet;
	BYTE ucTag,ucPcb;

	ucTag = 0x12;

	ucPcb = (ucPrePcb&0x40) ? 0xbf&ucPrePcb:0x40|ucPrePcb;  // 此处担心出现其他版本的Pcb
	ucPrePcb = ucPcb;

	iRet = SendData(hTerm,ucTag,ucPcb,ucCommand,iCommandLen,CT_CheckSum) ;
	if (iRet != CT_OK )
		return CT_Error_Send;

	ucTag = 0x21;

	iRet =  ReceData( hTerm, ucTag,&ucPcb ,ucResponse, iResponseLen);
	if (iRet != CT_OK )
		return CT_Error_Rece;
	return iRet;


}




int GetCTATR(HANDLE hTerm,BYTE*ucCTATR)
{
	//	BYTE ucATRHead[3];
	//	BYTE rawResp[0x10];
	int iLength = 0;
	int iTimes = 0x00;
	int iMultiplierTime = 10; 
	int iCheckSum = CT_CheckSum;

	int iRet;



	// 获取 synchronisation   category indicator length of ATR
	while(( ReadCharsFromPort(hTerm, ucCTATR, 03)!=CT_OK)&(iTimes <5))
	{
		Sleep(iMultiplierTime);
		iTimes++;
	}

	//获取ATR数据
	iRet =  ReadCharsFromPort(hTerm, ucCTATR+3, ucCTATR[2]+1);

	if (iRet == CT_OK)
		return ucCTATR[2]+3;
	else
		ZeroMemory(ucCTATR,0x1F);
	return 0;

}

int SetCTPTS(HANDLE hTerm,BYTE* ucPTS,int iPTSLen)
{
	BYTE *ucResult;
	int iLength = 0;
	int iTimes = 0x00;
	int iMultiplierTime = 10; 
	int iCheckSum = CT_CheckSum;

	int iRet;


	ucResult = new BYTE [iPTSLen];


	iRet = SendCharsToPort(hTerm,ucPTS,iPTSLen);
	if (iRet != CT_OK)
		return iRet;

	while(( ReadCharsFromPort(hTerm, ucResult, 03)!=CT_OK)&(iTimes <5))
	{
		Sleep(iMultiplierTime);
		iTimes++;
	}

	if (ucResult[0]!=0xff)
	{
		return CT_Error;
	}

	iRet = ReadCharsFromPort(hTerm, ucResult+3, iPTSLen - 3);
	if (iRet != CT_OK)
		return iRet;

	for (int i = 0;i<iPTSLen ; i++)
	{
		if (ucPTS[i]!= ucResult[i])
			return CT_Wrong_PTS;
	}

	return CT_OK;
}





int CCCR5 :: SetUpCCR550(int iPortNumber,BYTE* ucCTATR,int* iCTATRLen)
{


	hCom = Port_Open(iPortNumber,9600,2); // 2为even
	if (hCom == NULL)
		return CT_Error;
	*iCTATRLen =GetCTATR(hCom,ucCTATR);
	if (*iCTATRLen == 0)
	{
		hCom = NULL;
		return CT_Error;
	}
	return CT_OK;

}



int CCCR5 :: FreeCCR550()
{


	return Port_Close( hCom);


}

int CCCR5 ::SetDefaultCTPTS()
{
	BYTE *ucResult;
	int iLength = 0;
	int iTimes = 0x00;
	int iMultiplierTime = 10; 
	int iCheckSum = CT_CheckSum;

	int iRet;

	BYTE ucPTS[8] = {"\xff\x01\x03\x12\x00\x02\xed"};
	int iPTSLen   = 7;


	ucResult = new BYTE [iPTSLen];


	iRet = SendCharsToPort(hCom,ucPTS,iPTSLen);
	if (iRet != CT_OK)
		return iRet;



	while(( ReadCharsFromPort(hCom, ucResult, 01)!=CT_OK)&(iTimes <5))
	{
		Sleep(iMultiplierTime);
		iTimes++;
	}
	if (ucResult[0]!=0xff)
	{
		return CT_Error;
	}

	iRet = ReadCharsFromPort(hCom, ucResult+1, 02);
	if (iRet != CT_OK)
		return iRet;

	iRet = ReadCharsFromPort(hCom, ucResult+3, iPTSLen - 3);
	if (iRet != CT_OK)
		return iRet;

	for (int i = 0;i<iPTSLen ; i++)
	{
		if (ucPTS[i]!= ucResult[i])
			return CT_Wrong_PTS;
	}

	SetBaudRate(hCom,115200);
	return CT_OK;

}


int CCCR5 ::ConfirmCT()
{

	BYTE *ucResult;
	int iLength = 0;
	int iTimes = 0x00;
	int iMultiplierTime = 10; 
	int iCheckSum = CT_CheckSum;

	int iRet;

	BYTE ucConfirm[8] = {"\x12\xc0\x00\xd2"};
	int iPTSLen   = 4;


	ucResult = new BYTE [iPTSLen];


	iRet = SendCharsToPort(hCom,ucConfirm,iPTSLen);
	if (iRet != CT_OK)
		return iRet;


	while(( ReadCharsFromPort(hCom, ucResult, 01)!=CT_OK)&(iTimes <5))
	{
		Sleep(iMultiplierTime);
		iTimes++;
	}
	if (ucResult[0]!=0x21)
	{
		return CT_Error;
	}

	iRet = ReadCharsFromPort(hCom, ucResult+1, 02);
	if (iRet != CT_OK)
		return iRet;

	iRet = ReadCharsFromPort(hCom, ucResult+3,1);//checkSum
	return iRet;
}

int CCCR5 ::Setup_Write_Part1(BYTE * ucPart1Data,int iDatalen)
{
	memcpy(Command,"\x80\x81\x01\x01\x1f",5);
	Command[P3] = iDatalen;
	memcpy(Command+5,ucPart1Data,iDatalen);
	iResponseLen = 300;


	int iRet = ExchangeCommand(hCom,Command ,0x24, Response, &iResponseLen);
	if (iRet != CT_OK)
		return iRet;
	if ((Response[0]==0x90)&(Response[1]==0x00))
	{
		return CT_OK;
	}
	else
		return Response[0]*0x100+Response[1];
}
int CCCR5 ::Setup_Read_Part1(BYTE * ucPart1Data ,int iP2 = 00,int iP3 = 00)
{
	memcpy(Command,"\x80\x82\x01\x00\x1f",5);
	Command[P2]  = iP2;
	Command[P3]  = iP3;
	iResponseLen = 300;
	int iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);

	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucPart1Data,Response,iResponseLen);

		return CT_OK;
	}
	else
		return iSw;



}



int CCCR5 ::Setup_Write_Part2(BYTE * ucPart2Data,int iDatalen,int iP2 = 02)
{
	memcpy(Command,"\x80\x81\x01\x01\x1c",5);
	Command[P3] = iDatalen;
	memcpy(Command+5,ucPart2Data,iDatalen);
	iResponseLen = 300;

	Command[P2] = iP2;

	int iRet = ExchangeCommand(hCom,Command ,0x21, Response, &iResponseLen);
	if (iRet != CT_OK)
		return iRet;
	if ((Response[0]==0x90)&(Response[1]==0x00))
	{
		return CT_OK;
	}
	else
		return Response[0]*0x100+Response[1];
}
int CCCR5 ::Setup_Read_Part2( BYTE * ucPart2Data ,int iP2 = 02,int iP3 = 0x1c)
{
	memcpy(Command,"\x80\x82\x01\x00\x1c",5);
	Command[P2]  = iP2;
	Command[P3]  = iP3;
	iResponseLen = 300;
	int iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);

	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucPart2Data,Response,iResponseLen);
		return CT_OK;
	}
	else
		return iSw;
}


int CCCR5 ::Setup_Write_Part3(BYTE * ucPart3Data,int iP3 )
{
	memcpy(Command,"\x80\x81\x01\x11\x01",5);

	Command[P3] = iP3;
	memcpy(Command+5,ucPart3Data,iP3);
	iResponseLen = 300;


	int iRet = ExchangeCommand(hCom,Command ,05+iP3, Response, &iResponseLen);
	if (iRet != CT_OK)
		return iRet;
	if ((Response[0]==0x90)&(Response[1]==0x00))
	{
		return CT_OK;
	}
	else
		return Response[0]*0x100+Response[1];
}
int CCCR5 ::Setup_Read_Part3( BYTE * ucPart3Data  )
{
	memcpy(Command,"\x80\x82\x01\x11\x01\x04",6);
	iResponseLen = 300;
	int iRet = ExchangeCommand(hCom,Command ,6, Response, &iResponseLen);

	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucPart3Data,Response,iResponseLen);
		return CT_OK;
	}
	else
		return iSw;
}

int CCCR5 ::Reset_ICC(BYTE *ucICCRes,int iP1 = 01,int iP2 = 01)
{
	int iRet;

	Command[Cla] = CT_BasCla;
	Command[Ins] = CT_Reset;
	Command[P1] = iP1;
	Command[P2] = iP2;
	Command[P3] = 0x00;	

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if ((iSw == 0x9000)||(iSw == 0x9001))
	{
		if (iP2!=0)
			memcpy(ucICCRes,Response,iResponseLen-3);

		return iResponseLen-3;
	}
	else
		return iSw;
}


int CCCR5 ::Request_ICC(BYTE *ucICCRes,int iP2 = 01)
{
	int iRet;

	Command[Cla] = CT_BasCla;
	Command[Ins] = CT_Request;
	Command[P1]  = 0x00;
	Command[P2]  = iP2;
	Command[P3]  = 0x00;	

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if ((iSw == 0x9000)||(iSw == 0x9001))
	{
		if (iP2!=0)
			memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen-3;
	}
	else
		return iSw;



}

int CCCR5 ::Get_Statuse_CT_E(int iP2)
{

	int iRet;

	Command[Cla] = CT_BasCla;
	Command[Ins] = CT_GetStatuse;
	Command[P1]  = 0x00;
	Command[P2]  = iP2;
	Command[P3]  = 0x00;	
	iResponseLen = 300;

	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	if (iRet != CT_OK)
		return iRet;
	if ((Response[1]==0x90)&(Response[2]==0x00))
	{
		return Response[0];
	}
	else
		return Error_GetStatuse;
}



int CCCR5 ::Get_Statuse_CT()
{

	int iRet;

	Command[Cla] = CT_BasCla;
	Command[Ins] = CT_GetStatuse;
	Command[P1]  = 0x00;
	Command[P2]  = 0x80;
	Command[P3]  = 0x00;	
	iResponseLen = 300;

	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	if (iRet != CT_OK)
		return iRet;
	if ((Response[1]==0x90)&(Response[2]==0x00))
	{
		return Response[0];
	}
	else
		return Error_GetStatuse;
}

int CCCR5 ::SendPTS_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2,int iP3)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = ICC_PTS;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	
	memcpy(Command+5,ucICCPTS,iP3);

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5+iP3, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}

int CCCR5 ::Switch_Sequence_CT(BYTE *ucICCRes,int iP2 = 0)
{
	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_Sequence;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = 00;	
	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}
int CCCR5 ::Get_Sequence_CT( BYTE *ucICCRes,int iP2 = 0)
{
	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_GetSequence;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = 00;	
	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}

int CCCR5 ::Change_Signal_ICC(BYTE *ucICCRes,int iP2 = 0)
{
	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = ICC_ChangeSignal;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = 00;	
	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;

}
int CCCR5 ::Get_Time_ICC( BYTE *ucICCRes,int iP2 = 1,int iP3 = 0)
{
	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = ICC_GetTime;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	
	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}







int CCCR5 ::Trans_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2 = 0,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = ICC_Trans;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;

	if (iP3 <0x100)
	{
       Command[P3]  = iP3;	

	   memcpy(Command+5,ucICCPTS,iP3);


	}
	else
	if (iP3>0xff)
	{
	   Command[P3]  = 00;	
	   Command[P3+1]  = iP3/0x100;	
	   Command[P3+2]  = iP3%0x100;	
	   memcpy(Command+7,ucICCPTS,iP3);
	}
	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5+iP3, Response, &iResponseLen);

	if (iResponseLen<3)
	{
		return CT_Not_Res;
	}

	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	memcpy(ucICCRes,Response,iResponseLen-3);
	return iResponseLen - 3;	
}


int CCCR5 ::Send_Trans_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2 = 0,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = ICC_SendTrans;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	
	memcpy(Command+5,ucICCPTS,iP3);

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5+iP3, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}


int CCCR5 ::Trans2CMD_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2 = 1,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = ICC_Trans2CMD;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	
	memcpy(Command+5,ucICCPTS,iP3);

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5+iP3, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}


int CCCR5 ::Trans2CMD_Clock_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2 = 1,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = ICC_Trans2CMDClock;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	
	memcpy(Command+5,ucICCPTS,iP3);

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5+iP3, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}

int CCCR5 ::Get_Response2CMD_ICC(BYTE *ucICCRes,int iP2 = 0,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = ICC_GetRes2CMD;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}
int CCCR5 ::Set_Parity_CT(BYTE*ucICCPTS,int iP2 = 1,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_SetParity;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	
	memcpy(Command+5,ucICCPTS,iP3);

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5+iP3, Response, &iResponseLen);
	iSw =  Response[0]*0x100+Response[1];
	if (iSw == 0x9000)
		return CT_OK;
	else
		return iSw;
}

int CCCR5 ::Set_Listen_CT(BYTE*ucICCPTS,int iP2 = 1,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_SetListen;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	
	memcpy(Command+5,ucICCPTS,iP3);

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5+iP3, Response, &iResponseLen);
	iSw =  Response[0]*0x100+Response[1];
	if (iSw == 0x9000)
		return CT_OK;
	else
		return iSw;
}


int CCCR5 ::Get_Listen_Response_CT(BYTE *ucICCRes,int iP2 = 0,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_GetListenRes;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}


int CCCR5 ::Set_Ttigger_CT()
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_GetListenRes;
	Command[P1]  = 0x01;
	Command[P2]  = 0x01;
	Command[P3]  = 00;	

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[0]*0x100+Response[1];
	if (iSw == 0x9000)
	{

		return CT_OK;	
	}
	else
		return iSw;
}


int CCCR5 ::Statuse_CT(BYTE*ucICCRes,int iP2 = 0,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_Statuse;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}



int CCCR5 ::Config_CT(BYTE*ucICCSend,int iP2 = 1,int iP3 = 0)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_Config;
	Command[P1]  = 0x01;
	Command[P2]  = iP2;
	Command[P3]  = iP3;	
	memcpy(Command+5,ucICCSend,iP3);

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5+iP3, Response, &iResponseLen);
	iSw =  Response[0]*0x100+Response[1];
	if (iSw == 0x9000)
		return CT_OK;
	else
		return iSw;
}
int CCCR5 ::Version_CT(BYTE*ucICCRes)
{

	int iRet;
	Command[Cla] = CT_SpeCla;
	Command[Ins] = CT_Version;
	Command[P1]  = 0x00;
	Command[P2]  = 0x00;
	Command[P3]  = 0x00;	

	iResponseLen = 300;
	iRet = ExchangeCommand(hCom,Command ,5, Response, &iResponseLen);
	iSw =  Response[iResponseLen -3]*0x100+Response[iResponseLen -2];
	if (iSw == 0x9000)
	{
		memcpy(ucICCRes,Response,iResponseLen-3);
		return iResponseLen - 3;	
	}
	else
		return iSw;
}



int CCCR5::GetPortReader(int iPort)
{

	BYTE CT_ATR[30];
	int  iCT_ATRLen;
	int iRet;
	ZeroMemory(CT_ATR,30);
	iRet = SetUpCCR550(iPort,CT_ATR,&iCT_ATRLen);
	if (iRet!= CT_OK)
		return iRet;
	iRet = SetDefaultCTPTS();
	if (iRet!= CT_OK)
		return iRet;
	iRet = ConfirmCT();
	return iRet;

}


int CCCR5::GetCTStatuse(int *iICC_Statuse)
{
	int iRet;

	iRet = Get_Statuse_CT_E(0x81);
	iRet = Get_Statuse_CT();

	if (iRet!= Error_GetStatuse)
	{
		*iICC_Statuse = iRet;
		return ICC_OK;
	}
	else 
		return iRet;
}



//int CCCR5:: PowerOnCCR5()
//{
// int iRet;
// int iICC_Statuse;
// BYTE ICC_ATR[30];
// iRet = GetCTStatuse(&iICC_Statuse);
// if (iICC_Statuse != Statuse_Ready)
//	 return WrongStatuse;
// iRet = Reset_ICC(ICC_ATR,P1_PowerRs,P2_ATR);
// if (iRet > 0xff)
//	 return iRet;
// iRet = GetCTStatuse(&iICC_Statuse);
// if (iICC_Statuse != Statuse_OnVol)
//	 return ResetFalse;
// return CT_OK;
//}

int CCCR5::PowerOffCCR5()
{
	int iRet;
	BYTE CT_Response[255];
	int  iCT_ResLen,iICC_Statuse;
	ZeroMemory (CT_Response,255);
	iRet = Switch_Sequence_CT(CT_Response,03);
	if (iRet <= 0xff)
	{
		iCT_ResLen  = iRet;
		iRet = GetCTStatuse(&iICC_Statuse);
		if (iICC_Statuse != Statuse_OnVol)
		{
			return CT_OK;
		}
		return CT_Error;
	}
	else
		return iRet;

}




int CCCR5::SetCTBasPara(int iCLK ,int iVol ,int iFac ,int iICWT ,int iWWT, int iCWT,int iBWT,int iContol,int iExecution)
{


	int iRet;
	int iWTemp;
	BYTE Part1Para[0x50];

	int iICC_Statuse;

	iRet = GetCTStatuse(&iICC_Statuse);
	if (iRet !=CT_OK)
	{
		return iRet;
	}



	ZeroMemory (Part1Para,0x1f);
	iRet = Setup_Read_Part1(Part1Para,Basic_Para,0x1f);
	if (iRet != CT_OK)
		return iRet;


	if (iCLK!=0)
	{
		Part1Para[CLK_H] = iCLK/0x100;
		Part1Para[CLK_L] = iCLK%0x100;
		iHisCLK        = iCLK;
	}
	else
		iHisCLK  = Part1Para[CLK_H]*0x100+Part1Para[CLK_L];

	if (iVol!=0)
	{
		Part1Para[Vol_H] = iVol/0x100;
		Part1Para[Vol_L] = iVol%0x100;
		iHisVol       = iVol;
	}
	else
		iHisVol  = Part1Para[Vol_H]*0x100+Part1Para[Vol_L];


	if (iFac!=0)
	{
		Part1Para[Fac_H] = iFac/0x100;
		Part1Para[Fac_L] = iFac%0x100;
		iHisFac  = iFac;
	}
	else
		iHisVol  = Part1Para[Fac_H]*0x100+Part1Para[Fac_L];


	if (iICWT!=0)
	{
		//ICWT
		iWTemp = iICWT*iHisCLK/iHisFac;
		Part1Para[0x9] = iWTemp / 0x10000;
		Part1Para[10] =(iWTemp/0x100)%0x100;
		Part1Para[11] = iWTemp%0x100;
	}




	if ( iBWT!=0)
	{
		//BWT
		iWTemp = iBWT*iHisCLK/iHisFac;
		Part1Para[6] = iWTemp / 0x10000;
		Part1Para[7] =(iWTemp/0x100)%0x100;
		Part1Para[0x8] = iWTemp%0x100;
	}



	if (iCWT!=0)
	{
		//CWT
		Part1Para[12] = iCWT / 0x10000;
		Part1Para[13] =(iCWT/0x100)%0x100;
		Part1Para[15] = iCWT%0x100;
	}



	if (iWWT!=0)
	{
		//WWT
		iWTemp = iWWT*iHisCLK/iHisFac;
		Part1Para[15] = iWTemp / 0x10000;
		Part1Para[16] =(iWTemp/0x100)%0x100;
		Part1Para[17] = iWTemp%0x100;
	}



	if (iICC_Statuse == Statuse_OnVol)
		iContol = iContol|0x04;
	else 
		iContol = iContol&0xfb;

	Part1Para[18] = iContol;


	Part1Para[19] = iExecution;


	iRet = Setup_Write_Part1(Part1Para,0x1f);
	if (iRet != CT_OK)
		return iRet;

	iRet = Setup_Read_Part1(Part1Para,Basic_Para,0x1f);
	if (iRet != CT_OK)
		return iRet;

	if ((iHisCLK != (Part1Para[CLK_H]*0x100 +Part1Para[CLK_L]))||(iHisVol != (Part1Para[Vol_H]*0x100 +Part1Para[Vol_L]))||(iHisFac != (Part1Para[Fac_H]*0x100 +Part1Para[Fac_L])))
	{
		return SetupWriteFalse;
	}
	return CT_OK;

}



int CCCR5::SetCTDefaultPara(void)
{
	int iRet;
	BYTE BasPara[50]= {"\x0d\xf3\x13\x88\x01\x74\x00\x3c\x0b\x00\x25\x80\x00\x20\x0b\x00\x33\x9d\x83\x10\x14\x00\xc8\x00\x64\x00\x32\x00\xc8\x04\x4c"};
	iRet = Setup_Write_Part1(BasPara,0x1f);
	if (iRet != CT_OK)
		return iRet;

	BYTE SquPara[50]={"\x01\x00\x9c\x40\x20\x00\x0a\x11\x03\xe8\x71\x17\x70\x61\x17\xd4\x31\x1b\x58\x21\x1d\x1a\x00\x00\x00\x00\x00\x00"};
	iRet = Setup_Write_Part2(SquPara,0x1c,02);
	if (iRet != CT_OK)
		return iRet;
	memcpy(SquPara,"\x01\x00\x9c\x40\x20\x00\x0a\x21\x00\xd2\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",0x1c);
	iRet = Setup_Write_Part2(SquPara,0x1c,04);
	if (iRet != CT_OK)
		return iRet;

	iRet = Setup_Read_Part3(BasPara);
	if (iRet != CT_OK)
		return iRet;
	memcpy(BasPara,"\x00\x90\x00",03);
	iRet = Setup_Write_Part3(BasPara,03);
	if (iRet != CT_OK)
		return iRet;
	return CT_OK;




}

int CCCR5::SetSquencesPara(void)
{
	int iRet;
	BYTE SquPara[0x1c];
	memcpy(SquPara,"\x01\x00\x9c\x40\x20\x00\x0a\x11\x03\xe8\x71\x17\x70\x61\x17\xd4\x31\x1b\x58\x21\x1d\x1a\x00\x00\x00\x00\x00\x00",0x1c);

	iRet = Setup_Write_Part2(SquPara,0x1c,02);
	if (iRet != CT_OK)
		return iRet;

	memcpy(SquPara,"\x01\x00\x9c\x40\x20\x00\x0a\x21\x00\xd2\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",0x1c);

	iRet = Setup_Write_Part2(SquPara,0x1c,04);
	if (iRet != CT_OK)
		return iRet;
	return CT_OK;

}




//int CCCR5::SendPPS(BYTE* ucPPS,BYTE* ucRes)
//{

// int iRet;
// iRet = SendPTS_ICC(ucPPS,ucRes,00,04);
// if (iRet != CT_OK)
//	 return iRet;
// return CT_OK;
//}

int CCCR5::SendPPS_Ex(BYTE* ucPPS,BYTE* ucRes,int*iResLen)
{

	int iRet,iF,iD;
	int i;


	ZeroMemory(ucRes,04);

	iRet  = GenerateTransData(ucPPS,ucRes,04,iResLen);
	if (iRet != CT_OK)
		return iRet;

	if(ucRes[0] == 0x00)
		return PTSNotResponse;

	if (ucRes[1] == 0)
		return PTSICCNotSupport;


	for ( i= 0 ; i<*iResLen;i++)
	{
		if (ucPPS[i] !=ucRes[i])
			return PTSWrongResponse ;
	}


	iF = ucRes[2]/0x10;
	switch(iF )
	{
	case 01:
		iF = 372;
		break;
	case  0x09:
		iF = 512;
		break;
	default:
		iF = 0;
		break;
	}

	if (iF == 0)
		return PTSWrongChara;

	iD = 1;
	for ( i = 0 ;i< ((ucRes[2]%0x10) -1);i++)
		iD = iD*2;
	if (iD >iF)
	{
		return PTSWrongChara;
	}

	iRet = SetCTBasPara(iHisCLK,iHisVol,iF/iD,1000,1376,8203,1601,0x87);
	return iRet;

}


int CCCR5::SwitchSequence(int iP2,int *iCT_ResLen,BYTE *CT_Response)
{


	int iRet;
	*iCT_ResLen = 0;
	iRet = Switch_Sequence_CT(CT_Response,iP2);
	if (iRet > 0xff)
		return iRet;

	*iCT_ResLen  = iRet; 
	return CT_OK;

}




int CCCR5::PowerOn_Ex(BYTE *ICC_ATR,int *iICC_ATRLen)
{
	int iRet;
	int iTempLen;
	BYTE CT_Response[255];
	int  iCT_ResLen,iICC_Statuse;
	iRet = GetCTStatuse(&iICC_Statuse);
	if (iICC_Statuse != Statuse_Ready)
		return WrongStatuse;
	iRet = SwitchSequence(02,&iCT_ResLen,CT_Response);
	if (iRet != CT_OK)
		return iRet;
	iTempLen = 0;
	for (int i = 4 ; i < iCT_ResLen  ; i=i+2) //范围需要排除SW 所以 -2 
	{
		if (CT_Response[i+1] == 0x80)
		{
			ICC_ATR[iTempLen] = CT_Response[i];
			iTempLen ++;
		}
	}



	*iICC_ATRLen = iTempLen;
	if (*iICC_ATRLen == 0)
		return _NoATR;
	iRet = GetCTStatuse(&iICC_Statuse);
	if (iICC_Statuse != Statuse_OnVol)
		return ResetFalse;
	return CT_OK;
}



int CCCR5::SendCommand_T0(BYTE* ucSend,int iSendLen, BYTE* ucResponse,int* iResLen, int* iTime)
{

	int iRet;
	int iTempLen = 0;


	iRet = GenerateTransData(ucSend,ucResponse,iSendLen,iResLen);
	if (iRet!=CT_OK)
		return iRet;
	*iTime  = iWorkTime;
	if (iSendLen>=5)
	{
		if ((ucSend[P3]!=00)&&(*iResLen!=2))
		{
			*iResLen = *iResLen -1;
			memcpy( ucResponse,ucResponse+1,*iResLen);
		}
	}
	return CT_OK;
}


int CCCR5:: GenerateTransData(BYTE* ucICCCom,BYTE* ucICCRes,int iICCComLen,int* iICCResLen)
{

	BYTE ucTransCom[1000];
	BYTE ucTransRes[1000];
	int iRet;
	*iICCResLen= 0;

	ZeroMemory(ucTransCom,1000);
	ZeroMemory(ucTransRes,1000);

	for (int i= 0 ; i < iICCComLen ; i++)
	{
		ucTransCom[2*i] = ucICCCom[i];
		ucTransCom[2*i+1] = 00;
	}
	iWorkTime = 0;
	iRet = Trans_ICC(ucTransCom,ucTransRes,00,2*iICCComLen);
	if (iRet<0)
	{
		return iRet;
	}
	
	if (iRet>1000)
	{
		switch (iRet)
		{
		case 0x62f2:
			break;
		}
		
	}
	if (iRet >3)
	{
		iRet = iRet-3;
		iWorkTime = ucTransRes[iRet]*0x10000+ucTransRes[iRet +1]*0x100 +ucTransRes[iRet+2];
	}


	for (int i= 0 ; i < iRet/2 ; i++)
	{
		if ((ucTransRes[2*i+1]&0x80) == 0x80)
		{	
			ucICCRes[ *iICCResLen] =ucTransRes[2*i];
			*iICCResLen =  *iICCResLen +1;
		}
	}

	return CT_OK;

}



int CCCR5::SetInterruptTime(int iTimes)
{


	int iRet,iDefTime;
	BYTE ucPart2Data[0x1c];
	int iOffset;


	memcpy(ucPart2Data,"\x10\x00\x00\x00",4);

	//signal1
	iDefTime = 0x9F;

	iOffset = 4;
	ucPart2Data[iOffset]= 0x20;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)/0x100;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)%0x100;
	iOffset = iOffset +1;

	//signal2


	iDefTime = iDefTime+1;
    ucPart2Data[iOffset]= 0x30;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)/0x100;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)%0x100;
	iOffset = iOffset +1;

	//signal3

	iDefTime = iDefTime+1;

	ucPart2Data[iOffset]= 0x60;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)/0x100;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)%0x100;
	iOffset = iOffset +1;

	//signal4

	iDefTime = iDefTime+1;

	ucPart2Data[iOffset]= 0x70;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)/0x100;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)%0x100;
	iOffset = iOffset +1;

	//signal5

	iDefTime = iDefTime+1;

	ucPart2Data[iOffset]= 0x10;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)/0x100;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)%0x100;
	iOffset = iOffset +1;
	

	//signal6

	iDefTime = iDefTime+1;

	ucPart2Data[iOffset]= 0x40;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)/0x100;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)%0x100;
	iOffset = iOffset +1;

	//signal7

	iDefTime = iDefTime+1;

	ucPart2Data[iOffset]= 0x80;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)/0x100;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= (iTimes+iDefTime)%0x100;
	iOffset = iOffset +1;


	//signal7

	iDefTime = iDefTime+1;

	ucPart2Data[iOffset]= 0x00;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= 0x00;
	iOffset = iOffset +1;
	ucPart2Data[iOffset]= 0x00;
	iOffset = iOffset +1;


	iRet  = Setup_Write_Part2(ucPart2Data,0x1c,0x08);



	return iRet;
}

int CCCR5::SetInterrupt(int iFlag)
{
	int iRet;
	//0为不修改参数
    iRet = SetCTBasPara(0,0,0,0,0,0,0,0x83,iFlag);
	return iRet;

}
