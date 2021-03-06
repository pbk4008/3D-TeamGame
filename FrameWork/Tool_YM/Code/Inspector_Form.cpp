// ../Code/Inspector.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Inspector_Form.h"

#include "MainFrm.h"
#include "Menu_Form.h"
#include "Model_Inspector.h"
#include "Cam_Inspector.h"


// CInspector_Form

IMPLEMENT_DYNCREATE(CInspector_Form, CFormView)

CInspector_Form::CInspector_Form()
	: CFormView(IDD_CInspector)
{

}

CInspector_Form::~CInspector_Form()
{
}


void CInspector_Form::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Inspector Form 초기화

	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pMenu_Form = dynamic_cast<CMenu_Form*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 0));

	Ready_Inspector();
}


void CInspector_Form::Ready_Inspector(void)
{
	CString tabOne = _T("Map");
	CString tabTwo = _T("Camera");

	m_TabInspec.SetCurSel(0);

	m_TabInspec.InsertItem(1, tabOne);
	m_TabInspec.InsertItem(2, tabTwo);

	CRect rtTemp;
	m_TabInspec.GetWindowRect(&rtTemp);

	//첫번째 탭
	m_pModel_Inspec = new CModel_Inspector;
	m_pModel_Inspec->Create(IDD_CModel_Inspector, &m_TabInspec);
	m_pModel_Inspec->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pModel_Inspec->ShowWindow(SW_SHOW);

	//두번째 탭
	m_pCam_Inspec = new CCam_Inspector;
	m_pCam_Inspec->Create(IDD_CCam_Inspector, &m_TabInspec);
	m_pCam_Inspec->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pCam_Inspec->ShowWindow(SW_HIDE);
}

_int CInspector_Form::Update_Inspector_Form(const _double& _dTimeDelta)
{
	m_pModel_Inspec->Update_Model_Inspector(_dTimeDelta);

	return _int();
}

HRESULT CInspector_Form::Get_ModelInfo(const FILEINFO& _FileInfo)
{
	if (nullptr == m_pModel_Inspec)
		return E_FAIL;

	return m_pModel_Inspec->Get_ModelInfo(_FileInfo);
}

HRESULT CInspector_Form::Delete_Clone_ModelList(wstring _ModelName)
{
	return m_pModel_Inspec->Delete_Clone_ModelList(_ModelName);
}

void CInspector_Form::Clear_Clone_ModelList(void)
{
	m_pModel_Inspec->Clear_Clone_ModelList();
}

HRESULT CInspector_Form::Add_GameObjectToLayer(const MESHDESC& ModelDesc)
{
	return m_pModel_Inspec->Add_GameObjectToLayer(ModelDesc);;
}

void CInspector_Form::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabInspec);
}

BEGIN_MESSAGE_MAP(CInspector_Form, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CInspector_Form::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CInspector_Form 진단

#ifdef _DEBUG
void CInspector_Form::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInspector_Form::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInspector_Form 메시지 처리기

void CInspector_Form::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Menu Tab Click

	switch (m_eSelectTab)
	{
	case INSPEC_MODEL:
		m_pModel_Inspec->ShowWindow(SW_SHOW);
		m_pCam_Inspec->ShowWindow(SW_HIDE);
		break;
	case INSPEC_CAM:
		m_pModel_Inspec->ShowWindow(SW_HIDE);
		m_pCam_Inspec->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


void CInspector_Form::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	CFormView::PostNcDestroy();
}
