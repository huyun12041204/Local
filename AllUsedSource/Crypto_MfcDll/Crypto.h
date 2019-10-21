// Crypto.h : main header file for the CRYPTO DLL
//
#define CRYPTO_API extern   "C"   _declspec(dllexport) 

#if !defined(AFX_CRYPTO_H__F718F6A1_F467_4E6E_BE92_4ACFD1E82C7B__INCLUDED_)
#define AFX_CRYPTO_H__F718F6A1_F467_4E6E_BE92_4ACFD1E82C7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CCryptoApp
// See Crypto.cpp for the implementation of this class
//

class CCryptoApp : public CWinApp
{
public:
	CCryptoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryptoApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCryptoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


// define for DES
                                 /* Parameter 1, encrypt, decrypt, mac */
 #define  ENCRYPT   'E'          /* Encrypt, verschl乻seln */
 #define  DECRYPT   'D'          /* Decrypt, entschl乻seln */
 #define  MAC       'M'          /* message authentication code, Pr乫summe */

                                 /* Parameter 2, CBC oder ECB */
 #define  CBC       'C'          /* CBC - Cipherblock chaining */
 #define  ECB       'E'          /* ECB - electronic code book, Einzelblock */

                                 /* Parameter 3, Berechnung nach ISO */
 #define  ISO       'I'          /* ISO: Rest mit 0x80 auff乴len */
 #define  NOISO     'N'          /* oder Rest mit 0x00 auff乴len */

 #define  SINGLE    'S'          /* DES HASH Funktion */
 #define  DOUBLE    'D'          /* SINGLE oder DOUBLE length 8 / 16 */

 #define  FIRST     'F'          /* DES-Schl乻sel neu laden */
 #define  NEXT      'N'          /* DES-Schl乻sel ist letzter HASH-Wert */
// 


CRYPTO_API void A3A8(/*in*/ BYTE rand[16], /*in*/ BYTE key[16],/*out*/ BYTE simoutput[12]);
CRYPTO_API void Stephi(/*in*/ BYTE rand[8],/*in*/ BYTE key[8],/*out*/ BYTE output[8]);
CRYPTO_API void Feal32(/*in*/ BYTE rand[8],/*in*/ BYTE key[8],/*out*/ BYTE output[8]);

CRYPTO_API BOOL InitAKey(char cAKey[8]);
CRYPTO_API BOOL InitSSD_A(char cSSD_A[8]); 
CRYPTO_API BOOL InitSSD_B(char cSSD_B[8]);
CRYPTO_API BOOL InitESN(char * pcESN); 
CRYPTO_API BOOL A_Key_Verify(/*in*/char A_KEY_DIGITS[26], /*out*/char cAkey[8]); 
CRYPTO_API BOOL SSD_Generation(/*in*/const unsigned char RANDSSD[7], /*out*/ char cSSD_A[8], /*out*/ char cSSD_B[8]);
CRYPTO_API BOOL Auth_Signature(const unsigned char RAND_CHALLENGE[4], /*randbs */ const unsigned char AUTH_DATA[3], //IMSi_m_S1
			const unsigned char *SSD_AUTH/*ssd-A*/, const int SAVE_REGISTERS/*目前写的是0*/, char * pAuthBS);
CRYPTO_API BOOL UpdateSSD(void);
CRYPTO_API BOOL Key_VPM_Generation(char * key, char * vpm);
CRYPTO_API BOOL MD5_Hash(unsigned char* pucInput, int iDataLen, unsigned char pucOutput[16]);

CRYPTO_API BOOL MD5_Init();
CRYPTO_API BOOL MD5_Update(unsigned char* pucInput, int iDataLen);
CRYPTO_API BOOL MD5_Final(unsigned char pucOutput[16]);

CRYPTO_API unsigned int crc_16(unsigned char * pucInput, unsigned int uiInputDataLen);

CRYPTO_API BOOL Milenage_Set_C(unsigned char * pucInputC1, unsigned char * pucInputC2, unsigned char * pucInputC3, unsigned char * pucInputC4, unsigned char * pucInputC5);
CRYPTO_API BOOL Milenage_Set_R(int iR1, int iR2, int iR3, int iR4, int iR5);
CRYPTO_API BOOL Milenage_Set_OP(unsigned char * pucOP);
CRYPTO_API BOOL Milenage_Set_OP_c(unsigned char * pucOP_c);
CRYPTO_API BOOL Milenage_Set_OP_Ind(CString csOpInd);
CRYPTO_API BOOL Milenage_f1(unsigned char * pucK, unsigned char * pucRand, unsigned char * pucSQN, unsigned char * pucAMF, unsigned char * pucMAC_A);
CRYPTO_API BOOL Milenage_f1star(unsigned char * pucK, unsigned char * pucRand, unsigned char * pucSQN, unsigned char * pucAMF, unsigned char * pucMAC_S);
CRYPTO_API BOOL Milenage_f2345(unsigned char * pucK, unsigned char * pucRand, unsigned char * pucRes, unsigned char * pucCK, unsigned char * pucIK, unsigned char * pucAK);
CRYPTO_API BOOL Milenage_f5star(unsigned char * pucK, unsigned char * pucRand, unsigned char * pucAK);
CRYPTO_API BOOL Milenage_Generate_SRES(unsigned char * pucXRES,  unsigned char * pucSRES);
CRYPTO_API BOOL Milenage_Generate_Kc(unsigned char * pucCK, unsigned char * pucIK, unsigned char * pucKc);
CRYPTO_API BOOL Milenage_Compute_OP_c(unsigned char * pucOP_c);

CRYPTO_API int des(unsigned char *key, unsigned char *input, int len, unsigned char *output, char mode, char chain, char padding);
CRYPTO_API int triple_des(unsigned char *keyin, unsigned char *input, int length, unsigned char *output, char mode, char chain, char padding); 

#define DEF_HASH_SHA1   1
#define DEF_HASH_SHA224 2
#define DEF_HASH_SHA256 3
#define DEF_HASH_SHA384 4
#define DEF_HASH_SHA512 5
#define DEF_HASH_RMD160 6
#define DEF_KDF_KDF1    1
#define DEF_KDF_KDF2    2
CRYPTO_API int hash(unsigned char *message, unsigned int len, unsigned char *digest, unsigned int uiMode);
CRYPTO_API int hmac(unsigned char key[], unsigned long key_len, unsigned char data[], unsigned long data_len, unsigned char mac[], unsigned long mac_len, unsigned int uiMode);
CRYPTO_API int dochmac(unsigned char key[], unsigned long key_len, unsigned char data[], unsigned long data_len, unsigned char mac[], unsigned long mac_len, unsigned int uiMode);
CRYPTO_API void KDF1(unsigned char *ucKey, unsigned int uiKeyLen, unsigned char *ucData, unsigned int uiDataLen, unsigned int uiNewKeyLen, unsigned char *ucNewKey, unsigned int uiMode);
CRYPTO_API void KDF2(unsigned char *ucKey, unsigned int uiKeyLen, unsigned char *ucData, unsigned int uiDataLen, unsigned int uiNewKeyLen, unsigned char *ucNewKey, unsigned int uiMode);
CRYPTO_API void KDF(unsigned char *ucKey, unsigned int uiKeyLen, unsigned char *ucData, unsigned int uiDataLen, unsigned int uiNewKeyLen, unsigned char *ucNewKey, unsigned int uiKDFMode, unsigned int uiHashMode);
CRYPTO_API BOOL     DocMilenage_f1(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucSQN, unsigned int uiSQNLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucAMF, unsigned int uiAMFlen, unsigned char * pucMAC_A, unsigned int uiMAC_ALen, unsigned int uiKdfMode, unsigned int uiHashMode);
CRYPTO_API BOOL DocMilenage_f1star(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucSQN, unsigned int uiSQNLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucAMF, unsigned int uiAMFlen, unsigned char * pucMAC_S, unsigned int uiMAC_SLen, unsigned int uiKdfMode, unsigned int uiHashMode);
CRYPTO_API BOOL DocMilenage_f2(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucRES, unsigned int uiRESLen, unsigned int uiKdfMode, unsigned int uiHashMode);
CRYPTO_API BOOL     DocMilenage_f3(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucCK, unsigned int uiCKLen, unsigned int uiKDFMode, unsigned int uiHashMode);
CRYPTO_API BOOL     DocMilenage_f4(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucIK, unsigned int uiIKLen, unsigned int uiKDFMode, unsigned int uiHashMode);
CRYPTO_API BOOL     DocMilenage_f5(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucAK, unsigned int uiAKLen, unsigned int uiKDFMode, unsigned int uiHashMode);
CRYPTO_API BOOL DocMilenage_f5star(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucAK_S, unsigned int uiAK_SLen, unsigned int uiKDFMode, unsigned int uiHashMode);
/*
Docomo Milinage for F3 F4 F5, uiCKLen = 16, uiIKLen = 16, uiAKLen = 6
*/
CRYPTO_API BOOL DocMilenage_f345(unsigned char * pucK, unsigned int uiKeyLen, unsigned char * pucMID, unsigned int uiMIDLen, unsigned char * pucRand, unsigned int uiRandLen, unsigned char * pucCK, unsigned int uiCKLen, unsigned char *  pucIK, unsigned int uiIKLen, unsigned char * pucAK, unsigned int uiAKLen, unsigned int uiKDFMode, unsigned int uiHashMode);
CRYPTO_API unsigned long crc_32(unsigned char * pucInput, unsigned int uiInputDataLen);
//CRYPTO_API BOOL crc_32(unsigned char * pucInput, int iInputDataLen, unsigned char ucOutput[4]);

    /**********************************************************************
    *
    *      Function name:                     GenerateChallenge
    *      Creation date (DD.MM.YY):          03.06.2009
    *      Author:                            dyhe
    *
    *      Comment: 
    *          Generate a challenge
    *
    *      List of parameters:
	*           iLength : int, the length of the challenge
    *          
    *      Return value:
	*          unsigned char *  : the challenge
    *
    *      Revisions:
    *
    ***********************************************************************/

CRYPTO_API BOOL GenerateChallenge(int iLength, unsigned char * pucRand);

	
    /**********************************************************************
    *
    *      Function name:                     GenerateRandom
    *      Creation date (DD.MM.YY):          03.06.2009
    *      Author:                            dyhe
    *
    *      Comment: 
    *          Generate a random, iMIN <= random < iMAX
    *
    *      List of parameters:
	*           iMin : int, the minimum value
	*           iMax : int, the maximum malue
    *          
    *      Return value:
	*          int  : the random
    *
    *      Revisions:
    *
    ***********************************************************************/
CRYPTO_API int GenerateRandom(int iMin, int iMax);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRYPTO_H__F718F6A1_F467_4E6E_BE92_4ACFD1E82C7B__INCLUDED_)
