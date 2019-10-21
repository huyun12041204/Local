#include "stdafx.h"
#include "string.h"
#include <stdlib.h>
#include "c_feal32.h"
#include "c_stephi.h"
#include "Crypto.h"


_int64 CharStr_to_Int64(unsigned char * ucpCharStr, int iLen)
{ 
  _int64  i64Value = 0x00;
  unsigned char  ucChar;
  int  ii;
  for (ii=0;ii<iLen;ii++)
  {
    ucChar = ucpCharStr[ ii];
    i64Value = (i64Value << 0x08) + ucChar;
  }
  return i64Value;
}

int Int64_to_CharStr( _int64 i64InVal, unsigned char * ucpOutData, int iOutLen)
{
	int i=0;
	memset(ucpOutData, 0x00, iOutLen);
	for(i=0;i<iOutLen;i++)
		*(ucpOutData + iOutLen - 1 - i) = unsigned char (((i64InVal >> (8 * i))) & 0xff);
	return 0;
}

CRYPTO_API void Feal32(/*in*/ BYTE rand[8],/*in*/ BYTE key[8],/*out*/ BYTE output[8])
{
	c_feal32 StFeal;
	_int64 i64Rand, i64Key, i64OutPut;
	i64Rand = CharStr_to_Int64(rand, 0x08);
	i64Key = CharStr_to_Int64(key, 0x08);
	StFeal.Set_Random(i64Rand);
	StFeal.Set_Authkey(i64Key);
	i64OutPut = StFeal.Get_AuthReSult();
	Int64_to_CharStr(i64OutPut, output, 0x08);
	return;
}
CRYPTO_API void Stephi(/*in*/ BYTE rand[8],/*in*/ BYTE key[8],/*out*/ BYTE output[8])
{
	c_stephi stStephi;
	_int64 i64Rand, i64Key, i64OutPut;
	i64Rand = CharStr_to_Int64(rand, 0x08);
	i64Key = CharStr_to_Int64(key, 0x08);
	stStephi.Set_Random(i64Rand);
	stStephi.Set_Authkey(i64Key);
	i64OutPut = stStephi.Get_AuthReSult();
	Int64_to_CharStr(i64OutPut, output, 0x08);
	return;
}