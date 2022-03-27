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
#include "MFCEffect_floating.h"
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

	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>(0, L"Proto_Component_Renderer");

	if (FAILED(Ready_Component_Prototype()))
	{
		return E_FAIL;
	}
	
	//Effect Texture
	_finddata_t fd;
	ZeroMemory(&fd, sizeof(_finddata_t));

	intptr_t handle = _findfirst("../bin/Resources/Texture/Effect/*.dds", &fd);

	if (handle == 0)
		return E_FAIL;

	int iResult = 0;
	while (iResult != -1)
	{
		char szFullPath[MAX_PATH] = "../bin/Resources/Texture/Effect/";
		strcat_s(szFullPath, fd.name);

		_tchar fbxName[MAX_PATH] = L"";
		_tchar fbxPath[MAX_PATH] = L"";
		MultiByteToWideChar(CP_ACP, 0, fd.name, MAX_PATH, fbxName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, szFullPath, MAX_PATH, fbxPath, MAX_PATH);

		::PathRemoveExtension(fbxName); //이게 있으면 확장자도 지워짐

		if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, fbxName, fbxPath)))
		{
			return E_FAIL;
		}
		wstring Name = fbxName;
		wstring tag = L"Prototype_GameObject_Effect_" + Name;
		if (FAILED(g_pGameInstance->Add_Prototype(tag, CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
		{
			return E_FAIL;
		}

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);

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
	g_pGameInstance->Update_InputDev();

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

	/* 내 게임을 구성하는 객체들의 렌더 함수를 호출한다 */
	if (FAILED(m_pRenderer->Draw_RenderGroup()))
	{
		return E_FAIL;
	}

	/* 내 게임내의 기타 등등을 렌더링한다 */
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
	/* 특정 레벨로 시작하려면 무조건 로딩객체(레벨)을 먼저 만들고 시작할것. */
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

	if (FAILED(g_pGameInstance->Add_Prototype(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Prototype_Component_VIBuffer_PointInstance_Floating", CVIBuffer_PointInstance_Floating::Create(m_pDevice, m_pDeviceContext/*, L"../../Reference/ShaderFile/Shader_Particle.hlsl", 100*/))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(0, L"Prototype_Component_VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;

	/* Prototype_Component_Texture_Terrain */
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../bin/Resources/Texture/Plane_Default.png")))
	{
		return E_FAIL;
	}



	//////////////////////////////////////////Static////////////////////////////////////////////////
	_finddata_t fd;
	ZeroMemory(&fd, sizeof(_finddata_t));

	intptr_t handle = _findfirst("../bin/Resources/Texture/UI/Static/*.dds", &fd);

	if (handle == 0)
		return E_FAIL;

	int iResult = 0;
	while (iResult != -1)
	{
		char szFullPath[MAX_PATH] = "../bin/Resources/Texture/UI/Static/";
		strcat_s(szFullPath, fd.name);

		_tchar fbxName[MAX_PATH] = L"";
		_tchar fbxPath[MAX_PATH] = L"";
		MultiByteToWideChar(CP_ACP, 0, fd.name, MAX_PATH, fbxName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, szFullPath, MAX_PATH, fbxPath, MAX_PATH);

		::PathRemoveExtension(fbxName); //이게 있으면 확장자도 지워짐

		if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, fbxName, fbxPath)))
		{
			return E_FAIL;
		}
		wstring Name = fbxName;
		wstring tag = L"Prototype_GameObject_UI_" + Name;
		if (FAILED(g_pGameInstance->Add_Prototype(tag, CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
		{
			return E_FAIL;
		}

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);


	//////////////////////////////////////////Changing////////////////////////////////////////////////
	ZeroMemory(&fd, sizeof(_finddata_t));

	handle = _findfirst("../bin/Resources/Texture/UI/Dynamic/*.dds", &fd);

	if (handle == 0)
		return E_FAIL;

	iResult = 0;
	while (iResult != -1)
	{
		char szFullPath[MAX_PATH] = "../bin/Resources/Texture/UI/Dynamic/";
		strcat_s(szFullPath, fd.name);

		_tchar fbxName[MAX_PATH] = L"";
		_tchar fbxPath[MAX_PATH] = L"";
		MultiByteToWideChar(CP_ACP, 0, fd.name, MAX_PATH, fbxName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, szFullPath, MAX_PATH, fbxPath, MAX_PATH);

		::PathRemoveExtension(fbxName); //이게 있으면 확장자도 지워짐

		if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, fbxName, fbxPath)))
		{
			return E_FAIL;
		}
		wstring Name = fbxName;
		wstring tag = L"Prototype_GameObject_UI_" + Name;
		if (FAILED(g_pGameInstance->Add_Prototype(tag, CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
		{
			return E_FAIL;
		}

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);
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
	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Texture_0"), CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Texture_1"), CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}

	/*if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"), CMFCObject_UI::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}*/

	//////////////////////////////////////////////////////////////////////////
	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_bubble"), CMFCEffect::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"), CMFCEffect::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Floating"), CMFCEffect_Floating::Create(m_pDevice, m_pDeviceContext))))
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