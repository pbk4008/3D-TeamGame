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
#include "Loot_Shield.h"
#include "Pot.h"
#include "Circle_Floor.h"

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
#include "Boss_Solaris.h"

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
#include "Boss_Weapon.h"

//Effect & UI
#include "Effect_DashDust.h"
#include "Effect_HitParticle.h"
#include "Effect_Energy.h"
#include "Effect_HitFloating.h"
#include "Effect_Floating_Speed.h"
#include "Effect_Env_Floating.h"
#include "Effect_Env_Fire.h"
#include "Effect_Falling_Leaf.h"
#include "Effect_FloatingUp.h" 
#include "Effect_Hammer_Dust.h"
#include "Effect_Dead_Spray.h"
#include "Effect_DeathParticle.h"
#include "Effect_Guard.h"
#include "Explosion_Rock.h" 
#include "VIBuffer_PointInstance_Energy.h"


#include "UI_Ingame.h"
#include "UI_Player_HpBar.h"
#include "UI_Player_HpBar_Red.h"
#include "UI_Boss_HpBar_Red.h"
#include "UI_Boss_ShieldBar_Blue.h"
#include "UI_HpHeal_Num.h"
#include "UI_Bar_Mark.h"
#include "UI_Monster_Panel.h"
#include "UI_Monster_Back.h"
#include "UI_Monster_Level.h"
#include "UI_Monster_LevelNum.h"
#include "UI_Monster_HpBar.h"
#include "UI_Monster_BackHpBar.h"
#include "UI_Monster_GroggyBar.h"
#include "UI_Monster_Name.h"
//#include "UI_Tuto_Base.h"
//#include "UI_Tuto_Font.h"
#include "UI_Skill_Meter_Back.h"
#include "UI_Skill_Meter_Gauge.h"
#include "UI_Skill_Meter_Gauge_Right.h"
#include "UI_Shield_Meter.h"
#include "UI_Blank_CKey.h"
#include "UI_Blank_FKey.h"
#include "UI_Fill_CKey.h"
#include "UI_Fill_CKey2.h"
#include "UI_Indicator.h"

//Cinema
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "CinemaWeapon.h"


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
#include "BoxBridge.h"
#include "CapsuleObstacle.h"
#include "DropBox.h"
#include "DropObject.h"
#include "TrailEffect_Normal.h"
#include "TrailEffect_Distortion.h"
#include "Wall.h"
#include "MotionTrail.h"

#include "MeshEffect_Test.h"
#include "MeshEffect_Test2.h"
#include "MeshEffect_Boss_Effect.h"
#include "MeshEffect_Boss_Explosion.h"
#include "MeshEffect_Boss_Shield.h"
#include "MeshEffect_EyeRazer.h"
#pragma endregion

//Test
#include "TestObj.h"
#include "MeshEffect_Razer.h"
#include "MeshEffect_Jupiter.h"
#include "DamageFont.h"

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

	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::BGM);

	return S_OK;
}

HRESULT CLoader::SetUp_Stage1_Object()
{
	//if (FAILED(Load_Stage1FBXLoad()))
	//	return E_FAIL;

	if (FAILED(Load_Stage1Navi_SkyLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1PlayerLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1MonsterLoad()))
		return E_FAIL;
	
	//if (FAILED(Load_Stage1BossLoad()))
	//	return E_FAIL;

	if (FAILED(Load_Stage1StaticUILoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1UILoad()))
		return E_FAIL;

#pragma region 이펙트들
	//if (FAILED(Load_Stage1EffectLoad()))
	//	return E_FAIL;
	if (FAILED(Load_TrailEffects())) //소드
		return E_FAIL;
	if (FAILED(Load_MeshEffects())) //매쉬
		return E_FAIL;
	if (FAILED(Load_StaticEffects())) // static effect
		return E_FAIL;
#pragma endregion

	if (FAILED(Load_Stage1JumpTrigger()))
		return E_FAIL;

	if (FAILED(Load_Stage1_TreasureChest_Load()))
		return E_FAIL;

	//if (FAILED(Load_Stage1TriggerLod()))
	//	return E_FAIL;

	//if (FAILED(Load_Stage1Meteor()))
	//	return E_FAIL;

	//if (FAILED(Load_Stage1_Cinema_Object()))
	//	return E_FAIL;

	//if (FAILED(Load_Pot()))
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

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"PavementDecor_01_Lod0.fbx",
		CModel::Create(m_pDevice, m_pDeviceContext, 
			L"../bin/FBX/Floor/PavementDecor_01_Lod0.fbx", CModel::TYPE_STATIC,true))))
		return E_FAIL;

	if(FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Circle_Floor",CCircle_Floor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


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
	////Buffer
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Bar", CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_UI_Bar.hlsl"))))
	//	return E_FAIL;

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

	//Level
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Monster_LevelNum"), CUI_Monster_LevelNum::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Monster_LevelNum", L"../bin/Resources/Texture/UI/Number/%d.dds", 10)))
	{
		return E_FAIL;
	}

	//PlayerHpHealNum
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_HpHeal_Num"), CUI_HpHeal_Num::Create(m_pDevice, m_pDeviceContext))))
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

	//PlayerSkill Meter Back
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Player_Skill_Meter_Back"), CUI_Skill_Meter_Back::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Skill_Meter_Back", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_WeaponArt_Meter_BG.dds")))
	{
		return E_FAIL;
	}
	
	//PlayerSkill Meter Gauge //노란색 왼쪽
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Player_Skill_Meter_Gauge"), CUI_Skill_Meter_Gauge::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Skill_Meter_Gauge_Full", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_WeaponArt_Meter_Full.dds")))
	{
		return E_FAIL;
	}

	//PlayerSkill Meter Gauge Right //주황색 오른쪽 
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Player_Skill_Meter_Gauge_Right"), CUI_Skill_Meter_Gauge_Right::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Skill_Meter_Gauge_Fill", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_WeaponArt_Meter_Fill.dds")))
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

	//Shield_Meter
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Shield_Meter"), CUI_Shield_Meter::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Shield_Meter", L"../bin/Resources/Texture/UI/Shield_Meter/%d.dds", 16)))
	{
		return E_FAIL;
	}
 
	////Tuto_Base
	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Tuto_Base"), CUI_Tuto_Base::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}
	//if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Tuto_Base", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_Tutorial_Backplate.dds")))
	//{
	//	return E_FAIL;
	//}

	////Tuto_Font
	//if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Tuto_Font"), CUI_Tuto_Font::Create(m_pDevice, m_pDeviceContext))))
	//{
	//	return E_FAIL;
	//}
	//if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Tuto_Font", L"../bin/Resources/Texture/UI/Static/Active/Tuto_Font_%d.dds", 4)))
	//{
	//	return E_FAIL;
	//}

	//Blank_CKey
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Blank_CKey"), CUI_Blank_CKey::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Blank_Ckey", L"../bin/Resources/Texture/UI/Static/Active/Blank_CKey.dds")))
	{
		return E_FAIL;
	}
	//Blank_FKey
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Blank_FKey"), CUI_Blank_FKey::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Blank_Fkey", L"../bin/Resources/Texture/UI/Static/Active/Blank_FKey.dds")))
	{
		return E_FAIL;
	}

	//Fill_CKey
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Fill_CKey"), CUI_Fill_Ckey::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Fill_CKey2"), CUI_Fill_Ckey2::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Fill_Ckey", L"../bin/Resources/Texture/UI/Static/Active/T_Keyboard_Light_Key_C2.dds")))
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

	// DamageFont
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_DamageFont", L"../bin/Resources/Texture/UI/DamageFont.dds")))
		MSGBOX(L"데미지 폰트 Add_Texture() 실패");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_GreenGradi", L"../bin/Resources/Texture/UI/GreenGradi.dds")))
		MSGBOX(L"데미지 폰트 Add_Texture() 실패");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_DamageFontBG", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_BossHealth_Blob.dds")))
		MSGBOX(L"데미지 폰트 Add_Texture() 실패");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_DamageFontBG2", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_CompassMarker_TextBlob.dds")))
		MSGBOX(L"데미지 폰트 Add_Texture() 실패");
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_DamageFont"), CDamageFont::Create(m_pDevice, m_pDeviceContext))))
		MSGBOX(L"데미지 폰트 Add_Texture() 실패");


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

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_VIBuffer_PointInstance_Energy",
		CVIBuffer_PointInstance_Energy::Create(m_pDevice, m_pDeviceContext))))
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

	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_Effect_Energy"), CEffect_Energy::Create(m_pDevice, m_pDeviceContext))))
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
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_BoxBridge", CBoxBridge::Create(m_pDevice, m_pDeviceContext))))
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
	//Scree
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::	SCENE_STAGE1, L"Model_Cinema_Scree", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/Scree.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Floor_Crack
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Floor", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/Floor_Crack.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	////Silvermane
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Cinema_Silvermane", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/Silvermane_Cinema.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;



	//PhoenixWeapon
	_matrix matPivot = XMMatrixIdentity();
	std::wstring shaderPath = L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl";
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_PoenixWeapon", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Phoenix/", "PhoenixSword.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true)))) 
		return E_FAIL;

	//GrayHwakWeapon
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_GrayHawkWeapon", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/GrayHwak/", "GrayHwakWeapon.fbx",
		shaderPath, matPivot, CModel::TYPE::TYPE_STATIC, true))))
		return E_FAIL;



	//Camera1-1
	matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam1_1", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/","Shot_01_01_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot,CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera1-2
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam1_2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_01_02_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Camera2-1
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam2_1", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_02_01_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera2-2
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam2_2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_02_02_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera2-3
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam2_3", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_02_03_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera2-4
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam2_4", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_02_04_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;
	//Camera3-1
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam3_1", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_03_01_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Camera3-2
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam3_2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_03_02_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Camera3-3
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam3_3", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_03_03_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Camera3-4
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam3_4", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_03_04_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Camera3-5
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam3_5", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_03_05_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//Camera3-6
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE1, L"Model_Cinema_Cam3_6", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_03_06_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CinemaCamera", CCinemaCam::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if(FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CinemaActor",CCinemaActor::Create(m_pDevice,m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CinemaWeapon", CCinemaWeapon::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Pot()
{
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Pot1", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Pot/S_Moon_Urn_1_03_Bin.fbx", CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}	

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Pot2", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Pot/S_Sun_Urns_01_Bin.fbx", CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Pot2_Anim", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Pot/S_Sun_Urns_01_Broken_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Pot3", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Pot/S_Sun_Urns_Set02_Bin.fbx", CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Pot", CPot::Create(m_pDevice, m_pDeviceContext))))
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
	////BronzeAnimus
	_matrix matPivot = XMMatrixIdentity();;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_BronzeAnimus", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/BronzeAnimus_Bin.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_BronzeAnimus", CMonster_BronzeAnimus::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	////weapon
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_BronzeAnimus_Sword", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/BronzeAnimus_Sword/", "BronzeAnimus_Sword.fbx",
		L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_BronzeAnimus_Sword", CBronzeAnimus_Sword::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	////Bastion Spear
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_Spear", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_HonerGuard_Bin.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Spear", CMonster_Bastion_Spear::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	////weapon
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Polearm", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Polearm/", "Bastion_Polearm.fbx",
		L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Polearm", CPolearm::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Set_Stage3_Prototype()
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

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Buffer_Potal", CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Potal.hlsl"))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Environment", CEnvironment::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_SubEnvironment", CSubEnvironment::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Treasure_Chest", CDropBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Portal", CPotal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_LootShield", CLoot_Shield::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CapsuleObstacle", CCapsuleObstacle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX(L"캡슐 장애물 프로토타입 생성 실패")
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoader::Load_Stage3_Object()
{
	if (FAILED(Load_Stage3_Cinema_Object()))
		return E_FAIL;

	//Boss Solaris
	if(FAILED(Load_Stage3_BossLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1PlayerLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1FBXLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1Navi_SkyLoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1StaticUILoad()))
		return E_FAIL;

	if (FAILED(Load_Stage1UILoad()))
		return E_FAIL;

#pragma region 이펙트들
	if (FAILED(Load_Stage1EffectLoad()))
		return E_FAIL;
	if (FAILED(Load_TrailEffects())) //소드
		return E_FAIL;
	if (FAILED(Load_MeshEffects())) //매쉬
		return E_FAIL;
	//if (FAILED(Load_StaticEffects())) //
	//	return E_FAIL;
#pragma endregion

	_matrix matPivot = XMMatrixIdentity();
	matPivot *= XMMatrixRotationZ(XMConvertToRadians(90.f)); //정수리옆으로나옴..
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Cylinder_LowPoly_Idst_Razer", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_cylinder_lowPoly_idst.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");


	//Boss_HpBar_Red
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Boss_HpBar_Red"), CUI_Boss_HpBar_Red::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Boss_HpBar_Red", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_BossHealth_Meter_Fill_Red.dds")))
	{
		return E_FAIL;
	}

	//Boss_ShieldBar_Blue
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Boss_ShieldBar_Blue"), CUI_Boss_ShieldBar_Blue::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Boss_ShieldBar_Blue", L"../bin/Resources/Texture/UI/Dynamic/Active/T_HUD_BossHealth_Meter_Fill_Blue.dds")))
	{
		return E_FAIL;
	}

	//Boss_Bar_Mark
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Proto_GameObject_UI_Bar_Mark"), CUI_Bar_Mark::Create(m_pDevice, m_pDeviceContext))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Texture_Bar_Mark", L"../bin/Resources/Texture/UI/Static/Active/T_HUD_BossHealth_Chevron.dds")))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Load_Stage3_BossLoad()
{
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Boss_Solaris", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Solaris.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Solaris", CBoss_Solaris::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Boss", CBoss_Weapon::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Stage3_Cinema_Object()
{
	//4-1
	_matrix matPivot = XMMatrixIdentity();
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam4_1", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_04_01_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//4-2
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam4_2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_04_02_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//4-3
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam4_3", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_04_03_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//4-4
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam4_4", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_04_04_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//4-5
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam4_5", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_04_05_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//4-6
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam4_6", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_04_06_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//5-1
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam5_1", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_05_01_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//5-2
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam5_2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_05_02_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//5-3
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Cam5_3", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/FBX/Cinema/Camera/", "Shot_05_03_bone.fbx",
		L"../../Reference/ShaderFile/Shader_AnimMesh.hlsl", matPivot, CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STAGE3, L"Model_Cinema_Solaris", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/Solrais_Cinema.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	//지울것!!

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Cinema_Silvermane", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Cinema/Silvermane_Cinema.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CinemaCamera", CCinemaCam::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CinemaActor", CCinemaActor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CinemaWeapon", CCinemaWeapon::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

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

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_LootShield", CLoot_Shield::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_CapsuleObstacle", CCapsuleObstacle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX(L"캡슐 장애물 프로토타입 생성 실패");
		return E_FAIL;
	}

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
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))	
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))		
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Wall", CWall::Create(m_pDevice, m_pDeviceContext)))) MSGBOX(L"Wall 원본 생성 실패");
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_FlyingShield", CFlyingShield::Create(m_pDevice, m_pDeviceContext)))) MSGBOX(L"날으는 쉴드 원본 생성 실패");
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MotionTrail", CMotionTrail::Create(m_pDevice, m_pDeviceContext)))) MSGBOX(L"모션 더미 생성");
#pragma endregion

	//// Test Object
	//matPivot = XMMatrixIdentity();
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Test", CModel::Create(m_pDevice, m_pDeviceContext, "../bin/FBX/Cinema/Test_box/", "Test_box.fbx"
	//	, L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_TestObject", CTestObj::Create(m_pDevice, m_pDeviceContext)))) MSGBOX(L"테스트 오브젝트 생성");

	return S_OK;
}

HRESULT CLoader::Load_Stage1BossLoad()
{
	//Boss Bastion_Tier4
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Boss_Bastion_Tier4", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_Tier4.fbx",CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Boss_Bastion", CBoss_Bastion_Judicator::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	 
	//weapon
	//ShieldBreaker_BossWeapon
	_matrix matPivot = XMMatrixIdentity();

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_ShieldBreaker", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/ShieldBreaker/", "ShieldBreaker.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_ShieldBreaker", CShieldBreaker::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	////Boss Solaris
	//if (FAILED(Load_Stage3_BossLoad()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Stage1MonsterLoad()
{
	_matrix matPivot = XMMatrixIdentity();

	//////Monster Crystal_Crawler
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Crawler", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Crystal_Crawler.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Crawler", CMonster_Crawler::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	////Monster EarthAberrant
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_EarthAberrant", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/EarthAberrant.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_EarthAberrant", CMonster_EarthAberrant::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//////Monster EarthAberrant Weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_EarthAberrant_Pick", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Earth_Aberrant_Pick/", "EarthAberrant_Pick.fbx",
	//	L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_EarthAberrant_Pick", CEarthAberrant_Pick::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	////Monster Bastion_Sword
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Sword", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_Sword.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Sword", CMonster_Bastion_Sword::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//////Weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_Stargazer", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Stargazer/", "Stargazer(1H Sword).fbx",
	//	L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Stargazer", CStargazer::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	////Monster Bastion_Shooter
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_Bastion_Shooter", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_Shooter.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Shooter", CMonster_Bastion_Shooter::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//// Bullet
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Shooter_Bullet", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Bullet/", "Sphere.fbx",
	//	L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Shooter_Bullet", CBullet::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	////Bastion_2HSword
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_2HSword", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_2HSword_Bin.fbx", CModel::TYPE_ANIM, true))))
	{
		return E_FAIL;
	}
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
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Bastion_Healer", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Bastion_Healer_Bin.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_Bastion_Healer", CMonster_Bastion_Healer::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	////weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Staff", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Staff/", "Staff.fbx",
	//	L"../../Reference/ShaderFile/Shader_Weapon.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//{
	//	return E_FAIL;
	//}
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Staff", CStaff::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	////////////////////////////////////////////////////////// Stage2 용임 주석 풀기 ㄴㄴ
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
	if (FAILED(Set_Stage3_Prototype()))
		return E_FAIL;

	if(FAILED(Load_Stage3_Object()))
		return E_FAIL;

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
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Texture/Terrain/Plane_Default.dds")))
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
	//matPivot = XMMatrixIdentity();
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_JumpNode_Bin", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/Resources/Mesh/JumpNode/JumpNode_Bin.fbx", CModel::TYPE_ANIM, true))))
	//{
	//	return E_FAIL;
	//}	


	//Boss Bastion_Tier4
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Boss_Bastion_Tier4", CModel::Create(m_pDevice, m_pDeviceContext,
		L"../bin/FBX/Monster/Bastion_Tier4.fbx", CModel::TYPE_ANIM, true))))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Boss_Bastion", CBoss_Bastion_Judicator::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//weapon
	//ShieldBreaker_BossWeapon
	matPivot = XMMatrixIdentity();

	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_ShieldBreaker", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/ShieldBreaker/", "ShieldBreaker.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_ShieldBreaker", CShieldBreaker::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

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
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_JumpNode", CJumpNode::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_JumpBox", CJumpBox::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_FlyingShield", CFlyingShield::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MotionTrail", CMotionTrail::Create(m_pDevice, m_pDeviceContext)))) MSGBOX(L"모션 더미 생성");

	////Boss Solaris
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Boss_Solaris", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/Solaris.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Solaris", CBoss_Solaris::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_Boss", CBoss_Weapon::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
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

	////Monster EarthAberrant
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Monster_EarthAberrant", CModel::Create(m_pDevice, m_pDeviceContext,
	//	L"../bin/FBX/Monster/EarthAberrant.fbx", CModel::TYPE_ANIM, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Monster_EarthAberrant", CMonster_EarthAberrant::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//
	////Monster EarthAberrant Weapon
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_EarthAberrant_Pick", CModel::Create(m_pDevice, m_pDeviceContext,
	//	"../bin/Resources/Mesh/Earth_Aberrant_Pick/", "EarthAberrant_Pick.fbx",
	//	L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Weapon_EarthAberrant_Pick", CEarthAberrant_Pick::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//
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
	if (FAILED(Load_Stage1EffectLoad()))
		return E_FAIL;

	
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
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"TrailBase", L"../bin/Resources/Texture/Trail/T_Smoke_Trail_Soft.dds"))) 
		MSGBOX("Failed To Add SwordTrail Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"DistortionBase", L"../bin/Resources/Texture/Trail/t_distort_channels.dds"))) 
		MSGBOX("Failed To Add SwordTrail Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"DistortionMask", L"../bin/Resources/Texture/Trail/T_DistortionNoise.dds")))
		MSGBOX("Failed To Add SwordTrail Tex");
	// 추가 텍스처
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WispTrail", L"../bin/Resources/Texture/Trail/T_WispTrail.dds"))) 
		MSGBOX("Failed To Add WispTrail Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WispTrail_02", L"../bin/Resources/Texture/Trail/T_WispTrail_02.dds")))
		MSGBOX("Failed To Add WispTrail_02 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WispTrail_Thin", L"../bin/Resources/Texture/Trail/T_WispTrail_thin.dds"))) 
		MSGBOX("Failed To Add WispTrail_Thin Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Fire_02", L"../bin/Resources/Texture/Trail/T_Fire_02.dds"))) 
		MSGBOX("Failed To Add Fire_02 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Wisp_01", L"../bin/Resources/Texture/Trail/T_Wisp_01.dds"))) 
		MSGBOX("Failed To Add Wisp_01 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"EnergyPlasma_Tile_H_01", L"../bin/Resources/Texture/Trail/T_EnergyPlasma_Tile_H_01.dds"))) 
		MSGBOX("Failed To Add EnergyPlasma_Tile_H_01 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"FluidTile01_DA", L"../bin/Resources/Texture/Trail/T_FluidTile01_DA.dds")))
		MSGBOX("Failed To Add FluidTile01_DA Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Lightning_Beam_Tile", L"../bin/Resources/Texture/Trail/T_Lightning_Beam_Tile.dds")))
		MSGBOX("Failed To Add Lightning_Beam_Tile Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Melee_Arc_02", L"../bin/Resources/Texture/Trail/T_Melee_Arc_02.dds")))
		MSGBOX("Failed To Add Melee_Arc_02 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"PerlinElectric_Packed", L"../bin/Resources/Texture/Trail/T_PerlinElectric_Packed.dds"))) 
		MSGBOX("Failed To Add PerlinElectric_Packed Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plasma_Seamless", L"../bin/Resources/Texture/Trail/T_Plasma_Seamless.dds")))
		MSGBOX("Failed To Add Plasma_Seamless Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"SpeedTrails_Packed", L"../bin/Resources/Texture/Trail/T_SpeedTrails_Packed.dds")))
		MSGBOX("Failed To Add SpeedTrails_Packed Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Streak_Soft_Trail", L"../bin/Resources/Texture/Trail/T_Streak_Soft_Trail.dds")))
		MSGBOX("Failed To Add Streak_Soft_Trail Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Venus_Trail", L"../bin/Resources/Texture/Trail/T_Venus_Trail.dds")))
		MSGBOX("Failed To Add Venus_Trail Tex");
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
	/////////////////////////////////////// Textures
#pragma region Base
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"WispTrail_02_Red", L"../bin/Resources/Mesh/Effect/T_WispTrail_02_Red.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Hexgrid", L"../bin/Resources/Mesh/Effect/T_Hexgrid.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"spk_17", L"../bin/Resources/Mesh/Effect/eye_lwy_t_spk_17_single.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"magic_aura01", L"../bin/Resources/Mesh/Effect/mesh_03_magic_aura01_00_00_alba.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"magic_electric00", L"../bin/Resources/Mesh/Effect/mesh_03_magic_electric00_00_00_ALBA.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"magic_electric01", L"../bin/Resources/Mesh/Effect/mesh_03_magic_electric01_00_00_ALBA.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"magic_electric02", L"../bin/Resources/Mesh/Effect/mesh_03_magic_electric02_00_00_ALBA.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"wp01_007", L"../bin/Resources/Mesh/Effect/mesh_03_wp01_007_0000_01_00_MSK4.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"myfire", L"../bin/Resources/Mesh/Effect/myfire.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"waterfall02", L"../bin/Resources/Mesh/Effect/waterfall02_water_atos.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"T_ShieldPattern2", L"../bin/Resources/Mesh/Effect/T_ShieldPattern2.dds")))
		MSGBOX("Failed To Add 메쉬이펙트용 텍스처 Tex");
#pragma endregion
#pragma region FX12
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"dist_noise", L"../bin/Resources/Mesh/Effect/FX12/T_ky_dist_noise.dds")))
		MSGBOX("Failed To Add FX12 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"dust_longStar", L"../bin/Resources/Mesh/Effect/FX12/T_ky_dust_longStar.dds")))
		MSGBOX("Failed To Add FX12 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"flowmap", L"../bin/Resources/Mesh/Effect/FX12/T_ky_flowmap.dds")))
		MSGBOX("Failed To Add FX12 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"maskRGB3", L"../bin/Resources/Mesh/Effect/FX12/T_ky_maskRGB3.dds")))
		MSGBOX("Failed To Add FX12 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"maskRGB5", L"../bin/Resources/Mesh/Effect/FX12/T_ky_maskRGB5.dds")))
		MSGBOX("Failed To Add FX12 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"noise4", L"../bin/Resources/Mesh/Effect/FX12/T_ky_noise4.dds")))
		MSGBOX("Failed To Add FX12 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"noise5", L"../bin/Resources/Mesh/Effect/FX12/T_ky_noise5.dds")))
		MSGBOX("Failed To Add FX12 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"noise6", L"../bin/Resources/Mesh/Effect/FX12/T_ky_noise6.dds")))
		MSGBOX("Failed To Add FX12 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"noise16", L"../bin/Resources/Mesh/Effect/FX12/T_ky_noise16.dds")))
		MSGBOX("Failed To Add FX12 Tex");
#pragma endregion
#pragma region FX13
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"dust01", L"../bin/Resources/Mesh/Effect/FX13/T_ky_dust01.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"dust02", L"../bin/Resources/Mesh/Effect/FX13/T_ky_dust02.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"fire01", L"../bin/Resources/Mesh/Effect/FX13/T_ky_fire01.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"flare_a", L"../bin/Resources/Mesh/Effect/FX13/T_ky_flare_a.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"flare01", L"../bin/Resources/Mesh/Effect/FX13/T_ky_flare01.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"flareVertical02", L"../bin/Resources/Mesh/Effect/FX13/T_ky_flareVertical02.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"ice02", L"../bin/Resources/Mesh/Effect/FX13/T_ky_ice02.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"lensFlare01", L"../bin/Resources/Mesh/Effect/FX13/T_ky_lensFlare01.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"maskRGB06", L"../bin/Resources/Mesh/Effect/FX13/T_ky_maskRGB06.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"smoke1", L"../bin/Resources/Mesh/Effect/FX13/T_ky_smoke1.dds")))
		MSGBOX("Failed To Add FX13 Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"smoke03", L"../bin/Resources/Mesh/Effect/FX13/T_ky_smoke03.dds")))
		MSGBOX("Failed To Add FX13 Tex");
#pragma endregion
#pragma region FXVerietyPack
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"decoLines", L"../bin/Resources/Mesh/Effect/FXVarietyPack/T_ky_decoLines.dds")))
		MSGBOX("Failed To Add FXVerietyPack Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"magicCircle020", L"../bin/Resources/Mesh/Effect/FXVarietyPack/T_ky_magicCircle020.dds")))
		MSGBOX("Failed To Add FXVerietyPack Tex");
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"noise17", L"../bin/Resources/Mesh/Effect/FXVarietyPack/T_ky_noise17.dds")))
		MSGBOX("Failed To Add FXVerietyPack Tex");
#pragma endregion



	/////////////////////////////////////// Meshs
	_matrix matPivot = XMMatrixIdentity();
#pragma region Base
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_MeshEffect", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "Effect01.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Branch", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "branch.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Magic_Electric00", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "magic_eletric00.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Magic_Electric01", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "magic_eletric01.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Magic_Electric02", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "magic_eletric02.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Outer", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "outer.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_MagicShield", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "magic_shield_00_01_mesh.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Parts", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "parts.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Twirl", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "Twirl.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Smoke", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "smoke.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Sphere00", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "sphere00.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Portal", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/", "Portal.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
#pragma endregion
#pragma region FX12
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Circle_Midpoly", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX12/", "ky_circle_midpoly.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_CylinderBend04", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX12/", "ky_cylinderBend04.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Needle01", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX12/", "ky_needle01.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Quad_CenterRot", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX12/", "ky_quad_centerRot.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Sphere2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX12/", "ky_sphere.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_SphereHarf03", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX12/", "ky_sphereHarf03.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
#pragma endregion
#pragma region FX13
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_AuraHousya", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_auraHousya.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Circle_Inv", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_circle_inv.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Circle_Inv2", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_circle_inv2.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Circle_Lowpoly", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_circle_lowpoly.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_CircleDist5", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_circleDist5.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_CircleDist06", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_circleDist06.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Cylinder_LowPoly_Idst", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_cylinder_lowPoly_idst.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Cylinder_SuperLow_Inv", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_cylinder_superLow_inv.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_CylinderBend03", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_cylinderBend03.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_CylinderBend03b", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_cylinderBend03b.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_CylinderBend06", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_cylinderBend06.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Lans", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_lans.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_LansHarf", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_lansHarf.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Quads", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_quads.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_RayOffset", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_rayOffset.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Tornado06", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_tornado06.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Tornado06_Back", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_tornado06_back.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_WindLine30", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_windLine30.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_WindSmoke", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_windSmoke.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_WindSmokeVertRot", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FX13/", "ky_windSmokeVertRot.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
#pragma endregion
#pragma region FXVerietyPack
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_AuraWall_DistB_Kai", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FXVarietyPack/", "ky_auraWall_distB_kai.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Cylinder_LowPoly_Dist", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FXVarietyPack/", "ky_cylinder_lowPoly_dist.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Cylinder_SuperLow", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FXVarietyPack/", "ky_cylinder_superLow.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_CylinderBend07", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FXVarietyPack/", "ky_cylinderBend07.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_PlaneBend", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FXVarietyPack/", "ky_planeBend.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Tornado07", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Effect/FXVarietyPack/", "ky_tornado07.fbx",
		L"../bin/ShaderFile/Shader_MeshEffect.hlsl", matPivot, CModel::TYPE_STATIC, true)))) MSGBOX(L"메쉬 이펙트용 메쉬 로드 실패");
#pragma endregion



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
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MeshEffect_Test2", CMeshEffect_Test2::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MeshEffect_Razer", CMeshEffect_Razer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MeshEffect_Jupiter", CMeshEffect_Jupiter::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Explosion_Rock", CExplosion_Rock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MeshEffect_Boss_Explosion", CMeshEffect_Boss_Explosion::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MeshEffect_Boss_Shield", CMeshEffect_Boss_Shield::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_MeshEffect_EyeRazer", CMeshEffect_EyeRazer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//힐이펙트
	//Heal
	matPivot = XMMatrixScaling(0.005f, 0.005f, 0.005f);
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model_Player_HealEffect", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Bullet/", "Sphere.fbx",
		L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot, CModel::TYPE_STATIC, true))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_StaticEffects()
{
	//이펙트 매니저에 넣으면서 생성
	// 주의 사항!! 넣을때 순서가 ENUM순서
	//Manager에 넣을 Effect;
#pragma region 이펙트매니저에 들어가는것들, 순서지켜서 enum에 맞춰줘야됨 

//	//Player Dash
	CEffect* pEffect = nullptr;
	vector<CEffect_DashDust::EF_PAR_DASH_DESC> vecDashEffect;

	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_Player_Foot_Dash.dat");
	vecDashEffect[0].fAlpha = 0.05f;

	wstring FullName = L"Proto_GameObject_Effect_DashDust";

	////마지막에 받을 Effect변수 넣기
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Foot_Dash", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Foot_Dash in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//매니저에 이펙트 넣기 (마지막 매개변수 : 같은 이펙트 추가로 넣을 갯수)
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Foot_Dash", pEffect, 20)))
	{
		MSGBOX("Falild to Add_Effect_Player_Foot_Dash in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	////HitGroundSmoke
	vecDashEffect.clear();
	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_HitGround_Smoke.dat");

	vecDashEffect[0].fAlpha = 0.1f;

	FullName = L"Proto_GameObject_Effect_DashDust";

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGroundSmoke", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_HitGroundSmoke in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGroundSmoke", pEffect, 6)))
	{
		MSGBOX("Falild to Add_Effect_HitGroundSmoke in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Player Atttack1
	vector<CEffect_HitParticle::EF_PAR_HIT_DESC> vecHitParticle;
	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Player_Attack1.dat");

	FullName = L"Proto_GameObject_Effect_Explosion";
	vecHitParticle[0].ParticleColor = { 1.f , 0.6f, 0.3f ,1.f };
	vecHitParticle[0].Power = 2.5f;

	//마지막에 받을 Effect변수 넣기
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack1", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack1 in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	//매니저에 이펙트 넣기 (마지막 매개변수 : 같은 이펙트 추가로 넣을 갯수)
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack1", pEffect, 20)))
	{
		MSGBOX("Falild to Add_Effect_Player_Attack1 in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Player Atttack2 Floating
	vector<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC> vecHitFloating;
	g_pGameInstance->LoadFile<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC>(vecHitFloating, L"../bin/SaveData/Effect/Effect_Player_Attack2_Floating.dat");

	FullName = L"Proto_GameObject_Effect_Floating";
	vecHitFloating[0].ParticleColor = { 1.f , 0.8f, 0.5f ,1.f };
	vecHitFloating[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating", FullName, &vecHitFloating[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack2_Floating in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating", pEffect, 20)))
	{
		MSGBOX("Falild to Add_Effect_Player_Attack2_Floating in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Player Atttack2 Floating2
	vector<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC> vecFloatingSpeed;
	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack2_Floating_2.dat");

	FullName = L"Proto_GameObject_Effect_Floating_Speed";
	vecFloatingSpeed[0].ParticleColor = { 1.f,0.5f,0.4f, 1.f };
	vecFloatingSpeed[0].Power = 2.0f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating2", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack2_Floating2 in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating2", pEffect, 20)))
	{
		MSGBOX("Falild to Add_Effect_Player_Attack2_Floating2 in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//MonsterDeath
	vecHitParticle.clear();
	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Monster_Death.dat");

	FullName = L"Proto_GameObject_Effect_Explosion";
	vecHitParticle[0].ParticleColor = { 0.5f, 1.0f, 0.1f ,1.f };
	vecHitParticle[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Death", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Monster_Death in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Death", pEffect, 1)))
	{
		MSGBOX("Falild to Add_Effect_Monster_Death in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Player Hit
	vecHitParticle.clear();
	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Player_Hit.dat");

	FullName = L"Proto_GameObject_Effect_Explosion";
	vecHitParticle[0].ParticleColor = { 1.f , 0.6f, 0.3f ,1.f };
	vecHitParticle[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Hit", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Hit in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Hit", pEffect, 8)))
	{
		MSGBOX("Falild to Add_Effect_Player_Hit in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Monster Guard
	vecHitParticle.clear();
	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Monster_Guard.dat");

	FullName = L"Proto_GameObject_Effect_Explosion";
	vecHitParticle[0].ParticleColor = { 0.3f, 0.3f, 1.f ,1.f };
	vecHitParticle[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Guard", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Monster_Guard in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Guard", pEffect, 8)))
	{
		MSGBOX("Falild to Add_Effect_Monster_Guard in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//AttackLeft
	vecFloatingSpeed.clear();
	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack_Left.dat");

	FullName = L"Proto_GameObject_Effect_Floating_Speed";

	vecFloatingSpeed[0].ParticleColor = { 1.f, 0.9f, 0.5f, 1.f };
	vecFloatingSpeed[0].Power = 2.0f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Left", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack_Left in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Left", pEffect, 5)))
	{
		MSGBOX("Falild to Add_Effect_Player_Attack_Left in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//AttackRight
	vecFloatingSpeed.clear();
	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack_Right.dat");

	FullName = L"Proto_GameObject_Effect_Floating_Speed";
	vecFloatingSpeed[0].ParticleColor = { 1.f, 0.9f, 0.5f, 1.f };
	vecFloatingSpeed[0].Power = 2.0f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Right", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack_Right in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Right", pEffect, 10)))
	{
		MSGBOX("Falild to Add_Effect_Player_Attack_Right in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//AttackRight_Last
	vecFloatingSpeed.clear();
	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack_Right_Last.dat");

	FullName = L"Proto_GameObject_Effect_Floating_Speed";
	vecFloatingSpeed[0].ParticleColor = { 1.f, 0.9f, 0.5f, 1.f };
	vecFloatingSpeed[0].Power = 2.0f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Right_Last", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack_Right_Last in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Right_Last", pEffect, 5)))
	{
		MSGBOX("Falild to Add_Effect_Player_Attack_Right_Last in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//HitGround
	vecHitParticle.clear();
	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Hit_Ground.dat");

	FullName = L"Proto_GameObject_Effect_Explosion";
	vecHitParticle[0].ParticleColor = { 0.9f,0.5f,0.2f, 1.f };
	vecHitParticle[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hit_Ground", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Hit_Ground in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hit_Ground", pEffect, 6)))
	{
		MSGBOX("Falild to Add_Effect_Hit_Ground in CStage1::Ready_Effect()");
		return E_FAIL;
	}


	//OpenBox
	vecFloatingSpeed.clear();
	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Open_Box.dat");

	FullName = L"Proto_GameObject_Effect_Floating_Speed";
	vecFloatingSpeed[0].ParticleColor = { 0.3f,0.5f,1.f, 1.f };
	vecFloatingSpeed[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Open_Box", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Open_Box in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Open_Box", pEffect, 3)))
	{
		MSGBOX("Falild to Add_Effect_Open_Box in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//PlayerAttackGround
	vecFloatingSpeed.clear();
	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack_Ground.dat");

	FullName = L"Proto_GameObject_Effect_Floating_Speed";
	vecFloatingSpeed[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
	vecFloatingSpeed[0].Power = 1.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Ground", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack_Ground in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Ground", pEffect, 5)))
	{
		MSGBOX("Falild to Add_Effect_Player_Attack_Ground in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//PlayerAttackGround_2
	vecHitFloating.clear();
	g_pGameInstance->LoadFile<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC>(vecHitFloating, L"../bin/SaveData/Effect/Effect_Player_Attack_Ground_2.dat");

	FullName = L"Proto_GameObject_Effect_Floating";
	vecHitFloating[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
	vecHitFloating[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Ground2", FullName, &vecHitFloating[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack_Ground2 in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Ground2", pEffect, 5)))
	{
		MSGBOX("Falild to Add_Effect_Player_Attack_Ground2 in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Item 
	vector<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC> vecFloatingUp;
	g_pGameInstance->LoadFile<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC>(vecFloatingUp, L"../bin/SaveData/Effect/Effect_Item.dat");

	vecFloatingUp[0].ParticleColor = { 0.5f,1.0f,0.3f, 1.f };
	vecFloatingUp[0].Power = 1.5f;
	vecFloatingUp[0].fRespawnPosY = 5.f;
	vecFloatingUp[0].bSmall = true;

	FullName = L"Proto_GameObject_Effect_FloatingUp";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Item", FullName, &vecFloatingUp[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Item in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Item", pEffect, 30)))
	{
		MSGBOX("Falild to Add_Effect_Item in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Box 
	vecFloatingUp.clear();
	g_pGameInstance->LoadFile<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC>(vecFloatingUp, L"../bin/SaveData/Effect/Effect_Box.dat");

	vecFloatingUp[0].ParticleColor = { 0.5f,1.0f,0.7f, 1.f };
	vecFloatingUp[0].Power = 1.5f;
	vecFloatingUp[0].fRespawnPosY = 5.f;
	vecFloatingUp[0].bSmall = true;

	FullName = L"Proto_GameObject_Effect_FloatingUp";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Box", FullName, &vecFloatingUp[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Box in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Box", pEffect, 5)))
	{
		MSGBOX("Falild to Add_Effect_Box in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//EatItem
	vecFloatingSpeed.clear();
	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Eat_Item.dat");

	FullName = L"Proto_GameObject_Effect_Floating_Speed";
	vecFloatingSpeed[0].ParticleColor = { 1.f,1.f,0.7f, 1.f };
	vecFloatingSpeed[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Eat_Item", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Eat_Item in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Eat_Item", pEffect, 16)))
	{
		MSGBOX("Falild to Add_Effect_Eat_Item in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	////Hit_Image
	CEffect_Dead_Spray::IMAGEEFFECTDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	_tcscpy_s(tDesc.TextureTag, L"T_Hit");
	tDesc.iRenderPassNum = 1;
	tDesc.iImageCountX = 4;
	tDesc.iImageCountY = 2;
	tDesc.fFrame = 4;
	tDesc.fEffectPlaySpeed = 20.f;
	tDesc.fAlpha = 1.f;
	tDesc.fWeight = 1.f;
	tDesc.fSize = 3.f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_HitImage", L"Proto_GameObject_Effect_HitImage", &tDesc, (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitImage", pEffect, 16)))
	{
		MSGBOX("Falild to Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Death smoke image
	ZeroMemory(&tDesc, sizeof(tDesc));
	_tcscpy_s(tDesc.TextureTag, L"Smoke_4x4_1");
	tDesc.iRenderPassNum = 1;
	tDesc.iImageCountX = 4;
	tDesc.iImageCountY = 4;
	tDesc.fFrame = 16;
	tDesc.fEffectPlaySpeed = 1.f;
	tDesc.fAlpha = 0.8f;
	tDesc.fWeight = 0.f;
	tDesc.fSize = 6.f;


	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_DeadSmoke", L"Proto_GameObject_Effect_HitImage", &tDesc, (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_DeadSmoke", pEffect, 16)))
	{
		MSGBOX("Falild to Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
		return E_FAIL;
	}


	//Explosion Rock mesh
	CExplosion_Rock* pObj = nullptr;
	CExplosion_Rock::ROCKINFO DescRock;
	FullName = L"Proto_GameObject_Explosion_Rock";
	_tcscpy_s(DescRock.ModelTag, L"Model_Explosion_Rock_Up_Anim");

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Up", FullName, &DescRock, (CGameObject**)&pObj)))
	{
		MSGBOX("Failed to Creating Effect_Explosion Rock_Up in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Up", pObj, 10)))
	{
		MSGBOX("Falild to Add_Effect_Explosion Rock in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//explosion rock left mesh
	_tcscpy_s(DescRock.ModelTag, L"Model_Explosion_Rock_Left_Anim");

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Left", FullName, &DescRock, (CGameObject**)&pObj)))
	{
		MSGBOX("Failed to Creating Effect_Explosion Rock_Left in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Left", pObj, 5)))
	{
		MSGBOX("Falild to Add_Effect_Explosion Rock Left in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//explosion rock right mesh
	_tcscpy_s(DescRock.ModelTag, L"Model_Explosion_Rock_Right_Anim");

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Right", FullName, &DescRock, (CGameObject**)&pObj)))
	{
		MSGBOX("Failed to Creating Effect_Explosion Rock_Right in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Right", pObj, 5)))
	{
		MSGBOX("Falild to Add_Effect_Explosion ROck Right in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//explosion rock mesh
	_tcscpy_s(DescRock.ModelTag, L"Model_Explosion_Rock_Anim");

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock", FullName, &DescRock, (CGameObject**)&pObj)))
	{
		MSGBOX("Failed to Creating Effect_Explosion Rock in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock", pObj, 5)))
	{
		MSGBOX("Falild to Add_Effect_Explosion ROck CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Hammer_Dust image
	CEffect_Hammer_Dust* pHammer = nullptr;
	CEffect_Hammer_Dust::EFFECTDESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	_tcscpy_s(Desc.TextureTag, L"Hammer_Dust_2");
	Desc.iRenderPassNum = 1;
	Desc.iImageCountX = 8;
	Desc.iImageCountY = 4;
	Desc.fFrame = 32.f;
	Desc.fEffectPlaySpeed = 1.f;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hammer_Dust", L"Proto_GameObject_Effect_Hammer_Dust", &Desc, (CGameObject**)&pHammer)))
	{
		MSGBOX("Failed to Creating Effect_Hammer_Dust in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hammer_Dust", pHammer, 7)))
	{
		MSGBOX("Falild to Add_Effect_Explosion ROck in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Energy
	vector<CEffect_Energy::EF_PAR_ENERGY_DESC> vecEnergyParticle;
	g_pGameInstance->LoadFile<CEffect_Energy::EF_PAR_ENERGY_DESC>(vecEnergyParticle, L"../bin/SaveData/Effect/Effect_Energy.dat");

	FullName = L"Proto_GameObject_Effect_Energy";
	vecEnergyParticle[0].ParticleColor = { 1.f , 0.6f, 0.3f ,1.f };
	vecEnergyParticle[0].Power = 2.5f;

	//마지막에 받을 Effect변수 넣기
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Energy", FullName, &vecEnergyParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Player_Attack1 in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Energy", pEffect, 2)))
	{
		MSGBOX("Falild to Add_Effect_Energy in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//EyeRazer 
	vecHitParticle.clear();
	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Boss_Razer.dat");

	FullName = L"Proto_GameObject_Effect_Explosion";
	vecHitParticle[0].ParticleColor = { 1.f , 0.4f, 0.3f ,1.f };
	vecHitParticle[0].Power = 2.5f;

	//마지막에 받을 Effect변수 넣기
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Razer", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Razer in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Razer", pEffect, 30)))
	{
		MSGBOX("Falild to Add Effect_Razer in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Boss_Attack_Ground
	vecFloatingSpeed.clear();
	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Boss_Attack_Ground.dat");

	FullName = L"Proto_GameObject_Effect_Floating_Speed";
	vecFloatingSpeed[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
	vecFloatingSpeed[0].Power = 1.f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Attack_Ground", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Boss_Attack_Ground in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Attack_Ground", pEffect, 2)))
	{
		MSGBOX("Falild to Add_Effect_Boss_Attack_Ground in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Boss_AttackGround_2
	vecHitFloating.clear();
	g_pGameInstance->LoadFile<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC>(vecHitFloating, L"../bin/SaveData/Effect/Effect_Boss_Attack_Ground_2.dat");

	FullName = L"Proto_GameObject_Effect_Floating";
	vecHitFloating[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
	vecHitFloating[0].Power = 0.8f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Attack_Ground_2", FullName, &vecHitFloating[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Boss_Attack_Ground_2 in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Attack_Ground_2", pEffect, 3)))
	{
		MSGBOX("Falild to Add_Effect_Boss_Attack_Ground_2 in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	////HitGroundSmoke_Boss
	vecDashEffect.clear();
	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_HitGround_Smoke_Boss.dat");

	vecDashEffect[0].fAlpha = 0.1f;

	FullName = L"Proto_GameObject_Effect_DashDust";

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGround_Smoke_Boss", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_HitGround_Smoke_Boss in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGround_Smoke_Boss", pEffect, 5)))
	{
		MSGBOX("Falild to Add_Effect_HitGround_Smoke_Boss in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	////Boss_Razer_Smoke
	vecDashEffect.clear();
	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_Boss_Razer_Smoke.dat");

	vecDashEffect[0].fAlpha = 0.08f;

	FullName = L"Proto_GameObject_Effect_DashDust";

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Razer_Smoke", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Boss_Razer_Smoke in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Razer_Smoke", pEffect, 30)))
	{
		MSGBOX("Falild to Add_Effect_Boss_Razer_Smoke in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//boss_HitGround
	vecHitParticle.clear();
	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Boss_Hit_Ground.dat");

	FullName = L"Proto_GameObject_Effect_Explosion";
	vecHitParticle[0].ParticleColor = { 1.f,0.5f,0.2f, 1.f };
	vecHitParticle[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Hit_Ground", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Boss_Hit_Ground in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Hit_Ground", pEffect, 4)))
	{
		MSGBOX("Falild to Add_Effect_Boss_Hit_Ground in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	////Boss_Side_Attack_Smoke
	vecDashEffect.clear();
	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_Boss_Side_Attack_Smoke.dat");

	vecDashEffect[0].fAlpha = 0.08f;

	FullName = L"Proto_GameObject_Effect_DashDust";

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Side_Attack_Smoke", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Boss_Side_Attack_Smoke in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Side_Attack_Smoke", pEffect, 6)))
	{
		MSGBOX("Falild to Add_Effect_Boss_Side_Attack_Smoke in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Boss_Side_Attack
	vecHitFloating.clear();
	g_pGameInstance->LoadFile<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC>(vecHitFloating, L"../bin/SaveData/Effect/Effect_Boss_Side_Attack.dat");

	FullName = L"Proto_GameObject_Effect_Floating";
	vecHitFloating[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
	vecHitFloating[0].Power = 0.7f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Side_Attack", FullName, &vecHitFloating[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Boss_Side_Attack in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Side_Attack", pEffect, 6)))
	{
		MSGBOX("Falild to Add_Effect_Boss_Side_Attack in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//BrokenPod
	vecHitParticle.clear();
	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Broken_Pod.dat");

	FullName = L"Proto_GameObject_Effect_Explosion";
	vecHitParticle[0].ParticleColor = { 1.f, 0.8f, 0.4f,1.f };
	vecHitParticle[0].Power = 2.5f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Broken_Pod", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Broken_Pod in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Broken_Pod", pEffect, 7)))
	{
		MSGBOX("Falild to Add_Effect_Broken_Pod in CStage1::Ready_Effect()");
		return E_FAIL;
	}

#pragma endregion

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
