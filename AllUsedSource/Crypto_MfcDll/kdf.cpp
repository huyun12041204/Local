#include "StdAfx.h"
#include <stdlib.h>
#include "Crypto.h"
CRYPTO_API void KDF1(unsigned char *ucKey, unsigned int uiKeyLen, unsigned char *ucData, unsigned int uiDataLen, unsigned int uiNewKeyLen, unsigned char *ucNewKey, unsigned int uiMode)
{
	unsigned char * pucDataBuf, *pucOutBuf;
	pucDataBuf = (unsigned char *)malloc(uiKeyLen + uiDataLen);
	pucOutBuf = (unsigned char *)malloc(100);

	memcpy(pucDataBuf, ucKey, uiKeyLen);
	memcpy(pucDataBuf + uiKeyLen, ucData, uiDataLen);

	hash(pucDataBuf, uiKeyLen + uiDataLen, pucOutBuf, uiMode);
	memcpy(ucNewKey, pucOutBuf, uiNewKeyLen);
	return;
}

CRYPTO_API void KDF2(unsigned char *ucKey, unsigned int uiKeyLen, unsigned char *ucData, unsigned int uiDataLen, unsigned int uiNewKeyLen, unsigned char *ucNewKey, unsigned int uiMode)
{
	unsigned char * pucDataBuf, *pucOutBuf;
	unsigned char c[4];
	unsigned long uiCounter, uiHashDataLen, uiHashLen, uiOffset;
	pucDataBuf = (unsigned char *)malloc(uiKeyLen + uiDataLen + 4);
	pucOutBuf = (unsigned char *)malloc(100);
	uiCounter = 1;
	uiOffset = 0;
	do{
        c[0]=(unsigned char)(uiCounter>>24)&0xff;
        c[1]=(unsigned char)(uiCounter>>16)&0xff;
        c[2]=(unsigned char)(uiCounter>>8)&0xff;
        c[3]=(unsigned char)(uiCounter)&0xff;
	
	    memcpy(pucDataBuf, ucKey, uiKeyLen);
	    memcpy(pucDataBuf + uiKeyLen, c, 4);
	    memcpy(pucDataBuf + uiKeyLen + 4, ucData, uiDataLen);
        uiHashDataLen = uiKeyLen + 4 + uiDataLen;
	    uiHashLen = hash(pucDataBuf, uiHashDataLen, pucOutBuf, uiMode);
		uiCounter += 1;
		if(uiNewKeyLen - uiOffset < uiHashLen)
		{
			memcpy(ucNewKey + uiOffset, pucOutBuf, uiNewKeyLen - uiOffset);
			uiOffset += uiNewKeyLen - uiOffset;
		}
		else
		{
			memcpy(ucNewKey + uiOffset, pucOutBuf, uiHashLen);
		    uiOffset += uiHashLen;
		}
	}while(uiCounter <= ((uiNewKeyLen/uiHashLen)+1));
	free(pucOutBuf);
	free(pucDataBuf);

	return;
}

CRYPTO_API void KDF(unsigned char *ucKey, unsigned int uiKeyLen, unsigned char *ucData, unsigned int uiDataLen, unsigned int uiNewKeyLen, unsigned char *ucNewKey, unsigned int uiKDFMode, unsigned int uiHashMode)
{
    if(uiKDFMode == DEF_KDF_KDF1)
		KDF1(ucKey, uiKeyLen, ucData, uiDataLen, uiNewKeyLen, ucNewKey, uiHashMode);
	else if(uiKDFMode == DEF_KDF_KDF2)
		KDF2(ucKey, uiKeyLen, ucData, uiDataLen, uiNewKeyLen, ucNewKey, uiHashMode);
}

