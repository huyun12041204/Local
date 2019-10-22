#pragma once

#define DEF_SIGLE_WIDE  40
#define DEF_TEXT_HEIGHT 20
#define DEF_TEXT_WIDE   40
#define DEF_TEXT_INDENT 10

class CWaveForm :public CStatic
{
	DECLARE_DYNAMIC(CWaveForm)

public:
	CWaveForm();
	virtual ~CWaveForm();
protected:
	DECLARE_MESSAGE_MAP()



public:
	int DrawBackGround(CDC* pDC, CRect& rect);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


// CWaveView

class CWaveView : public CDockablePane
{
	DECLARE_DYNAMIC(CWaveView)

public:
	CWaveView();
	virtual ~CWaveView();

	CWaveForm m_pWaveForm;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnPaint();
	afx_msg void OnPaintClipboard(CWnd* pClipAppWnd, HGLOBAL hPaintStruct);
	afx_msg void OnMove(int x, int y);
};


