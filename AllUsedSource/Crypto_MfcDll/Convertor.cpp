// Convertor.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "Convertor.h"
#include "string.h"
#include <stdlib.h>

CONVERTOR_API int gbk_to_ucs2(const unsigned char *ucpGbk, int iGbkLen, unsigned short *uipUcs, int iUcsLen)
{
	return MultiByteToWideChar(0, 0, (const char *)ucpGbk, iGbkLen, uipUcs, iUcsLen);
}

CONVERTOR_API int ucs2_to_gbk(const unsigned short *uipUcs, int iUcsLen, unsigned char *ucpGbk, int iGbkLen)
{
	return WideCharToMultiByte(0, 0, uipUcs, iUcsLen, (char *)ucpGbk, iGbkLen, NULL, NULL);
}

CONVERTOR_API int gbk_to_ucs2(const unsigned char *ucpGbk, int iGbkLen, unsigned char *ucpUcs, int iUcsLen)
{
	int i;
    unsigned short * uipUcs;
	int iUcsDataLen = 0;
	if(iUcsLen < (iGbkLen * 2))
		return 0;
	uipUcs = (unsigned short *)malloc(iUcsLen);
    iUcsDataLen = gbk_to_ucs2(ucpGbk, iGbkLen, uipUcs, iUcsLen);
	if(iUcsLen < (iUcsDataLen * 2))
		return 0;
	for(i=0; i<iUcsDataLen; i++)
	{
		*(ucpUcs + i * 2)     = ((*(uipUcs + i)) >> 8) & 0xff;
		*(ucpUcs + i * 2 + 1) = (*(uipUcs + i)) & 0xff;
	}
	return iUcsDataLen * 2;
}

CONVERTOR_API int ucs2_to_gbk(const unsigned char *ucpUcs, int iUcsLen, unsigned char *ucpGbk, int iGbkLen)
{
	int i;
    unsigned short * uipUcs;
	int iGbkDataLen = 0;
    if(iUcsLen > (iGbkLen * 2))
		return 0;
	if(iUcsLen%2)
		return 0;
	iUcsLen = iUcsLen/2;
	uipUcs = (unsigned short *)malloc(iUcsLen);
	for(i=0; i<iUcsLen; i++)
	{
        *(uipUcs + i) = ((*(ucpUcs + i * 2)) * 0x100) + (*(ucpUcs + i * 2 + 1));
	}
    
	iGbkDataLen = ucs2_to_gbk(uipUcs, iUcsLen, ucpGbk, iGbkLen);
	return iGbkDataLen;
}
