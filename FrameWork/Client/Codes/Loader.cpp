#include "pch.h"
#include "Loader.h"

#include "Effect_DashDust.h"
#include "UI_Ingame.h"

#pragma region TestScene_JS

#include "Silvermane.h"
#include "Camera_Silvermane.h"

#pragma endregion


CLoader::CLoader() 
	: m_hThread(nullptr)
	, m_bFinish(false)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_eNextScene(SCENEID::SCENE_END)
{
	ZeroMemory(&m_Critical, sizeof(CRITICAL_SECTION));
}

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) 
	: m_hThread(nullptr)
	, m_bFinish(false)
	, m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_eNextScene(SCENEID::SCENE_END)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	ZeroMemory(&m_Critical, sizeof(CRITICAL_SECTION));
}
HRESULT CLoader::Init_Loader(SCENEID eID)
{
	InitializeCriticalSection(&m_Critical);
	m_eNextScene = eID;
	
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0,nullptr);
	
	if (!m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::LoadForScene()
{
	HRESULT hr = 0;
	switch (m_eNextScene)
	{
	case SCENEID::SCENE_LOGO:
		hr = Ready_Logo();
		break;
	case SCENEID::SCENE_STAGE1:
		hr = Ready_Stage1();
		break;
	case SCENEID::SCENE_TEST_JS:
		hr = Ready_Test_JS();
		break;
	default:
		return E_FAIL;
	}

	if (FAILED(hr))
		return E_FAIL;
	return S_OK;
}

_uint CLoader::Thread_Main(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;
	_uint iFlag = 0;

	EnterCriticalSection(pLoader->getCritical());
	if (FAILED(pLoader->LoadForScene()))
		iFlag = E_FAIL;

	pLoader->m_bFinish = true;
	LeaveCriticalSection(pLoader->getCritical());
	return iFlag;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, SCENEID eID)
{
	CLoader* pInsance = new CLoader(pDevice, pDeviceContext);
	if (FAILED(pInsance->Init_Loader(eID)))
	{
		MSGBOX("Loader Create Fail");
		Safe_Release(pInsance);
	}
	return pInsance;
}

HRESULT CLoader::Ready_Logo()
{
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Title", L"../bin/Resources/Texture/Loading/logo_godfall_3d.tga")))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CLoader::Ready_Stage1()
{
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"StageBackGround", L"../bin/Resources/Texture/Loading/T_HUD_Tutorial_Backplate.tga")))
		return E_FAIL;

	//Effect
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Prototype_Component_VIBuffer_PointInstance_Explosion",
		CVIBuffer_PointInstance_Explosion::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"), CEffect_DashDust::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"bubble", L"../bin/Resources/Texture/Effect/bubble.tga")))
	{
		return E_FAIL;
	}

	//UI
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"T_HUD_Player_Shield_Icon", L"../bin/Resources/Texture/UI/T_HUD_Player_Shield_Icon.tga")))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_T_HUD_Player_Shield_Icon"), CUI_Ingame::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Ready_Test_JS()
{

#pragma region 컴포넌트

	_matrix matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Silvermane", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Silvermane/", "Silvermane.fbx",
		L"../../Reference/ShaderFile/Shader_MeshAnim.hlsl",
		matPivot,
		CModel::TYPE_ANIM))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma endregion
#pragma region 오브젝트

	if (FAILED(g_pGameInstance->Add_Prototype(L"Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma endregion


	cout << "TestScene_JS 로딩 완료" << endl;
	return S_OK;
}

void CLoader::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Critical);
}
