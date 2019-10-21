// c_stephi.cpp: implementation of the c_stephi class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "c_stephi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

c_stephi::c_stephi()
{
	m_authkey = 0;
	m_random = 0;
	SK[1] = 0x529abfde;	/* the eight SK given */
	SK[2] = 0xb76c28af;
	SK[3] = 0xe8013fa7;
	SK[4] = 0xf6c4f62b;
	SK[5] = 0x529abfde;
	SK[6] = 0xb76c28af;
	SK[7] = 0xe8013fa7;
	SK[8] = 0xf6c4f62b;
}

c_stephi::c_stephi(unsigned _int64 authkey,unsigned _int64 random)
{
	m_authkey = authkey;
	m_random = random;
	SK[1] = 0x529abfde;	/* the eight SK given */
	SK[2] = 0xb76c28af;
	SK[3] = 0xe8013fa7;
	SK[4] = 0xf6c4f62b;
	SK[5] = 0x529abfde;
	SK[6] = 0xb76c28af;
	SK[7] = 0xe8013fa7;
	SK[8] = 0xf6c4f62b;
}

c_stephi::~c_stephi()
{

}

void c_stephi::Set_Authkey(unsigned _int64 authkey)
{
	m_authkey = authkey;
}

void c_stephi::Set_Random(unsigned _int64 random)
{
	m_random = random;
}

unsigned _int32 c_stephi::f(unsigned _int32 IR,unsigned _int32 K)
{
	unsigned _int32 a,b,c,d,e,g,h;	/* the results of each steps */
	unsigned _int32 O;				/* the final output */
	int i;

	unsigned _int16 IRL,IRR,KL,KR,aL,aR;					/* variables for step 1 */
	unsigned _int32 K1,K11,K12,t1,t2,bit_b,bit_c,bit_k;		/* variables for step 3 */
	unsigned _int16 gL,gR,hL,hR;							/* variables for step 7 */
	
/* step 1 */
	IRL = (unsigned _int16)(IR >> 16);
	IRR = (unsigned _int16)(IR & 0xffff);
	KL  = (unsigned _int16)(K >> 16);
	KR  = (unsigned _int16)(K & 0xffff);
	aL  = IRL + KL;	/* just be added in 16 bits block */
	aR  = IRR + KR;
	a   = (unsigned _int32)aL << 16;
	a   = a | ((unsigned _int32)aR & 0xffff);

/* step 2 */
	b = ~a;					/* bit invertion */

/* step 3 */
	K1 = cycleright(K,1);	/* get K' */
	t1 = fun_step3(K1,b);	/* get t1 */
	K11 = cycleright(K1,1);	/* right1(k') */
	t2 = fun_step3(K11,b);	/* get t2 */
	K12 = ~K1;				/* invert(K') */

	c = 0;
	for( i = 1 ; i <= 32 ; i++)
	{
		/*  get bi and K12i */
		bit_b = (b >> (i - 1)) & 0x1;
		bit_k = (K12 >> (i - 1)) & 0x1;
		
		if((i == 7 || i == 23) && t1 == 0)/* get ci */
		{
			bit_c = bit_b;
		}
		else if((i == 7 || i == 23) && t1 == 1)
		{
			bit_c = bit_b ^ bit_k;
		}
		else if(i != 7 && i != 23 && t2 == 0)
		{
			bit_c = bit_b;
		}
		else if(i != 7 && i != 23 && t2 == 1)
		{
			bit_c = bit_b ^ bit_k;
		}
		/* get c according each ci */
		bit_c = bit_c << (i - 1);
		c = c | bit_c;
	}

/* step 4 */
	d = fun_step4(16,16,1,c);/* rearrange by bit */
	d = d | fun_step4(15,7,1,c);
	d = d | fun_step4(14,14,1,c);
	d = d | fun_step4(13,5,1,c);
	d = d | fun_step4(12,12,1,c);
	d = d | fun_step4(11,3,1,c);
	d = d | fun_step4(10,10,1,c);
	d = d | fun_step4(9,1,1,c);
	d = d | fun_step4(8,9,1,c);
	d = d | fun_step4(7,2,1,c);
	d = d | fun_step4(6,11,1,c);
	d = d | fun_step4(5,4,1,c);
	d = d | fun_step4(4,13,1,c);
	d = d | fun_step4(3,6,1,c);
	d = d | fun_step4(2,15,1,c);
	d = d | fun_step4(1,8,1,c);

	c = d;
	d = fun_step4(4,2,8,c);/* rearrange by block */
	d = d | fun_step4(3,4,8,c);
	d = d | fun_step4(2,3,8,c);
	d = d | fun_step4(1,1,8,c);

/* step 5 */
	e = cycleleft(d,7) ^ cycleleft(d,1) ^ d;

/* step 6 */
	g = fun_step4(4,4,8,e);/* rearrange by block */
	g = g | fun_step4(3,3,8,e);
	g = g | fun_step4(2,1,8,e);
	g = g | fun_step4(1,2,8,e);

	e = g;
	g = fun_step4(16,16,1,e);/* rearrange by bit */
	g = g | fun_step4(15,2,1,e);
	g = g | fun_step4(14,14,1,e);
	g = g | fun_step4(13,13,1,e);
	g = g | fun_step4(12,12,1,e);
	g = g | fun_step4(11,11,1,e);
	g = g | fun_step4(10,10,1,e);
	g = g | fun_step4(9,9,1,e);
	g = g | fun_step4(8,1,1,e);
	g = g | fun_step4(7,7,1,e);
	g = g | fun_step4(6,6,1,e);
	g = g | fun_step4(5,5,1,e);
	g = g | fun_step4(4,4,1,e);
	g = g | fun_step4(3,3,1,e);
	g = g | fun_step4(2,15,1,e);
	g = g | fun_step4(1,8,1,e);

/* step 7 */
	gL = (unsigned _int16)(g >> 16);
	gR = (unsigned _int16)(g & 0xffff);
	KL = (unsigned _int16)(K >> 16);
	KR = (unsigned _int16)(K & 0xffff);
	KL = KL + 0x1357;
	KR = KR + 0x1357;
	hL = gL + KL;
	hR = gR + KR;
	h = (unsigned _int32)hL << 16;
	h = h | ((unsigned _int32)hR & 0xffff);


/* step 8 */
	O = cycleleft(h,1);

	return O;
}

/* algorithm include 8 units */
unsigned _int64 c_stephi::unit(unsigned _int64 I,unsigned _int32 K)
{
	unsigned _int32 IL,IR,OL,OR;		/* L means high bits and R means low bits */
	unsigned _int64 O;					/* the final output */

	IL = (unsigned _int32)(I >> 32);
	IR = (unsigned _int32)(I & 0xffffffff);
	
	OR = f(IR,K);
	OR = IL ^ OR;
	OL = IR;
	O = (unsigned _int64)OL << 32;
	O = O | ((unsigned _int64)OR & 0xffffffff);

	return O;
}


unsigned _int32 c_stephi::fun_step3(unsigned _int32 K1,unsigned _int32 b) /* the function in step3,the result is t */
{
	unsigned _int32 temp1,temp2;
	unsigned _int32 t;	/* the output */
	int i;

	temp1 = b & K1;
	t = 0;
	for( i = 1 ; i <= 32 ; i++)
	{
		temp2 = (temp1 >> (i - 1)) & 0x1;
		t = t ^ temp2;
	}

	return t;
}

/* the function in step4,rearrange the input */
unsigned _int32 c_stephi::fun_step4(int original_pos,int destination_pos,int length,unsigned _int32 c)
{
	unsigned _int32 d;	/* the output */
	unsigned _int16 L,R;	/* L means high bits and R means low bits */

	if(length == 1) /* rearrange by bit */
	{
		L = (unsigned _int16)(c >> 16);
		R = (unsigned _int16)(c & 0xffff);
		if((original_pos >= 1) && (destination_pos >= 1))
		{
			L = ((L >> (original_pos - 1)) & 0x1) << (destination_pos - 1);
			R = ((R >> (original_pos - 1)) & 0x1) << (destination_pos - 1);
		}
		d = (unsigned _int32)L << 16;
		d = d | ((unsigned _int32)R & 0xffff); 
	}
	else if(length == 8) /* rearrange by block */
	{
		if((original_pos >= 1) && (destination_pos >= 1))
		{
			d = ((c >> ((original_pos - 1)*8)) & 0xff) << ((destination_pos - 1)*8);
		}
	}

	return d;
}

unsigned _int32 c_stephi::cycleright(unsigned _int32 input,int n) /* move to right cyclely bit by bit */
{
	unsigned _int32 output;
	unsigned _int32 temp;

	output = input >> n;
	temp = input << (32 - n);
	output = output | temp;
		
	return output;
}

unsigned _int32 c_stephi::cycleleft(unsigned _int32 input,int n) /* move to left cyclely bit by bit */
{
	unsigned _int32 output;
	unsigned _int32 temp;

	output = input << n;
	temp = input >> (32 - n);
	output = output | temp;
	
	return output;
}

unsigned _int64 c_stephi::Get_AuthReSult()
{		
	unsigned K[9];
	int i;
	unsigned _int64 authkey = m_authkey;

	for(i = 1 ; i <= 8 ; i++)
	{
		authkey = unit(authkey,SK[i]);
		/* get eight K ,K is the right 32 bits in the output*/
		K[i] = (unsigned _int32)(authkey & 0xffffffff);
	}

	m_authresult = m_random;
	for(i = 1 ; i <= 8 ; i++)
	{
		m_authresult = unit(m_authresult,K[i]);
	}

	/* need exchange */ 
	m_authresult = ((m_authresult>>32)&0x00000000ffffffff) | (m_authresult<<32);

	return m_authresult;
}