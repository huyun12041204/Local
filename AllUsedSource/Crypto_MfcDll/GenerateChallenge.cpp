#include "stdafx.h"
#include "Crypto.h"

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

CRYPTO_API BOOL GenerateChallenge(int iLength, unsigned char * pucRand)
{
	int iTemp, iRand;
	
	srand( (unsigned)time( NULL ) );
	for(iTemp = 0; iTemp < iLength; iTemp++)
	{
		iRand = rand();
		*(pucRand + iTemp)  = (unsigned char) (iRand & 0xff);
	}
	return TRUE;
}
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
CRYPTO_API int GenerateRandom(int iMin, int iMax)
{
	int iRand;
	int iRet;
	
	if(iMax < iMin)
	   return 0x00;
	srand( (unsigned)time( NULL ) );

	for(; 1; )
	{
		iRand = rand();
		iRet = (unsigned int)(iRand % iMax);
		if((iRet >= iMin) && (iRet <iMax))
			break;
	}
	return iRet;
} 