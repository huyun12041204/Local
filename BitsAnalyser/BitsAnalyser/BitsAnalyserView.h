// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����
#pragma once



#include "BitsAnalyserDoc.h"
#include "OutputWnd.h"
#include "BasicConvert\BasicConvert\BasicConvert.h"


//ViewAPDU 
//P3 iVirtualEvent
#define DEF_Virtual_Event 0
#define DEF_TRUE_Event    1

#define DEF_CLK_BITS 0x80
#define DEF_IO_BITS  0x40
#define DEF_VCC_BITS 0x20
#define DEF_RST_BITS 0x10

#define DEF_WORK_BITS     0x30
#define DEF_POWEROFF_BITS 0x00

#define DEF_DFAULT_PRESCALE  372

#define DEF_SINGLE_VIEW_MAX 512

#define  _BYTE_Result  3
#define  _BYTE_Wait    2
#define  _BYTE_Success 1
#define  _BYTE_NoData  0
#define  _BYTE_Error  -1
#define  _TPDU_NULL  0x00
#define  _TPDU_ATR   0x4
#define  _TPDU_PPS   0x8
#define  _TPDU_TPDU  0xC

#define  _TPDU_TYPE  0xC


#define _TPDU_APDU_CLA  0x10
#define _TPDU_APDU_INS  0x20
#define _TPDU_APDU_P1   0x30
#define _TPDU_APDU_P2   0x40
#define _TPDU_APDU_P3   0x50
#define _TPDU_APDU_ACK  0x60
#define _TPDU_APDU_NULL 0x70
#define _TPDU_APDU_DATA 0x80
#define _TPDU_APDU_SW   0x90
#define _TPDU_APDU_SW1   _TPDU_APDU_SW
#define _TPDU_APDU_SW2   0xA0
#define _TPDU_APDU_COMPLETE _TPDU_APDU_SW2
#define _TPDU_APDU_PALL  0xF0


#define _TPDU_ATR_TS   0x10
#define _TPDU_ATR_T0   0x20
#define _TPDU_ATR_TA  0x30
#define _TPDU_ATR_TB  0x40
#define _TPDU_ATR_TC   0x50
#define _TPDU_ATR_TD   0x60
#define _TPDU_ATR_T_NUMBER   0xD0
#define _TPDU_ATR_TCK      0xE0
#define _TPDU_ATR_COMPLETE _TPDU_ATR_TCK
#define _TPDU_ATR_PALL   0xF0

//#define _TPDU_ATR_DATA 0x60

#define _TPDU_PPS_PTSS    0x10
#define _TPDU_PPS_PTS0    0x20
#define _TPDU_PPS_PTS1    0x30
#define _TPDU_PPS_PTS2    0x40
#define _TPDU_PPS_PTS3    0x50
#define _TPDU_PPS_PCK     0x60
#define _TPDU_PPS_RPTSS   0x70
#define _TPDU_PPS_RPTS0   0x80
#define _TPDU_PPS_RPTS1   0x90
#define _TPDU_PPS_RPTS2   0xA0
#define _TPDU_PPS_RPTS3   0xB0
#define _TPDU_PPS_RPCK    0xC0
#define _TPDU_PPS_COMPLETE    _TPDU_PPS_RPCK
#define _TPDU_PPS_PALL   0xF0
class CBitsAnalyserView : public CView
{
protected: // �������л�����
	CBitsAnalyserView();
	DECLARE_DYNCREATE(CBitsAnalyserView)

// ����
public:
	CBitsAnalyserDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CBitsAnalyserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	CRichEditCtrl m_wndEdit;
	COutputList   m_pAPDU;    
	int bbitsLen ;
	int bdiflen  ;
	BYTE bbits[10];

// ���ɵ���Ϣӳ�亯��
protected:
	//afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	void UpdateFonts();



	//************************************
	// Method:    ViewAPDU
	// FullName:  CBitsAnalyserView::ViewAPDU
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: BYTE * ucBits
	// Parameter: UINT BitsLen
	// Parameter: int iVirtualEvent 
	//************************************

	int ViewAPDU(BYTE* ucBits, UINT BitsLen, int iVirtualEvent = DEF_TRUE_Event);

	//************************************
	// Method:    _Handle_Pin_Bytes
	// FullName:  �ж��������������ϵ磬�����µ� 
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: BYTE ucPre
	// Parameter: BYTE ucCur
	//************************************
	void _Handle_Pin_Bytes(BYTE ucPre,BYTE ucCur);
	//************************************
	// Method:    _InitializeBits
	// FullName:  ��������Bits����
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void _InitializeBits();

	int AddEvent(BYTE* ucbits, int ibitslen);
	//************************************
	// Method:    ModifyDescription
	// FullName:  ������һ��event ��BYTE
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: BYTE __BYTE
	//************************************
	int ModifyDescription(BYTE __BYTE, int __Type = 0, int __BitOff = 0);
	int RemoveAllAPDU(void);
};

#ifndef _DEBUG  // BitsAnalyserView.cpp �еĵ��԰汾
inline CBitsAnalyserDoc* CBitsAnalyserView::GetDocument() const
   { return reinterpret_cast<CBitsAnalyserDoc*>(m_pDocument); }
#endif

