#include "pch.h"
#include "framework.h"
#include "MFCMainApp.h"
#include "GameInstance.h"
#include "MFCLevel_Logo.h"
#include "MFCObject_UI.h"
#include "MFCCamera.h"


CMFCMainApp::CMFCMainApp()
{
}

HRESULT CMFCMainApp::NativeConstruct()
{
	m_pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, TOOL_LEVEL::TOOL_LEVEL_END, CGraphic_Device::WINMODE::MODE_WIN, WINCX, WINCY, &m_pDevice, &m_pDeviceContext)))
	{
		ERR_MSG(L"Failed to Creating Initialize_Engine in CMFCMainApp::NativeConstruct()");
		return E_FAIL;
	}

	if(g_pGameInstance->SetUpBaseComponent(m_pDevice, m_pDeviceContext))
	{
		ERR_MSG(L"Failed to SetUpBaseComponent Engine In CMFCMainApp::NativeConstruct ");
		return false;
	}

	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>(0, L"Renderer");

	/* ���������� ����� ����ƽ ������ü���� ���⼭ ������ */
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

	//���⼭ �׸� �� �ҷ�����

	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_0", L"../bin/Resource/Textures/Texture_0.jpg")))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_1", L"../bin/Resource/Textures/Texture_1.jpg")))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Texture_0"), CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Texture_1"), CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"), CMFCCamera::Create(m_pDevice, m_pDeviceContext))))
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

	///* For. Prototype_GameObject_Background */
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Background"), CMFCBackground::Create(m_pDevice, m_pDeviceContext))))
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