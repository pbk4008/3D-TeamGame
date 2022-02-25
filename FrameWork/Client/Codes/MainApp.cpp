#include "pch.h"
#include "MainApp.h"
#include "BackGround.h"
#include "MainCamera.h"
#include "Loading.h"

CMainApp::CMainApp()
{	
}

HRESULT CMainApp::NativeConstruct()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pInstance->Initialize_Engine(g_hInst, g_hWnd, (_uint)SCENEID::SCENE_END, CGraphic_Device::MODE_WIN, g_iWinCx, g_iWinCy, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	//if (FAILED(Ready_Fonts()))
	//	return E_FAIL;
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(SetUp_StartLevel(SCENEID::SCENE_LOGO)))
		return E_FAIL;

	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	m_TimeAcc += TimeDelta;


	g_pGameInstance->Tick_Engine(TimeDelta);

	return _int();
}

HRESULT CMainApp::Render()
{
	if (FAILED(g_pGameInstance->Clear_BackBuffer_View(XMFLOAT4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;	

	/* 내 게임을 구성하는객체들의 렌더함수르리 호출하낟. */
	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;

	/* 내 게임내의 기타등ㄷ응을 렌더링하낟. */
	if (FAILED(g_pGameInstance->Render_Engine()))
		return E_FAIL;

#ifdef _DEBUG
	++m_iNumRender;

	if (m_TimeAcc >= 1.0)
	{
		wsprintf(m_szFPS, TEXT("Frame : %d"), m_iNumRender);
		m_iNumRender = 0;
		m_TimeAcc = 0.0;	
	}

	//if (FAILED(g_pGameInstance->Render_Font(TEXT("Font_Arial"), XMVectorSet(1.f, 0.0f, 0.f, 1.f), m_szFPS)))
	//	return E_FAIL;

#endif // _DEBUG

	if (FAILED(g_pGameInstance->Present()))
		return E_FAIL;


		
	return S_OK;
}

HRESULT CMainApp::SetUp_StartLevel(SCENEID eLevel)
{
	HRESULT		hr = 0;

	switch (eLevel)
	{
	case SCENEID::SCENE_LOGO:
		hr = g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, eLevel));
	default:
		hr = E_FAIL;
	}	

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Component_Prototype()
{
	if(FAILED(g_pGameInstance->SetUpBaseComponent(m_pDevice, m_pDeviceContext)))
		return E_FAIL;
	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>((_uint)SCENEID::SCENE_STATIC, L"Renderer");

	if (!m_pRenderer)
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_GameObject_Prototype()
{
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MainCamer"), CMainCamera::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	// MakeSpriteFont "Gulim" /FontSize:16 /characterregion:0xa960 - 0xa97f Gulim.spritefont
	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Arial"), TEXT("../Bin/Fonts/Arial.spritefont"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Gulim"), TEXT("../Bin/Fonts/Qulim.spritefont"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_HY"), TEXT("../Bin/Fonts/hy.spritefont"))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}	

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	
	CGameInstance::Release_Engine();
}

