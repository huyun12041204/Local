#include "stdafx.h"
#include "Crypto.h"

CRYPTO_API BOOL     DocMilenage_f1(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucSQN, unsigned int uiSQNLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucAMF, unsigned int uiAMFlen, unsigned char * pucMAC_A, unsigned int uiMAC_ALen, unsigned int uiKdfMode, unsigned int uiHashMode)
{
// 01
// 1. Define a new variable FID as "function identifier." FID = 0x01.
// 2. Let key = K.
// 3. Let data = FID || SQN || MID || RAND || AMF.
// 4. Let tLen = 8.
// 5. Compute MAC-A = HMAC (key, data, tLen).
// 02
// 1. Define a new variable FID as "function identifier." FID = 0x01.
// 2. Let key = K.
// 3. Let data = MID || FID || SQN || RAND || AMF.
// 4. Let tLen = 8.
// 5. Compute MAC-A = HMAC (key, data, tLen).

	unsigned int uiBufferLen;
	unsigned char * pucBuffer;
	unsigned int iC;
	int iRet;
	uiBufferLen = 1 + uiSQNLen + uiMIDLen + uiRandLen + uiAMFlen;
	uiBufferLen = uiBufferLen + 8;
	pucBuffer = (unsigned char *)malloc(uiBufferLen);
	memset(pucBuffer, 0x00, uiBufferLen);
	if(uiKdfMode == DEF_KDF_KDF1)
	{
	iC = 0;
	pucBuffer[iC] = 0x01;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucSQN, uiSQNLen);
	iC += uiSQNLen;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	memcpy(pucBuffer + iC, pucAMF, uiAMFlen);
	iC += uiAMFlen;
	}
	else if (uiKdfMode == DEF_KDF_KDF2)
	{
	iC = 0;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	pucBuffer[iC] = 0x01;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucSQN, uiSQNLen);
	iC += uiSQNLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	memcpy(pucBuffer + iC, pucAMF, uiAMFlen);
	iC += uiAMFlen;
	}
	iRet = dochmac(pucK, uiKeyLen, pucBuffer, iC, pucMAC_A, uiMAC_ALen, uiHashMode);
	if((unsigned)iRet != uiMAC_ALen)
		return FALSE;
    return TRUE;
}

CRYPTO_API BOOL DocMilenage_f1star(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucSQN, unsigned int uiSQNLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucAMF, unsigned int uiAMFlen, unsigned char * pucMAC_S, unsigned int uiMAC_SLen, unsigned int uiKdfMode, unsigned int uiHashMode)
{
// 01
// 1. Define a new variable FID as "function identifier." FID = 0x02.
// 2. Let key = K.
// 3. Let data = FID || SQNMS || MID || RAND || AMF.
// 4. Let tLen = 8.
// 5. Compute MAC-S = HMAC (key, data, tLen).
/*
02
1. Define a new variable FID as "function identifier." FID = 0x02.
2. Let key = K.
3. Let data = MID || FID || SQNMS || RAND || AMF.
4. Let tLen = 8.
5. Compute MAC-S = HMAC (key, data, tLen).
*/
	unsigned int uiBufferLen;
	unsigned char * pucBuffer;
	unsigned int iC;
	int iRet;
	uiBufferLen = 1 + uiSQNLen + uiMIDLen + uiRandLen + uiAMFlen;
	uiBufferLen = uiBufferLen + 8;
	pucBuffer = (unsigned char *)malloc(uiBufferLen);
	memset(pucBuffer, 0x00, uiBufferLen);
	if(uiKdfMode == DEF_KDF_KDF1)
	{
	iC = 0;
	pucBuffer[iC] = 0x02;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucSQN, uiSQNLen);
	iC += uiSQNLen;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	memcpy(pucBuffer + iC, pucAMF, uiAMFlen);
	iC += uiAMFlen;
	}
	else if(uiKdfMode == DEF_KDF_KDF2)
	{
	iC = 0;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	pucBuffer[iC] = 0x02;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucSQN, uiSQNLen);
	iC += uiSQNLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	memcpy(pucBuffer + iC, pucAMF, uiAMFlen);
	iC += uiAMFlen;
	}
	iRet = dochmac(pucK, uiKeyLen, pucBuffer, iC, pucMAC_S, uiMAC_SLen, uiHashMode);
	if((unsigned) iRet != uiMAC_SLen)
		return FALSE;
    return TRUE;	
}

CRYPTO_API BOOL DocMilenage_f2(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucRES, unsigned int uiRESLen, unsigned int uiKdfMode, unsigned int uiHashMode)
{
// 01
// 1. Define a new variable FID as "function identifier." FID = 0x03.
// 2. Let key = K.
// 3. Let data = FID || MID || RAND.
// 4. Let tLen = the appropriate length for RES.
// 5. Compute RES = HMAC (key, data, tLen).
	/*
02
1. Define a new variable FID as "function identifier." FID = 0x03.
2. Let key = K.
3. Let data = MID || FID || RAND.
4. Let tLen = the appropriate length for RES.
5. Compute RES = HMAC (key, data, tLen).
*/

	unsigned int uiBufferLen;
	unsigned char * pucBuffer;
	unsigned int iC;
	int iRet;
	uiBufferLen = 1 + uiMIDLen + uiRandLen;
	uiBufferLen = uiBufferLen + 8;
	pucBuffer = (unsigned char *)malloc(uiBufferLen);
	memset(pucBuffer, 0x00, uiBufferLen);
	if(uiKdfMode == DEF_KDF_KDF1)
	{
	iC = 0;
	pucBuffer[iC] = 0x03;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	else if(uiKdfMode == DEF_KDF_KDF2)
	{
	iC = 0;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	pucBuffer[iC] = 0x03;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	iRet = dochmac(pucK, uiKeyLen, pucBuffer, iC, pucRES, uiRESLen, uiHashMode);
	if((unsigned) iRet != uiRESLen)
		return FALSE;
    return TRUE;	
}

CRYPTO_API BOOL DocMilenage_f3(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucCK, unsigned int uiCKLen, unsigned int uiKDFMode, unsigned int uiHashMode)
{
// 01
// 1. Define a new variable FID as "function identifier." FID = 0x04.
// 2. Let key = K.
// 3. Let para = FID || MID || RAND.
// 4. Let nLen = 16.
// 5. Compute CK = KDF1 (key, para, nLen).
/*
02
1. Define a new variable FID as "function identifier." FID = 0x04.
2. Let key = K.
3. Let para = MID || FID || RAND.
4. Let nLen = 16.
5. Compute CK = KDF2 (key, para, nLen).


*/
	unsigned int uiBufferLen;
	unsigned char * pucBuffer;
	unsigned int iC;
	uiBufferLen = 1 + uiMIDLen + uiRandLen;
	uiBufferLen = uiBufferLen + 8;
	pucBuffer = (unsigned char *)malloc(uiBufferLen);
	memset(pucBuffer, 0x00, uiBufferLen);
	if(uiKDFMode == DEF_KDF_KDF1)
	{
	iC = 0;
	pucBuffer[iC] = 0x04;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	else if(uiKDFMode == DEF_KDF_KDF2)
	{
	iC = 0;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	pucBuffer[iC] = 0x04;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	KDF(pucK, uiKeyLen, pucBuffer, iC, uiCKLen, pucCK, uiKDFMode, uiHashMode);
    return TRUE;	
}
CRYPTO_API BOOL DocMilenage_f4(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucIK, unsigned int uiIKLen, unsigned int uiKDFMode, unsigned int uiHashMode)
{
// 01
// 1. Define a new variable FID as "function identifier." FID = 0x05.
// 2. Let key = K.
// 3. Let para = FID || MID || RAND.
// 4. Let nLen = 16.
// 5. Compute IK = KDF1 (key, para, nLen).
/*
02
1. Define a new variable FID as "function identifier." FID = 0x05.
2. Let key = K.
3. Let para = MID || FID || RAND.
4. Let nLen = 16.
5. Compute IK = KDF2 (key, para, nLen).
*/
	unsigned int uiBufferLen;
	unsigned char * pucBuffer;
	unsigned int iC;
	uiBufferLen = 1 + uiMIDLen + uiRandLen;
	uiBufferLen = uiBufferLen + 8;
	pucBuffer = (unsigned char *)malloc(uiBufferLen);
	memset(pucBuffer, 0x00, uiBufferLen);
	if(uiKDFMode == DEF_KDF_KDF1)
	{
	iC = 0;
	pucBuffer[iC] = 0x05;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	else if(uiKDFMode == DEF_KDF_KDF2)
	{
	iC = 0;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	pucBuffer[iC] = 0x05;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	KDF(pucK, uiKeyLen, pucBuffer, iC, uiIKLen, pucIK, uiKDFMode, uiHashMode);
    return TRUE;	
}
CRYPTO_API BOOL DocMilenage_f5(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucAK, unsigned int uiAKLen, unsigned int uiKDFMode, unsigned int uiHashMode)
{
// 01
// 1. Define a new variable FID as "function identifier." FID = 0x06.
// 2. Let key = K.
// 3. Let para = FID || MID || RAND.
// 4. Let nLen = 6.
// 5. Compute AK = KDF1 (key, para, nLen).
/*
02
1. Define a new variable FID as "function identifier." FID = 0x06.
2. Let key = K.
3. Let para = MID || FID || RAND.
4. Let nLen = 6.
5. Compute AK = KDF2 (key, para, nLen).
*/
	unsigned int uiBufferLen;
	unsigned char * pucBuffer;
	unsigned int iC;
	uiBufferLen = 1 + uiMIDLen + uiRandLen;
	uiBufferLen = uiBufferLen + 8;
	pucBuffer = (unsigned char *)malloc(uiBufferLen);
	memset(pucBuffer, 0x00, uiBufferLen);
	if(uiKDFMode == DEF_KDF_KDF1)
	{
	iC = 0;
	pucBuffer[iC] = 0x06;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	else if(uiKDFMode == DEF_KDF_KDF2)
	{
	iC = 0;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	pucBuffer[iC] = 0x06;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	KDF(pucK, uiKeyLen, pucBuffer, iC, uiAKLen, pucAK, uiKDFMode, uiHashMode);
    return TRUE;	
}
CRYPTO_API BOOL DocMilenage_f5star(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucAK_S, unsigned int uiAK_SLen, unsigned int uiKDFMode, unsigned int uiHashMode)
{
// 01
// 1. Define a new variable FID as "function identifier." FID = 0x07.
// 2. Let key = K.
// 3. Let para = FID || MID || RAND.
// 4. Let nLen = 6.
// 5. Compute AK-S = KDF1 (key, para, nLen).
/*
02
1. Define a new variable FID as "function identifier." FID = 0x07.
2. Let key = K.
3. Let para = MID || FID || RAND.
4. Let nLen = 6.
5. Compute AK-S = KDF2 (key, para, nLen).

*/
	unsigned int uiBufferLen;
	unsigned char * pucBuffer;
	unsigned int iC;
	uiBufferLen = 1 + uiMIDLen + uiRandLen;
	uiBufferLen = uiBufferLen + 8;
	pucBuffer = (unsigned char *)malloc(uiBufferLen);
	memset(pucBuffer, 0x00, uiBufferLen);
	if(uiKDFMode == DEF_KDF_KDF1)
	{
	iC = 0;
	pucBuffer[iC] = 0x07;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	else if(uiKDFMode == DEF_KDF_KDF2)
	{
	iC = 0;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	pucBuffer[iC] = 0x07;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
	}
	KDF(pucK, uiKeyLen, pucBuffer, iC, uiAK_SLen, pucAK_S, uiKDFMode, uiHashMode);
    return TRUE;	
}

CRYPTO_API BOOL DocMilenage_f345(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucCK, unsigned int uiCKLen, unsigned char *  pucIK, unsigned int uiIKLen, unsigned char * pucAK, unsigned int uiAKLen, unsigned int uiKDFMode, unsigned int uiHashMode)
{
// 01
// 1. Define a new variable FID as "function identifier." FID = 0x04.
// 2. Let key = K.
// 3. Let para = FID || MID || RAND.
// 4. Let nLen = 16.
// 5. Compute CK = KDF1 (key, para, nLen).

	unsigned int uiBufferLen;
	unsigned char * pucBuffer, * pucNewKey;
	unsigned int iC;
	uiBufferLen = 1 + uiMIDLen + uiRandLen;
	uiBufferLen = uiBufferLen + 8;
	pucBuffer = (unsigned char *)malloc(uiBufferLen);
	pucNewKey = (unsigned char *)malloc(50);
	memset(pucNewKey, 0x00, 50);

	memset(pucBuffer, 0x00, uiBufferLen);
	if(uiKDFMode == DEF_KDF_KDF1)
	{
	iC = 0;
	pucBuffer[iC] = 0x08;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
    KDF(pucK, uiKeyLen, pucBuffer, iC, 20, pucNewKey, uiKDFMode, uiHashMode);
	
	iC = 0;
	pucBuffer[iC] = 0x09;
	iC += 0x01;
	memcpy(pucBuffer + iC, pucMID, uiMIDLen);
	iC += uiMIDLen;
	memcpy(pucBuffer + iC, pucRand, uiRandLen);
	iC += uiRandLen;
    KDF(pucK, uiKeyLen, pucBuffer, iC, 20, pucNewKey+20, uiKDFMode, uiHashMode);
	}
	else
		return FALSE;
	if((uiCKLen + uiIKLen + uiAKLen) > 40)
		return false;
	memcpy(pucCK, pucNewKey, uiCKLen);
	memcpy(pucIK, pucNewKey + uiCKLen, uiIKLen);
	memcpy(pucAK, pucNewKey + uiCKLen + uiIKLen, uiAKLen);
    return TRUE;	
}



