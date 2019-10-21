// c_feal32.h: interface for the c_feal32 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_C_FEAL32_H__141CADF9_BF9B_4057_BCA2_4D1F978E26CA__INCLUDED_)
#define AFX_C_FEAL32_H__141CADF9_BF9B_4057_BCA2_4D1F978E26CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  N  32
#define  M  (N/2+4)
class c_feal32  
{
private:
	unsigned short K[N+7+1];
	unsigned _int64 m_authkey;
	unsigned _int64 m_random;
	unsigned _int64 m_authresult;
	unsigned _int64 feal(unsigned _int64  rad     );
	unsigned _int64 defeal(unsigned _int64  rad   );
	unsigned int  f(unsigned int A,unsigned int B );
	unsigned char  s0(unsigned char x1,unsigned x2);
	unsigned char  s1(unsigned char x1,unsigned x2);
	unsigned int  fk(unsigned int A,unsigned int B);
	void generatekey(unsigned _int64 inkl,bool parity,unsigned short *K);
public:
	c_feal32(unsigned _int64 authkey,unsigned _int64 random);
	c_feal32();
	virtual ~c_feal32();
	void Set_Authkey(unsigned _int64 authkey);
	void Set_Random(unsigned _int64 random);
	unsigned _int64 Get_AuthReSult();
};


#endif // !defined(AFX_C_FEAL32_H__141CADF9_BF9B_4057_BCA2_4D1F978E26CA__INCLUDED_)
