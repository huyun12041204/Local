// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// ProfileExplorerView.h: CProfileExplorerView 类的接口
//

#pragma once
#include "ProfileExplorerDoc.h"

class CProfileExplorerView : public CView
{
protected: // 仅从序列化创建
	CProfileExplorerView() noexcept;
	DECLARE_DYNCREATE(CProfileExplorerView)

// 特性
public:
	CProfileExplorerDoc* GetDocument() const;
	CMFCPropertyGridCtrl m_wndFileProperty;
// 操作
public:

	int   InputStatuse(_variant_t varStatuse);
	int   InputInforamtion(int iIDInProperties, _variant_t varInput);
	int  _InputFileInformation(int iIDInProperties, _variant_t varInput);
	int   InputFileInformation(PCFS pcfsFile);
	void _SetPropertyStatueOfLink(BOOL bIsLink);

	_variant_t __GetPropertyValue(int iCode);
	BOOL _GetFile(PCFS* _File);

	BOOL _FileIsModified();
	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CProfileExplorerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);


	void AdjustLayout();
	
	
public:
	int InitializePropertyGrid(void);
	int InitializeFilePropertyGrid(void);
	int UpdateViewStatus();
	DECLARE_MESSAGE_MAP()
	
	



public:
	
};

#ifndef _DEBUG  // ProfileExplorerView.cpp 中的调试版本
inline CProfileExplorerDoc* CProfileExplorerView::GetDocument() const
   { return reinterpret_cast<CProfileExplorerDoc*>(m_pDocument); }
#endif

