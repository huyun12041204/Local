#pragma once





#define DEF_SIGLE_WIDE  40
#define DEF_TEXT_HEIGHT 20
#define DEF_TEXT_WIDE   40
#define DEF_TEXT_INDENT 10
#define DEF_FRAME_INDENT 2
#define DEF_TOP_FRAME_INDENT   30
#define DEF_LEFT_FRAME_INDENT     DEF_FRAME_INDENT
#define DEF_RIGHT_FRAME_INDENT    DEF_FRAME_INDENT
#define DEF_BOTTON_FRAME_INDENT   DEF_FRAME_INDENT

#define DEF_LINE_Y_OFFSET   10

#define DEF_CLK_PIN 0x80
#define DEF_IO_PIN  0x40
#define DEF_VCC_PIN 0x20
#define DEF_RST_PIN 0x10
#define  DEF_EVENTLIST_DATA 1

#define DEF_SIGHT_LIMIT 480


#if DEF_EVENTLIST_DATA
#include "EventList.h"
#endif

class CWaveForm :public CStatic
{
	DECLARE_DYNAMIC(CWaveForm)

public:
	CWaveForm();
	virtual ~CWaveForm();
protected:
	DECLARE_MESSAGE_MAP()

	BYTE* ucBits;
	int   iBitsLen;

	// 一个单元格表示的 CLK 数量
	int iGroupCLK;

	//三条波形的  幅度位置 
	int iIOUP;
	int iIODOWN;
	int iVCCUP;
	int iVCCDOWN;
	int iRSTUP;
	int iRSTDOWN;

	int iCLKUP;
	int iCLKDOWN;

	int iStartX;

	//保存当前显示的POS
	int iStartPos;
	int iEndPos;




#if DEF_EVENTLIST_DATA

	CEventList* m_hEventList;

#endif


public:
	
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	void DrawBackGround(CDC* pDC, CRect& rect);
	void DrawDescription(CDC* pDC, int iTextX, CString csText);
	void DrawLine(CDC* pDC, CRect& rect, POINT* pSelect = NULL);
	void DrawWave(CDC* pDC, POINT pStart, POINT* pEnd, int iType = DEF_IO_PIN, int iLimit = 0, int iSighLimit = DEF_SIGHT_LIMIT);
	void InputBitsDatas(BYTE* Bits, int iBitsLen);
	//void GenerateStartPoint(int* BitsOffset, POINT* pIO, POINT* pVCC, POINT* pRST);
	//void GeneratePoint     (int* BitsOffset, POINT* pIO, POINT* pVCC, POINT* pRST);
	int  GetBits(BYTE* ucBitss = NULL);
	int  SetPos(int iPos);

	//会从EventList里面获取数据的

	
	int GeneratePrePoint(BYTE* bits, int bitSize, BYTE prebits0, POINT* pIO, POINT* pVCC, POINT* pRST, POINT* pCLK);

	int GenerateStartPoint(BYTE* bits, int bitSize, POINT* pIO, POINT* pVCC, POINT* pRST, POINT* pCLK);

	int GeneratePoint(BYTE* bits, int bitSize, POINT* pIO, POINT* pVCC, POINT* pRST, POINT* pCLK);

	int InputEventWnd(CWnd* EventWnd);
	int InputPrescale(int iPrescale);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void DrawSignLine(CDC* pDC, int x, int StartY, int EndY);
	void DrawEventCLK(CDC* pDC, CString csEent, POINT pEventCLK);
};


// CWaveView

class CWaveView : public CDockablePane
{
	DECLARE_DYNAMIC(CWaveView)

public:
	CWaveView();
	virtual ~CWaveView();

	CWaveForm  m_pWaveForm;
	CScrollBar m_pScrollBar;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	int GetBitsEvenSum();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int InputEventWnd(CWnd* EventWnd);
	int InputPrescale(int iPrescale);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


