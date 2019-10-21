#include "StdAfx.h"
#include "sha1.h"
#include "sha2.h"
#include "rmd160.h"
#include "Crypto.h"
CRYPTO_API int hmac(unsigned char key[], unsigned long key_len, unsigned char data[], unsigned long data_len, unsigned char mac[], unsigned long mac_len, unsigned int uiMode)
{
	int iRet;
	switch(uiMode)
	{
	case DEF_HASH_SHA1:
		hmac_sha1(key, key_len, data, data_len,mac, mac_len);
		iRet = mac_len;
		break;
	case DEF_HASH_SHA224:
		hmac_sha224(key, key_len, data, data_len,mac, mac_len);
		iRet = mac_len;
		break;
	case DEF_HASH_SHA256:
		hmac_sha256(key, key_len, data, data_len,mac, mac_len);
		iRet = mac_len;
		break;
	case DEF_HASH_SHA384:
		hmac_sha384(key, key_len, data, data_len,mac, mac_len);
		iRet = mac_len;
		break;
	case DEF_HASH_SHA512:
		hmac_sha512(key, key_len, data, data_len,mac, mac_len);
		iRet = mac_len;
		break;
	case DEF_HASH_RMD160:
		hmac_rmd160(key, key_len, data, data_len,mac, mac_len);
		iRet = mac_len;
		break;
	}
	return iRet;
}

CRYPTO_API int dochmac(unsigned char key[], unsigned long key_len, unsigned char data[], unsigned long data_len, unsigned char mac[], unsigned long mac_len, unsigned int uiMode)
{
	int i, iBlockSize, iHashDataLen, iHashLen;
	unsigned char *t1, *t2, *t3, *t4, *t5;
	iBlockSize = 0x40;
	iHashDataLen = iBlockSize + data_len;
	t1 = (unsigned char *)malloc(iBlockSize);
	t2 = (unsigned char *)malloc(iHashDataLen);
	t3 = (unsigned char *)malloc(64);
	t5 = (unsigned char *)malloc(64);
	
	memset(t1, 0x00, iBlockSize);
	memset(t2, 0x00, iHashDataLen);
	memset(t3, 0x00, 64);
	memset(t5, 0x00, 64);
	if(key_len > (unsigned)iBlockSize)
	{
		free(t1);
		free(t2);
		free(t3);
		free(t5);
		return 0;
	}
	memcpy(t1, key, key_len);
	for(i=0; i<iBlockSize; i++)
	{
		t2[i] = t1[i] ^ 0x36;
	}
	memcpy(t2 + iBlockSize, data, data_len);
	iHashLen = hash(t2, iHashDataLen, t3, uiMode);
	if(iHashLen == 0x00)
	{
		free(t1);
		free(t2);
		free(t3);
		free(t5);
		return 0;
	}
	iHashDataLen = iBlockSize + iHashLen;
	t4 = (unsigned char *)malloc(iHashDataLen);
	memset(t4, 0x00, iHashDataLen);
	for(i=0; i<iBlockSize; i++)
	{
		t4[i] = t1[i] ^ 0x5c;
	}
	memcpy(t4 + iBlockSize, t3, iHashLen);
	iHashLen = hash(t4, iHashDataLen, t5, uiMode);
	if(iHashLen == 0x00)
	{
		free(t1);
		free(t2);
		free(t3);
		free(t4);
		free(t5);
		return 0;
	}
	if(mac_len > (unsigned)iHashLen)
	{
		free(t1);
		free(t2);
		free(t3);
		free(t4);
		free(t5);
		return 0;
	}
	memcpy(mac, t5, mac_len);
	free(t1);
	free(t2);
	free(t3);
	free(t4);
	free(t5);
	return mac_len;
}

CRYPTO_API int hash(unsigned char *message, unsigned int len, unsigned char *digest, unsigned int uiMode)
{
	int iRet;

	switch (uiMode)
	{
	case DEF_HASH_SHA1:
		sha1(message, len, digest);
		iRet = SHA1_DIGEST_SIZE;
		break;
	case DEF_HASH_SHA224:
		sha224(message, len, digest);
		iRet = SHA224_DIGEST_SIZE;
		break;
	case DEF_HASH_SHA256:
		sha256(message, len, digest);
		iRet = SHA256_DIGEST_SIZE;
		break;
	case DEF_HASH_SHA384:
		sha384(message, len, digest);
		iRet = SHA384_DIGEST_SIZE;
		break;
	case DEF_HASH_SHA512:
		sha512(message, len, digest);
		iRet = SHA512_DIGEST_SIZE;
		break;
	case DEF_HASH_RMD160:
		rmd160(message, len, digest);
		iRet = RMD160_DIGEST_SIZE;
		break;
	}
    return iRet;
}