#include "stdafx.h"
#include "Crypto.h"
#include "c_cave.h"
c_cave StCave;

// initialize the SSD_A
CRYPTO_API BOOL InitSSD_A(char cSSD_A[8]) 
{
	memcpy(StCave.SSD_A, cSSD_A, 8);
	return TRUE;
}
// initialize the SSD_B
CRYPTO_API BOOL InitSSD_B(char cSSD_B[8]) 
{
	memcpy(StCave.SSD_B, cSSD_B, 8);
	return TRUE;
}

// initialize the Akey
CRYPTO_API BOOL InitAKey(char cAKey[8]) 
{
	memcpy(StCave.A_key, cAKey, 8);
	return TRUE;
}

// initialize the ESN
CRYPTO_API BOOL InitESN(char * pcESN) 
{
	memcpy(StCave.ESN, pcESN, 4);
	return TRUE;
}

// Varify the Akey, input Akey digits
CRYPTO_API BOOL A_Key_Verify(/*in*/char A_KEY_DIGITS[26], /*out*/char cAkey[8]) 
{
	BOOL bRet;
	bRet = StCave.A_Key_Verify(A_KEY_DIGITS);
	memcpy(cAkey, StCave.A_key, 8);
	return bRet;
}

// generate the SSD, should init the Akey before use this function
CRYPTO_API BOOL SSD_Generation(/*in*/const unsigned char RANDSSD[7], /*out*/ char cSSD_A[8], /*out*/ char cSSD_B[8]) 
{
	StCave.SSD_Generation(RANDSSD);
	memcpy(cSSD_A, StCave.SSD_A_NEW, 8);
	memcpy(cSSD_B, StCave.SSD_B_NEW, 8);
	return TRUE;
}

// 
CRYPTO_API BOOL Auth_Signature(const unsigned char RAND_CHALLENGE[4], /*randbs */ const unsigned char AUTH_DATA[3], //IMSi_m_S1
			const unsigned char *SSD_AUTH/*ssd-A*/, const int SAVE_REGISTERS/*目前写的是0*/, char * pAuthBS) 

{
	unsigned long ulRet;
	ulRet = StCave.Auth_Signature(RAND_CHALLENGE, AUTH_DATA, SSD_AUTH, SAVE_REGISTERS);
    *(pAuthBS) =  (char) ((ulRet >> 16) & 0xff);
    *(pAuthBS + 1) = (char) ((ulRet >> 8) & 0xff);
    *(pAuthBS + 2) = (char) ((ulRet ) & 0xff);
    return TRUE;
}

// update SSD
CRYPTO_API BOOL UpdateSSD(void)
{
	StCave.SSD_Update();
	return TRUE;
}

// Generate Key and VPM, should initialize the SSD_B and ESN before call this function
CRYPTO_API BOOL Key_VPM_Generation(char * key, char * vpm)
{
	StCave.Key_VPM_Generation();
	memcpy(key, StCave.cmeakey, 8);
	memcpy(vpm, StCave.VPM, 65);
	return TRUE;
}








