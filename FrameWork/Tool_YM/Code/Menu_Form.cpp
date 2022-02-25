// ../Code/MenuForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Menu_Form.h"

#include "MainFrm.h"
#include "Map_Tool.h"
#include "Cam_Tool.h"

// CMenu_Form
IMPLEMENT_DYNCREATE(CMenu_Form, CFormView)

CMenu_Form::CMenu_Form()
	: CFormView(IDD_CMenu_Form)
{
	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
}

void CMenu_Form::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: MenuForm 초기화
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	Ready_Tab();
}

CMenu_Form::~CMenu_Form()
{
}

void CMenu_Form::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MenuTab);
}

HRESULT CMenu_Form::Ready_Tab(void)
{
	CString tabOne = _T("Map");
	CString tabTwo = _T("Camera");

	m_MenuTab.SetCurSel(0);

	m_MenuTab.InsertItem(1, tabOne);
	m_MenuTab.InsertItem(2, tabTwo);

	CRect rtTemp;
	m_MenuTab.GetWindowRect(&rtTemp);

	//첫번째 탭
	m_pMap_Tool = new CMap_Tool;
	m_pMap_Tool->Create(IDD_CMap_Tool, &m_MenuTab);
	m_pMap_Tool->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pMap_Tool->ShowWindow(SW_SHOW);

	//두번째 탭
	m_pCam_Tool = new CCam_Tool;
	m_pCam_Tool->Create(IDD_CCam_Tool, &m_MenuTab);
	m_pCam_Tool->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pCam_Tool->ShowWindow(SW_HIDE);

	return S_OK;
}

_int CMenu_Form::Update_Menu_Form(const _double& _dTimeDelta)
{
	return _int();
}

BEGIN_MESSAGE_MAP(CMenu_Form, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDCANCEL, &CMenu_Form::OnTcnSelchangeCancel)
END_MESSAGE_MAP()


// CMenu_Form 진단

#ifdef _DEBUG
void CMenu_Form::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMenu_Form::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMenu_Form 메시지 처리기

void CMenu_Form::OnTcnSelchangeCancel(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Tab 선택

	m_eSelectTab = (TAB_ID)m_MenuTab.GetCurSel();

	switch (m_eSelectTab)
	{
	case TAB_MAP:
		m_pMap_Tool->ShowWindow(SW_SHOW);
		m_pCam_Tool->ShowWindow(SW_HIDE);
		break;
	case TAB_CAM:
		m_pMap_Tool->ShowWindow(SW_HIDE);
		m_pCam_Tool->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}
