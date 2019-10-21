#include "stdafx.h"
#include "Crypto.h"
#include "md5.h"
MD5_CTX context;
CRYPTO_API BOOL MD5_Hash(unsigned char* pucInput, int iDataLen, unsigned char pucOutput[16])
{
//	MD5_CTX context;
	MD5Init(&context);
    MD5Update (&context, pucInput, iDataLen);
    MD5Final  (pucOutput, &context);
	return TRUE;
}

CRYPTO_API BOOL MD5_Init()
{
	MD5Init(&context);
	return TRUE;
}
CRYPTO_API BOOL MD5_Update(unsigned char* pucInput, int iDataLen)
{
    MD5Update (&context, pucInput, iDataLen);
	return TRUE;
}
CRYPTO_API BOOL MD5_Final(unsigned char pucOutput[16])
{
    MD5Final  (pucOutput, &context);
	return TRUE;
}

