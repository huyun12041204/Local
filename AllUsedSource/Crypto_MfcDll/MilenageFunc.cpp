#include "stdafx.h"
#include "Crypto.h"
#include "UNI_Aut_Algorithm.h"

CMilenage StMilenage;

CRYPTO_API BOOL Milenage_Set_C(unsigned char * pucInputC1, unsigned char * pucInputC2, unsigned char * pucInputC3, unsigned char * pucInputC4, unsigned char * pucInputC5)
{
	StMilenage.mv_SetC1(pucInputC1);
	StMilenage.mv_SetC2(pucInputC2);
	StMilenage.mv_SetC3(pucInputC3);
	StMilenage.mv_SetC4(pucInputC4);
	StMilenage.mv_SetC5(pucInputC5);
	return TRUE;
}

CRYPTO_API BOOL Milenage_Set_R(int iR1, int iR2, int iR3, int iR4, int iR5)
{
	int iRot[5];
	iRot[0] = iR1;
	iRot[1] = iR2;
	iRot[2] = iR3;
	iRot[3] = iR4;
	iRot[4] = iR5;
	StMilenage.mv_SetRot(iRot);
	return TRUE;

}

CRYPTO_API BOOL Milenage_Compute_OP_c(unsigned char * pucOP_c)
{
	StMilenage.mv_ComputeOPc(pucOP_c);
	return TRUE;
}

CRYPTO_API BOOL Milenage_Set_OP(unsigned char * pucOP)
{
	StMilenage.mv_SetOP(pucOP);
	Milenage_Set_OP_Ind("OP");
	return TRUE;
}

CRYPTO_API BOOL Milenage_Set_OP_c(unsigned char * pucOP_c)
{
	StMilenage.mv_SetOP_c(pucOP_c);
	Milenage_Set_OP_Ind("OP_c");
	return TRUE;
}

CRYPTO_API BOOL Milenage_Set_OP_Ind(CString csOpInd)
{
	StMilenage.mv_SetOPind(csOpInd);
	return TRUE;
}

CRYPTO_API BOOL Milenage_f1(unsigned char * pucK, unsigned char * pucRand, unsigned char * pucSQN, unsigned char * pucAMF, unsigned char * pucMAC_A)
{
    StMilenage.mv_f1( pucK, pucRand, pucSQN, pucAMF, pucMAC_A);
	return TRUE;
}

CRYPTO_API BOOL Milenage_f1star(unsigned char * pucK, unsigned char * pucRand, unsigned char * pucSQN, unsigned char * pucAMF, unsigned char * pucMAC_S)
{
    StMilenage.mv_f1star( pucK, pucRand, pucSQN, pucAMF, pucMAC_S);
	return TRUE;
}

CRYPTO_API BOOL Milenage_f2345(unsigned char * pucK, unsigned char * pucRand, unsigned char * pucRes, unsigned char * pucCK, unsigned char * pucIK, unsigned char * pucAK)
{
    StMilenage.mv_f2345( pucK, pucRand, pucRes, pucCK, pucIK, pucAK );
	return TRUE;
}

CRYPTO_API BOOL Milenage_f5star(unsigned char * pucK, unsigned char * pucRand, unsigned char * pucAK)
{
    StMilenage.mv_f5star( pucK, pucRand, pucAK );
	return TRUE;
}

CRYPTO_API BOOL Milenage_Generate_SRES(unsigned char * pucXRES,  unsigned char * pucSRES)
{
	int i;
	for(i = 0; i< 4; i++)
	{
	    *(pucSRES + i) = *(pucXRES + i) ^ *(pucXRES + i + 4);
 	}
	return TRUE;
}

CRYPTO_API BOOL Milenage_Generate_Kc(unsigned char * pucCK, unsigned char * pucIK, unsigned char * pucKc)
{
	int i;
	for(i = 0; i< 8; i++)
	{
	    *(pucKc + i) = *(pucCK + i) ^ *(pucCK + i + 8) ^ *(pucIK + i) ^ *(pucIK + i + 8);
 	}
	return TRUE;
}





