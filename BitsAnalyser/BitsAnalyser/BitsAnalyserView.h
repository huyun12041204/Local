// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。
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
protected: // 仅从序列化创建
	CBitsAnalyserView();
	DECLARE_DYNCREATE(CBitsAnalyserView)

// 特性
public:
	CBitsAnalyserDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
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

// 生成的消息映射函数
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
	// FullName:  判断是重启，还是上电，或者下电 
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: BYTE ucPre
	// Parameter: BYTE ucCur
	//************************************
	void _Handle_Pin_Bytes(BYTE ucPre,BYTE ucCur);
	//************************************
	// Method:    _InitializeBits
	// FullName:  用于重置Bits参数
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void _InitializeBits();

	int AddEvent(BYTE* ucbits, int ibitslen);
	//************************************
	// Method:    ModifyDescription
	// FullName:  添加最后一个event 的BYTE
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: BYTE __BYTE
	//************************************
	int ModifyDescription(BYTE __BYTE, int __Type = 0);
	int RemoveAllAPDU(void);
};

#ifndef _DEBUG  // BitsAnalyserView.cpp 中的调试版本
inline CBitsAnalyserDoc* CBitsAnalyserView::GetDocument() const
   { return reinterpret_cast<CBitsAnalyserDoc*>(m_pDocument); }
#endif

