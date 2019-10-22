// WaveView.cpp: 实现文件
//

//#include "pch.h"
#include "stdafx.h"
#include "BitsAnalyser.h"
#include "WaveView.h"

IMPLEMENT_DYNAMIC(CWaveForm, CStatic)

CWaveForm::CWaveForm()
{

}

CWaveForm::~CWaveForm()
{
}


BEGIN_MESSAGE_MAP(CWaveForm, CStatic)

	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CWaveView

IMPLEMENT_DYNAMIC(CWaveView, CDockablePane)

CWaveView::CWaveView()
{

}

CWaveView::~CWaveView()
{
}


BEGIN_MESSAGE_MAP(CWaveView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_PAINTCLIPBOARD()
	ON_WM_MOVE()
END_MESSAGE_MAP()



// CWaveView 消息处理程序




int CWaveView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	m_pWaveForm.Create("波形图", WS_CHILD | WS_VISIBLE | SS_CENTER, rectDummy, this, IDC_WAVEFORM);


	return 0;
}


void CWaveView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_pWaveForm.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	//m_pWaveForm.OnSize(nType, cx, cy);
}


void CWaveView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDockablePane::OnPaint()
}




void CWaveView::OnPaintClipboard(CWnd* pClipAppWnd, HGLOBAL hPaintStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDockablePane::OnPaintClipboard(pClipAppWnd, hPaintStruct);
}


void CWaveView::OnMove(int x, int y)
{
	CDockablePane::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
}


void CWaveForm::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CStatic::OnPaint()

	CStatic::OnPaint();
	CRect rect;
	GetClientRect(&rect);//把picture的控件尺寸付给rectPicture对象，传递给以便DrawWave
	DrawBackGround(GetDC(), rect);
	


	
}

int CWaveForm::DrawBackGround(CDC* pDC, CRect& rect)
{
	//float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	//float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   
	CPen newPen;       // 用于创建新画笔   
	CPen* pOldPen;     // 用于存放旧画笔   
	CBrush newBrush;   // 用于创建新画刷   
	CBrush* pOldBrush; // 用于存放旧画刷   

	SIZE iLT, iLB, iRT, iRB;

	iLT.cx = rect.left + 2;
	iLT.cy = rect.top  + 20;

	iLB.cx = rect.left   + 2;
	iLB.cy = rect.bottom - 2;

	iRT.cx = rect.right - 2;
	iRT.cy = rect.top   + 20;

	iRB.cx = rect.right  - 2;
	iRB.cy = rect.bottom - 2;

	

	// 计算fDeltaX和fDeltaY   
	//fDeltaX = (float)rect.Width() / (POINT_COUNT - 1);;
	//fDeltaY = (float)rect.Height() / 1;

	newBrush.CreateSolidBrush(RGB(0, 0, 0)); // 创建黑色新画刷
	pOldBrush = pDC->SelectObject(&newBrush);// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pDC->Rectangle(rect);    // 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	pDC->SelectObject(pOldBrush);    // 恢复旧画刷
	newBrush.DeleteObject();    // 删除新画刷


	//此处 画边框

	newPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 100));    // 创建实心画笔，粗度为1，颜色为绿色
	pOldPen = pDC->SelectObject(&newPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen


	//画到左边上 顶点 

	pDC->MoveTo(iLT.cx, iLT.cy);

	//画到左边下 顶点 
	pDC->LineTo(iLB.cx, iLB.cy);

	//画到右边下 顶点 
	pDC->LineTo(iRB.cx, iRB.cy);

	//画到右边上 顶点 
	pDC->LineTo(iRT.cx, iRT.cy);

	//回到开始位置 
	pDC->LineTo(iLT.cx, iLT.cy);

	//此处开始 画  IO VCC 和 RST 分割线
	int SigleRow = (iLB.cy - iLT.cy)/3;

	pDC->MoveTo(iLT.cx, iLT.cy + SigleRow);
	pDC->LineTo(iRT.cx, iLT.cy + SigleRow);

	pDC->MoveTo(iLT.cx, iLT.cy + SigleRow*2);
	pDC->LineTo(iRT.cx, iLT.cy + SigleRow*2);



	//开始写 分割线 标识 

	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->SetBkColor  (RGB(0, 0, 0));

	CRect* cTextRect;
	nX = iLT.cx + DEF_TEXT_INDENT;
	nY = iLT.cy + DEF_TEXT_INDENT;
	cTextRect = new CRect(nX, nY,               nX + DEF_TEXT_WIDE, nY + DEF_TEXT_HEIGHT);
	pDC->DrawText(_T("IO"), cTextRect , DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
	delete(cTextRect);

	cTextRect = new CRect(nX, nY + SigleRow   , nX + DEF_TEXT_WIDE, nY + DEF_TEXT_HEIGHT + SigleRow);
	pDC->DrawText(_T("VCC"), cTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);

	cTextRect = new CRect(nX, nY + SigleRow *2, nX + DEF_TEXT_WIDE, nY + DEF_TEXT_HEIGHT + SigleRow * 2);
	pDC->DrawText(_T("RST"), cTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);



	//此处开始画单元格的虚线
	pDC->SelectObject(pOldPen);    // 恢复旧画笔   
	newPen.DeleteObject();         // 删除新画笔  


	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(0, 100, 0);
	logBrush.lbHatch = HS_VERTICAL;
	newPen.CreatePen(PS_DASH | PS_GEOMETRIC | PS_ENDCAP_ROUND, 1, &logBrush);
	pOldPen = pDC->SelectObject(&newPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen

	
	for (int i = iLT.cx + DEF_SIGLE_WIDE *2; i < iRT.cx; i += DEF_SIGLE_WIDE)
	{
		nX = rect.left + i;
		pDC->MoveTo(nX, iLT.cy);
		pDC->LineTo(nX, iLB.cy);
	}


	return 0;
}


void CWaveForm::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//删除之前绘制内容,此函数会发送 重绘消息
	Invalidate();

}
