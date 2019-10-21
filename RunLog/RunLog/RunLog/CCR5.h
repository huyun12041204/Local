#ifndef CCR5H
#define CCR5H

//---------------------------------------------------------------------------

#pragma once


#define   CT_PTSOK                0
#define   CT_PTSNotSppot         -1


#define   CT_OK               0
#define   CT_INVALID         -1
#define   CT_Error           -8
#define   CT_Not_Res         -9
#define   CT_TRANS          -10
#define   CT_MEMORY         -11 
#define   CT_HOST          -127  
#define   CT_HTSI          -128

#define  CT_Error_Send     -9
#define  CT_Error_Rece     -10


#define   CT_Wrong_Tag    -1
#define   CT_Wrong_Pcb    -2

#define   CT_Wrong_PTS    -3


#define   CT_CheckSum      1
#define   CT_UnCheckSum    0

#define   Error_GetStatuse   -0xf1

#define   Cla  00
#define   Ins  01
#define   P1   02
#define   P2   03
#define   P3   04

#define  CT_BasCla      0x20
#define  CT_SpeCla      0x80

#define  CT_Reset       0x11
#define  CT_Request     0x12
#define  CT_GetStatuse  0x13
#define  CT_Eject       0x15  // µ¯³ö



#define  ICC_PTS     0x18
#define  CT_SetupWrite     0x81
#define  CT_SetupRead      0x82

#define CT_Sequence        0x80
#define CT_GetSequence     0x84

#define ICC_ChangeSignal     0x85

#define ICC_GetTime         0x86

#define ICC_Trans           0x87
#define ICC_SendTrans       0x8c

#define  ICC_Trans2CMD      0x89

#define ICC_Trans2CMDClock  0xA0

#define  ICC_GetRes2CMD     0x8A

#define  CT_SetParity      0x88

#define  CT_SetListen      0x8D

#define  CT_GetListenRes    0x8E
#define  CT_SetTrigger     0x8F

#define  CT_Statuse   0x16
#define  CT_Config    0x17
#define  CT_Version   0x19




#define  ICC_OK  00;
#define  ICC_Err -8; 


// Reset CT
#define  P1_SoftRs   00
#define  P1_PowerRs  01
#define  P1_PinRs    81

#define  P2_NoData   00
#define  P2_ATR      01
#define  P2_HisChara 02


#define  ResetFalse  -0x10
#define  _NoATR       -0x11
#define  WrongATR    -0x12

//Statuse ICC

#define  Statuse_NoCar 00
#define  Statuse_Ready  03
#define  Statuse_NoCon  01
#define  Statuse_OnVol  05

#define  WrongStatuse  -0xf0

//Setup Read

#define  Basic_Para 01

#define  CLK_H      00
#define  CLK_L      01

#define  Vol_H      02
#define  Vol_L      03


#define  Fac_H      04
#define  Fac_L      05

// Setup Write

#define  SetupWriteFalse  -0x20;


// Trans

#define PPSFalse         -0x40
#define PTSICCNotSupport  01
#define PTSNotResponse    02
#define PTSWrongResponse -0x41
#define PTSWrongChara    -0x42


//interrupt

#define Rupt_N         0x10
#define Rupt_R         0xE0


class CCCR5
{

protected:
	HANDLE hCom;
	BYTE Command[1000];
	BYTE Response[1000];
	unsigned int iResponseLen;
	int iSw;

	int SetUpCCR550(int iPortNumber,BYTE* ucCTATR,int* iCTATRLen);
	int SetDefaultCTPTS( );
	int ConfirmCT( );
	int Setup_Write_Part1(BYTE * ucPart1Data,int iDatalen);

	int Setup_Read_Part1(BYTE * ucPart1Data ,int iP2 ,int iP3);
	int Setup_Write_Part2(BYTE * ucPart2Data,int iDatalen,int iP2 );
	int Setup_Read_Part2( BYTE * ucPart2Data ,int iP2 ,int iP3 );


	int Setup_Write_Part3(BYTE * ucPart3Data,int iP3 = 0x01);
	int Setup_Read_Part3( BYTE * ucPart3Data );
	int Reset_ICC(BYTE *ucICCRes,int iP1,int iP2);
	int Request_ICC(BYTE *ucICCRes,int iP2 );
	int Get_Statuse_CT();

        int Get_Statuse_CT_E(int iP2);
	int SendPTS_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2,int iP3);
	int Switch_Sequence_CT(BYTE *ucICCRes,int iP2);
	int Get_Sequence_CT(BYTE *ucICCRes,int iP2 );
	int Change_Signal_ICC(BYTE *ucICCRes,int iP2 );
	int Get_Time_ICC(BYTE *ucICCRes,int iP2,int iP3);



	int Trans_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2,int iP3);

	int Send_Trans_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2,int iP3);

	int Trans2CMD_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2 ,int iP3);

	int Trans2CMD_Clock_ICC(BYTE*ucICCPTS,BYTE *ucICCRes,int iP2 ,int iP3);

	int Get_Response2CMD_ICC(BYTE *ucICCRes,int iP2 ,int iP3);

	int Set_Parity_CT(BYTE*ucICCPTS,int iP2,int iP3);

	int Set_Listen_CT(BYTE*ucICCPTS,int iP2 ,int iP3);

	int Get_Listen_Response_CT(BYTE *ucICCRes,int iP2 ,int iP3);

	int Set_Ttigger_CT( );

	int Statuse_CT( BYTE*ucICCRes,int iP2 ,int iP3);

	int Config_CT(BYTE*ucICCSend,int iP2 ,int iP3 );

	int Version_CT(BYTE*ucICCRes);

	int ExchangeCommand_Ex(HANDLE hTerm,BYTE ucPcb,BYTE *Command ,unsigned int CommandLen, BYTE *Response, unsigned int *ResponseLen);


public:

	int iWorkTime;
	int iHisFac;
	int iHisVol;
	int iHisCLK;

	int GetPortReader(int iPort);
	//	int PowerOnCCR5();
	//	int SendPPS(BYTE* ucPPS,BYTE* ucRes);
	int PowerOffCCR5();
	int GetCTStatuse(int *iICC_Statuse);
	int SetCTBasPara(int iCLK = 3571 ,int iVol = 5000 ,int iFac = 372,int iICWT = 1000 ,int iWWT = 1376, int iCWT =8203,int iBWT = 1601,int iContol = 0x83, int iExecution = 0x10);
        //int SetCTBasPara(int iCLK = 3201 ,int iVol = 1800 ,int iFac = 372,int iICWT = 1000 ,int iWWT = 1376, int iCWT =8203,int iBWT = 1601,int iContol = 0x83, int iExecution = 0x10);

	int GenerateTransData(BYTE* ucICCCom,BYTE* ucICCRes,int iICCComLen,int* iICCResLen);
	int SetSquencesPara(void);
	int SetCTDefaultPara(void);

	int SwitchSequence(int iP2,int *iCT_ResLen,BYTE *CT_Response);
	int SendPPS_Ex(BYTE* ucPPS,BYTE* ucRes,int*iResLen);

	int PowerOn_Ex(BYTE *ICC_ATR,int *iICC_ATRLen);
	int SendCommand_T0(BYTE* ucSend,int iSendLen, BYTE* ucResponse,int* iResLen, int* iTime);
public:
	int SetInterruptTime(int iTimes);
public:
	int SetInterrupt(int iFlag);

	int FreeCCR550();

};

//---------------------------------------------------------------------------
#endif

