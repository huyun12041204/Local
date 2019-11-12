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

// ProfileExplorerView.cpp: CProfileExplorerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ProfileExplorer.h"
#endif
#include "ProfileExplorer.h"
#include "ProfileExplorerDoc.h"
#include "ProfileExplorerView.h"
#include "FileProperties.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProfileExplorerView

IMPLEMENT_DYNCREATE(CProfileExplorerView, CView)

BEGIN_MESSAGE_MAP(CProfileExplorerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CProfileExplorerView::OnPropertyChanged)
END_MESSAGE_MAP()

// CProfileExplorerView 构造/析构

CProfileExplorerView::CProfileExplorerView() noexcept
{
	// TODO: 在此处添加构造代码

}

CProfileExplorerView::~CProfileExplorerView()
{
}

BOOL CProfileExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CProfileExplorerView 绘图

void CProfileExplorerView::OnDraw(CDC* /*pDC*/)
{
	CProfileExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CProfileExplorerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CProfileExplorerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CProfileExplorerView 诊断

#ifdef _DEBUG
void CProfileExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CProfileExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProfileExplorerDoc* CProfileExplorerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProfileExplorerDoc)));
	return (CProfileExplorerDoc*)m_pDocument;
}
#endif //_DEBUG


// CProfileExplorerView 消息处理程序


int CProfileExplorerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	GetClientRect(rectDummy);

	// 创建组合:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndFileProperty.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	InitializePropertyGrid();

	InitializeFilePropertyGrid();
	AdjustLayout();
	return 0;
}

void CProfileExplorerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);


	AdjustLayout();
}

void CProfileExplorerView::AdjustLayout()
{
	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	if ((rectClient.IsRectEmpty()) || (rectClient.IsRectNull()))
	{
		return;
	}

	if (m_wndFileProperty.m_hWnd != NULL)
	{
		m_wndFileProperty.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}

	uiChildViewWide = rectClient.Width();
}

int CProfileExplorerView::InitializePropertyGrid(void)
{
	m_wndFileProperty.RemoveAll();
	m_wndFileProperty.EnableHeaderCtrl(FALSE);
	m_wndFileProperty.EnableDescriptionArea(TRUE);
	m_wndFileProperty.SetVSDotNetLook();
	m_wndFileProperty.MarkModifiedProperties();

	HDITEM Item1;
	Item1.cxy = 300;
	Item1.mask = HDI_WIDTH;
	m_wndFileProperty.GetHeaderCtrl().SetItem(0, new HDITEM(Item1));

	return TRUE;
}

int CProfileExplorerView::InitializeFilePropertyGrid(void)
{
	//SetPropListFont();


	m_wndFileProperty.AddProperty(CreateBasicInformation());
	m_wndFileProperty.AddProperty(CreateSpeciality());
	m_wndFileProperty.AddProperty(CreateDFSpeciality());

	m_wndFileProperty.AddProperty(CreateAccessInGSM());
	m_wndFileProperty.AddProperty(CreateAccessInUICC());
	m_wndFileProperty.AddProperty(CreateOTASepeciality());

	//m_property_grid.ModifyStyle(WS_BORDER,WS_CAPTION);

	m_wndFileProperty.ModifyStyle(0, WS_CLIPCHILDREN);



	//CChildFrame* Frm = (CChildFrame*)GetParentFrame();
	//if (!Frm->pcfsOriFile.IsEmpty())
	//	InputFileInformation(Frm->pcfsOriFile);

	//UpdatePropertyStatuse(&m_wndFileProperty);
	//UpdatePropertyOption(&m_wndFileProperty, &((CProfileExplorerApp*)AfxGetApp())->pcFile);
	//((CCardFilesApp*)AfxGetApp())->_DisconnectAccess();
	//iPropMode = DEF_PROPPAGE_FILE_CHANGE;
	return 0;
}

int CProfileExplorerView::UpdateViewStatus()
{
	UpdatePropertyStatuse(&m_wndFileProperty);
	UpdatePropertyOption(&m_wndFileProperty, &((CProfileExplorerApp*)AfxGetApp())->pcFile);
	return TRUE;
}
// 输入文件 Statuse
int CProfileExplorerView::InputStatuse(_variant_t varStatuse)
{
	return _InputStatuse(&m_wndFileProperty, varStatuse);
}

int CProfileExplorerView::InputInforamtion(int iIDInProperties, _variant_t varInput)
{
	if (iIDInProperties <= 0)
		return 0;

	return _InputInforamtion(&m_wndFileProperty, iIDInProperties, varInput);

}

int CProfileExplorerView::_InputFileInformation(int iIDInProperties, _variant_t varInput)
{
	int iRet = FALSE;


	switch (iIDInProperties)
	{
	case 0:
	case DEF_PROPERTIES_BASICINFORMATION:
	case DEF_PROPERTIES_2GAC:
	case DEF_PROPERTIES_3GAC:break;
	case DEF_PROPERTIES_SPECIALITY: iRet = InputStatuse(varInput); break;
	default:                        iRet = InputInforamtion(iIDInProperties, varInput); break;

	}

	return iRet;
}

int CProfileExplorerView::InputFileInformation(PCFS pcfsFile)
{

	for (int i = DEF_FILELIST_HANDLE_COLUMN; i < DEF_FILELIST_CONTENT_COLUMN; i++)
		int iRet = _InputFileInformation(ExchangeFileListID2PropertiesID(i), pcfsFile.GetPrivateCardFileStructData(i));
	return 0;
}

void CProfileExplorerView::_SetPropertyStatueOfLink(BOOL bIsLink)
{

	GetPropertyByID(&m_wndFileProperty, DEF_PROPERTIES_SIZE)->AllowEdit(!bIsLink);
	GetPropertyByID(&m_wndFileProperty, DEF_PROPERTIES_RECORDLENGTH)->AllowEdit(!bIsLink);
	GetPropertyByID(&m_wndFileProperty, DEF_PROPERTIES_RECORDNUMBER)->AllowEdit(!bIsLink);
	//((CChildFrame*)GetParentFrame())->m_wndFileContentView->GetDlgItem(IDC_Content_Edit)->EnableWindow(!bIsLink);
}

LRESULT CProfileExplorerView::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{

	int iID = _GetPropertyID(&m_wndFileProperty, (CMFCPropertyGridProperty*)lParam);

	CChildFrame* cForm = (CChildFrame*)GetParentFrame();
	CDocument  * pDoc = GetDocument();
	BOOL bLink        = FALSE;
	BOOL bModified    = FALSE;


	int iStructure = GetPropertyIntByID(&m_wndFileProperty, DEF_PROPERTIES_FILESTRUCTURE);
	if ((GetPropertyIntByID(&m_wndFileProperty, DEF_PROPERTIES_TYPE) == DEF_FILETYPE_EF) &&
		((iStructure == DEF_FILESTRUCTURE_LLINK)||
		(iStructure == DEF_FILESTRUCTURE_TLINK) ||
		(iStructure == DEF_FILESTRUCTURE_CLINK)))
			bLink = TRUE;


	_SetPropertyStatueOfLink(bLink);
	_UpdateProperty(&m_wndFileProperty, iID, &((CProfileExplorerApp*)AfxGetApp())->pcFile);

	bModified = _FileIsModified();


	//设置修改表示, 为标题多出'*'
	((CProfileExplorerDoc*)pDoc)->SetModifiedFlag(bModified);

	return TRUE;
}



_variant_t CProfileExplorerView::__GetPropertyValue(int iCode)
{
	_variant_t _temp;
	CChildFrame* Frm;
	Frm = (CChildFrame*)GetParentFrame();

	if (iCode != DEF_FILELIST_CONTENT_COLUMN)
		_temp = GetPropertyValue(&m_wndFileProperty, ExchangeFileListID2PropertiesID(iCode));

	return _temp;
}
BOOL CProfileExplorerView::_GetFile(PCFS* _File)
{
	CChildFrame* pFrm = ((CChildFrame*)GetParentFrame());
	BOOL bRet = TRUE;
	_variant_t _temp;

	//部分参数不会存在于View内, 所有先置成原始值.便于排出非有用途参数的干扰
	*_File = pFrm->pcfsOriFile;

	try
	{
		for (int i = 0; i < DEF_FILELIST_CONTENT_COLUMN; i++)
		{
			_temp = this->__GetPropertyValue(i);

			bRet &= _File->SetPrivateCardFileStructData(i, _temp);
		}
	}
	catch (...)
	{
		return FALSE;

	}
	return TRUE;
}

BOOL CProfileExplorerView::_FileIsModified()
{
	PCFS _ViewFile;
	if (!_GetFile(&_ViewFile))
	{
		MessageBox("从 View中获取文件失败,检查 view中参数是否都正确!");
		return TRUE;
	}
	return  (_ViewFile.ComparePCFS(((CChildFrame*)GetParentFrame())->pcfsOriFile) < 1);
}


