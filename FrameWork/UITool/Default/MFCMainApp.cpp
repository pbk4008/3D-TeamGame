#include "pch.h"
#include "framework.h"
#include "MFCMainApp.h"
#include "GameInstance.h"

#include "MFCLevel_Logo.h"
#include "MFCLevel_Play.h"
#include "MFCObject_UI.h"
#include "MFCCamera.h"
#include "MFCCamera_Proj.h"
#include "MFCEffect.h"
#include "MFCTerrain.h"


CMFCMainApp::CMFCMainApp()
{
}

HRESULT CMFCMainApp::NativeConstruct()
{
	cout << "Level MainApp" << endl;


	m_pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, TOOL_LEVEL::TOOL_LEVEL_END, CGraphic_Device::WINMODE::MODE_WIN, WINCX, WINCY, &m_pDevice, &m_pDeviceContext)))
	{
		ERR_MSG(L"Failed to Creating Initialize_Engine in CMFCMainApp::NativeConstruct()");
		return E_FAIL;
	}

	if(FAILED(g_pGameInstance->SetUpBaseComponent(m_pDevice, m_pDeviceContext)))
	{
		ERR_MSG(L"Failed to SetUpBaseComponent Engine In CMFCMainApp::NativeConstruct ");
		return E_FAIL;
	}

	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>(0, L"Renderer");

	if (FAILED(Ready_Component_Prototype()))
	{
		return E_FAIL;
	}
	
	if (FAILED(Ready_GameObject_Prototype()))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_StartLevel(TOOL_LEVEL::TOOL_LEVEL_LOGO)))
	{
		ERR_MSG(L"Failed MainApp_SetUp_StartLevel_NativeConstruct");
		return E_FAIL;
	}


	return S_OK;
}

_int CMFCMainApp::Tick(_double TimeDelta)
{
	if (nullptr == g_pGameInstance)
	{
		return -1;
	}

	g_pGameInstance->Tick_Engine(TimeDelta);

	return _int();
}

HRESULT CMFCMainApp::Render()
{
	if (nullptr == g_pGameInstance)
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Clear_BackBuffer_View(XMFLOAT4(0.f, 0.5f, 0.5f, 1.f))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Clear_DepthStencil_View()))
	{
		return E_FAIL;
	}

	/* �� ������ �����ϴ� ��ü���� ���� �Լ��� ȣ���Ѵ� */
	if (FAILED(m_pRenderer->Draw_RenderGroup()))
	{
		return E_FAIL;
	}

	/* �� ���ӳ��� ��Ÿ ����� �������Ѵ� */
	if (FAILED(g_pGameInstance->Render_Engine()))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Present()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMFCMainApp::SetUp_StartLevel(TOOL_LEVEL eLevel)
{
	/* Ư�� ������ �����Ϸ��� ������ �ε���ü(����)�� ���� ����� �����Ұ�. */
	if (nullptr == g_pGameInstance)
	{
		return E_FAIL;
	}

	HRESULT hr = 0;

	switch (eLevel)
	{
	case TOOL_LEVEL::TOOL_LEVEL_LOGO:
		hr = g_pGameInstance->Open_Level((_uint)TOOL_LEVEL::TOOL_LEVEL_LOGO, CMFCLevel_Logo::Create(m_pDevice, m_pDeviceContext));
		break;
	case TOOL_LEVEL::TOOL_LEVEL_GAMEPLAY:
		hr = g_pGameInstance->Open_Level((_uint)TOOL_LEVEL::TOOL_LEVEL_GAMEPLAY, CMFCLevel_Play::Create(m_pDevice, m_pDeviceContext));
		break;
	default:
		MSGBOX("Failed MainApp_SetUp_StartLevel");
		return E_FAIL;
	}

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMFCMainApp::Ready_Component_Prototype()
{
	if (nullptr == g_pGameInstance)
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Prototype_Component_VIBuffer_PointInstance_Explosion", CVIBuffer_PointInstance_Explosion::Create(m_pDevice, m_pDeviceContext/*, L"../../Reference/ShaderFile/Shader_Particle.hlsl", 100*/))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(0, L"Prototype_Component_VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;

	/* Prototype_Component_Texture_Terrain */
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../bin/Resource/Textures/Plane_Default.png")))
	{
		return E_FAIL;
	}

	//���⼭ �׸� �� �ҷ�����
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_0", L"../bin/Resource/Textures/UI/Texture_0.jpg")))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_1", L"../bin/Resource/Textures/UI/Texture_1.jpg")))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"T_HUD_Player_Shield_Icon", L"../bin/Resource/Textures/UI/T_HUD_Player_Shield_Icon.tga")))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"bubble", L"../bin/Resource/Textures/Effect/bubble.png")))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Smoke_loop8x8_00", L"../bin/Resource/Textures/Effect/Smoke_loop8x8_00.tga")))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../bin/Resource/Textures/Plane_Default.png")))
	{
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CMFCMainApp::Ready_GameObject_Prototype()
{
	if (nullptr == g_pGameInstance)
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CMFCTerrain::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"), CMFCCamera::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Proj"), CMFCCamera_Proj::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Texture_0"), CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Texture_1"), CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_T_HUD_Player_Shield_Icon"), CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	//////////////////////////////////////////////////////////////////////////
	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_bubble"), CMFCEffect::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"), CMFCEffect::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CMFCTerrain::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

CMFCMainApp* CMFCMainApp::Create()
{
	CMFCMainApp* pInstance = new CMFCMainApp;
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating MainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMFCMainApp::Free()
{
	Safe_Release(m_pRenderer);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	Safe_Release(g_pGameInstance);
	CGameInstance::Release_Engine();
}