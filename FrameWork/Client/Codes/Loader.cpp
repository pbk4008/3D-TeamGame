#include "pch.h"
#include "Loader.h"
#include "MeshLoader.h"
#include "Animator.h"
#include "Material.h"

//Object
#include "Instancing_Mesh.h"
#include "Client_Trigger.h"
#include "SkyBox.h"
#include "Environment.h"
#include "SubEnvironment.h"
#include "Treasure_Chest.h"
#include "Potal.h"

//Monster
#include "Monster_Crawler.h"
#include "Monster_EarthAberrant.h"
#include "Monster_BronzeAnimus.h"
#include "Monster_Bastion_Sword.h"
#include "Monster_Bastion_Shooter.h"
#include "Monster_Bastion_Spear.h"
#include "Monster_Bastion_2HSword.h"
#include "Monster_Bastion_Healer.h"
#include "Boss_Bastion_Judicator.h"
//Weapon
#include "BronzeAnimus_Sword.h"
#include "ShieldBreaker.h"
#include "EarthAberrant_Pick.h"
#include "Stargazer.h"
#include "Bullet.h"
#include "RetributionBlade.h"
#include "Staff.h"
#include "Polearm.h"
#include "Meteor.h"


//Effect & UI
#include "Effect_DashDust.h"
#include "Effect_HitParticle.h"
#include "Effect_HitFloating.h"
#include "Effect_Floating_Speed.h"
#include "Effect_Env_Floating.h"
#include "Effect_Env_Fire.h"
#include "Effect_Falling_Leaf.h"
#include "Effect_FloatingUp.h"
#include "Effect_Hammer_Dust.h"
#include "Effect_Dead_Spray.h"
//#include "Effect_DeathParticle.h"
//#include "Effect_Guard.h"
#include "Explosion_Rock.h"


#include "UI_Ingame.h"
#include "UI_Player_HpBar.h"
#include "UI_Player_HpBar_Red.h"
#include "UI_Monster_Panel.h"
#include "UI_Monster_Back.h"
#include "UI_Monster_Level.h"
#include "UI_Monster_HpBar.h"
#include "UI_Monster_BackHpBar.h"
#include "UI_Monster_GroggyBar.h"
#include "UI_Monster_Name.h"
#include "UI_Tuto_Base.h"
#include "UI_Tuto_Font.h"
#include "UI_Blank_CKey.h"
#include "UI_Fill_CKey.h"
#include "UI_Indicator.h"

//Cinema
#include "CinemaCam.h"
#include "CinemaActor.h"


#pragma region TestScene_JS
#include "Camera_Debug.h"
#include "Plane_Test.h"
#include "TestObj.h"
#include "Silvermane.h"
#include "Camera_Silvermane.h"
#include "Needle.h"
#include "Fury.h"
#include "Shield.h"
#include "FlyingShield.h"


#include "JumpNode.h"
#include "JumpTrigger.h"
#include "JumpBox.h"
#include "DropBox.h"
#include "DropObject.h"
#include "TrailEffect_Normal.h"
#include "TrailEffect_Distortion.h"
#include "Wall.h"
#include "MotionTrail.h"

#include "MeshEffect_Test.h"
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
	case SCENEID::SCENE_STAGE2:
		hr = Ready_Stage2();
		break;
	case SCENEID::SCENE_STAGE3:
		hr = Ready_Stage3();
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

HRESULT CLoader::SetUp_Stage1_Object()
{
	if (FAILED(Load_Stage1FBXLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1Navi_SkyLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1PlayerLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1MonsterLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1BossLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1StaticUILoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1UILoad()))
		return E_FAIL;

#pragma region 이펙트들
	if (FAILED(Load_Stage1EffectLoad()))
		return E_FAIL;
	/////////////////////////////// 소드 트레일
	if (FAILED(Load_TrailEffects()))
		return E_FAIL;
	///////////////////////////////////// 메쉬 이펙트
	if (FAILED(Load_MeshEffects()))
		return E_FAIL;
#pragma endregion

	if (FAILED(Load_Stage1JumpTrigger()))
		return E_FAIL;

	if (FAILED(Load_Stage1TriggerLod()))
		return E_FAIL;

	if (FAILED(Load_Stage1_TreasureChest_Load()))
		return E_FAIL;

	//if (FAILED(Load_Stage1Meteor()))
	//	return E_FAIL;

	//if (FAILED(Load_Stage1_Cinema_Object()))
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

	/*	CMeshLoader::MESHTYPE* tMeshType = new CMeshLoader::MESHTYPE;
		ZeroMemory(tMeshType, sizeof(CMeshLoader::MESHTYPE));*/

		//tMeshType->szFBXName = new _tchar[MAX_PATH];
		//tMeshType->szFBXPath = new _tchar[MAX_PATH];

		//lstrcpy(tMeshType->szFBXName, fbxName);
		//lstrcpy(tMeshType->szFBXPath, fbxPath);
		//tMeshType->iType = 2;

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

		if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, fbxName,
			CInstancing_Mesh::Create(m_pDevice, m_pDeviceContext, fbxPath, CInstancing_Mesh::INSTANCE_TYPE::STATIC))))
			return E_FAIL;

	/*	Safe_Delete_Array(tMeshType->szFBXName);
		Safe_Delete_Array(tMeshType->szFBXPath);
		Safe_Delete(tMeshType);*/

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);

	/*pMeshLoader->Resume_Thread();
	while (true)
	{
		pMeshLoader->Update();
		if (pMeshLoader->Get_Clear())
			break;
	}*/
	
	RELEASE_INSTANCE(CMeshLoader);

	return S_OK;
}

HRESULT CLoader::Load_Stage1Navi_SkyLoad()
{
	// Component Prototype
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_NavMeshCollider", CNavMeshCollider::Create(m_pDevice, m_pDeviceContext))))
		MSGBOX("Failed To Creating NavMeshCollider");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"VIBuffer_Cube", CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Cube.hlsl"))))
		MSGBOX("Failed To Creating Cube Buffer");

	// Object Prototype
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_SkyBox", CSkyBox::Create(m_pDevice, m_pDeviceContext)))) MSGBOX("Failed To Creating SkyBox Prototype");
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Plane_Test", CPlane_Test::Create(m_pDevice, m_pDeviceContext)))) MSGBOX("Failed To Creating PlaneTest Prototype");

	// Texture
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Sky_Texture1", L"../Bin/Resources/Texture/SkyBox/SkyBox_Stage1.dds"))) MSGBOX("Failed Add To SkyBoxTex");

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
	//Buffer
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Bar", CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_UI_Bar.hlsl"))))
		return E_FAIL;

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
	//MonsterHpBar
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Monster_BackHpBar"), CUI_Monster_BackHpBar::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	//MonsterGroggyBar
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Monster_GroggyBar"), CUI_Monster_GroggyBar::Create(m_pDevice, m_pDeviceContext))))
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


	//Tuto_Base
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Tuto_Base"), CUI_Tuto_Base::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Tuto_Base", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_Tutorial_Backplate.dds")))
	{
		return E_FAIL;
	}

	//Tuto_Font
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Tuto_Font"), CUI_Tuto_Font::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Tuto_Font", L"../bin/Resources/Texture/UI/Static/Active/Tuto_Font_%d.dds", 4)))
	{
		return E_FAIL;
	}

	//Blank_CKey
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Blank_CKey"), CUI_Blank_CKey::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Blank_Ckey", L"../bin/Resources/Texture/UI/Static/Active/T_Keyboard_Dark_Key_Shift.dds")))
	{
		return E_FAIL;
	}

	//Fill_CKey
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Fill_CKey"), CUI_Fill_Ckey::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Fill_Ckey", L"../bin/Resources/Texture/UI/Static/Active/T_Keyboard_Light_Key_Shift.dds")))
	{
		return E_FAIL;
	}

	//Indicator
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Indicator"), CUI_Indicator::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Indicator", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_CompassMarker_OuterPointer.dds")))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoader::Load_Stage1EffectLoad()
{
	//Effect buffer
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Effect", CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Effect_Fire.hlsl"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Explosion",
		CVIBuffer_PointInstance_Explosion::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Floating",
		CVIBuffer_PointInstance_Floating::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Floating_Speed",
		CVIBuffer_PointInstance_Floating_Speed::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Env_Floating",
		CVIBuffer_PointInstance_Env_Floating::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Respawn",
		CVIBuffer_PointInstance_Respawn::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Respawn_Up",
		CVIBuffer_PointInstance_Respawn_Up::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Disappear",
		CVIBuffer_PointInstance_Floating_Disappear::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//object
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_DashDust"), CEffect_DashDust::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Explosion"), CEffect_HitParticle::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Floating"), CEffect_HitFloating::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Floating_Speed"), CEffect_Floating_Speed::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Env_Floating"), CEffect_Env_Floating::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Falling_Leaf"), CEffect_Falling_Leaf::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_FloatingUp"), CEffect_FloatingUp::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Guard"), CEffect_Guard::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Death"), CEffect_DeathParticle::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}

	//Effect Image
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Env_Fire"), CEffect_Env_Fire::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Hammer_Dust"), CEffect_Hammer_Dust::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_HitImage"), CEffect_Dead_Spray::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}

	//Effect Folder Image Load
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

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);

	return S_OK;
}

HRESULT CLoader::Load_Stage1JumpTrigger()
{
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_JumpNode_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/Resources/Mesh/JumpNode/JumpNode_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_JumpNode", CJumpNode::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_JumpTrigger", CJumpTrigger::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_JumpBox", CJumpBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

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

		if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_DropBox", CModel::Create(m_pDevice, m_pDeviceContext,
			fbxPath, CModel::TYPE_ANIM, TRUE))))
			return E_FAIL;

		iResult = _findnext(handle, &fd);
	}
	_findclose(handle);

	/* for. Drop Object*/
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_DropObject", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/dropObject.fbx", CModel::TYPE_STATIC, true)))) return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_DropObject", CDropObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Stage1_Cinema_Object()
{
	//Grayehawk
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_GrayHawk", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/Grayehawk.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Phoenix
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Phoenix", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/Phoenix.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//MidBoss
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_MidBoss", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/MidBoss_Cinema.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	////Silvermane
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Cinema_Silvermane", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/Silvermane_Cinema.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Camera1-1
	_matrix matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam1_1", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/","camera_bone_01.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot,CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera1-2
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam1_2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "camera_bone_02.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Camera2-1
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam2_1", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "camera_bone_03_01.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera2-2
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam2_2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "camera_bone_03_02.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera2-3
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam2_3", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "camera_bone_03_03.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera2-4
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam2_4", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "camera_bone_03_04.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;


	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CinemaCamera", CCinemaCam::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if(FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CinemaActor",CCinemaActor::Create(m_pDevice,m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Set_Stage2_Prototype()
{
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Sky_Texture2", L"../Bin/Resources/Texture/SkyBox/SkyBox_Stage2_1.dds"))) MSGBOX("Failed Add To SkyBoxTex");


	return S_OK;
}

HRESULT CLoader::Load_Stage2_Object()
{
	//BronzeAnimus
	//_matrix matPivot = XMMatrixIdentity();
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_BronzeAnimus", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/BronzeAnimus_Bin.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_BronzeAnimus", CMonster_BronzeAnimus::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//

	////weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_BronzeAnimus_Sword", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/BronzeAnimus_Sword/", "BronzeAnimus_Sword.fbx",
	//	L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//{
	//	return E_FAIL;
	//}
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_BronzeAnimus_Sword", CBronzeAnimus_Sword::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	////Bastion Spear
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_Spear", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_HonerGuard_Bin.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Spear", CMonster_Bastion_Spear::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	//////weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Polearm", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Polearm/", "Bastion_Polearm.fbx",
	//	L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Polearm", CPolearm::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;


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

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", CBoxCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SphereCollider", CSphereCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", CAnimator::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", CCharacterController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if(FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC,L"Proto_Component_Rect_Buffer_Potal", CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Potal.hlsl"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Environment", CEnvironment::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_SubEnvironment", CSubEnvironment::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Treasure_Chest", CDropBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Portal", CPotal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Stage1PlayerLoad()
{
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
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl", matPivot, CModel::TYPE::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}
#pragma region 	YM 추가한 무기들
	matPivot = XMMatrixIdentity();
	std::wstring shaderPath = L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl";
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_ArgentBlade", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/ArgentBlade/", "1H_Sword_ArgentBlade.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Crossblade", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Crossblade/", "1H_sword_Crossblade.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Dragon", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Dragon/", "1H_sword_dragon.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Eclipse", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Eclipse/", "1H_Sword_Eclipse.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Justice", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Justice/", "1H_sword_Justice.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_GodHammer", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/GodHammer/", "2H_Hammer_GodHammer.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Harbinger", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Harbinger/", "2H_Hammer_Harbinger.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Hinterclaw", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Hinterclaw/", "2H_hammer_Hinterclaw.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Ironstone", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Ironstone/", "2H_hammer_Ironstone.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Legend1", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Legend1/", "2H_hammer_Legend1.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Legend2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Legend2/", "2H_hammer_Legend2.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Legend5", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Legend5/", "2H_hammer_Legend5.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Mesa", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Mesa/", "2H_hammer_Mesa.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Skymourne", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Skymourne/", "2H_hammer_Skymourne.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Soothsayer", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Soothsayer/", "2H_Hammer_Soothsayer.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) return E_FAIL;
#pragma endregion
	// 던지기용 방패
	matPivot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(180.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_FlyingShield", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Shield/", "Shield.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}

	//Wall
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WallBase", L"../bin/Resources/Texture/T_Portal_Space_D.dds"))) MSGBOX("Failed To Add WallBase Tex");

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_Wall", CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Distortion.hlsl"))))
		MSGBOX("Failed to Create Wall Buffer");

	// dissolve 
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"DissovleBase", L"../bin/Resources/Texture/dissolve.dds"))) MSGBOX("Failed To Add dissolve Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"DissovleGradient", L"../bin/Resources/Texture/T_FlameGradient_01.dds"))) MSGBOX("Failed To Add dissolveGradient Tex");

#pragma region 오브젝트
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))	return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Wall", CWall::Create(m_pDevice, m_pDeviceContext)))) MSGBOX(L"Wall 원본 생성 실패");
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_FlyingShield", CFlyingShield::Create(m_pDevice, m_pDeviceContext)))) MSGBOX(L"날으는 쉴드 원본 생성 실패");
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MotionTrail", CMotionTrail::Create(m_pDevice, m_pDeviceContext)))) MSGBOX(L"모션 더미 생성");
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

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Weapon_ShieldBreaker", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/ShieldBreaker/", "ShieldBreaker.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_ShieldBreaker", CShieldBreaker::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLoader::Load_Stage1MonsterLoad()
{
	_matrix matPivot = XMMatrixIdentity();

	////////////Monster Crystal_Crawler
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Crawler", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Crystal_Crawler.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Crawler", CMonster_Crawler::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//Monster EarthAberrant
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_EarthAberrant", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/EarthAberrant.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_EarthAberrant", CMonster_EarthAberrant::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//Monster EarthAberrant Weapon
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_EarthAberrant_Pick", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Earth_Aberrant_Pick/", "EarthAberrant_Pick.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_EarthAberrant_Pick", CEarthAberrant_Pick::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	////////Monster Bastion_Sword
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Sword", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_Sword.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Sword", CMonster_Bastion_Sword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	////Weapon
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_Stargazer", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Stargazer/", "Stargazer(1H Sword).fbx",
		L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Stargazer", CStargazer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	////////Monster Bastion_Shooter
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Shooter", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_Shooter.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Shooter", CMonster_Bastion_Shooter::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Shooter_Bullet", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Bullet/", "Sphere.fbx",
		L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Shooter_Bullet", CBullet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	////Bastion_2HSword
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_2HSword", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_2HSword_Bin.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_2HSword", CMonster_Bastion_2HSword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	////Weapon
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_RetributionBlade", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/RetributionBlade/", "RetributionBlade(2H).fbx",
		L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_RetributionBlade", CRetributionBlade::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	 
	//Bastion_Healer
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_Healer", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_Healer_Bin.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Healer", CMonster_Bastion_Healer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//weapon
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Staff", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Staff/", "Staff.fbx",
		L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Staff", CStaff::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Stage1Meteor()
{
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Meteor_Static", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Meteor_Static.fbx", CModel::TYPE_STATIC, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Meteor_Anim", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Meteor_Anim.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Meteor
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Meteor", CMeteor::Create(m_pDevice, m_pDeviceContext))))
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
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Title", L"../bin/Resources/Texture/Loading/logo_godfall_3d.dds")))
		return E_FAIL;

	g_pGameInstance->Init_SoundManager();

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
	if (FAILED(Set_Stage2_Prototype()))
		return E_FAIL;
	if (FAILED(Load_Stage2_Object()))
		return E_FAIL;

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
	// 스카이박스
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Sky_Texture1", L"../Bin/Resources/Texture/SkyBox/SkyBox_Stage1.dds"))) 
		MSGBOX("Failed Add To SkyBoxTex");
	//if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Sky_Texture2", L"../Bin/Resources/Texture/SkyBox/SkyBox_Stage2_1.dds")))
	//	MSGBOX("Failed Add To SkyBoxTex");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"VIBuffer_Cube", CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Cube.hlsl"))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_SkyBox", CSkyBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma region 리소스
	if(FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Texture/Terrain/Plane_Default.dds")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;
#pragma endregion

#pragma region 모델
	cout << "TestScene_JS 모델 프로토타입 생성중..." << endl;
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
		L"../../Reference/ShaderFile/Shader_Mesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(180.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_FlyingShield", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Shield/", "Shield.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_JumpNode_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/Resources/Mesh/JumpNode/JumpNode_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}	
#pragma endregion

#pragma region 컴포넌트
	cout << "TestScene_JS 컴포넌트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CharacterController", CCharacterController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_NavMeshCollider", CNavMeshCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", CBoxCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", CCapsuleCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_SphereCollider", CSphereCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Animator", CAnimator::Create(m_pDevice, m_pDeviceContext))))
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
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_JumpBox", CJumpBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_FlyingShield", CFlyingShield::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
#pragma endregion



#pragma region 몬스터
	///////////////////////////////////////////////////////////////////////////////////////몬스터
	//matPivot = XMMatrixIdentity();
	////Monster Crystal_Crawler
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Crawler", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Crystal_Crawler.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Crawler", CMonster_Crawler::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	//Monster EarthAberrant
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_EarthAberrant", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/EarthAberrant.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_EarthAberrant", CMonster_EarthAberrant::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//Monster EarthAberrant Weapon
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_EarthAberrant_Pick", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Earth_Aberrant_Pick/", "EarthAberrant_Pick.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_EarthAberrant_Pick", CEarthAberrant_Pick::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	////Bastion_2HSword
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_2HSword", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_2HSword_Bin.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_2HSword", CMonster_Bastion_2HSword::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	////Weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_RetributionBlade", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/RetributionBlade/", "RetributionBlade(2H).fbx",
	//	L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//{
	//	return E_FAIL;
	//}
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_RetributionBlade", CRetributionBlade::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	//// 슈터
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Shooter", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_Shooter.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Shooter", CMonster_Bastion_Shooter::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Shooter_Bullet", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Bullet/", "Sphere.fbx",
	//	L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Shooter_Bullet", CBullet::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//if (FAILED(Load_Stage1MonsterLoad()))
	//	return E_FAIL;

	//////BronzeAnimus
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_BronzeAnimus", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/BronzeAnimus_Bin.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_BronzeAnimus", CMonster_BronzeAnimus::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//////weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_BronzeAnimus_Sword", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/BronzeAnimus_Sword/", "BronzeAnimus_Sword.fbx",
	//	L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//{
	//	return E_FAIL;
	//}
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_BronzeAnimus_Sword", CBronzeAnimus_Sword::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	//////Bastion Spear
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_Spear", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_HonerGuard_Bin.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Spear", CMonster_Bastion_Spear::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//////weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Polearm", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Polearm/", "Bastion_Polearm.fbx",
	//	L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Polearm", CPolearm::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
#pragma endregion


	// dissolve 
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"DissovleBase", L"../bin/Resources/Texture/dissolve.dds"))) MSGBOX("Failed To Add dissolve Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"DissovleGradient", L"../bin/Resources/Texture/T_FlameGradient_01.dds"))) MSGBOX("Failed To Add dissolveGradient Tex");

	// UI
	if(FAILED(Load_Stage1StaticUILoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1UILoad()))
		return E_FAIL;

	//// Effect
	//if (FAILED(Load_Stage1EffectLoad()))
	//	return E_FAIL;

	///////////////////////////////////////// 소드 트레일
	if (FAILED(Load_TrailEffects()))
		return E_FAIL;

	///////////////////////////////////////// 메쉬 이펙트
	if (FAILED(Load_MeshEffects()))
		return E_FAIL;

	cout << "TestScene_JS 로딩 완료..." << endl;
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


HRESULT CLoader::Load_TrailEffects()
{
	///////////////////////////////////////// 소드 트레일
	// 기본 텍스처
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"TrailBase", L"../bin/Resources/Texture/Trail/T_Smoke_Trail_Soft.dds"))) MSGBOX("Failed To Add SwordTrail Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"DistortionBase", L"../bin/Resources/Texture/Trail/t_distort_channels.dds"))) MSGBOX("Failed To Add SwordTrail Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"DistortionMask", L"../bin/Resources/Texture/Trail/T_DistortionNoise.dds"))) MSGBOX("Failed To Add SwordTrail Tex");
	// 추가 텍스처
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WispTrail", L"../bin/Resources/Texture/Trail/T_WispTrail.dds"))) MSGBOX("Failed To Add WispTrail Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WispTrail_02", L"../bin/Resources/Texture/Trail/T_WispTrail_02.dds"))) MSGBOX("Failed To Add WispTrail_02 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WispTrail_Thin", L"../bin/Resources/Texture/Trail/T_WispTrail_thin.dds"))) MSGBOX("Failed To Add WispTrail_Thin Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Fire_02", L"../bin/Resources/Texture/Trail/T_Fire_02.dds"))) MSGBOX("Failed To Add Fire_02 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Wisp_01", L"../bin/Resources/Texture/Trail/T_Wisp_01.dds"))) MSGBOX("Failed To Add Wisp_01 Tex");
	// 버퍼
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_Trail_Distortion",
		CVIBuffer_Trail::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Distortion.hlsl", 256))))
		MSGBOX(L"트레일 버퍼 프로토타입 생성 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_Trail_Normal",
		CVIBuffer_Trail::Create(m_pDevice, m_pDeviceContext, L"../bin/ShaderFile/Shader_Trail.hlsl", 256))))
		MSGBOX(L"트레일 버퍼 프로토타입 생성 실패");
	// 노말
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_TrailEffect_Normal", CTrailEffect_Normal::Create(m_pDevice, m_pDeviceContext))))
		MSGBOX(L"트레일이펙트_노말 프로토타입 생성 실패");
	// 디스토션
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_TrailEffect_Distortion", CTrailEffect_Distortion::Create(m_pDevice, m_pDeviceContext))))
		MSGBOX(L"트레일이펙트_디스토션 프로토타입 생성 실패");

	return S_OK;
}

HRESULT CLoader::Load_MeshEffects()
{
	//////////////////////// Textures
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WispTrail_02_Red", L"../bin/Resources/Mesh/Effect/T_WispTrail_02_Red.dds"))) MSGBOX("Failed To Add T_WispTrail_02_Red Tex");

	//////////////////////// Meshs
	_matrix matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_MeshEffect", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "Effect01.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Outer", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "outer.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_MagicShield", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "magic_shield_00_01_mesh.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}


	///////////////Rock
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Explosion_Rock_Up_Anim", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Eff_fbx/Rock_Explosion_Up.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Explosion_Rock_Anim", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Eff_fbx/Rock_Explosion.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Explosion_Rock_Left_Anim", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Eff_fbx/Rock_Explosion_Left.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Explosion_Rock_Right_Anim", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Eff_fbx/Rock_Explosion_Right.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;


	//////////////////// Objects
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MeshEffect_Test", CMeshEffect_Test::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Explosion_Rock", CExplosion_Rock::Create(m_pDevice, m_pDeviceContext))))
		return S_OK;

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
