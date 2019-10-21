// c_cave.h: interface for the cave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAVE_H__44FBD2C2_FF05_4DDF_97EF_7F9FE1EE25A1__INCLUDED_)
#define AFX_CAVE_H__44FBD2C2_FF05_4DDF_97EF_7F9FE1EE25A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class c_cave  
{
public:
	unsigned char cmeakey[8];
    unsigned char VPM[65]; 
    unsigned char SAVED_LFSR[4]; 
    int SAVED_OFFSET_1; 
    int SAVED_OFFSET_2; 
    unsigned char SAVED_RAND[4]; 
    unsigned char SAVED_DATA[3]; 
//	unsigned char CaveTable[256]; 
//    unsigned char ibox[256];
    unsigned char ESN[4]; 
    unsigned char A_key[8]; 
    unsigned char SSD_A_NEW[8], SSD_A[8];
    unsigned char SSD_B_NEW[8], SSD_B[8]; 
    BOOL A_Key_Verify(const char A_KEY_DIGITS[26]);
    void SSD_Generation(const unsigned char RANDSSD[7]);
    unsigned long Auth_Signature(const unsigned char RAND_CHALLENGE[4], /*randbs */ const unsigned char AUTH_DATA[3], //IMSi_m_S1
		const unsigned char *SSD_AUTH/*ssd-A*/, const int SAVE_REGISTERS/*目前写的是0*/); 
    void SSD_Update(void);
    void Key_VPM_Generation(void);
	
	c_cave();
	virtual ~c_cave();

private:

#define AAV 0xC7 
#define LOMASK 0x0F 
#define HIMASK 0xF0 
#define TRUE 1 
#define FALSE 0 


/* saved outputs */

    unsigned char LFSR[4]; 
#define LFSR_A LFSR[0] 
#define LFSR_B LFSR[1] 
#define LFSR_C LFSR[2] 
#define LFSR_D LFSR[3] 

    unsigned char Register[16]; 

	unsigned char bit_val(const unsigned char octet, const int bit);
	void LFSR_cycle(void);
	void Rotate_right_registers(void);
	void CAVE(const int number_of_rounds,int *offset_1, int *offset_2);
	void mul10(unsigned char i64[8], unsigned int carry);
	unsigned long Calc_Checksum(const unsigned char A_key[8]);
	void A_Key_Checksum(const char A_KEY_DIGITS[20], char A_KEY_CHECKSUM[6]); 
    void roll_LFSR(void);


};

#endif // !defined(AFX_CAVE_H__44FBD2C2_FF05_4DDF_97EF_7F9FE1EE25A1__INCLUDED_)
