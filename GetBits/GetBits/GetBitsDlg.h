
// GetBitsDlg.h: 头文件
//

#pragma once
#include "afxwin.h"
#include "mscomm1.h"


// CGetBitsDlg 对话框
class CGetBitsDlg : public CDialogEx
{
// 构造
public:
	CGetBitsDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GETBITS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_pComPort;
	int InitCombo();
	// 从 COM口  读取 bit 信息 ，包含CLK，高低电平， 以及 Pin口
	bool ReadBitData(BYTE* BitData, UINT* iBitLen, UINT iMaxBitLen);
	// STM32 USART1 的句柄
	HANDLE hSTMUSART1;
	HANDLE hThread;
	int DisplayBit();
	afx_msg void OnBnClickedComconnectButton();
	void NewGetDataThread();
	static int GetDataThread(void);
	int GetBitsData();
	CListBox m_BitsList;
	// com口 
	CMscomm1 m_ctrlComm;
	DECLARE_EVENTSINK_MAP()
	//void OnComm();
	void OnCommMscomm1();
	bool bConnect;
	int bbitsLen ;
	int bdiflen  ;
	BYTE bbits[10];


	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_BYTE_EDIT;
	CString m_Value_EDIT;
	CListBox m_TPDU;
	afx_msg void OnLbnDblclkList2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();	
	afx_msg void OnBnClickedButton3();
	// 获取设备信息
	int GetInformation(BYTE* ucInfo, UINT* uiInfoLen);
	// 查询是否有数据需要被传出
	int GetSectorCount(ULONG64* ulSector,UINT* uiRamLen);

	int GetBitsBuf(UINT uiOffset,ULONG32 ulSector,BYTE* bBits, UINT* uiBitsLen);

	int SendBuf2Comm(BYTE* ucBuf , UINT ucBufLen);
	int ReceiveBuf4Comm(BYTE* ucBuf , UINT* ucBufLen);

public:
	int DisplayBitsData(BYTE* ucBits, UINT BitsLen);
	int _DisplayBitsData(BYTE* ucBits, UINT BitsLen);
	int _DisplayBitsData_Exp();
};
