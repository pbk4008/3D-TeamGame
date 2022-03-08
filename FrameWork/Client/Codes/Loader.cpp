#include "pch.h"
#include "Loader.h"

#include "Effect_DashDust.h"
#include "UI_Ingame.h"
#include "Material.h"
#include "Instancing_Mesh.h"
#include "Environment.h"


#pragma region TestScene_JS

#include "Camera_Debug.h"
#include "Plane_Test.h"
#include "Silvermane.h"
#include "Camera_Silvermane.h"
#include "Needle.h"
#include "Monster_Bastion_2HSword.h"
#include "Monster_Bastion_Healer.h"
#include "Monster_Bastion_Shooter.h"
#include "Monster_Bastion_Spear.h"
#include "Monster_Bastion_Sword.h"

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

HRESULT CLoader::SetUp_Stage1_ProtoComponent()
{
	if (FAILED(Load_Stage1FBXLoad()))
		return E_FAIL;

	g_pGameInstance->Add_Prototype(L"Environment", CEnvironment::Create(m_pDevice, m_pDeviceContext));

	return S_OK;
}

HRESULT CLoader::Load_Stage1FBXLoad()
{
	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"_BossPlatform_QuarterOpen_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/_BossPlatform_QuarterOpen_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"_DoorSteps_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/_DoorSteps_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"AlcoveArchway_02_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/AlcoveArchway_02_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"AlcoveLanding_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/AlcoveLanding_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"AlcoveTrimHeights_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/AlcoveTrimHeights_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"ArchDoor_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/ArchDoor_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Archstone_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Archstone_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Archway_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Archway_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Balcony_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Balcony_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Balcony_Handrail_4m_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Balcony_Handrail_4m_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Balcony_Handrail_8m_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Balcony_Handrail_8m_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"BalconyArchway_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/BalconyArchway_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"BalconyHalf_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/BalconyHalf_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"BossPlatform_Discs_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/BossPlatform_Discs_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"BossPlatform_Quarter_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/BossPlatform_Quarter_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Brazier_02.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Brazier_02.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Brazier_03.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Brazier_03.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Brazier_03_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Brazier_03_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Bush_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Bush_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Bush_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Bush_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Ceiling_RoundedHalf_02_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Ceiling_RoundedHalf_02_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Ceiling_Window_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Ceiling_Window_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Ceiling_Window_03_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Ceiling_Window_03_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Ceiling_WindowHalf_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Ceiling_WindowHalf_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Chain_01a_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Chain_01a_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"ConcreteBlock_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/ConcreteBlock_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"ConcreteBlockWide_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/ConcreteBlockWide_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"ConcreteLanding_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/ConcreteLanding_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"CrownMoldings_Corner_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/CrownMoldings_Corner_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"CrownMoldings_Straight_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/CrownMoldings_Straight_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Curved_Stairs_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Curved_Stairs_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"DungeonPieces_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/DungeonPieces_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"DungeonPieces_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/DungeonPieces_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Flower_Red_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Flower_Red_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"OctagonSand_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/OctagonSand_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PavementDecor_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PavementDecor_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Pillar_EndCap_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Pillar_EndCap_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarLarge_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarLarge_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarLarge_02_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarLarge_02_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarLarge_03_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarLarge_03_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarLarge_04_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarLarge_04_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarMedium_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarMedium_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Pillars_Tall_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Pillars_Tall_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarsDouble_Large_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarsDouble_Large_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarsDouble_Large_01b_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarsDouble_Large_01b_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarSmall_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarSmall_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"PillarSmall_03_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/PillarSmall_03_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"rock01sunforge_LP_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/rock01sunforge_LP_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Rock02sunforge_LP_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Rock02sunforge_LP_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"rock03sunforge_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/rock03sunforge_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_Grass_Red_01_HP_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_Grass_Red_01_HP_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_GroundPlants_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_GroundPlants_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_GroundPlants_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_GroundPlants_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_GroundPlants_03_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_GroundPlants_03_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_GroundPlants_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_GroundPlants_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_IvyWall_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_IvyWall_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_IvyWall_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_IvyWall_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_IvyWall_03_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_IvyWall_03_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Rock03sunforge_LP_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Rock03sunforge_LP_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_IvyWall_04_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_IvyWall_04_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_IvyWall_05_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_IvyWall_05_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_IvyWallMerge_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_IvyWallMerge_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_StairsRailingCurved_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_StairsRailingCurved_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_StairsRailingCurved_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_StairsRailingCurved_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_SunDoor_01.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_SunDoor_01.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_SunDoor_01b.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_SunDoor_01b.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_SunDoor_01c_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_SunDoor_01c_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_TriPlated_Deco_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_TriPlated_Deco_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforge_TrimFloor_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforge_TrimFloor_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StairsCurved_02_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StairsCurved_02_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StairsCurved_LavaCanel_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StairsCurved_LavaCanel_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StairsCurved_LavaEntrance_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StairsCurved_LavaEntrance_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StairsHandrail_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StairsHandrail_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StairsSides_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StairsSides_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StandingArchway_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StandingArchway_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StandingArchway_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StandingArchway_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StandingArchway_02.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StandingArchway_02.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_StandingArchway_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_StandingArchway_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_SteelGate_Rails_02.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_SteelGate_Rails_02.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_SunGate_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_SunGate_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_SunGate_Arch_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_SunGate_Arch_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_SunGate_Arch_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_SunGate_Arch_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_SunGate_Column_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_SunGate_Column_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_SunGate_Column_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_SunGate_Column_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_SunGate_Door_01b.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_SunGate_Door_01b.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_SunGate_Top_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_SunGate_Top_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_ThickWall_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_ThickWall_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_Trim_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_Trim_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_Trim_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_Trim_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_Trim_02_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_Trim_02_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_Trim_Blocks_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_Trim_Blocks_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_TriPlated_Deco_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_TriPlated_Deco_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_TriPlated_Deco_01C.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_TriPlated_Deco_01C.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_WallStep_Alcove_01.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_WallStep_Alcove_01.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_WallStep_Corner_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_WallStep_Corner_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_WallStep_Straight_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_WallStep_Straight_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_WallTrim_Top_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_WallTrim_Top_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_WallTrims_Chest_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_WallTrims_Chest_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"S_Sunforged_WallTrims_Waist_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/S_Sunforged_WallTrims_Waist_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Silver.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Silver.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"SM_MERGED_BP_SunforgeBrigde_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/SM_MERGED_BP_SunforgeBrigde_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"SM_PROXYGEN_Sunforge_Sungate_Clockwork_Vista_00.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/SM_PROXYGEN_Sunforge_Sungate_Clockwork_Vista_00.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Sphere.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Sphere.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StairEdging_11_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StairEdging_11_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StairEfging_12.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StairEfging_12.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StairEfging_18.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StairEfging_18.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Stairs_01a_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Stairs_01a_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Stairs_01b_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Stairs_01b_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Stairs_02_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Stairs_02_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StairsCurved_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StairsCurved_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StairsCurved_02_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StairsCurved_02_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_AlcoveTrimHeights_Object029_159.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_AlcoveTrimHeights_Object029_159.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_decooration_pavement14.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_decooration_pavement14.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_decooration_pavement14_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_decooration_pavement14_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_DungeonAssets4_DungeonPieces02_0_DungeonA.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_DungeonAssets4_DungeonPieces02_0_DungeonA.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_S_Banner_VcutHISMA_712_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_S_Banner_VcutHISMA_712_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_S_Ceiling_LightsHISMA.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_S_Ceiling_LightsHISMA.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_S_GildenTreeBarkgolden8_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_S_GildenTreeBarkgolden8_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_S_GildenTreeBarkgolden10_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_S_GildenTreeBarkgolden10_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_S_Pillar01HISMA_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_S_Pillar01HISMA_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_SM_BrazierHISMA_18_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_SM_BrazierHISMA_18_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_SM_FloorTrim_6_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_SM_FloorTrim_6_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"StaticMesh02_0_SM_FloorTrim_16_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/StaticMesh02_0_SM_FloorTrim_16_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Sunforge_Sungate_StaticMesh02_0_S_GildenTreeBarkgoldenHISMA_0_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Sunforge_Sungate_StaticMesh02_0_S_GildenTreeBarkgoldenHISMA_0_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Sunforge_Sungate_StaticMesh02_0_S_GildenTreeBarkgoldenHISMA_2_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Sunforge_Sungate_StaticMesh02_0_S_GildenTreeBarkgoldenHISMA_2_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Sunforge_Sungate_StaticMesh02_0_S_GildenTreeBarkgoldenHISMA_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Sunforge_Sungate_StaticMesh02_0_S_GildenTreeBarkgoldenHISMA_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Sunforge_Sungate_StaticMesh02_0_S_Hanging_Bangle_Plates_3_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Sunforge_Sungate_StaticMesh02_0_S_Hanging_Bangle_Plates_3_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Sunforged_PlazaLanding_01_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Sunforged_PlazaLanding_01_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"towerALLunreal_Rock_01sunforge_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/towerALLunreal_Rock_01sunforge_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"towerALLunreal_Rock_05sunforge_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/towerALLunreal_Rock_05sunforge_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"towerALLunreal_Rock_07sunforge_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/towerALLunreal_Rock_07sunforge_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"towerALLunreal_Rock_08sunforge_Lod0.fbx"
	//	, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/towerALLunreal_Rock_08sunforge_Lod0.fbx",
	//		L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
	//	return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"TriPlated_Deco_01C_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/TriPlated_Deco_01C_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"WallStep_Corner_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/WallStep_Corner_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"WallStep_Straight_01_Lod0.fbx"
		, CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/WallStep_Straight_01_Lod0.fbx",
			L"../../Reference/ShaderFile/Shader_InstanceMesh.hlsl", CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
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
	//if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"StageBackGround", L"../bin/Resources/Texture/penguin001.jpg")))
	//	return E_FAIL;

	if (FAILED(SetUp_Stage1_ProtoComponent()))
		return E_FAIL;

	//g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1,L"")
	
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
	cout << "TestScene_JS 로딩 시작..." << endl;

#pragma region 리소스

	if(FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Texture/Terrain/Plane_Default.tga")))
		return E_FAIL;

	cout << "TestScene_JS 리소스 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;


	cout << "TestScene_JS 마테리얼 생성중..." << endl;
#pragma region 플레이어
	CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Hairs_d_new.tga", 1);
	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Hair", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Silvermane_Hair", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Cloak_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Cloak", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Silvermane_Cloak", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Down_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Down", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Silvermane_Down", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Top_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Top", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Silvermane_Top", pMtrl);
#pragma endregion
#pragma region 몬스터
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_Shooter/T_Bastion_Tier1_Down_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Bastion_Tier1_Down", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Bastion_Tier1_Down", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_Shooter/T_Bastion_Tier1_Top_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Bastion_Tier1_Top", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Bastion_Tier1_Top", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_Shooter/T_Bastion_Fur_Coeff.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Bastion__Fur", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Bastion__Fur", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_Shooter/T_bastion_crossbow_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Bastion_Crossbow", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Bastion_Crossbow", pMtrl);



	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_2HSword/T_BastionTierII_Down_D.dds", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BastionTierII_Down", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"BastionTierII_Down", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_2HSword/T_BastionTierII_Top_D.dds", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BastionTierII_Top", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"BastionTierII_Top", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_2HSword/T_BastionTierII_Fur_Coeff.dds", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BastionTierII_Fur", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"BastionTierII_Fur", pMtrl);
#pragma endregion

	

#pragma endregion

#pragma region 모델
	cout << "TestScene_JS 모델 프로토타입 생성중..." << endl;
	_matrix matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Silvermane", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Silvermane/", "Silvermane.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl",
		matPivot,
		CModel::TYPE_ANIM))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Needle", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Needle/", "Needle.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Bastion_2HSword", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Bastion_2HSword/", "Bastion_2HSword.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl",
		matPivot,
		CModel::TYPE_ANIM))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Bastion_Healer", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Bastion_Healer/", "Bastion_Healer.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl",
		matPivot,
		CModel::TYPE_ANIM))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Bastion_Shooter", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Bastion_Shooter/", "Bastion_Shooter.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl",
		matPivot,
		CModel::TYPE_ANIM))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Bastion_Spear", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Bastion_Spear/", "Bastion_Spear.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl",
		matPivot,
		CModel::TYPE_ANIM))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Bastion_Sword", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Bastion_Sword/", "Bastion_Sword.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl",
		matPivot,
		CModel::TYPE_ANIM))))
	{
		return E_FAIL;
	}
#pragma endregion


#pragma region 컴포넌트
	cout << "TestScene_JS 컴포넌트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Component_AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Component_StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma endregion
#pragma region 오브젝트

	cout << "TestScene_JS 오브젝트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Plane_Test", CPlane_Test::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Camera_Debug", CCamera_Debug::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Needle", CNeedle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_2HSword", CMonster_Bastion_2HSword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_Healer", CMonster_Bastion_Healer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_Shooter", CMonster_Bastion_Shooter::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_Spear", CMonster_Bastion_Spear::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_Sword", CMonster_Bastion_Sword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion

	cout << "TestScene_JS 로딩 완료..." << endl;
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
