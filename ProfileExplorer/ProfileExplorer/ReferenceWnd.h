#pragma once
#include "CAcroAXDocShim.h"

// cReferenceWnd

class CReferenceWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CReferenceWnd)

public:
	CReferenceWnd();
	virtual ~CReferenceWnd();

	CAcroAXDocShim m_wndPDF;
	CString csFileName;
	BOOL    bInstall; 

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//************************************
	// Method:    OpenReferenceList
	// FullName:  CReferenceWnd::OpenReferenceList
	// Access:    public 
	// Returns:   BOOL TRUE FALSE
	// Qualifier: 
	// Parameter: CString csName "31.102", "102.221"
	//************************************
	BOOL OpenReferenceList(CString csName);
	//************************************
	// Method:    SetPosition
	// FullName:  CReferenceWnd::SetPosition
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: long ulPage  
	// Parameter: unsigned int iPosition 1��ͷλ��,2�м�λ��,3ĩβλ��
	//************************************
	BOOL SetPosition(long ulPage, unsigned int iPosition);
	//************************************
	// Method:    GetDFName
	// FullName:  CReferenceWnd::GetDFName
	// Access:    public 
	// Returns:   int
	// Qualifier: See DFName.csv
	// Parameter: CString & csRoute
	//************************************
	int GetDFName(CString &csRoute);
	//************************************
	// Method:    GetReferenceInformation
	// FullName:  CReferenceWnd::GetReferenceInformation
	// Access:    public 
	// Returns:   int
	// Qualifier: See ReferenceList.csv
	// Parameter: CString csRoute  Ŀ¼ "USIM","7FF0","3F00","7F105F3A"
	// Parameter: CString csFID    FID  "6F07"
	// Parameter: long * ulPage    �ĵ���ҳ��  
	// Parameter: unsigned int * uiPosition ����λ�� ��SetPosition
	//************************************
	int GetReferenceInformation(CString csRoute, CString csFID, CString& csDOC, long* ulPage, unsigned int* uiPosition);



	//************************************
	// Method:    GetReferenceDOCFileName
	// FullName:  CReferenceWnd::GetReferenceDOCFileName
	// Access:    public 
	// Returns:   int
	// Qualifier: See SpecificationList.csv
	// Parameter: CString csDOC         11.11                   31.102
	// Parameter: CString & csFileName  ts_100977v081400p.pdf   ts_131102v150100p.pdf
		//************************************
	int GetReferencecsSpecificationName(CString csDOC,CString&csSpecificationName);
	//************************************
	// Method:    OpenReference
	// FullName:  CReferenceWnd::OpenReference
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: CString csRoute See GetReferenceInformation
	// Parameter: CString csFID   See GetReferenceInformation
	//************************************
	int OpenReference(CString csRoute, CString csFID);

	//************************************
// Method:    IsValid
// FullName:  CReferenceWnd::IsValid
// Access:    public 
// Returns:   bool
// Qualifier:
//************************************
	bool IsValid();

};

