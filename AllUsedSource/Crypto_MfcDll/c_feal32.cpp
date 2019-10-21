// c_feal32.cpp: implementation of the c_feal32 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "c_feal32.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

c_feal32::c_feal32()
{
	m_authkey = 0;
	m_random = 0;
}

c_feal32::~c_feal32()
{

}

c_feal32::c_feal32(unsigned _int64 authkey,unsigned _int64 random)
{
	m_authkey = authkey;
	m_random = random;
}
unsigned int  c_feal32::fk(unsigned int A,unsigned int B)
{
	unsigned char fk[4]        ;
	unsigned char a[4],b[4]    ;
	int ret                    ;
	a[0]=(A&0xff000000)>>24    ;
	a[1]=(A&0x00ff0000)>>16    ;
	a[2]=(A&0x0000ff00)>>8     ;
	a[3]=(A&0x000000ff)        ;
	b[0]=(B&0xff000000)>>24    ;
	b[1]=(B&0x00ff0000)>>16    ;
	b[2]=(B&0x0000ff00)>>8     ;
	b[3]=(B&0x000000ff)        ;
    
	fk[1] =a[1]^a[0]           ;
	fk[2] =a[2]^a[3]           ;

    fk[1] =s1(fk[1],fk[2]^b[0]);
    fk[2] =s0(fk[2],fk[1]^b[1]);
    fk[0] =s0(a[0],fk[1]^b[2])   ;
	fk[3] =s1(a[3],fk[2]^b[3])   ;
	
    ret=(((unsigned int)fk[0])<<24)|(((unsigned int)fk[1])<<16)|(((unsigned int)fk[2])<<8)|(((unsigned int)fk[3]));
	return ret;
}

void c_feal32::generatekey(unsigned _int64 inkl,bool parity,unsigned short *K)
{
	int r;
	unsigned int A[N+7],B[N+7];
	unsigned int D[M+1];
//	unsigned short K[N+7];
	unsigned _int64 kl=inkl;
	if(parity==true)
	{
		kl=kl&0xfefefefefefefefe;
	}
	A[0]=(unsigned int)((kl&0xffffffff00000000)>>32);
	B[0]=(unsigned int)( kl&0x00000000ffffffff);

	D[0]=0;
	for(r=1;r<=N/2+4;r++)
	{
		D[r]=A[r-1];
		A[r]=B[r-1];
		B[r]=fk(A[r-1],B[r-1]^D[r-1]);
		K[2*(r-1)]  =(B[r] &0xffff0000)>>16;
		K[2*(r-1)+1]=B[r]  &0x0000ffff;
	}

}


unsigned char  c_feal32::s0(unsigned char x1,unsigned x2)
{
	unsigned int retint;
	retint=(unsigned int)x1+(unsigned int)x2;
    retint=retint%256;
	retint=retint<<2;
	retint=retint|((retint&0xff00)>>8);
	return (unsigned char)retint;
}
unsigned char  c_feal32::s1(unsigned char x1,unsigned x2)
{
	unsigned int retint;
	retint=(unsigned int)x1+(unsigned int)x2 +1;
    retint=retint%256;
	retint=retint<<2;
	retint=retint|((retint&0xff00)>>8);
	return (unsigned char)retint;
}

unsigned int c_feal32::f(unsigned int A,unsigned int B)
{
	unsigned char a[4],b[2],f[4];
	int ret;
    a[0]=(A&0xff000000)>>24;
    a[1]=(A&0x00ff0000)>>16;
    a[2]=(A&0x0000ff00)>>8 ;
    a[3]=(A&0x000000ff)    ;
    b[0]=(B&0xff00)>>8      ;
    b[1]=(B&0x00ff)         ;

    f[1]=a[1]^b[0]         ;
    f[2]=a[2]^b[1]         ;
	f[1]=f[1]^a[0]         ;
	f[2]=f[2]^a[3]         ;
	f[1]=s1(f[1],f[2])     ;
	f[2]=s0(f[2],f[1])     ;
	f[0]=s0(a[0],f[1])     ;
	f[3]=s1(a[3],f[2])     ;
    ret=((int)f[0]<<24)|((int)f[1]<<16)|((int)f[2]<<8)|((int)f[3]);
	return ret;
}

unsigned _int64 c_feal32::defeal(unsigned _int64  cipher)
{
   unsigned _int64 ret;
   int r;
   unsigned int L[N+1];
   unsigned int R[N+1];

   R[N]=(unsigned int)((cipher&0xffffffff00000000)>>32);
   L[N]=(unsigned int)((cipher&0xffffffff));

   R[N]=R[N]^((((unsigned int)K[N+4])<<16 )|((unsigned int)K[N+5]));//??
   L[N]=L[N]^((((unsigned int)K[N+6])<<16 )|((unsigned int)K[N+7]));//??

   R[N]=R[N]^0x00000000              ;
   L[N]=L[N]^R[N]                    ;

   for(r=N;r>=1;r--)
   {
	   L[r-1]=R[r]^f(L[r],K[r-1])    ;
	   R[r-1]=L[r]                   ;
   }
   L[0]=L[0]^0x00000000              ;
   R[0]=R[0]^L[0]      	             ;
   
   L[0]=L[0]^((((unsigned int)K[N])<<16   )|((unsigned int)K[N+1]));//??
   R[0]=R[0]^((((unsigned int)K[N+2])<<16 )|((unsigned int)K[N+3]));//??

   ret=(((unsigned _int64)L[0])<<32)|((unsigned _int64)R[0]);
   return ret;	
}

void c_feal32::Set_Authkey(unsigned _int64 authkey)
{
	m_authkey = authkey;
}

void c_feal32::Set_Random(unsigned _int64 random)
{
	m_random = random;
}



unsigned _int64 c_feal32::feal(unsigned _int64  rad)
{
   unsigned _int64 ret;
   unsigned int L[N+1];
   unsigned int R[N+1];
   L[0]=(unsigned int)((rad&0xffffffff00000000)>>32);
   R[0]=(unsigned int)( rad&0x00000000ffffffff);

   L[0]=L[0]^((((unsigned int)K[N  ])<<16 )|((unsigned int)K[N+1]));//??
   R[0]=R[0]^((((unsigned int)K[N+2])<<16 )|((unsigned int)K[N+3]));//??

   L[0]=L[0]^0x00000000              ;
   R[0]=R[0]^L[0]                    ;
   int r                             ;
   for(r=1;r<=N;r++)
   {
	 R[r]=L[r-1]^f(R[r-1],K[r-1])    ;
     L[r]=R[r-1]                     ;
   }

   R[N]=R[N]^0x00000000              ;
   L[N]=L[N]^R[N]                    ;

   R[N]=R[N]^((((unsigned int)K[N+4])<<16 )|((unsigned int)K[N+5]));//??
   L[N]=L[N]^((((unsigned int)K[N+6])<<16 )|((unsigned int)K[N+7]));//??


   ret=(((unsigned _int64)R[N])<<32)|((unsigned _int64)L[N]);
   return ret;	
}

unsigned _int64 c_feal32::Get_AuthReSult()
{
	generatekey(m_authkey,false,K);
	m_authresult = feal(m_random);
	return m_authresult;
}
