#include "framework.h"
#include "Loader.h"

#include "Material.h"
#include "GameInstance.h"
#include "Floor.h"
#include "SkyBox.h"
#include "Player.h"

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

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	if (!m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::LoadForScene()
{
	HRESULT hr = 0;
	switch (m_eNextScene)
	{
	case SCENEID::SCENE_STAGE1:
		hr = Ready_Stage1();
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

HRESULT CLoader::Ready_Stage1()
{
	//if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"StageBackGround", L"")))
	//	return E_FAIL;

	// Ready Texutre
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"FloorBase", L"../bin/Resources/Textures/Terrain/Grass_%d.dds", 2)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"FloorFilter", L"../bin/Resources/Textures/Terrain/Filter.bmp")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"FloorBrush", L"../bin/Resources/Textures/Terrain/Brush.png")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"SkyBox", L"../bin/Resources/Textures/SkyBox/burger%d.dds", 4)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"PBR_Player_Top", L"../bin/Resources/Meshes/fbx/T_Silvermane_Top_MRA.tga",1)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"PBR_Player_Down", L"../bin/Resources/Meshes/fbx/T_Silvermane_Down_MRA.tga",1)))
		return E_FAIL;
	// Ready Component

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PrototypeTerrainVIBuffer", CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, L"../bin/ShaderFiles/Shader_Terrain.hlsl", L"../Bin/Resources/Textures/Terrain/Height.bmp"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PrototypeCubeVIBuffer", CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext, L"../bin/ShaderFiles/Shader_Cube.hlsl"))))
		return E_FAIL;

	_matrix PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f));

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PlayerModel", CModel::Create(m_pDevice, m_pDeviceContext, "../bin/Resources/Meshes/fbx/", "Player.fbx", L"../bin/ShaderFiles/Shader_Mesh.hlsl", PivotMatrix, CModel::TYPE_ANIM))))
		return E_FAIL;

	// Ready Object
	if (FAILED(g_pGameInstance->Add_Prototype(L"Floor", CFloor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Sky", CSkyBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Player", CPlayer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Critical);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
