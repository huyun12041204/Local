// c_stephi.h: interface for the c_stephi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_C_STEPHI_H__158BC3F4_745C_436F_970B_1FF460D4CC3A__INCLUDED_)
#define AFX_C_STEPHI_H__158BC3F4_745C_436F_970B_1FF460D4CC3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class c_stephi  
{
private:
	unsigned _int64 m_authkey;
	unsigned _int64 m_random;
	unsigned _int64 m_authresult;
	unsigned _int32 SK[9];
	/* algorithm include 8 units */
	unsigned _int64 unit(unsigned _int64 I,unsigned _int32 K);
	/* function f */
	unsigned _int32 f(unsigned _int32 IR,unsigned _int32 K);
	/* the function in step3,the result is t */
	unsigned _int32 fun_step3(unsigned _int32 K1,unsigned _int32 b);
	/* the function in step4,rearrange the input */
	unsigned _int32 fun_step4(int original_pos,int destination_pos,int length,unsigned _int32 c);
	/* move to right cyclely bit by bit */
	unsigned _int32 cycleright(unsigned _int32 input,int n);
	/* move to left cyclely bit by bit */
	unsigned _int32 cycleleft(unsigned _int32 input,int n);
public:
	c_stephi(unsigned _int64 authkey,unsigned _int64 random);
	c_stephi();
	virtual ~c_stephi();
	void Set_Authkey(unsigned _int64 authkey);
	void Set_Random(unsigned _int64 random);
	unsigned _int64 Get_AuthReSult();
};

#endif // !defined(AFX_C_STEPHI_H__158BC3F4_745C_436F_970B_1FF460D4CC3A__INCLUDED_)
