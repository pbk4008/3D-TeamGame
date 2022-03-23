#include "pch.h"
#include "Loader.h"
#include "MeshLoader.h"

#include "Animator.h"
#include "Material.h"
#include "Effect_DashDust.h"
#include "UI_Ingame.h"
#include "UI_Player_HpBar.h"
#include "UI_Player_HpBar_Red.h"
#include "UI_Monster_Panel.h"
#include "UI_Monster_Back.h"
#include "UI_Monster_Level.h"
#include "UI_Monster_HpBar.h"
#include "UI_Monster_Name.h"
#include "Instancing_Mesh.h"
#include "Client_Trigger.h"
#include "Environment.h"
#include "Treasure_Chest.h"
#include "Monster_Crawler.h"
#include "Monster_EarthAberrant.h"
#include "Monster_BronzeAnimus.h"
#include "Monster_Bastion_Sword.h"
#include "Boss_Bastion_Judicator.h"
#include "ShieldBreaker.h"
#include "EarthAberrant_Pick.h"
#include "Stargazer.h"

#pragma region TestScene_JS

#include "Camera_Debug.h"
#include "Plane_Test.h"
#include "TestObj.h"
#include "Silvermane.h"
#include "Camera_Silvermane.h"
#include "Needle.h"
#include "Fury.h"
#include "Shield.h"
#include "Monster_Bastion_2HSword.h"
#include "Monster_Bastion_Healer.h"
#include "Monster_Bastion_Shooter.h"
#include "Monster_Bastion_Spear.h"
#include "JumpNode.h"

#pragma endregion

#pragma region TestScene_YM

#include "RetributionBlade.h"
#include "Staff.h"
#pragma endregion

#include "SkyBox.h"

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
	case SCENEID::SCENE_STAGE2:
		hr = Ready_Stage2();
		break;
	case SCENEID::SCENE_STAGE3:
		hr = Ready_Stage3();
		break;
	case SCENEID::SCENE_TEST_JS:
		hr = Ready_Test_JS();
		break;
	case SCENEID::SCENE_TEST_YM:
		hr = Ready_Test_YM();
		break;
	case SCENEID::SCENE_TEST_SB:
		hr = Ready_Test_SB();
		break;
	default:
		return E_FAIL;
	}

	if (FAILED(hr))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CLoader::SetUp_Stage1_Object()
{
	if (FAILED(Load_Stage1FBXLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1PlayerLoad()))
		return E_FAIL;

	/*if (FAILED(Load_Stage1MonsterLoad()))
		return E_FAIL;*/

	//if (FAILED(Load_Stage1BossLoad()))
	//	return E_FAIL;

	/*if (FAILED(Load_Stage1StaticUILoad()))
		return E_FAIL;*/

	//if (FAILED(Load_Stage1UILoad()))
	//	return E_FAIL;

	//if (FAILED(Load_Stage1EffectLoad()))
	//	return E_FAIL;

	//if (FAILED(Load_Stage1TriggerLod()))
	//	return E_FAIL;

	//if (FAILED(Load_Stage1_TreasureChest_Load()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Stage1FBXLoad()
{
	_finddata_t fd;
	ZeroMemory(&fd, sizeof(_finddata_t));

	intptr_t handle = _findfirst("../bin/FBX/*.fbx", &fd);

	if (handle == 0)
		return E_FAIL;

	int iResult = 0;
	CMeshLoader* pMeshLoader = GET_INSTANCE(CMeshLoader);
	while (iResult != -1)
	{
		if (!strcmp(fd.name, ""))
			break;
		char szFullPath[MAX_PATH] = "../bin/FBX/";

		strcat_s(szFullPath, fd.name);

		_tchar fbxName[MAX_PATH] = L"";
		_tchar fbxPath[MAX_PATH] = L"";
		MultiByteToWideChar(CP_ACP, 0, fd.name, MAX_PATH, fbxName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, szFullPath, MAX_PATH, fbxPath, MAX_PATH);

		CMeshLoader::MESHTYPE* tMeshType = new CMeshLoader::MESHTYPE;
		ZeroMemory(tMeshType, sizeof(CMeshLoader::MESHTYPE));

		tMeshType->szFBXName = new _tchar[MAX_PATH];
		tMeshType->szFBXPath = new _tchar[MAX_PATH];

		lstrcpy(tMeshType->szFBXName, fbxName);
		lstrcpy(tMeshType->szFBXPath, fbxPath);
		tMeshType->iType = 2;

		/*_bool bCheck = false;
		Sleep(10);
		pMeshLoader->Add_MeshLoader(tMeshType, bCheck);
		if (bCheck)
		{
			Safe_Delete_Array(tMeshType->szFBXName);
			Safe_Delete_Array(tMeshType->szFBXPath);
			Safe_Delete(tMeshType);
			continue;
		}*/

		if(FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, tMeshType->szFBXName,
			CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, tMeshType->szFBXPath, CInstancing_Mesh::INSTANCE_TYPE::STATIC))))

		Safe_Delete_Array(tMeshType->szFBXName);
		Safe_Delete_Array(tMeshType->szFBXPath);
		Safe_Delete(tMeshType);

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);

	pMeshLoader->Resume_Thread();
	while (true)
	{
		pMeshLoader->Update();
		if (pMeshLoader->Get_Clear())
			break;
	}
	
	RELEASE_INSTANCE(CMeshLoader);

	return S_OK;
}

HRESULT CLoader::Load_Stage1StaticUILoad()
{
	/////////////////////////////////////////////////////Static/////////////////////////////////////////////////////
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

		_tchar UIName[MAX_PATH] = L"";
		_tchar UIPath[MAX_PATH] = L"";
		MultiByteToWideChar(CP_ACP, 0, fd.name, MAX_PATH, UIName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, szFullPath, MAX_PATH, UIPath, MAX_PATH);

		wstring Name = UIName;

		//ext extract
		size_t lastIndex = Name.find_last_of(L".");
		wstring rawName = Name.substr(0, lastIndex);
		wstring NewName = L"Texture_" + rawName;

		if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, NewName, UIPath)))
		{
			return E_FAIL;
		}
		
		wstring tag = L"Proto_GameObject_UI_" + rawName;
		if (FAILED(g_pGameInstance->Add_Prototype(tag, CUI_Ingame::Create(m_pDevice, m_pDeviceContext))))
		{
			return E_FAIL;
		}

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);


	/////////////////////////////////////////////////////Changing/////////////////////////////////////////////////////

	ZeroMemory(&fd, sizeof(_finddata_t));

	handle = _findfirst("../bin/Resources/Texture/UI/Dynamic/*.dds", &fd);

	if (handle == 0)
		return E_FAIL;

	iResult = 0;
	while (iResult != -1)
	{
		char szFullPath[MAX_PATH] = "../bin/Resources/Texture/UI/Dynamic/";
		strcat_s(szFullPath, fd.name);

		_tchar UIName[MAX_PATH] = L"";
		_tchar UIPath[MAX_PATH] = L"";
		MultiByteToWideChar(CP_ACP, 0, fd.name, MAX_PATH, UIName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, szFullPath, MAX_PATH, UIPath, MAX_PATH);

		wstring Name = UIName;

		//ext extract
		size_t lastIndex = Name.find_last_of(L".");
		wstring rawName = Name.substr(0, lastIndex);
		wstring NewName = L"Texture_" + rawName;

		if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, NewName, UIPath)))
		{
			return E_FAIL;
		}

		wstring tag = L"Proto_GameObject_UI_" + rawName;
		if (FAILED(g_pGameInstance->Add_Prototype(tag, CUI_Ingame::Create(m_pDevice, m_pDeviceContext))))
		{
			return E_FAIL;
		}

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);
	return S_OK;
}

HRESULT CLoader::Load_Stage1UILoad()
{
	//Panel
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Monster_Panel"), CUI_Monster_Panel::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	//BarBack
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Monster_Back"), CUI_Monster_Back::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Monster_Back", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_Enemy_HealthMeter_BG.dds")))
	{
		return E_FAIL;
	}

	//Level
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Monster_Level"), CUI_Monster_Level::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Monster_Level_1", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_Enemy_Icon_Tier1.dds")))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Monster_Level_2", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_Enemy_Icon_Tier2.dds" )))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Monster_Level_3", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_Enemy_Icon_Tier3.dds" )))
	{
		return E_FAIL;
	}

	//PlayerHpBar
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Player_HpBar"), CUI_Player_HpBar::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Player_HpBar", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_Player_HealthMeter_Fill.dds")))
	{
		return E_FAIL;
	}

	//PlayerHpBar Red
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Player_HpBar_Red"), CUI_Player_HpBar_Red::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Player_HpBar_Red", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_Player_HealthMeter_DamageFill.dds")))
	{
		return E_FAIL;
	}

	//MonsterHpBar
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Monster_HpBar"), CUI_Monster_HpBar::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Monster_HpBar", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_Enemy_HealthMeter.dds")))
	{
		return E_FAIL;
	}

	//MonsterName
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Monster_Name"), CUI_Monster_Name::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Monster_Name", L"../bin/Resources/Texture/UI/Static/Active/MonsterName_%d.dds", 10)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Load_Stage1EffectLoad()
{
	//Effect
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_VIBuffer_PointInstance_Explosion",
		CVIBuffer_PointInstance_Explosion::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect"), CEffect_DashDust::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_bubble", L"../bin/Resources/Texture/Effect/bubble.tga")))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CLoader::Load_Stage1TriggerLod()
{
	/* Prototype_GameObject_Trigger */
	if (FAILED(g_pGameInstance->Add_Prototype(L"Prototype_GameObject_Trigger", CClient_Trigger::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Stage1_TreasureChest_Load()
{
	_finddata_t fd;
	ZeroMemory(&fd, sizeof(_finddata_t));

	intptr_t handle = _findfirst("../bin/FBX/Treasure_Chest/*.fbx", &fd);

	if (handle == 0)
		return E_FAIL;

	int iResult = 0;
	while (iResult != -1)
	{
		if (!strcmp(fd.name, ""))
			break;

		char szFullPath[MAX_PATH] = "../bin/FBX/Treasure_Chest/";

		strcat_s(szFullPath, fd.name);


		_tchar fbxName[MAX_PATH] = L"";
		_tchar fbxPath[MAX_PATH] = L"";
		MultiByteToWideChar(CP_ACP, 0, fd.name, MAX_PATH, fbxName, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, szFullPath, MAX_PATH, fbxPath, MAX_PATH);

		CMeshLoader::MESHTYPE tMeshType;
		ZeroMemory(&tMeshType, sizeof(tMeshType));

		lstrcpy(tMeshType.szFBXName, fbxName);
		lstrcpy(tMeshType.szFBXPath, fbxPath);
		tMeshType.iType = 2;

		if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Treasure_Chest", CModel::Create(m_pDevice, m_pDeviceContext,
			tMeshType.szFBXPath, CModel::TYPE_STATIC, TRUE))))
			return E_FAIL;

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);

	return S_OK;
}

void CLoader::Add_LoadingThread(const wstring& pComponetTag, const wstring& pFilePath, _uint iType)
{
	CMeshLoader* pLoader = GET_INSTANCE(CMeshLoader);

	CMeshLoader::MESHTYPE* tDesc = new CMeshLoader::MESHTYPE;
	ZeroMemory(tDesc, sizeof(CMeshLoader::MESHTYPE));

	tDesc->szFBXName = new _tchar[MAX_PATH];
	tDesc->szFBXPath = new _tchar[MAX_PATH];

	lstrcpy(tDesc->szFBXName, pComponetTag.c_str());
	lstrcpy(tDesc->szFBXPath, pFilePath.c_str());

	tDesc->iType = iType;
	_bool bCheck = true;
	while (true)
	{
		Sleep(1);
		pLoader->Add_MeshLoader(tDesc, bCheck);
		if (!bCheck)
			break;
	}

	RELEASE_INSTANCE(CMeshLoader);
}

HRESULT CLoader::SetUp_Stage1_Prototype()
{
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", CCapsuleCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", CAnimator::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Environment", CEnvironment::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Treasure_Chest", CTreasure_Chest::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoader::Load_Stage1PlayerLoad()
{
	/*if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Texture/Terrain/Plane_Default.dds")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;*/

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_NavMeshCollider", CNavMeshCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	//// 네비메쉬 플레인
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Plane_Test", CPlane_Test::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Sky_Texture", L"../Bin/Resources/Texture/SkyBox/SkyBox_Stage1.dds")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"VIBuffer_Cube", CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Cube.hlsl"))))
		return E_FAIL;

	CTexture* pTexture = nullptr;
#pragma region 마테리얼
	CMaterial* pMtrl = nullptr;
#pragma region 무기
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_D.dds", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Shield", L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", CMaterial::EType::Static);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Shield", pMtrl);
#pragma endregion
#pragma endregion

#pragma region 모델
	
	//쓰레드 추가로 변경(위 함수)
	//CMeshLoader* pLoader = GET_INSTANCE(CMeshLoader);

	//CMeshLoader::MESHTYPE* tDesc = new CMeshLoader::MESHTYPE;
	//ZeroMemory(tDesc, sizeof(CMeshLoader::MESHTYPE));

	//tDesc->szFBXName = new _tchar[MAX_PATH];
	//tDesc->szFBXPath = new _tchar[MAX_PATH];

	//lstrcpy(tDesc->szFBXName, );
	//lstrcpy(tDesc->szFBXPath, );
	//tDesc->iType = 1;
	//_bool bCheck = true;
	//pLoader->Add_MeshLoader(tDesc, bCheck);

	//RELEASE_INSTANCE(CMeshLoader);

	_matrix matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Silvermane_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Silvermane/Silvermane_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}

	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Needle", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Needle/", "Needle.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Fury", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Fury/", "Fury.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Shield", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Shield/", "Shield.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
#pragma endregion
#pragma region 컴포넌트

	//위에 만들어 놨음
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Proto_Component_StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", CCharacterController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion
#pragma region 오브젝트
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_SkyBox", CSkyBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CLoader::Load_Stage1BossLoad()
{
	//Boss Bastion_Tier4
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Boss_Bastion_Tier4", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_Tier4.fbx",CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Boss_Bastion", CBoss_Bastion_Judicator::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	 
	//weapon

	//ShieldBreaker_BossWeapon
	_matrix matPivot = XMMatrixIdentity();
	//matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Weapon_ShieldBreaker", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/ShieldBreaker/", "ShieldBreaker.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_ShieldBreaker", CShieldBreaker::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	
	//1
	CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/ShieldBreaker/T_2H_hammer_Shieldbreaker_D.dds", 1)))
		return E_FAIL;

	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"2H_hammer_Shieldbreaker", L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", CMaterial::EType::Static);
	if (FAILED(pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0)))
		return E_FAIL;
	/*if (FAILED(pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0)))
		return E_FAIL;
	if (FAILED(pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0)))
		return E_FAIL;
	if (FAILED(pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0)))
		return E_FAIL;*/
	if (FAILED(g_pGameInstance->Add_Material(L"MI_2H_hammer_Shieldbreaker", pMtrl)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Stage1MonsterLoad()
{
	////Monster Crystal_Crawler
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_Crawler", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Crystal_Crawler.fbx",CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Crawler", CMonster_Crawler::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//Monster EarthAberrant
	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_EarthAberrant", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/EarthAberrant.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;*/
	
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_EarthAberrant", CMonster_EarthAberrant::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	//Monster EarthAberrant Weapon
	_matrix matPivot = XMMatrixIdentity();
	//matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Weapon_EarthAberrant_Pick", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Earth_Aberrant_Pick/", "EarthAberrant_Pick.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_EarthAberrant_Pick", CEarthAberrant_Pick::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	//CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	//if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Earth_Aberrant_Pick/T_IcePick_D.dds", 1)))
	//	return E_FAIL;

	//CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Earth_Aberrant_Pick", L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", CMaterial::EType::Static);
	//if (FAILED(pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0)))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Material(L"MI_Earth_Aberrant_Pick", pMtrl)))
	//	return E_FAIL;

	////Monster BronzeAnimus
	//matPivot = XMMatrixIdentity();
	//matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_BronzeAnimus", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/BronzeAnimus/", "BronzeAnimus.fbx",
	//	L"../../Reference/ShaderFile/Shader_Mesh.hlsl",
	//	matPivot,
	//	CModel::TYPE_ANIM))))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_BronzeAnimus", CMonster_BronzeAnimus::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	//pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/BronzeAnimus/T_BronzeAnimus_Cloth_D.tga", 1);
	//pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BronzeAnimus_Cloth", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	//pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	//g_pGameInstance->Add_Material(L"Mtrl_BronzeAnimus_Cloth", pMtrl);

	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	//pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/BronzeAnimus/T_BronzeAnimus_Down_D.tga", 1);
	//pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BronzeAnimus_Down", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	//pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	//g_pGameInstance->Add_Material(L"Mtrl_BronzeAnimus_Down", pMtrl);

	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	//pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/BronzeAnimus/T_BronzeAnimus_Top_D.tga", 1);
	//pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BronzeAnimus_Top", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	//pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	//g_pGameInstance->Add_Material(L"Mtrl_BronzeAnimus_Top", pMtrl);

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_Bastion_Shooter", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_Maskman.fbx",CModel::TYPE_ANIM, true))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_Shooter", CMonster_Bastion_Shooter::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;


	////Monster Bastion_Sword

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_Bastion_Sword", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_Sword.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_Sword", CMonster_Bastion_Sword::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	////Weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Weapon_Stargazer",
	//	CModel::Create(m_pDevice, m_pDeviceContext, L"../bin/FBX/Monster/Weapon/Stargazer(1H Sword).fbx", CModel::TYPE_STATIC, true))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Stargazer", CStargazer::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

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
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Title", L"../bin/Resources/Texture/Loading/logo_godfall_3d.dds")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Stage1()
{
	if (FAILED(SetUp_Stage1_Prototype()))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_Stage1_Object()))
	{
		return E_FAIL;
	}

	

	return S_OK;
}

HRESULT CLoader::Ready_Stage2()
{
	return S_OK;
}

HRESULT CLoader::Ready_Stage3()
{
	return S_OK;
}

HRESULT CLoader::Ready_Test_JS()
{
	cout << "TestScene_JS 로딩 시작..." << endl;
	cout << "TestScene_JS 리소스 생성중..." << endl;
#pragma region 리소스
	if(FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Texture/Terrain/Plane_Default.dds")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;


	cout << "TestScene_JS 마테리얼 생성중..." << endl;
	CTexture* pTexture = nullptr;
	CMaterial* pMtrl = nullptr;
#pragma region 플레이어
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Hair_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Hair", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Silvermane_Hair", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Cloak_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Cloak", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Silvermane_Cloak", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Down_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Down", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Silvermane_Down", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Top_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Top", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Silvermane_Top", pMtrl);
#pragma endregion
#pragma endregion


#pragma region 모델
	cout << "TestScene_JS 모델 프로토타입 생성중..." << endl;
	_matrix matPivot = XMMatrixIdentity();
	//matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Silvermane", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Silvermane/", "Silvermane.fbx",
	//	L"../../Reference/ShaderFile/Shader_Mesh.hlsl",
	//	matPivot,
	//	CModel::TYPE_ANIM, true))))
	//{
	//	return E_FAIL;
	//}
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Silvermane_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Silvermane/Silvermane_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Needle", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Needle/", "Needle.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Fury", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Fury/", "Fury.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Shield", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Shield/", "Shield.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_JumpNode", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/JumpNode/", "JumpNode.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}
#pragma endregion


#pragma region 컴포넌트
	cout << "TestScene_JS 컴포넌트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Proto_Component_AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Proto_Component_StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Proto_Component_CharacterController", CCharacterController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Proto_Component_MeshCollider", CMeshCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Proto_Component_BoxCollider", CBoxCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Proto_Component_RayCollider", CRay_Collider::Create(m_pDevice, m_pDeviceContext, CRay_Collider::TYPE_AABB))))
		return E_FAIL;
#pragma endregion
#pragma region 오브젝트
	cout << "TestScene_JS 오브젝트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Plane_Test", CPlane_Test::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_JumpNode", CJumpNode::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion

	cout << "TestScene_JS 로딩 완료..." << endl;
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


HRESULT CLoader::Ready_Test_YM()
{
	cout << "TestScene_YM 로딩 시작..." << endl;
	cout << "TestScene_YM 리소스 생성중..." << endl;

	CTexture* pTexture = nullptr;
#pragma region 플레이어
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Hair_D.tga", 1);
	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Hair", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Silvermane_Hair", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Cloak_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Cloak", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Silvermane_Cloak", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Down_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Down", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Silvermane_Down", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Top_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Top", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Silvermane_Top", pMtrl);
#pragma endregion
#pragma region 몬스터
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_Shooter/T_Bastion_Tier1_Down_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Bastion_Tier1_Down", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Bastion_Tier1_Down", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_Shooter/T_Bastion_Tier1_Top_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Bastion_Tier1_Top", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Bastion_Tier1_Top", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_Shooter/T_Bastion_Fur_Coeff.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Bastion_Fur", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Bastion_Fur", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_Shooter/T_bastion_crossbow_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Bastion_Crossbow", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Bastion_Crossbow", pMtrl);



	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_2HSword/T_BastionTierII_Down_D.dds", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BastionTierII_Down", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_BastionTierII_Down", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_2HSword/T_BastionTierII_Top_D.dds", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BastionTierII_Top", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_BastionTierII_Top", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Bastion_2HSword/T_BastionTierII_Fur_Coeff.dds", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"BastionTierII_Fur", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_BastionTierII_Fur", pMtrl);
#pragma endregion
#pragma region 무기
	//pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	//pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Staff/T_bastion_staff_D.tga", 1);
	//pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Staff", L"../../Reference/ShaderFile/Shader_Mesh.hlsl", CMaterial::EType::Anim);
	//pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	//g_pGameInstance->Add_Material(L"Mtrl_Staff", pMtrl);
#pragma endregion

#pragma region 리소스
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Texture/Terrain/Plane_Default.dds")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_YM, L"VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;

#pragma region 모델
	cout << "TestScene_YM 모델 프로토타입 생성중..." << endl;
	_matrix matPivot = XMMatrixIdentity();

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Silvermane_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Silvermane/Silvermane_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_YM, L"Model_Bastion_2HSword_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/Resources/Mesh/Bastion_2HSword/Bastion_2HSword_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_YM, L"Model_Bastion_Healer_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/Resources/Mesh/Bastion_Healer/Bastion_Healer_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}

	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Needle", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Needle/", "Needle.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Fury", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Fury/", "Fury.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Shield", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Shield/", "Shield.fbx",
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}

	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_YM, L"Model_RetributionBlade", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/RetributionBlade/", "RetributionBlade(2H).fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}

	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_YM, L"Model_Staff", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Staff/", "Staff.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}

#pragma endregion
#pragma region 컴포넌트
	cout << "TestScene_YM 컴포넌트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", CCharacterController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", CAnimator::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", CBoxCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_NavMeshCollider", CNavMeshCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", CCapsuleCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion
#pragma region 오브젝트
	cout << "TestScene_YM 오브젝트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Plane_Test", CPlane_Test::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_TestObj", CTestObj::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Camera_Debug", CCamera_Debug::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Weapon_Needle", CNeedle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Weapon_RetributionBlade", CRetributionBlade::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Weapon_Staff", CStaff::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_2HSword", CMonster_Bastion_2HSword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_Healer", CMonster_Bastion_Healer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion

	cout << "TestScene_YM 로딩 완료..." << endl;
	return S_OK;
}

HRESULT CLoader::Ready_Test_SB()
{
	cout << "TestScene_SB 로딩 시작..." << endl;
	cout << "TestScene_SB 리소스 생성중..." << endl;
#pragma region 리소스
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Texture/Terrain/Plane_Default.tga")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Sky_Texture", L"../Bin/Resources/Texture/SkyBox/SkyBox_Stage1.dds")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"VIBuffer_Cube", CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Cube.hlsl"))))
		return E_FAIL;

	cout << "TestScene_SB 마테리얼 생성중..." << endl;
	CTexture* pTexture = nullptr;
#pragma region 플레이어

#pragma endregion
#pragma region 무기
	cout << "TestScene_SB 모델 프로토타입 생성중..." << endl;
	_matrix matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Silvermane_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Silvermane/Silvermane_Bin.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Needle", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Needle/", "Needle.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Marksman", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_Maskman.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	cout << "TestScene_SB 컴포넌트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", CCharacterController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_MeshCollider", CMeshCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", CAnimator::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Monster_Bastion_Marksman", CMonster_Bastion_Shooter::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Plane_Test", CPlane_Test::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_SkyBox", CSkyBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Weapon_Needle", CNeedle::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


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
