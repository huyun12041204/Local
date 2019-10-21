
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CONVERTOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CONVERTOR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CONVERTOR_EXPORTS
#define CONVERTOR_API __declspec(dllexport)
#else
#define CONVERTOR_API __declspec(dllimport)
#endif

CONVERTOR_API void A3A8(/*in*/ BYTE rand[16], /*in*/ BYTE key[16],/*out*/ BYTE simoutput[12]);
// contains all declarations to be exported to the interpreter
// (defines, typedefs, variables and functions

CONVERTOR_API void Feal32(/*in*/ BYTE rand[8], /*in*/ BYTE key[8], /*out*/ BYTE output[8]);
CONVERTOR_API void Stephi(/*in*/ BYTE rand[8], /*in*/ BYTE key[8], /*out*/ BYTE output[8]);

CONVERTOR_API int gbk_to_ucs2(const unsigned char *ucpGbs, int iGbsLen, unsigned short *uipUniBuf, int iUniBufLen);
CONVERTOR_API int ucs2_to_gbk(const unsigned short *uipUcs, int iUcsLen, unsigned char *ucpGbkBuf, int iGbkBufLen);
CONVERTOR_API int gbk_to_ucs2(const unsigned char *ucpGbs, int iGbsLen, unsigned char *ucpUniBuf, int iUniBufLen);
CONVERTOR_API int ucs2_to_gbk(const unsigned char *ucpUcs, int iUcsLen, unsigned char *ucpGbkBuf, int iGbkBufLen);

// contains local declarations not exported to the interpreter
/** ***************************
 **  Char_to_Hex
 **  change one char to hex string, do not check the limit of the output string  
 **  '1' = '\x31' -> "31" , 'A' = '\x41' -> "41", 'a' = '\x61' -> "61"
 ** **************************/
CONVERTOR_API void Char_to_Hex(unsigned char ucChar, unsigned char* ucpHex);

/** ***************************
 **  AscII_to_Hex
 **  change one char to hex   
 **  '1' = '\x31' -> 0x01 , 'A' = '\x41' -> 0x0a , 'a' = '\x61' -> 0x0a
 ** **************************/
CONVERTOR_API unsigned char AscII_to_Hex(unsigned char ucAsc);

/** ***************************
 **  HexStr_to_CharStr
 **  "31a0" -> "\x31\xa0" iLen = 2
 ** **************************/
CONVERTOR_API void HexStr_to_CharStr(  const unsigned char *ucpHexStr, int iLen, unsigned char *ucpCharStr); 

/** ***************************
 **  CharStr_to_HexStr
 **  change char string to hex string
 **  "12Aa" = "\x31\x32\x41\x61" -> "31324161" iLen = 4
 ** **************************/
CONVERTOR_API void CharStr_to_HexStr(  const unsigned char *ucpCharStr, int iLen, unsigned char *ucpHexStr);

/** ***************************
 **  HexStr_to_Long
 **  change char hex string to long
 **  "12Aa" -> 0x12aa iLen = 4
 ** **************************/
CONVERTOR_API unsigned long HexStr_to_Long(  unsigned char * ucpHexStr, int iLen);

/** ***************************
 **  CharStr_to_Long
 **  change char string to long
 **  "\x12\xAa" -> 0x12aa, "32a" -> 0x333265 iLen = 2
 ** **************************/
CONVERTOR_API unsigned long CharStr_to_Long(unsigned char * ucpCharStr, int iLen);

/** ***************************
 **  CharStr_to_Int64
 **  Change char string to Int 64
 **  "\x12\x34\x56\x78\x9a\xbc\xde\xf0" -> 0x123456789abcdef0
 **  "12345678" -> 0x3132333435363738
 ** **************************/
CONVERTOR_API _int64 CharStr_to_Int64(unsigned char * ucpCharStr, int iLen);

/** ***************************
 **  Long_to_HexStr
 **  change long to char string
 **  if iOutLen = 0x04 0x9000 -> "9000"
 **  if iOutLen = 0x08 0x9000 -> "00009000"
 ** **************************/
CONVERTOR_API int Long_to_HexStr( unsigned long ulInVal, unsigned char * ucpOutData, int iOutLen);

/** ***************************
 **  Long_to_CharStr
 **  change long to char string
 **  if iOutLen = 0x02 0x9000 -> "\x90\x00"
 **  if iOutLen = 0x04 0x9000 -> "\x00\x00\x90\x00"
 ** **************************/
CONVERTOR_API int Long_to_CharStr(unsigned long ulInVal, unsigned char * ucpOutData, int iOutLen);

/** ***************************
 **  Int64_to_CharStr
 **  change int 64 to char string
 **  if iOutLen = 0x02 0x9000 -> "\x90\x00"
 **  if iOutLen = 0x08 0x9000 -> "\x00\x00\x00\x00\x00\x00\x90\x00"
 ** **************************/
CONVERTOR_API int Int64_to_CharStr( _int64 i64InVal, unsigned char * ucpOutData, int iOutLen);

CONVERTOR_API int Unpacking_CharStr_7bto8b(  const unsigned char *ucpInCharStr, int iLen, unsigned char *ucpOutCharStr, int iOutBufLen);
CONVERTOR_API int Packing_CharStr_8bto7b(  const unsigned char *ucpInCharStr, int iLen, unsigned char *ucpOutCharStr, int iOutBufLen);
