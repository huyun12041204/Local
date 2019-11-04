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

#define  _BYTE_Wait    2
#define  _BYTE_Success 1
#define  _BYTE_NoData  0
#define  _BYTE_Error  -1
#define  _TPDU_NULL  0x00
#define  _TPDU_ATR   0x10
#define  _TPDU_PPS   0x20
#define  _TPDU_TPDU  0x30

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
	int ModifyDescription(BYTE __BYTE, int __Type = 0);
	int RemoveAllAPDU(void);
};

#ifndef _DEBUG  // BitsAnalyserView.cpp �еĵ��԰汾
inline CBitsAnalyserDoc* CBitsAnalyserView::GetDocument() const
   { return reinterpret_cast<CBitsAnalyserDoc*>(m_pDocument); }
#endif

