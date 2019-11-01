// WaveView.cpp: 实现文件
//

//#include "pch.h"
#include "stdafx.h"
#include "BitsAnalyser.h"
#include "WaveView.h"
#include "../../AllUsedSource/BasicConvert/BasicConvert/BasicConvert.h"



IMPLEMENT_DYNAMIC(CWaveForm, CStatic)

CWaveForm::CWaveForm()
{
	extern BYTE* __Bits;
	ucBits    = NULL;
	iBitsLen  = 0;
	iGroupCLK = 100;

	iStartPos = 0;



}
CWaveForm::~CWaveForm()
{

}
void CWaveForm::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CStatic::OnPaint()

	CStatic::OnPaint();
	CRect rect;
	GetClientRect(&rect);//把picture的控件尺寸付给rectPicture对象，传递给以便DrawWave
	//DrawBackGround(GetDC(), rect);
	//DrawLine(GetDC(), rect);
	CDC* pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(&rect, RGB(255, 255, 255));

	DrawBackGround(&memDC, rect);
	DrawLine(&memDC, rect, NULL);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);




}

void CWaveForm::DrawBackGround(CDC* pDC, CRect& rect)
{
	//float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	//float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   
	CPen newPen;       // 用于创建新画笔   
	CPen* pOldPen;     // 用于存放旧画笔   
	CBrush newBrush;   // 用于创建新画刷   
	CBrush* pOldBrush; // 用于存放旧画刷   

	POINT iLT, iLB, iRT, iRB;


	iLT.x = rect.left + DEF_LEFT_FRAME_INDENT;
	iLT.y = rect.top  + DEF_TOP_FRAME_INDENT;

	iLB.x = rect.left   + DEF_LEFT_FRAME_INDENT;
	iLB.y = rect.bottom - DEF_BOTTON_FRAME_INDENT;

	iRT.x = rect.right - DEF_RIGHT_FRAME_INDENT;
	iRT.y = rect.top   + DEF_TOP_FRAME_INDENT;

	iRB.x = rect.right  - DEF_RIGHT_FRAME_INDENT;
	iRB.y = rect.bottom - DEF_BOTTON_FRAME_INDENT;

	int SigleRow = (iLB.y - iLT.y) / 4;


	// 计算fDeltaX和fDeltaY   
	//fDeltaX = (float)rect.Width() / (POINT_COUNT - 1);;
	//fDeltaY = (float)rect.Height() / 1;

	newBrush.CreateSolidBrush(RGB(0, 0, 0)); // 创建黑色新画刷
	pOldBrush = pDC->SelectObject(&newBrush);// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pDC->Rectangle(rect);    // 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	pDC->SelectObject(pOldBrush);    // 恢复旧画刷
	newBrush.DeleteObject();    // 删除新画刷


	//此处 画边框

	newPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 100));    // 创建实心画笔，粗度为2，颜色为暗红色 
	pOldPen = pDC->SelectObject(&newPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen


	//画到左边上 顶点 

	pDC->MoveTo(iLT.x, iLT.y);

	//画到左边下 顶点 
	pDC->LineTo(iLB.x, iLB.y);

	//画到右边下 顶点 
	pDC->LineTo(iRB.x, iRB.y);

	//画到右边上 顶点 
	pDC->LineTo(iRT.x, iRT.y);

	//回到开始位置 
	pDC->LineTo(iLT.x, iLT.y);

	//此处开始 画  IO VCC 和 RST 分割线


	pDC->MoveTo(iLT.x, iLT.y + SigleRow);
	pDC->LineTo(iRT.x, iLT.y + SigleRow);

	pDC->MoveTo(iLT.x, iLT.y + SigleRow * 2);
	pDC->LineTo(iRT.x, iLT.y + SigleRow * 2);

	pDC->MoveTo(iLT.x, iLT.y + SigleRow * 3);
	pDC->LineTo(iRT.x, iLT.y + SigleRow * 3);

	//开始写 分割线 标识 

	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->SetBkColor(RGB(0, 0, 0));

	CRect* cTextRect;
	nX = iLT.x + DEF_TEXT_INDENT;
	nY = iLT.y + DEF_TEXT_INDENT;
	cTextRect = new CRect(nX, nY, nX + DEF_TEXT_WIDE, nY + DEF_TEXT_HEIGHT);
	pDC->DrawText(_T("IO"), cTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
	delete(cTextRect);

	cTextRect = new CRect(nX, nY + SigleRow, nX + DEF_TEXT_WIDE, nY + DEF_TEXT_HEIGHT + SigleRow);
	pDC->DrawText(_T("VCC"), cTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);

	cTextRect = new CRect(nX, nY + SigleRow * 2, nX + DEF_TEXT_WIDE, nY + DEF_TEXT_HEIGHT + SigleRow * 2);
	pDC->DrawText(_T("RST"), cTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);

	cTextRect = new CRect(nX, nY + SigleRow * 3, nX + DEF_TEXT_WIDE, nY + DEF_TEXT_HEIGHT + SigleRow * 3);
	pDC->DrawText(_T("CLK"), cTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);

	//此处开始画单元格的虚线
	pDC->SelectObject(pOldPen);    // 恢复旧画笔   
	newPen.DeleteObject();         // 删除新画笔  


	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(0, 100, 0);
	logBrush.lbHatch = HS_VERTICAL;
	newPen.CreatePen(PS_DASH | PS_GEOMETRIC | PS_ENDCAP_ROUND, 1, &logBrush);
	pOldPen = pDC->SelectObject(&newPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen


	for (int i = iLT.x + DEF_SIGLE_WIDE * 2; i < iRT.x; i += DEF_SIGLE_WIDE)
	{
		nX = rect.left + i;
		pDC->MoveTo(nX, iLT.y);
		pDC->LineTo(nX, iLB.y);
	}




	pDC->SelectObject(pOldPen);    // 恢复旧画笔   
	newPen.DeleteObject();         // 删除新画笔  


	//生成一些绘制波形的参数 

	iIOUP    = rect.top + DEF_TOP_FRAME_INDENT + SigleRow / 5;
	iIODOWN  = iIOUP    +  SigleRow * 3 / 5;

	iVCCUP = rect.top + DEF_TOP_FRAME_INDENT + SigleRow*6 / 5;
	iVCCDOWN = iVCCUP + SigleRow * 3 / 5;

	iRSTUP = rect.top + DEF_TOP_FRAME_INDENT + SigleRow*11 / 5;
	iRSTDOWN = iRSTUP + SigleRow * 3 / 5;

	iCLKUP = rect.top + DEF_TOP_FRAME_INDENT + SigleRow * 16 / 5;
	iCLKDOWN = iCLKUP + SigleRow * 3 / 5;

	iStartX = iLT.x + DEF_SIGLE_WIDE * 2;

}

void CWaveForm::DrawWave(CDC* pDC, POINT pStart, POINT* pEnd, int iType,int iLimit, int iSighLimit)
{
	CPen  newPen;       // 用于创建新画笔   
	CPen* pOldPen = NULL;     // 用于存放旧画笔   

	pDC->MoveTo(pStart.x, pStart.y);



	//此处表示超宽的波形此处需要省略
	if ((pEnd->x - pStart.x) > iSighLimit )
	{
		if ((iType == DEF_IO_PIN))
		{
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = RGB(0, 255, 0);
			logBrush.lbHatch = HS_VERTICAL;
			newPen.CreatePen(PS_DASHDOTDOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, 2, &logBrush);
			pOldPen = pDC->SelectObject(&newPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen
		}
		pEnd->x = pStart.x + iSighLimit;
	}

	//此处需要画 CLK 部位波形, 此处为全局都有竖线 



	//超出当前可视范围用于后续不画竖线
	BOOL bOverLimit = FALSE;
	int iEndX;

	if ((iLimit != 0) && (pEnd->x > iLimit))
	{
		//

		iEndX = iLimit;
		bOverLimit = TRUE;
	}
	else
		iEndX = pEnd->x;



	if ((iType == DEF_CLK_PIN) &&
		(pStart.y == iCLKUP) &&
		(pEnd->y == iCLKUP))

	{

		for (int i = pStart.x; i < iEndX; i++)
		{
			if ((i%3) == 0)
			{
				pDC->MoveTo(i, iCLKUP);
				pDC->LineTo(i, iCLKDOWN);

			}

		}
		pDC->MoveTo(iEndX, pStart.y);
	}
	else
	{

		pDC->LineTo(iEndX, pStart.y);

		if (pOldPen != NULL)
		{
			pDC->SelectObject(pOldPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen
			newPen.DeleteObject();         // 删除新画笔 

		}
		if (!bOverLimit)
		{
			pDC->LineTo(pEnd->x, pEnd->y);
		}
	}
		
	
}

void CWaveForm::DrawDescription(CDC* pDC, int iTextX,CString csText)
{
	//开始写 分割线 标识 
	int nX, nY;

//	POINT pStart;

	CFont* Oldfont;
	CFont font;
	font.CreateFont(12,                                    //   字体的高度   
		0,                                          //   字体的宽度  
		0,                                          //  nEscapement 
		0,                                          //  nOrientation   
		FW_NORMAL,                                  //   nWeight   
		FALSE,                                      //   bItalic   
		FALSE,                                      //   bUnderline   
		0,                                                   //   cStrikeOut   
		ANSI_CHARSET,                             //   nCharSet   
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
		DEFAULT_QUALITY,                       //   nQuality   
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		_T("宋体"));

	Oldfont = pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->SetBkColor(RGB(0, 0, 0));

	CRect* cTextRect;
	nX = iTextX   ;
	nY = iIODOWN  ;
	cTextRect = new CRect(nX, nY, nX + DEF_TEXT_WIDE, nY + DEF_TEXT_HEIGHT);
	pDC->DrawText(csText, cTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);

	pDC->SelectObject(Oldfont);


}

CString  GenerateEventText(CString csEvent,  BYTE ___Pre)
{
	CString csText;
	BYTE* __Event;
	__Event = new BYTE[csEvent.GetLength() / 2];
	int Len = _CString2UcHex(csEvent, __Event);
	UINT CLK = 0;
	for (int i = 1; i < Len; i++)
	{
		CLK += (__Event[i] << (i - 1) * 8);
	}



	csText.Format("%d", CLK);
	if ((___Pre & 0x80) == 0x80)
		csText = _T("TIME : ") + csText + _T(" CLK");
	else
		csText = _T("TIME : ") + csText + _T(" us");

	return csText;


}

void CWaveForm::DrawLine(CDC* pDC, CRect& rect,POINT* pSelect)
{

	CPen newPen;       // 用于创建新画笔   
	CPen* pOldPen;     // 用于存放旧画笔    

	POINT pDesIO ,pDesVCC, pDesRST,pDesCLK;
	POINT pOriIO, pOriVCC, pOriRST,pOriCLK;

	int iOffset = iStartPos;
	iEndPos = iStartPos;

	newPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));    // 创建实心画笔，粗度为1，颜色为绿色
	pOldPen = pDC->SelectObject(&newPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen

#if DEF_EVENTLIST_DATA
	CString csEvent,csDes;
	BYTE __Event[8];
	int  __EventSize = 0;
	int  __EventSum = m_hEventList->GetEventCount();
	BYTE __Pre;

	if (__EventSum == 0)
	{
		return;

	}
	

	if (iOffset == 0)
	{

		if (m_hEventList->GetEvent(0, csEvent, csDes)<=0)
		{
			return;
		}

		__EventSize = _CString2UcHex(csEvent, __Event);

		if (!GeneratePrePoint(__Event, __EventSize, __Event[0],&pOriIO, &pOriVCC, &pOriRST, &pOriCLK))
		{
			AfxMessageBox("EVENTData is wrong　");
			return;
		}

	}
	else
	{
		m_hEventList->GetEvent(iOffset-1, csEvent, csDes);
		__EventSize = _CString2UcHex(csEvent, __Event);

		__Pre = __Event[0];

		m_hEventList->GetEvent(iOffset, csEvent, csDes);
		__EventSize = _CString2UcHex(csEvent, __Event);


		if (!GeneratePrePoint(__Event, __EventSize, __Pre, &pOriIO, &pOriVCC, &pOriRST, &pOriCLK))
		{
			AfxMessageBox("EVENTData is wrong　");
			return;
		}
	}

	
	//生成第一个网格线之前的图形
	if (!GenerateStartPoint(__Event, __EventSize, &pDesIO, &pDesVCC, &pDesRST, &pDesCLK))
	{
		AfxMessageBox("EVENTData is wrong　");
		return;
	}


	//绘制起始位置之前的
	int iLimit = rect.right - DEF_RIGHT_FRAME_INDENT;

	DrawWave(pDC, pOriIO, &pDesIO  , DEF_IO_PIN ,iLimit);
	DrawWave(pDC, pOriVCC, &pDesVCC, DEF_VCC_PIN,iLimit);
	DrawWave(pDC, pOriRST, &pDesRST, DEF_RST_PIN,iLimit);
	DrawWave(pDC, pOriCLK, &pDesCLK, DEF_CLK_PIN,iLimit);

	pOriIO = pDesIO;
	pOriVCC = pDesVCC;
	pOriRST = pDesRST;
	pOriCLK = pDesCLK;

	//生成第一个事件
	for (int i = iOffset+1; i < __EventSum; i++)
	{
		iEndPos = i;
		__Pre = __Event[0];
		m_hEventList->GetEvent(i, csEvent, csDes);
		__EventSize = _CString2UcHex(csEvent, __Event);
		GeneratePoint(__Event, __EventSize, &pDesIO, &pDesVCC, &pDesRST, &pDesCLK);




		//DrawWave(pDC, pOriIO, &pDesIO, iLimit);
		//DrawWave(pDC, pOriVCC, &pDesVCC, iLimit);
		//DrawWave(pDC, pOriRST, &pDesRST, iLimit);
		//DrawWave(pDC, pOriCLK, &pDesCLK, iLimit);
		DrawWave(pDC, pOriIO, &pDesIO, DEF_IO_PIN, iLimit);
		DrawWave(pDC, pOriVCC, &pDesVCC, DEF_VCC_PIN, iLimit);
		DrawWave(pDC, pOriRST, &pDesRST, DEF_RST_PIN, iLimit);
		DrawWave(pDC, pOriCLK, &pDesCLK, DEF_CLK_PIN, iLimit);

		DrawDescription(pDC, pOriIO.x, csDes);

		if (pSelect != NULL)
		{
			if ((pSelect->x >= pOriIO.x) &&
				(pSelect->x < pDesIO.x))

			{
				DrawSignLine(pDC, pOriIO.x, rect.top + DEF_TOP_FRAME_INDENT, rect.bottom - DEF_BOTTON_FRAME_INDENT);
				POINT pCLKText;
				pCLKText.x =  rect.left + 85;
				pCLKText.y =  rect.top + 5;
				DrawEventCLK(pDC, GenerateEventText(csEvent, __Pre) , pCLKText);
			}
		}


		if (pDesIO.x >= iLimit)
		{
			return;
		}

		pOriIO  = pDesIO;
		pOriVCC = pDesVCC;
		pOriRST = pDesRST;
		pOriCLK = pDesCLK;

	}





#else

	GenerateStartPoint(&iOffset, &pOriIO, &pOriVCC, &pOriRST);

	pDesIO = pOriIO;
	pDesVCC = pOriVCC;
	pDesRST = pOriRST;

	for (1; iOffset < iBitsLen; 1)
	{



		GeneratePoint(&iOffset, &pDesIO, &pDesVCC, &pDesRST);

		if (pDesIO.y > rect.right)
		{
			return;

		}

		DrawWave(pDC, pOriIO, pDesIO);
		DrawWave(pDC, pOriVCC, pDesVCC);
		DrawWave(pDC, pOriRST, pDesRST);

		pOriIO = pDesIO;
		pOriVCC = pDesVCC;
		pOriRST = pDesRST;

#endif

	
	


	//pVCCFARME.x = rect.left + DEF_LEFT_FRAME_INDENT + DEF_SIGLE_WIDE;
	//pVCCFARME.y = rect.top +  DEF_TOP_FRAME_INDENT  + SigleRow * 2 - SigleRow * 1 / 5;

	//pRSTFARME.x = rect.left + DEF_LEFT_FRAME_INDENT + DEF_SIGLE_WIDE;
	//pRSTFARME.y = rect.bottom - DEF_BOTTON_FRAME_INDENT - SigleRow * 1 / 5;

	//POINT pIO, pVCC, pRST;

	////此处 画边框

	//newPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));    // 创建实心画笔，粗度为1，颜色为绿色
	//pOldPen = pDC->SelectObject(&newPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen

	//pIO.x = pIOFARME.x;
	//pIO.y = pIOFARME.y - SigleRow*3/5;

	//pDC->MoveTo(pIO.x, pIO.y);

	//pDC->LineTo(pIO.x + 40, pIO.y);

	//if(pIOFARME.y != pIO.y)
	//	pIO.y = pIOFARME.y;

	//pDC->LineTo(pIO.x + 40, pIO.y);






	pDC->SelectObject(pOldPen);    // 恢复旧画笔   
	newPen.DeleteObject();         // 删除新画笔  

}

void CWaveForm::RemoveWave()
{

	CRect rect;
	GetClientRect(&rect);
	DrawBackGround(GetDC(), rect);
	iStartPos = 0;
	iEndPos = 0;
}


//void  CWaveForm::GenerateStartPoint(int* BitsOffset, POINT* pIO, POINT* pVCC, POINT* pRST)
//{
//	int iOffset = *BitsOffset;
//	int iCLKNum = 0;
//	int iCLKTEMP = 0;
//	
//	for (int i = 0; i < (ucBits[iOffset] & 0x7); i++)
//	{
//		iCLKTEMP = ucBits[iOffset + 1 + i];
//		iCLKTEMP = (iCLKTEMP << (i * 8));
//		iCLKNum += iCLKTEMP;
//	}
//
//	int iDetal = iCLKNum * 40 / iGroupCLK;
//	if (iDetal == 0)
//		iDetal = 1;
//
//	if (iDetal > 40)
//		iDetal = 40;
//
//	pIO->x  = iStartX - iDetal;
//	pVCC->x = iStartX - iDetal;
//	pRST->x = iStartX - iDetal;
//
//	if ((ucBits[iOffset] & DEF_IO_PIN) == DEF_IO_PIN)
//		pIO->y = iIOUP;
//	else
//		pIO->y = iIODOWN;
//
//
//	if ((ucBits[iOffset] & DEF_VCC_PIN) == DEF_VCC_PIN)
//		pVCC->y = iVCCUP;
//	else
//		pVCC->y = iVCCDOWN;
//
//
//	if ((ucBits[iOffset] & DEF_RST_PIN) == DEF_RST_PIN)
//		pRST->y = iRSTUP;
//	else
//		pRST->y = iRSTDOWN;
//	*BitsOffset += ((ucBits[iOffset] & 0x7) + 1);
//}
//
//
//
////此时三个点必须有数据,此时是作为初始点存在,会在当前基础上 生成绝对的点
//void CWaveForm::GeneratePoint(int* BitsOffset, POINT* pIO, POINT* pVCC, POINT* pRST)
//{
//	int iOffset  = *BitsOffset;
//	int iCLKNum  = 0;
//	int iCLKTEMP = 0;
//
//	for (int i = 0; i < (ucBits[iOffset]&0x7); i++)
//	{
//		iCLKTEMP = ucBits[iOffset + 1 + i];
//		iCLKTEMP = (iCLKTEMP << (i * 8));
//		iCLKNum += iCLKTEMP;
//	}
//	int iDetal = iCLKNum*40 / iGroupCLK;
//	if (iDetal == 0)
//		iDetal = 1;
//
//	//if (iDetal > 40*8)
//	//	iDetal = 320;
//
//
//	pIO ->x = pIO ->x + iDetal;
//	pVCC->x = pVCC->x + iDetal;
//	pRST->x = pRST->x + iDetal;
//
//	if ((ucBits[iOffset] & DEF_IO_PIN) == DEF_IO_PIN)
//		pIO->y = iIOUP;
//	else
//		pIO->y = iIODOWN;
//
//	
//	if ((ucBits[iOffset] & DEF_VCC_PIN) == DEF_VCC_PIN)
//		pVCC->y = iVCCUP;
//	else
//		pVCC->y = iVCCDOWN;
//
//
//	if ((ucBits[iOffset] & DEF_RST_PIN) == DEF_RST_PIN)
//		pRST->y = iRSTUP;
//	else
//		pRST->y = iRSTDOWN;
//
//
//	*BitsOffset += ((ucBits[iOffset] & 0x7) + 1);
//}

void CWaveForm::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//删除之前绘制内容,此函数会发送 重绘消息
	Invalidate();

}


void CWaveForm::InputBitsDatas(BYTE* Bits, int BitsLen)
{

	ucBits   = Bits;
	iBitsLen = BitsLen;

}
int  CWaveForm::GetBits(BYTE* ucBitss)
{
	// TODO: 在此处添加实现代码.
	if(ucBitss != NULL)
	ucBitss = ucBits;
	return iBitsLen;
}
int  CWaveForm::SetPos(int iPos)
{

#if DEF_EVENTLIST_DATA
	iStartPos = iPos -1;
	return iPos;
#else
	int iOffset = 0;
	int __Len;
	iPos = iPos - 1;
	if (iPos >= iBitsLen)
	{
		return 0;
	}
	do
	{
		__Len = (ucBits[iOffset] & 0x7);
		iOffset += (1 + __Len);
	} while (iOffset < iPos);
	return iOffset + 1;
#endif

	

}



int  CWaveForm::GeneratePrePoint(BYTE* bits, int bitSize, BYTE prebits0,
	POINT* pIO, POINT* pVCC, POINT* pRST, POINT* pCLK)
{

	ULONG iCLKNum = 0;
	ULONG iCLKTEMP = 0;

	if ((bits[0] & 7) != (bitSize - 1))
		return FALSE;

	for (int i = 0; i < (bits[0] & 0x7); i++)
	{
		iCLKTEMP = bits[1 + i];
		iCLKTEMP = (iCLKTEMP << (i * 8));
		iCLKNum += iCLKTEMP;
	}

	ULONG iDetal = iCLKNum * 40 / iGroupCLK;
	if (iDetal == 0)
		iDetal = 1;

	if ((iDetal > 40) || (iDetal < 0))
		iDetal = 40;

	pIO->x  = iStartX - iDetal;
	pVCC->x = iStartX - iDetal;
	pRST->x = iStartX - iDetal;
	pCLK->x = iStartX - iDetal;

	if ((prebits0 & DEF_IO_PIN) == DEF_IO_PIN)
		pIO->y = iIOUP;
	else
		pIO->y = iIODOWN;


	if ((prebits0 & DEF_VCC_PIN) == DEF_VCC_PIN)
		pVCC->y = iVCCUP;
	else
		pVCC->y = iVCCDOWN;


	if ((prebits0 & DEF_RST_PIN) == DEF_RST_PIN)
		pRST->y = iRSTUP;
	else
		pRST->y = iRSTDOWN;


	if ((prebits0 & DEF_CLK_PIN) == DEF_CLK_PIN)
		pCLK->y = iCLKUP;
	else
		pCLK->y = iCLKDOWN;

	return TRUE;
}


int  CWaveForm::GenerateStartPoint(BYTE* bits,int bitSize, 
	POINT* pIO, POINT* pVCC, POINT* pRST,POINT* pCLK)
{

	//int iCLKNum = 0;
	//int iCLKTEMP = 0;

	//if ((bits[0]&7) != (bitSize-1))
	//	return FALSE;

	//for (int i = 0; i < (bits[0] & 0x7); i++)
	//{
	//	iCLKTEMP = bits[ 1 + i];
	//	iCLKTEMP = (iCLKTEMP << (i * 8));
	//	iCLKNum += iCLKTEMP;
	//}

	//int iDetal = iCLKNum * 40 / iGroupCLK;
	//if (iDetal == 0)
	//	iDetal = 1;

	//if ((iDetal > 40)||(iDetal <0))
	//	iDetal = 40;




	pIO->x  = iStartX ;
	pVCC->x = iStartX ;
	pRST->x = iStartX ;
	pCLK->x = iStartX;

	if ((bits[0] & DEF_IO_PIN) == DEF_IO_PIN)
		pIO->y = iIOUP;
	else
		pIO->y = iIODOWN;


	if ((bits[0] & DEF_VCC_PIN) == DEF_VCC_PIN)
		pVCC->y = iVCCUP;
	else
		pVCC->y = iVCCDOWN;


	if ((bits[0] & DEF_RST_PIN) == DEF_RST_PIN)
		pRST->y = iRSTUP;
	else
		pRST->y = iRSTDOWN;

	if ((bits[0] & DEF_CLK_PIN) == DEF_CLK_PIN)
		pCLK->y = iCLKUP;
	else
		pCLK->y = iCLKDOWN;

	

	return TRUE;
}

int CWaveForm::GeneratePoint(BYTE* bits, int bitSize, POINT* pIO, POINT* pVCC, POINT* pRST, POINT* pCLK)
{

	long iCLKNum = 0;
	long iCLKTEMP = 0;

	if ((bits[0] & 7) != (bitSize - 1))
		return FALSE;
	for (int i = 0; i < (bits[0] & 0x7); i++)
	{
		iCLKTEMP = bits[ 1 + i];
		iCLKTEMP = (iCLKTEMP << (i * 8));
		iCLKNum += iCLKTEMP;
	}
	long iDetal = iCLKNum * 40 / iGroupCLK;
	if (iDetal == 0)
		iDetal = 1;



	//此处 防止出现负值出现, 最大也不会超出 10000
	if ((iDetal > 10000)||(iDetal< 0))
		iDetal = 10000;


	pIO->x = pIO->x + iDetal;
	pVCC->x = pVCC->x + iDetal;
	pRST->x = pRST->x + iDetal;
	pCLK->x = pCLK->x + iDetal;


	if ((bits[0] & DEF_IO_PIN) == DEF_IO_PIN)
		pIO->y = iIOUP;
	else
		pIO->y = iIODOWN;


	if ((bits[0] & DEF_VCC_PIN) == DEF_VCC_PIN)
		pVCC->y = iVCCUP;
	else
		pVCC->y = iVCCDOWN;


	if ((bits[0] & DEF_RST_PIN) == DEF_RST_PIN)
		pRST->y = iRSTUP;
	else
		pRST->y = iRSTDOWN;


	if ((bits[0] & DEF_CLK_PIN) == DEF_CLK_PIN)
		pCLK->y = iCLKUP;
	else
		pCLK->y = iCLKDOWN;

	return TRUE;
}

int CWaveForm::InputEventWnd(CWnd* EventWnd)
{
	m_hEventList = (CEventList*)EventWnd;
	return TRUE;
}

int CWaveForm::InputPrescale(int iPrescale)
{

	iGroupCLK = iPrescale;

	return iPrescale;

}


void CWaveForm::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	CDC* pDC = GetDC();
	//Invalidate();

	GetClientRect(&rect);
	//DrawBackGround(pDC, rect);
	//DrawLine(pDC, rect, &point);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(&rect, RGB(255, 255, 255));

	DrawBackGround(&memDC, rect);
	DrawLine(&memDC, rect, &point);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);

}

void CWaveForm::DrawSignLine(CDC* pDC,int x,int StartY,int EndY)
{
	CPen  newPen;         // 用于创建新画笔   
	CPen* pOldPen = NULL;     // 用于存放旧画笔   

	newPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 100));    // 创建实心画笔，粗度为2，颜色为暗红色 
	pOldPen = pDC->SelectObject(&newPen);    // 选择新画笔，并将旧画笔的指针保存到pOldPen
	pDC->MoveTo(x, StartY);
	pDC->LineTo(x, EndY);
	pDC->SelectObject(pOldPen);    // 恢复旧画笔   
	newPen.DeleteObject();         // 删除新画笔  
}


void CWaveForm::DrawEventCLK(CDC* pDC, CString csEvent,POINT pEventCLK)
{
	CFont* Oldfont;
	CFont font;
	font.CreateFont(12,                                    //   字体的高度   
		0,                                          //   字体的宽度  
		0,                                          //  nEscapement 
		0,                                          //  nOrientation   
		FW_NORMAL,                                  //   nWeight   
		FALSE,                                      //   bItalic   
		FALSE,                                      //   bUnderline   
		0,                                                   //   cStrikeOut   
		ANSI_CHARSET,                             //   nCharSet   
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
		DEFAULT_QUALITY,                       //   nQuality   
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		_T("宋体"));

	Oldfont = pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->SetBkColor(RGB(0, 0, 0));

	CRect* cTextRect;

	cTextRect = new CRect(pEventCLK.x, pEventCLK.y, pEventCLK.x + 200, pEventCLK.y + DEF_TEXT_HEIGHT);


	pDC->DrawText(csEvent, cTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);

	pDC->SelectObject(Oldfont);

}



BEGIN_MESSAGE_MAP(CWaveForm, CStatic)

	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
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
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CWaveView 消息处理程序




int CWaveView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	m_pWaveForm.Create("", WS_CHILD | WS_VISIBLE | SS_CENTER, rectDummy, this, IDC_WAVEFORM);

	m_pScrollBar.Create(  SBS_HORZ | SBS_BOTTOMALIGN| WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_WAVESCROLL);


	m_pScrollBar.SetScrollRange(1, 100);
	return 0;
}


void CWaveView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_pWaveForm.SetWindowPos(this, -1, -1, cx, cy - 20, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	m_pScrollBar.SetWindowPos(this, -1, cy - 20, cx, 20, SWP_NOACTIVATE| SWP_NOZORDER);

	int iCount = m_pScrollBar.GetScrollLimit();
	m_pScrollBar.SetScrollRange(1, iCount);

	

}


void CWaveView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int __pos = m_pScrollBar.GetScrollPos();
	int __Oripos = __pos;
	int __Newpos;

	//switch (nSBCode)
	//{
	//	// 如果向左滚动一列，则pos减1  
	//case SB_LINELEFT:
	//	__pos -= 2;
	//	break;
	//	// 如果向右滚动一列，则pos加1  
	//case SB_LINERIGHT:
	//	__pos += 2;
	//	break;
	//	//	// 如果向左滚动一页，则pos减10  
	//	//case SB_PAGELEFT:
	//	//	__pos -= 10;
	//	//	break;
	//	//	// 如果向右滚动一页，则pos加10  
	//	//case SB_PAGERIGHT:
	//	//	__pos += 10;
	//	//	break;
	//	//	// 如果滚动到最左端，则pos为1  
	//	//case SB_LEFT:
	//	//	__pos = 1;
	//	//	break;
	//	//	// 如果滚动到最右端，则pos为100  
	//	//case SB_RIGHT:
	//	//	__pos = 100;
	//	//	break;
	//		// 如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值  
	//case SB_THUMBPOSITION:
	//	__pos = nPos;
	//	break;
	//	// 下面的m_horiScrollbar.SetScrollPos(pos);执行时会第二次进入此函数，最终确定滚动块位置，并且会直接到default分支，所以在此处设置编辑框中显示数值  
	//default:
	//	//SetDlgItemInt(IDC_HSCROLL_EDIT, pos);
	//	return;
	//}


	SCROLLINFO   info;
	pScrollBar->GetScrollInfo(&info, SIF_ALL);

//	do
//	{
		switch (nSBCode)
		{
			// 如果向左滚动一列，则pos减1  
		case SB_LINELEFT:
			if (__pos < 1)
				return;
			__pos -= 1;
			break;
			// 如果向右滚动一列，则pos加1  
		case SB_LINERIGHT:
			__pos += 1;
			break;
			// 如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值  
			// 如果向左滚动一页，则pos减10  
		case SB_PAGELEFT:
			__pos -= 20;
			break;
			// 如果向右滚动一页，则pos加20  
		case SB_PAGERIGHT:
			__pos += 20;
			break;

		case SB_THUMBPOSITION:
			//npos 只有16位 ,此处不够
			__pos = info.nTrackPos;
			break;
		default:return;
		}

		if (__pos < 1)
			__pos = 1;
		if (__pos > m_pScrollBar.GetScrollLimit())
			__pos = m_pScrollBar.GetScrollLimit();

		__Newpos = m_pWaveForm.SetPos(__pos);



//	} while (__Newpos == __Oripos);


	m_pWaveForm.OnPaint();
	m_pScrollBar.SetScrollPos(__Newpos);

	//CDockablePane::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CWaveView ::InputEventWnd(CWnd* EventWnd)
{
	return m_pWaveForm.InputEventWnd(EventWnd);
}

int CWaveView::InputPrescale(int iPrescale)
{


	return m_pWaveForm.InputPrescale(iPrescale);
	
}

void CWaveView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_pWaveForm.OnLButtonDown(nFlags, point);

	CDockablePane::OnLButtonDown(nFlags, point);
}

void CWaveView::RemoveWave()
{

	m_pWaveForm.RemoveWave();
	m_pScrollBar.SetScrollRange(1, 1);
}