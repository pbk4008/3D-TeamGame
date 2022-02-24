// ../Code/MenuForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/MenuForm.h"

#include "MainFrm.h"
#include "Map_Tool.h"
#include "Cam_Tool.h"

// CMenuForm

IMPLEMENT_DYNCREATE(CMenuForm, CFormView)

CMenuForm::CMenuForm()
	: CFormView(IDD_CMenuForm)
{
	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
}

void CMenuForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: MenuForm 초기화
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	Ready_Tab();
}


CMenuForm::~CMenuForm()
{
}

void CMenuForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_tabMenu);
}

HRESULT CMenuForm::Ready_Tab(void)
{
	CString tabOne = _T("Map");
	CString tabTwo = _T("Camera");

	m_tabMenu.SetCurSel(0);

	m_tabMenu.InsertItem(1, tabOne);
	m_tabMenu.InsertItem(2, tabTwo);

	CRect rtTemp;
	m_tabMenu.GetWindowRect(&rtTemp);

	//첫번째 탭
	m_pMap_Tool = new CMap_Tool;
	m_pMap_Tool->Create(IDD_CMap_Tool, &m_tabMenu);
	m_pMap_Tool->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pMap_Tool->ShowWindow(SW_SHOW);

	//두번째 탭
	m_pCam_Tool = new CCam_Tool;
	m_pCam_Tool->Create(IDD_CCam_Tool, &m_tabMenu);
	m_pCam_Tool->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pCam_Tool->ShowWindow(SW_HIDE);

	return S_OK;
}

BEGIN_MESSAGE_MAP(CMenuForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDCANCEL, &CMenuForm::OnTcnSelchangeCancel)
END_MESSAGE_MAP()


// CMenuForm 진단

#ifdef _DEBUG
void CMenuForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMenuForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMenuForm 메시지 처리기

void CMenuForm::OnTcnSelchangeCancel(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Tab 선택

	m_eSelectTab = (TAB_ID)m_tabMenu.GetCurSel();

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
