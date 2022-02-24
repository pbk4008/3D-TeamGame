// ../Code/Inspector.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Inspector.h"

#include "MainFrm.h"

// CInspector

IMPLEMENT_DYNCREATE(CInspector, CFormView)

CInspector::CInspector()
	: CFormView(IDD_CInspector)
{

}

CInspector::~CInspector()
{
}

void CInspector::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Inspector 초기화
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	Ready_Inspector();
}


void CInspector::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabInspector);
}

HRESULT CInspector::Ready_Inspector(void)
{
	CString tabOne = _T("Model");
	CString tabTwo = _T("Cam");

	m_tabInspector.SetCurSel(0);

	m_tabInspector.InsertItem(1, tabOne);
	m_tabInspector.InsertItem(2, tabTwo);

	CRect rtTemp;
	m_tabInspector.GetWindowRect(&rtTemp);

	//첫번째 탭
	m_pModel_Inspector = new CModel_Inspector;
	m_pModel_Inspector->Create(IDD_CModel_Inspector, &m_tabInspector);
	m_pModel_Inspector->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pModel_Inspector->ShowWindow(SW_SHOW);

	//두번째 탭
	m_pCam_Inspector = new CCam_Inspector;
	m_pCam_Inspector->Create(IDD_Cam_Inspector, &m_tabInspector);
	m_pCam_Inspector->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pCam_Inspector->ShowWindow(SW_HIDE);

	return S_OK;
}

BEGIN_MESSAGE_MAP(CInspector, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CInspector::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CInspector 진단

#ifdef _DEBUG
void CInspector::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInspector::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInspector 메시지 처리기


void CInspector::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Inspector Select

	m_eSelectInspec = (ID_INSPEC)m_tabInspector.GetCurSel();

	switch (m_eSelectInspec)
	{
	case ID_MODEL:
		m_pModel_Inspector->ShowWindow(SW_SHOW);
		m_pCam_Inspector->ShowWindow(SW_HIDE);
		break;
	case ID_CAM:
		m_pModel_Inspector->ShowWindow(SW_HIDE);
		m_pCam_Inspector->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}
