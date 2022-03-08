#include "pch.h"
#include "MainApp.h"
#include "BackGround.h"
#include "MainCamera.h"
#include "MainCamera_Ortho.h"
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

	if (FAILED(Ready_Fonts()))
		return E_FAIL;
	if (FAILED(Load_Texture()))
		return E_FAIL;
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Init_Camera()))
		return E_FAIL;

#ifdef _DEBUG
	//CDebugSystem* pDebug = GET_INSTANCE(CDebugSystem);
	//if (FAILED(pDebug->Init_DebugSystem(m_pDevice, m_pDeviceContext)))
	//	return E_FAIL;
#endif

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

	/* �� ������ �����ϴ°�ü���� �����Լ����� ȣ���ϳ�. */
	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;

	/* �� ���ӳ��� ��Ÿ����� �������ϳ�. */
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
		break;
	default:
		hr = E_FAIL;
		break;
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

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MainCamera"), CMainCamera::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MainOrthoCamera"), CMainCamera_Ortho::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Load_Texture()
{
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"BackGround", L"../bin/Resources/Texture/Loading/T_LoadScreen_KeyArt_5.tga")))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Init_Camera()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"MainCamera";
	/*tDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);*/
	tDesc.vEye = _float4(0.f, 2.f, -5.f, 1.f);
	tDesc.vAt = _float4(0.f, 2.f, 0.f, 1.f);
	tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 1.f);
	tDesc.fFovy = XMConvertToRadians(60.f);;
	tDesc.fFar = 300.f;
	tDesc.fNear = 0.1f;
	tDesc.fAspect = (_float)g_iWinCx/g_iWinCy;

	if(FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Static", L"Prototype_GameObject_MainCamera", &tDesc)))
		return E_FAIL;

	CCamera::CAMERADESC tDesc2;
	tDesc2.eType = CCamera::CAMERATYPE::CAMERA_ORTHO;
	tDesc2.pCameraTag = L"MainOrthoCamera";
	tDesc2.fWinCX = g_iWinCx;
	tDesc2.fWinCY = g_iWinCy;
	tDesc2.fNear = 0.01f;
	tDesc2.fFar = 1.f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Static", L"Prototype_GameObject_MainOrthoCamera", &tDesc2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	// MakeSpriteFont "Gulim" /FontSize:16 /characterregion:0xa960 - 0xa97f Gulim.spritefont
	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Arial"), TEXT("../bin/Resources/Font/Arial.spritefont"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Gulim"), TEXT("../bin/Resources/Font/Qulim.spritefont"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_HY"), TEXT("../bin/Resources/Font/hy.spritefont"))))
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
	//RELEASE_INSTANCE(CDebugSystem);
	//CDebugSystem::Stop_DebugSystem();
	//if (FAILED(CDebugSystem::DestroyInstance()))
	//	MSGBOX("CDebugSystem Destroy Fail");

	Safe_Release(m_pRenderer);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	
	CGameInstance::Release_Engine();
}

