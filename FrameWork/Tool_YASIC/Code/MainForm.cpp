// MainForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YASIC.h"
#include "MainForm.h"

#include "MaterialTool.h"

// CMainForm

IMPLEMENT_DYNCREATE(CMainForm, CFormView)

CMainForm::CMainForm()
	: CFormView(IDD_CMainForm)
	, m_pMaterialTool(nullptr)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{

}

CMainForm::~CMainForm()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Delete(m_pMaterialTool);
}

void CMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainForm, CFormView)
END_MESSAGE_MAP()


// CMainForm 진단

#ifdef _DEBUG
void CMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainForm 메시지 처리기


void CMainForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (FAILED(g_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, 0, CGraphic_Device::MODE_WIN, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDeviceContext)))
		return;

	


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CMainForm::Tick()
{

}
