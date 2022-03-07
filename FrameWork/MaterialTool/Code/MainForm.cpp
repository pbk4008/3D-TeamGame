// MainForm.cpp: 구현 파일
//
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "pch.h"
#include "Tool_YASIC.h"
#include "MainForm.h"
#include "MaterialTool.h"

// CMainForm

IMPLEMENT_DYNCREATE(CMainForm, CFormView)

CMainForm::CMainForm()
	: CFormView(IDD_CMainForm)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pRenderer(nullptr)
{

}

CMainForm::~CMainForm()
{
	Free();
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
	
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pInstance->Initialize_Engine(g_hInst, g_hWnd, 1, CGraphic_Device::MODE_WIN, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDeviceContext)))
		return;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(g_pGameInstance->SetUpBaseComponent(m_pDevice, m_pDeviceContext)))
		return;


	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>(0, L"Renderer");
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

_int CMainForm::Tick(_double dDeltaTime)
{
	g_pGameInstance->Tick_Engine(dDeltaTime);

	return 0;
}

HRESULT CMainForm::Render()
{
	if (FAILED(g_pGameInstance->Clear_BackBuffer_View(XMFLOAT4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;

	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Render_Engine()))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Present()))
		return E_FAIL;


	return S_OK;
}

void CMainForm::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pRenderer);

	g_pGameInstance->Release_Engine();
}
