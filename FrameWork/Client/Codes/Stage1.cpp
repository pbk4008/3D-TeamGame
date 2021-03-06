#include "pch.h"
#include "Loading.h"
#include "Stage1.h"
#include "Environment.h"
#include "SubEnvironment.h"

#include "Boss_Bastion_Judicator.h"

#include "Effect_DashDust.h"
#include "Effect_HitParticle.h"
#include "Effect_HitFloating.h"
#include "Effect_Floating_Speed.h"
#include "Effect_DeathParticle.h"
#include "Effect_Env_Floating.h"
#include "Effect_Env_Fire.h"
#include "Effect_Guard.h"
#include "Effect_Falling_Leaf.h"
#include "Effect_FloatingUp.h"
#include "Effect_Hammer_Dust.h"
#include "Effect_Dead_Spray.h"
#include "Effect_Energy.h"
#include "Explosion_Rock.h"


#include "UI_Ingame.h"
#include "UI_Player_HpBar.h"
#include "UI_HpHeal_Num.h"
#include "UI_Shield_Meter.h"
#include "UI_Blank_CKey.h"
#include "UI_Blank_FKey.h"
#include "UI_Fill_CKey.h"
#include "UI_Indicator.h"
#include "UI_Fill_Space.h"
#include "UI_Blank_Space.h"
#include "UI_Skill_Meter_Back.h"
#include "UI_Skill_Meter_Gauge.h"
#include "UI_Skill_Meter_Gauge_Right.h"

#include "JumpNode.h"
#include "JumpBox.h"

#include "Meteor.h"
#include <Monster_Crawler.h>
#include <Monster_EarthAberrant.h>
#include <Monster_Bastion_Healer.h>
#include <Monster_Bastion_2HSword.h>
#include <Monster_Bastion_Sword.h>
#include <Monster_Bastion_Shooter.h>
#include "Monster_Bastion_Spear.h"
#include "Monster_BronzeAnimus.h"

#include "InteractManager.h"
#include "DropManager.h"
#include "ScenematicManager.h"

#include "Indicator_Manager.h"
#include "WeaponGenerator.h"
#include "Loot_Shield.h"

#include "Wall.h"
#include "Potal.h"
#include "DropBoxData.h"
#include "Pot.h"

//Cinema
#include "Cinema1_1.h"
#include "Cinema1_2.h"
#include "Cinema2_1.h"
#include "Cinema2_2.h"
#include "Cinema2_3.h"
#include "Cinema2_4.h"
#include "Cinema3_1.h"
#include "Cinema3_2.h"
#include "Cinema3_3.h"
#include "Cinema3_4.h"
#include "Cinema3_5.h"
#include "MeshEffect_Razer.h"
#include "DamageFont.h"
#include "CapsuleObstacle.h"

CStage1::CStage1()
	: m_pTriggerSystem(nullptr)
	, m_bDebug(false)
	, m_iCountMonster(0)
	, m_bFirst(false)
	, m_pScenemaManager(nullptr)
	, m_fAccMeteorSpawn(0.f)
	, m_fRandomMeteorSpawnTime(0.f)
	, m_fAccMeteorStartTime(0.f)
	, m_iPortalCount(0)
	, m_bPortalClear(false)
{
}

CStage1::CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
	, m_pTriggerSystem(nullptr)
	, m_bDebug(false)
	, m_iCountMonster(0)
	, m_bFirst(false)
	, m_pScenemaManager(nullptr)
	, m_fAccMeteorSpawn(0.f)
	, m_fRandomMeteorSpawnTime(0.f)
	, m_fAccMeteorStartTime(0.f)
	, m_iPortalCount(0)
	, m_bPortalClear(false)
{
}

HRESULT CStage1::NativeConstruct()
{
	//?????? ?????? ?????????? true, ???????? ?????????? false (?????????? ?????? ?? ??????????)

	m_bDebug = false;//false?? ?????? ?????? ?????? ?????? ??????????.
#ifndef _DEBUG 
	m_bDebug = false;
#endif // DEBUG

	g_pWeaponGenerator = CWeaponGenerator::GetInstance();

	if (FAILED(CLevel::NativeConstruct()))
	{
		MSGBOX("Stage1 Level");
		return E_FAIL;
	}

	if (FAILED(Ready_Light()))
	{
		MSGBOX("Stage1 Light");
		return E_FAIL;
	}

	if (FAILED(Ready_Trigger_Jump()))
	{
		MSGBOX("Stage1 Jump");
		return E_FAIL;
	}

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
	{
		MSGBOX("Stage1 Player");
		return E_FAIL;
	}

	if (FAILED(Ready_MapObject()))
	{
		MSGBOX("Stage1 MapObject");
		return E_FAIL;
	}

	if (FAILED(Ready_TriggerSystem(L"../bin/SaveData/Trigger/MonsterSpawnTrigger.dat")))
	{
		MSGBOX("Stage1 Trigger");
		return E_FAIL;
	}

	if (FAILED(Ready_Data_UI(L"../bin/SaveData/UI/UI.dat")))
	{
		MSGBOX("Stage1 UI Data");
		return E_FAIL;
	}

	if (FAILED(Ready_Data_Effect()))
	{
		MSGBOX("Stage1 Effect");
		return E_FAIL;
	}

	if (FAILED(Ready_UI(L"Layer_UI")))
	{
		MSGBOX("Stage1 Ui");
		return E_FAIL;
	}

	if (FAILED(Ready_Treasure_Chest()))
	{
		MSGBOX("Stage1 Box");
		return E_FAIL;
	}

	if (FAILED(Ready_GameManager()))
	{
		MSGBOX("Stage1 Manager");
		return E_FAIL;
	}

	g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");

	if (FAILED(Ready_Meteor()))
	{
		MSGBOX("Meteor");
		return E_FAIL;
	}

	if (FAILED(Ready_Indicator()))
	{
		MSGBOX("Indicator");
		return E_FAIL;
	}

	if (FAILED(Ready_Portal()))
	{
		MSGBOX("Portal");
		return E_FAIL;
	}

	if (FAILED(Ready_Wall()))
	{
		MSGBOX("Wall");
		return E_FAIL;
	}

	if (FAILED(Ready_Pot()))
		return E_FAIL;

	////// ?????? ?? ?????? ?????? ?? ???? ?????????? ?????????? ?????????? ???? 
	if (FAILED(Ready_Cinema()))
	{
		MSGBOX("Cinema");
		return E_FAIL;
	}

	//g_pGameInstance->PlayBGM(L"Stage1_BGM");

	m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA1_1);

	if (FAILED(Ready_Obstacle()))
		return E_FAIL;
	 
	return S_OK;
}

_int CStage1::Tick(_double TimeDelta)
{	
	//_float3 fPos = { 0.f,5.f,20.f };

	//m_pPot->Tick(TimeDelta);
	/*_vector vTmp = g_pObserver->Get_PlayerPos();
	cout << XMVectorGetX(vTmp) << ", " << XMVectorGetY(vTmp) << ", " << XMVectorGetZ(vTmp) << endl;*/

	//cout << "monster count : " << m_iCountMonster << endl;

	if (m_pIndicatorManager)
		m_pIndicatorManager->Active_Indicator();

#ifdef  _DEBUG
	_int iLevel = 0;
	if (g_pDebugSystem->Get_LevelMoveCheck(iLevel))
	{
		CLoading* pLoading = CLoading::Create(m_pDevice, m_pDeviceContext, (SCENEID)iLevel);
		if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, pLoading)))
			return -1;
		g_pDebugSystem->Set_LevelcMoveCheck(false);
		m_pTriggerSystem = nullptr;
		g_pDropManager = nullptr;
		return 0;
	}
#endif //  _DEBUG

	CheckTriggerForQuest();

	g_pInvenUIManager->Tick(TimeDelta);

	if (g_pGameInstance->getkeyDown(DIK_I))
	{
		if (g_pInvenUIManager->IsOpenModal())
		{
			g_pInvenUIManager->CloseModal();
			g_pMainApp->Set_DeltaTimeZero(false);
		}
		else
		{
			SHOW_GUIDE();
			PLAY_SOUND(L"UI_InvenOpen", CHANNEL::EFFECT);
			g_pInvenUIManager->OpenModal();
			g_pMainApp->Set_DeltaTimeZero(true);
		}
	}

	if (nullptr != m_pTriggerSystem)
	{
		if (g_pGameInstance->getkeyDown(DIK_BACKSPACE))
		{
			m_pTriggerSystem->CurrentTriggerMonsterAllDelete();
			m_iCountMonster = 0;
		}

		m_pTriggerSystem->Tick(TimeDelta);

		if (m_iCountMonster > 10000)
			m_iCountMonster = 0;

		if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 2)
			Portal_Spot1();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 4)
			Portal_Spot2();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 5)
			Portal_Spot3();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 6)
			Portal_Spot4();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 7)
			Portal_Spot5();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 9)
			Portal_Spot6();
		else
		{
			if (m_iCountMonster == 0&&m_pTriggerSystem->Get_CurrentTriggerNumber() != -1)
			{
				CLEAR_QUEST(L"T_HUD_KillAllMonster");
				m_pTriggerSystem->Check_Clear();
			}
		}

		CBoss_Bastion_Judicator* pBoss = (CBoss_Bastion_Judicator*)g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Boss")->front();
		if (nullptr != pBoss)
		{
			if (pBoss->Get_HpRatio() <= 0.3f && !m_bBossClear)
			{
				m_bBossClear = true;
				m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA3_1);
			}
				
			/*if (true == pBoss->Get_Dead())
			{
				if (!m_bBossClear)
				{
					m_bBossClear = true;
					m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA3_1);
				}
			}*/
		}
	}

#pragma region Using Debug
	_float3 fPos = { 0.f,5.f,20.f };

	////////////// ?????? ??????
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD0))
	//{
	//	CMeshEffect_Razer* pRazer = nullptr;
	//	CGameObject* pPlayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Silvermane")->front();
	//	XMStoreFloat3(&fPos, pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION) - _vector{ 0.f, 2.f, 0.f, 0.f });
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_MeshEffect_Razer", &fPos, (CGameObject**)&pRazer)))
	//		return -1;
	//	//pRazer->setActive(true);
	//}
	// boss
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD0))
	//{
	//	CBoss_Bastion_Judicator* pMidBoss = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Boss_Bastion", &fPos, (CGameObject**)&pMidBoss)))
	//		return -1;
	//	pMidBoss->setActive(true);
	//}
	// monster
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD0))
	//{
	//	CMonster_Crawler* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Monster_Crawler", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//	pMonster->setActive(true);
	//}
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD1))
	//{
	//	CMonster_EarthAberrant* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Monster_EarthAberrant", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//	pMonster->setActive(true);
	//}
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD2))
	//{
	//	CMonster_Bastion_Sword* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Monster_Bastion_Sword", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//	pMonster->setActive(true);
	//}
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD3))
	//{
	//	CMonster_Bastion_Shooter* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Monster_Bastion_Shooter", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//	pMonster->setActive(true);
	//}

	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD4))
	//{
	//	CMonster_Bastion_Healer* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Monster_Bastion_Healer", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//	pMonster->setActive(true);
	//}
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD5))
	//{
	//	CMonster_Bastion_2HSword* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Monster_Bastion_2HSword", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//	pMonster->setActive(true);
	//}
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD6))
	//{
	//	CMonster_Bastion_Spear* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Monster_Bastion_Spear", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//	pMonster->setActive(true);
	//}
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD7))
	//{
	//	CBoss_Bastion_Judicator* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Boss_Bastion", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//}
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD8))
	//{
	//	CMonster_BronzeAnimus* pMonster = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Monster_BronzeAnimus", &fPos, (CGameObject**)&pMonster)))
	//		return -1;
	//	pMonster->setActive(true);
	//}
#pragma endregion

	if(g_pDropManager)
		g_pDropManager->Tick();

	if (g_pInteractManager)
		g_pInteractManager->Tick(TimeDelta);

	if(m_pIndicatorManager)
		m_pIndicatorManager->Active_Indicator();

	if (m_pScenemaManager)
	{
		if (g_pGameInstance->getkeyDown(DIK_N))
			m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA2_4);;

		m_pScenemaManager->Tick(TimeDelta);
	}

	/*for Meteor*/
	m_fAccMeteorStartTime += (_float)TimeDelta;
	if (m_fAccMeteorStartTime > 15.f)
		Shoot_Meteor(TimeDelta);

	if(g_pQuestManager)
		g_pQuestManager->Tick(g_dImmutableTime);
	
	if (g_pGuideManager)
		g_pGuideManager->Tick(g_dImmutableTime);

	if (g_pVoiceManager)
		g_pVoiceManager->Tick(TimeDelta);

	Open_Wall();


	return _int();
}

_int CStage1::LateTick(_double TimeDelta)
{

	if (m_pScenemaManager)
	{
		_uint iProgress = m_pScenemaManager->LateTick(TimeDelta);
		if (iProgress == 1)
			return 0;
	}

	if(g_pQuestManager)
		g_pQuestManager->Late_Tick(TimeDelta);

	if (g_pGuideManager)
		g_pGuideManager->Late_Tick(g_dImmutableTime);

	if (g_pVoiceManager)
		g_pVoiceManager->Late_Tick(TimeDelta);

	return _int();
}

HRESULT CStage1::Render()
{
#ifdef _DEBUG
	if (nullptr != m_pTriggerSystem)
		m_pTriggerSystem->Render();
#endif

	if(g_pQuestManager)
		g_pQuestManager->Render();

	if (g_pGuideManager)
		g_pGuideManager->Render();

	return S_OK;
}

HRESULT CStage1::Ready_MapObject()
{
	vector<ENVIRONMENTLOADDATA> vecEnvironmentData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecEnvironmentData, L"../bin/SaveData/Stage_1.dat")))
		return E_FAIL;

	vector<CEnvironment::ENVIRONMENTDESC> tEnvironmentDesc;
	tEnvironmentDesc.resize(1000);
	_uint iIndex = 0;
	tEnvironmentDesc[iIndex].wstrInstaneTag = vecEnvironmentData[0].FileName;
	for (auto& pData : vecEnvironmentData)
	{
		if (lstrcmp(tEnvironmentDesc[iIndex].wstrInstaneTag.c_str(), pData.FileName))
		{
			iIndex++;
			tEnvironmentDesc[iIndex].wstrInstaneTag = pData.FileName;
			tEnvironmentDesc[iIndex].tInstanceDesc.vecMatrix.emplace_back(pData.WorldMat);
		}
		else
			tEnvironmentDesc[iIndex].tInstanceDesc.vecMatrix.emplace_back(pData.WorldMat);
	}
	_uint iTmpIndx = 0;
	for (auto& pDesc : tEnvironmentDesc)
	{
		if (pDesc.wstrInstaneTag == L"PavementDecor_01_Lod0.fbx")
		{
			for (auto pMatrix : pDesc.tInstanceDesc.vecMatrix)
			{
				if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Floor", L"Proto_GameObject_Circle_Floor", &pMatrix)))
					MSGBOX("Failed To Clone Circle_Floor");
			}
		}
		else if (pDesc.wstrInstaneTag == L"Brazier_Large_01.fbx")
		{
			for (auto& iter : pDesc.tInstanceDesc.vecMatrix)
			{
				CEffect_Env_Fire::EFFECTDESC Desc;
				_tcscpy_s(Desc.TextureTag, L"Env_Fire");
				Desc.iRenderPassNum = 1;
				Desc.iImageCountX = 8;
				Desc.iImageCountY = 8;
				Desc.fFrame = 64.f;
				Desc.fEffectPlaySpeed = 1.f;
				Desc.ParticleMat = XMLoadFloat4x4(&iter);
				Desc.bUsingGravity = true;
				Desc.IDTag = 3;

				if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_NoisFire", L"Proto_GameObject_Effect_Env_Fire", &Desc)))
					MSGBOX("Failed To Clone NoisFire");

				//fire smoke
				ZeroMemory(&Desc, sizeof(Desc));
				_tcscpy_s(Desc.TextureTag, L"fx_smoke_a");
				Desc.iRenderPassNum = 1;
				Desc.iImageCountX = 5;
				Desc.iImageCountY = 6;
				Desc.fFrame = 30.f;
				Desc.fEffectPlaySpeed = 1.f;
				Desc.ParticleMat = XMLoadFloat4x4(&iter);
				Desc.ParticleMat.r[3] = XMVectorSetY(Desc.ParticleMat.r[3], XMVectorGetY(Desc.ParticleMat.r[3]) + 0.5f);
				Desc.bUsingGravity = true;
				Desc.IDTag = 3;

				if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Fire_Smoke", L"Proto_GameObject_Effect_Env_Fire", &Desc)))
				{
					MSGBOX("Failed to Creating Effect_Env_Fire_Smoke in CStage1::Ready_Effect()");
					return E_FAIL;
				}
			}
		}

		if (pDesc.wstrInstaneTag == L"")
			break;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Environment", L"Proto_GameObject_Environment", &pDesc)))
			return E_FAIL;
	}

	//------------------------------------------- Tree --------------------------------------------------------------------//

	vector<ENVIRONMENTLOADDATA> vecSubEnvData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecSubEnvData, L"../bin/SaveData/Tree_Data.dat")))	
		return E_FAIL;

	vector<CSubEnvironment::ENVIRONMENTDESC> tSubEnvDesc;
	tSubEnvDesc.resize(100);
	_uint idx = 0;
	tSubEnvDesc[idx].wstrInstaneTag = vecSubEnvData[0].FileName;
	for (auto& pData : vecSubEnvData)
	{
		if (lstrcmp(tSubEnvDesc[idx].wstrInstaneTag.c_str(), pData.FileName))
		{
			idx++;
			tSubEnvDesc[idx].wstrInstaneTag = pData.FileName;
			tSubEnvDesc[idx].tInstanceDesc.vecMatrix.emplace_back(pData.WorldMat);
		}
		else
			tSubEnvDesc[idx].tInstanceDesc.vecMatrix.emplace_back(pData.WorldMat);
	}

	for (auto& pDesc : tSubEnvDesc)
	{
		if (pDesc.wstrInstaneTag == L"") 
			break;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_SubEnvironment", L"Proto_GameObject_SubEnvironment", &pDesc))) 
			return E_FAIL;
	}

	//wstring strTag = L"StageBackGround";
	//g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Stage1_Back", L"Prototype_GameObject_BackGround", &strTag);

	return S_OK;
}

HRESULT CStage1::Ready_Camera(const _tchar* LayerTag)
{
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Camera_Silvermane")))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CStage1::Ready_Player(const _tchar* LayerTag)
{
	//// ????????
	wstring wstrNaviFile = L"../Data/NavMesh/Stage_1_Nav.dat";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Plane", L"Proto_GameObject_Plane_Test",&wstrNaviFile)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;


	////Test
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Test", L"Proto_GameObject_TestObject")))
	//	return E_FAIL;
	// 
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Test", L"Proto_GameObject_MeshEffect_Test2")))
	//	MSGBOX(L"???? ?????? ??????2 ???? ????");

	//CDamageFont::DESC tDamageDesc;
	//tDamageDesc.vPos = { 0.f, 2.f, 0.f };
	//tDamageDesc.fDamage = 108;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_DamageFont", L"Proto_GameObject_DamageFont", &tDamageDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Boss(const _tchar* LayerTag)
{
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Boss_Bastion")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Monster(const _tchar* LayerTag)
{
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_Crawler")))
	//	return E_FAIL;
	//
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_EarthAberrant")))
	//	return E_FAIL;
	//
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_Bastion_2HSword")))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_Bastion_Healer")))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_Bastion_Sword")))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_Bastion_Shooter")))
	//	return E_FAIL;
	 
	return S_OK;
}

HRESULT CStage1::Ready_UI(const _tchar* LayerTag)
{
	//Player HpBar Green
	CUI_Player_HpBar::UIDESC Desc;
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar");
	Desc.bMinus = true;
	Desc.fAngle = 0.35f;
	Desc.fPos = { 333.f, 912.f, 0.f };
	Desc.fSize = { 265.f , 38.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI_Green", L"Proto_GameObject_UI_Player_HpBar", &Desc)))
		return E_FAIL;

	//Player HpBar Red
	ZeroMemory(&Desc, sizeof(CUI_Player_HpBar::UIDESC));
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar_Red");
	Desc.bMinus = true;
	Desc.fAngle = 0.35f;
	Desc.fPos = { 333.f, 912.f, 0.f };
	Desc.fSize = { 265.f , 38.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Player_HpBar_Red", &Desc)))
		return E_FAIL;

	//Player HpHeal Num
	CUI_HpHeal_Num::UIACTIVEDESC Desc0;
	ZeroMemory(&Desc0, sizeof(CUI_HpHeal_Num::UIACTIVEDESC));
	_tcscpy_s(Desc0.TextureTag, L"Texture_Monster_LevelNum");
	Desc0.bMinus = false;
	Desc0.fAngle = 0.f;
	Desc0.fPos = { 197.f, 911.f, 0.08f };
	Desc0.fSize = { 50.f , 50.f };
	Desc0.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_HpHeal_Num", &Desc0)))
		return E_FAIL;

	//Shield_Meter
	CUI_Shield_Meter::UIACTIVEDESC Desc1;
	ZeroMemory(&Desc1, sizeof(CUI_Shield_Meter::UIACTIVEDESC));
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Shield_Meter");
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 150.f, 920.f, 0.06f };
	Desc1.UIDesc.fSize = { 60.f, 60.f };
	Desc1.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Shield_Meter", &Desc1)))
		return E_FAIL;

	//Player Skill_Meter_Back
	CUI_Skill_Meter_Back::UIDESC DescBack;
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Back");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1600.f, 880.f, 0.1f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Back", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Back2
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Back");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1710.f, 880.f, 0.1f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Back", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Gauge
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Gauge_Full");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1600.f, 880.f, 0.08f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Gauge", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Gauge
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Gauge_Fill");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1710.f, 882.f, 0.08f };
	DescBack.fSize = { 102.f , 17.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Gauge_Right", &DescBack)))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Gauge_Right", &DescBack)))
		return E_FAIL;

	//Blank_Ckey
	CUI_Blank_CKey::UIACTIVEDESC Desc3;
	ZeroMemory(&Desc3, sizeof(CUI_Blank_CKey::UIACTIVEDESC));
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Fill_Ckey");
	Desc3.UIDesc.bMinus = false;
	Desc3.UIDesc.fAngle = 0.f;
	Desc3.UIDesc.fPos = { 1000.f, 570.f, 0.1f };
	Desc3.UIDesc.fSize = { 50.f , 50.f };
	Desc3.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI_BlankC", L"Proto_GameObject_UI_Blank_CKey", &Desc3)))
		return E_FAIL;

	//Fill_Ckey
	CUI_Fill_Ckey::UIACTIVEDESC Desc4;
	ZeroMemory(&Desc4, sizeof(CUI_Fill_Ckey::UIACTIVEDESC));
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Blank_Ckey");
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 1000.f, 570.f, 0.09f };
	Desc4.UIDesc.fSize = { 50.f , 50.f };
	Desc4.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI_FillC", L"Proto_GameObject_UI_Fill_CKey", &Desc4)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CStage1::Ready_Light()
{
	g_pGameInstance->RemoveLight();

	LIGHTDESC			LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(-1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	/*LightDesc.vPosition = _float3(-132.f, 87.f, 396.f);*/
	LightDesc.bactive = true;

	LightDesc.mOrthinfo[0] = 50.f;

	if (FAILED(g_pGameInstance->CreateLightCam(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Creating DirectionLight Cam");

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.fRange = 10.f;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.bactive = true;
	LightDesc.vPosition = _float3(2.f, 15.f, 110.f);

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Adding PointLight");

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.fRange = 7.f;
	LightDesc.vDiffuse = _float4(1.0f, 0.34509f, 0.1333f, 1.f);
	LightDesc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.bactive = true;
	LightDesc.vPosition = _float3(-36.9665833f, -4.63226175f, 86.0816040f);

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Adding PointLight");

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_GameManager(void)
{
	g_pDropManager = CDropManager::GetInstance();
	if (FAILED(g_pDropManager->NativeConstruct((SCENEID::SCENE_STAGE1))))
		return E_FAIL;

	m_pIndicatorManager = GET_INSTANCE(CIndicator_Manager);
	m_pScenemaManager = GET_INSTANCE(CScenematicManager);

	g_pVoiceManager = CVoiceManager::GetInstance();
	if (FAILED(g_pVoiceManager->NativeConstruct(SCENEID::SCENE_STAGE1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Data_Effect()
{
#pragma region ?????????????? ??????????????
	////??????????????????????
	//Env floating
	wstring FullName = L"";

	vector<CEffect_Env_Floating::EFFECTDESC> vecEnvFloating;
	g_pGameInstance->LoadFile<CEffect_Env_Floating::EFFECTDESC>(vecEnvFloating, L"../bin/SaveData/Effect/Effect_Env_Floating_2.dat");
	FullName = L"Proto_GameObject_Effect_Env_Floating";
	vecEnvFloating[0].fMyPos = { -5.f, 1.f, 20.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Floating_1", FullName, &vecEnvFloating[0])))
	{
		MSGBOX("Failed to Creating Effect_Env_Floating_1 in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	//envFloating
	vecEnvFloating[0].fMyPos = { 17.f,4.f, 100.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -68.f,3.f, 110.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -68.f,5.f, 125.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -68.f,8.f, 140.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -110.f,15.f, 210.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -140.f,19.f, 210.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -135.f,22.f, 280.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -145.f,23.f, 300.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -185.f,40.f, 320.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;
	//envFloating
	vecEnvFloating[0].fMyPos = { -182.f,55.f, 380.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFloating[0])))
		return E_FAIL;


	//Env Falling Leaf
	vector<CEffect_Falling_Leaf::FLOATINGLEAFDESC> vecEnvFallingLeaf;
	g_pGameInstance->LoadFile<CEffect_Falling_Leaf::FLOATINGLEAFDESC>(vecEnvFallingLeaf, L"../bin/SaveData/Effect/Effect_Falling_Leaf.dat");
	vecEnvFallingLeaf[0].fMyPos = { -7.f, -10.0f, 3.f, 1.f };
	vecEnvFallingLeaf[0].fRespawnPosY = -20.f;
	vecEnvFallingLeaf[0].bSmall = false;
	FullName = L"Proto_GameObject_Effect_Falling_Leaf";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
	{
		MSGBOX("Failed to Creating Effect_Env_FallingLeaf in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -7.f, -10.0f, 30.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { 8.f, -10.0f, 50.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { 11.f, 0.0f, 100.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -36.f, 0.0f, 76.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -50.f, -16.0f, 73.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -66.f, 0.0f, 78.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -71.f, 10.0f, 180.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -160.f, 18.0f, 204.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -146.f, 16.0f, 254.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -138.f, 17.0f, 278.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -187.f, 45.0f, 390.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
	//Leaf
	vecEnvFallingLeaf[0].fMyPos = { -187.f, 45.0f, 414.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Falling_Leaf", FullName, &vecEnvFallingLeaf[0])))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CStage1::Ready_Data_UI(const _tchar* pDataFilePath)
{
	//UI_Ingame_Static
	vector<CUI::UIDESC> vecUI;
	g_pGameInstance->LoadFile<CUI::UIDESC>(vecUI, pDataFilePath);

	for (int i = 0; i < vecUI.size(); ++i)
	{
		wstring Tag = vecUI[i].TextureTag;
		wstring FullName = L"Proto_GameObject_UI_" + Tag;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", FullName, &vecUI[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_UI()");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_Indicator()
{
	//monster Indicator
	if (FAILED(m_pIndicatorManager->Add_Indicator((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI_Indicator", 30)))
	{
		MSGBOX("Failed to Create Indicator");
		return E_FAIL;
	}
	

	return S_OK;
}

HRESULT CStage1::Ready_TriggerSystem(const _tchar* pTriggerFile)
{
	m_pTriggerSystem = CTriggerSystem<Client::CStage1>::Create(m_pDevice, m_pDeviceContext, pTriggerFile, this);

	if (!m_pTriggerSystem)
		return E_FAIL;

	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE1, CTriggerSystem<CStage1>::MONSTER::MON_SWORD, L"../bin/SaveData/MonsterSpawn1Stage/Sword.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE1, CTriggerSystem<CStage1>::MONSTER::MON_2H, L"../bin/SaveData/MonsterSpawn1Stage/2H.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE1, CTriggerSystem<CStage1>::MONSTER::MON_CRYSTAL, L"../bin/SaveData/MonsterSpawn1Stage/Crystal.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE1, CTriggerSystem<CStage1>::MONSTER::MON_EARTH, L"../bin/SaveData/MonsterSpawn1Stage/Earth.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE1, CTriggerSystem<CStage1>::MONSTER::MON_HEAL, L"../bin/SaveData/MonsterSpawn1Stage/Healer.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE1, CTriggerSystem<CStage1>::MONSTER::MON_SHOOTER, L"../bin/SaveData/MonsterSpawn1Stage/Shooter.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE1, CTriggerSystem<CStage1>::MONSTER::MON_MID, L"../bin/SaveData/MonsterSpawn1Stage/MidBoss.dat")))
		return E_FAIL;


	if (FAILED(Ready_TriggerFunctionSetting()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_TriggerFunctionSetting()
{
	void(Client::CStage1:: * fp)();

	//1??Trigger?? ???? ????
	fp = &CStage1::Trgger_Function1;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function2;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function3;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function4;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function5;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function7;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function8;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function9;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function10;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_Function11;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage1::Trgger_FunctionBoss;;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	return S_OK;
}

HRESULT CStage1::Ready_Cinema()
{
	if (!m_pScenemaManager)
		return E_FAIL;

	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema1_1::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema1_2::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema2_1::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema2_2::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema2_3::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema2_4::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema3_1::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema3_2::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema3_3::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema3_4::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;
	if (FAILED(m_pScenemaManager->Add_Scenema(CCinema3_5::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Wall()
{
	CWall::WALLDESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.pos = _float4(-61.f, 18.f, 194.f, 1.f);
	desc.scale = _float2(6.f, 10.f);
	desc.radian = 0.f;
	desc.color = _float4(0.f, 0.f, 1.f, 1.f);
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Wall", L"Proto_GameObject_Wall",&desc))) return E_FAIL;

	ZeroMemory(&desc, sizeof(desc));
	desc.pos = _float4(-91.f, 25.f, 218.f, 1.f);
	desc.scale = _float2(10.f, 24.f);
	desc.radian = 90.f;
	desc.color = _float4(0.f, 0.f, 1.f, 1.f);
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Wall", L"Proto_GameObject_Wall", &desc))) return E_FAIL;
	
	ZeroMemory(&desc, sizeof(desc));
	desc.pos = _float4(-137.f, 19.f, 224.f, 1.f);
	desc.scale = _float2(10.f, 24.f);
	desc.radian = 0.f;
	desc.color = _float4(0.f, 0.f, 1.f, 1.f);
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Wall", L"Proto_GameObject_Wall", &desc))) return E_FAIL;

	ZeroMemory(&desc, sizeof(desc));
	desc.pos = _float4(-175.f, 47.f, 373.f, 1.f);
	desc.scale = _float2(12.f, 24.f);
	desc.radian = 0.f;
	desc.color = _float4(0.f, 0.f, 1.f, 1.f);
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Wall", L"Proto_GameObject_Wall", &desc))) return E_FAIL;

	ZeroMemory(&desc, sizeof(desc));
	desc.pos = _float4(-176.f, 51.f, 400.f, 1.f);
	desc.scale = _float2(10.f, 24.f);
	desc.radian = 0.f;
	desc.color = _float4(0.f, 0.f, 1.f, 1.f);
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Wall", L"Proto_GameObject_Wall", &desc))) return E_FAIL;
	return S_OK;
}

HRESULT CStage1::Ready_Pot()
{
	vector<ENVIRONMENTLOADDATA> vecEnvironmentData;
	g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecEnvironmentData, L"../bin/SaveData/Stage1_Pot.dat");

	for (auto& pData : vecEnvironmentData)
	{
		CPot::POTDESC tDesc;
		ZeroMemory(&tDesc, sizeof(tDesc));
		if (!lstrcmp(pData.FileName, L"S_Moon_Urn_1_03.fbx"))
			tDesc.iType = 0;
		if (!lstrcmp(pData.FileName, L"S_Sun_Urns_01.fbx"))
			tDesc.iType = 1;
		if (!lstrcmp(pData.FileName, L"S_Sun_Urns_Set02.fbx"))
			tDesc.iType = 2;

		tDesc.matWorld = pData.WorldMat;
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Pot", L"Proto_GameObject_Pot", &tDesc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CStage1::Ready_Obstacle()
{
	CCapsuleObstacle::DESC tObstacleDesc;
	tObstacleDesc.vPosition = { -36.9665833f, -4.63226175f, 86.0816040f };
	tObstacleDesc.fHeight = 1.f;
	tObstacleDesc.fRadius = 0.5f;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_CapsuleObstacle", &tObstacleDesc)))
	{
		MSGBOX(L"????????1 ???? ?????? ???? ????");
		return E_FAIL;
	}



	tObstacleDesc.vPosition = { -175.753589f, 28.702583f, 308.261066f};
	tObstacleDesc.fHeight = 2.f;
	tObstacleDesc.fRadius = 1.7f;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_CapsuleObstacle", &tObstacleDesc)))
	{
		MSGBOX(L"????????1 ???? ?????? ???? ????");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CStage1::Ready_Portal()
{
	for (_uint i = 0; i < 59; i++)
	{
		if (i <20)//20
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Sword")))
			{
				return E_FAIL;
				MSGBOX("Failed to Create Portal");
			}
		}
		else if(i>=20 && i<37)//17
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Shooter")))
			{
				return E_FAIL;
				MSGBOX("Failed to Create Portal");
			}
		}
		else if(i>=37 && i<50)//13??
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Healer")))
			{
				return E_FAIL;
				MSGBOX("Failed to Create Portal"); 
			}
		}
		else if (i >= 50 && i <55)//5??
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Crawler")))
			{
				return E_FAIL;
				MSGBOX("Failed to Create Portal");
			}
		}
		else if (i >=55 && i <59)//4??
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_EarthAberrant")))
			{
				return E_FAIL;
				MSGBOX("Failed to Create Portal"); 
			}
		}
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Portal", L"Proto_GameObject_Portal")))
		{
			return E_FAIL;
			MSGBOX("Failed to Create Portal");
		}
	}

	return S_OK;
}
void CStage1::Open_Potal(_fvector vPos, _uint iMonTag)
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Portal");
	if (!pLayer)
		return;

	for (auto& pObj : *pLayer)
	{
		CPotal* pPotal = nullptr;
		if (!pObj->getActive())
		{
			pPotal = static_cast<CPotal*>(pObj);
			pPotal->Open_Potal(iMonTag, vPos);
			pPotal->setActive(true);
			break;
		}
	}
}
void CStage1::CheckTriggerForQuest(void)
{
}

void CStage1::Portal_Spot1()
{
	if (m_iCountMonster <= 2 && m_iPortalCount == 0)
	{
		m_iPortalCount = 1;
		Open_Potal(XMVectorSet(-46.f, -3.f, 79.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-33.f, -3.f, 87.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-37.f, -3.f, 83.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster <= 3 && m_iPortalCount == 1)
	{
		m_iPortalCount = 2;
		Open_Potal(XMVectorSet(-46.f, -3.f, 79.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-46.f, -3.f, 79.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 2;
	}
	else if (m_iCountMonster <= 0 && m_iPortalCount == 2)
		m_pTriggerSystem->Check_Clear();
}

void CStage1::Portal_Spot2()
{
	if (m_iCountMonster <= 5 && m_iPortalCount == 2)
	{
		m_iPortalCount = 3;
		Open_Potal(XMVectorSet(-84.f, 15.f, 215.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-78.f, 15.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-70.f, 15.f, 228.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-61.f, 15.f, 227.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-60.f, 15.f, 217.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 5;
	}
	else if (m_iCountMonster <= 5 && m_iPortalCount == 3)
	{
		m_iPortalCount = 4;
		Open_Potal(XMVectorSet(-81.f, 15.f, 208.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-74.f, 15.f, 207.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-70.f, 15.f, 202.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-73.f, 15.f, 205.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		m_iCountMonster += 4;
	}
	else if (m_iCountMonster <= 3 && m_iPortalCount == 4)
	{
		m_iPortalCount = 5;
		Open_Potal(XMVectorSet(-76.f, 15.f, 206.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-67.f, 15.f, 202.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-57.f, 15.f, 224.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-67.f, 15.f, 230.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-54.f, 15.f, 224.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-56.f, 15.f, 234.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 6;
	}
	else if (m_iCountMonster <= 5 && m_iPortalCount == 5)
	{
		m_iPortalCount = 6;
		Open_Potal(XMVectorSet(-59.f, 15.f, 223.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-71.f, 15.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-76.f, 15.f, 233.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster <= 0 && m_iPortalCount == 6)
	{
		CLEAR_QUEST(L"T_HUD_KillAllMonster");
		STOP_SOUND(CHANNEL::BATTLE);
		VOLUME_CHANGE(CHANNEL::BGM, 1.0f);
		m_pTriggerSystem->Check_Clear();
	}
}

void CStage1::Portal_Spot3()
{
	if (m_iPortalCount == 6 && m_iCountMonster <= 1)
	{
		m_iPortalCount = 7;
		Open_Potal(XMVectorSet(-131.f, 19.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-130.f, 19.f, 214.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-133.f, 19.f, 219.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 3;
	}
	else if (m_iPortalCount == 7 && m_iCountMonster <= 2)
	{
		m_iPortalCount = 8;
		Open_Potal(XMVectorSet(-141.f, 19.f, 214.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-139.f, 19.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-133.f, 19.f, 220.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-129.f, 19.f, 214.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 4;
	}
	else if (m_iPortalCount == 8 && m_iCountMonster <= 0)
	{
		CLEAR_QUEST(L"T_HUD_KillAllMonster");
		m_pTriggerSystem->Check_Clear();
	}
}

void CStage1::Portal_Spot4()
{
	if (m_iPortalCount == 8 && m_iCountMonster <= 3)
	{
		m_iPortalCount = 9;
		Open_Potal(XMVectorSet(-130.f, 18.f, 255.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-139.f, 18.f, 255.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-126.f, 18.f, 267.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-135.f, 18.f, 273.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster +=4;
	}
	else if (m_iPortalCount == 9 && m_iCountMonster <= 4)
	{
		m_iPortalCount = 10;
		Open_Potal(XMVectorSet(-131.f, 18.f, 256.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-141.f, 18.f, 258.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-136.f, 18.f, 261.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-125.f, 18.f, 271.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-139.f, 18.f, 274.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 5;
	}
	else if (m_iPortalCount == 10 && m_iCountMonster <= 0)
	{
		CLEAR_QUEST(L"T_HUD_KillAllMonster");
		m_pTriggerSystem->Check_Clear();
		STOP_SOUND(CHANNEL::BATTLE);
		VOLUME_CHANGE(CHANNEL::BGM, 1.0f);
	}
}

void CStage1::Portal_Spot5()
{
	if (m_iPortalCount == 10 && m_iCountMonster <= 3)
	{
		m_iPortalCount = 11;
		Open_Potal(XMVectorSet(-178.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-178.f, 29.f, 309.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-171.f, 29.f, 310.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-171.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-165.f, 29.f, 309.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 5;
	}
	else if (m_iPortalCount == 11 && m_iCountMonster <= 5)
	{
		m_iPortalCount = 12;
		Open_Potal(XMVectorSet(-170.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-176.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-181.f, 29.f, 307.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-175.f, 29.f, 314.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-175.f, 30.f, 320.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-173.f, 30.f, 323.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 6;
	}
	else if (m_iPortalCount == 12 && m_iCountMonster <= 6)
	{
		m_iPortalCount = 13;
		Open_Potal(XMVectorSet(-171.f, 29.f, 300.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-181.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-179.f, 29.f, 305.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 3;
	}
	else if (m_iPortalCount == 13 && m_iCountMonster <= 0)
	{
		CLEAR_QUEST(L"T_HUD_KillAllMonster");
		m_pTriggerSystem->Check_Clear();
	}
}

void CStage1::Portal_Spot6()
{
	list<CGameObject*>* vecObj = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_2HSword");
	if (vecObj && !vecObj->empty() && m_iPortalCount == 13)
	{
		auto iter = vecObj->begin();
		CActor* pActor = static_cast<CActor*>(*iter);
		if (pActor->Get_HpRatio() < 0.3f)
		{
			m_iPortalCount = 14;
			Open_Potal(XMVectorSet(-180.f, 52.f, 393.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
			Open_Potal(XMVectorSet(-172.f, 52.f, 393.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
			Open_Potal(XMVectorSet(-175.f, 52.f, 388.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
			m_iCountMonster += 3;
		}
	}
	else if (m_iPortalCount == 14 && m_iCountMonster <= 3)
	{
		m_iPortalCount = 15;
		Open_Potal(XMVectorSet(-178.f, 52.f, 391.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-170.f, 52.f, 390.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-171.f, 52.f, 395.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 3;
	}
}

void CStage1::Open_Wall()
{
	if (!m_pTriggerSystem)
		return;
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Wall");

	if (!pLayer)
		return;

	auto iter = pLayer->begin();
	if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 4)
	{
		if (m_iCountMonster == 8)
		{
			advance(iter, 0);
			if((*iter)->getActive())
				static_cast<CWall*>(*iter)->Destroy();
			//PLAY_SOUND(L"Monster_Battle", CHANNEL::BATTLE);
			//VOLUME_CHANGE(CHANNEL::BGM, 0.5f);
		}
		if (m_iCountMonster == 0)
		{
			advance(iter, 1);
			if ((*iter)->getActive())
				static_cast<CWall*>(*iter)->Destroy();
		}
	}
	else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 5)
	{
		if (m_iCountMonster == 0)
		{
			advance(iter, 2);
			if ((*iter)->getActive())
				static_cast<CWall*>(*iter)->Destroy();
			PLAY_SOUND(L"Monster_Battle_2", CHANNEL::BATTLE);
			VOLUME_CHANGE(CHANNEL::BGM, 0.5f);
		}
	}
	else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 8)
	{
		if (m_iCountMonster == 0)
		{
			advance(iter, 3);
			if ((*iter)->getActive())
				static_cast<CWall*>(*iter)->Destroy();
		}
	}
	else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 9)
	{
		if (m_iCountMonster == 0&& m_iPortalCount == 15)
		{
			advance(iter, 4);
			if ((*iter)->getActive())
			{
				static_cast<CWall*>(*iter)->Destroy();
				m_pTriggerSystem->setAllTriggerClear(true);
			}
		}
	}
}


//???????? 3????
void CStage1::Trgger_Function1()
{
	SHOW_MAPINFO(TRUE, 0);

	//TriggerSystem???? ?????? ?????? ?????? ????????(MonsterType)
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");
	//?????? ?????? ?????? ????????
	m_bFirst = true;
	if (!pLayer)
		return;
	//??????
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 3;

	START_QUEST(EQuestHeaderType::FirestStep, L"T_HUD_Find_Sunforge");
	START_QUEST(EQuestHeaderType::FirestStep, L"T_HUD_Find_DropBox");

}

//???? 1????
void CStage1::Trgger_Function2()
{
	SHOW_MAPINFO(TRUE, 0);

	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_EarthAberrant");
	//?????? ?????? ?????? ????????
	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		//????
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		//????
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 1;
}
//???????? 2???? ???? 2????
void CStage1::Trgger_Function3()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");

	if (!pLayer)
		return;
	//??????

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 3);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		//??????
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		//??????
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 4;

	START_QUEST(EQuestHeaderType::FirestStep, L"T_HUD_EquipNewWeapon");
}
//???????? 3????
void CStage1::Trgger_Function4()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");
	if (!pLayer)
		return;
	//??????

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 3;
}
//?????? ??????(????3??)
void CStage1::Trgger_Function5()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_EarthAberrant");
	if (!pLayer)
		return;

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 3);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 8);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 9);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 10);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor = static_cast<CActor*>((*iter));
			pActor->Set_Remove(true);
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 10;
	START_QUEST(EQuestHeaderType::FirestStep, L"T_HUD_KillAllMonster");
}
//????1???? ???? 2????(???? 3??)
void CStage1::Trgger_Function7()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
	if (!pLayer)
		return;

	if(m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Healer");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();

		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}

		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Healer");
		if (!pLayer)
			return;
		iter = pLayer->begin();
		pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 3;
	START_QUEST(EQuestHeaderType::FirestStep, L"T_HUD_KillAllMonster");

}
// ????3 ??1
void CStage1::Trgger_Function8()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 8);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 9);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 10);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		//????
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		//????
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 4;
}
//????1 ??2 ????2(???? 4??)
void CStage1::Trgger_Function9()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 11);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 12);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Healer");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Healer");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 5;
}
//???? 5????
void CStage1::Trgger_Function10()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_EarthAberrant");
	if (!pLayer)
		return;

	if(m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 3);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 5;
}
//?????? 1????
void CStage1::Trgger_Function11()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_2HSword");
	if (!pLayer)
		return;

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		CActor* pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 1;
}
//????
void CStage1::Trgger_FunctionBoss()
{
	//g_pGameInstance->StopSound(CSoundMgr::CHANNELID::BGM);
	//g_pGameInstance->PlayBGM(L"Stage1_Boss_BGM");

	if(m_pScenemaManager)
		m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA2_1);
	else
	{
		list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Boss");
		if (!pLayer)
			return;
		if (m_bDebug)
		{
			auto iter = pLayer->begin();
			advance(iter, 0);
			(*iter)->setActive(true);
			m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		}
		else
		{
			auto iter = pLayer->begin();
			CActor* pActor = static_cast<CActor*>((*iter));
			while (pActor->Get_HpRatio() == 0)
			{
				iter++;
				pActor->Set_Remove(true);
				pActor = static_cast<CActor*>((*iter));
			}
			(*iter)->setActive(true);
			m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		}
	}
}

HRESULT CStage1::Ready_Trigger_Lod(const _tchar* pDataFilePath)
{
	//???????? ?????? ??????
	vector<TRIGGER> vecTrigger;
	
	g_pGameInstance->LoadFile<TRIGGER>(vecTrigger, pDataFilePath);
	 
	for (int i = 0; i < vecTrigger.size(); ++i)
	{
		TRIGGER TriggerDesc;

		TriggerDesc.eTrigger_Type = vecTrigger[i].eTrigger_Type;
		TriggerDesc.fTrigger_Point = vecTrigger[i].fTrigger_Point;
		TriggerDesc.iIndex = vecTrigger[i].iIndex;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Trigger_Lod", L"Prototype_GameObject_Trigger", &TriggerDesc)))
		{
			MSGBOX("?????? ?????? ???????? ???? ?????? ????????????. Stage1.cpp Line 204");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_Trigger_Light(const _tchar* pDataFilePath)
{
	return E_NOTIMPL;
}

HRESULT CStage1::Ready_Trigger_Scene(const _tchar* pDataFilePath)
{
	return E_NOTIMPL;
}

HRESULT CStage1::Ready_Trigger_Monster(const _tchar* pDataFilePath)
{
	return E_NOTIMPL;
}

HRESULT CStage1::Ready_Trigger_Quest(const _tchar* pDataFilePath)
{
	vector<TRIGGER> vecTrigger;

	g_pGameInstance->LoadFile<TRIGGER>(vecTrigger, pDataFilePath);

	for (int i = 0; i < vecTrigger.size(); ++i)
	{
		TRIGGER TriggerDesc;

		TriggerDesc.eTrigger_Type = vecTrigger[i].eTrigger_Type;
		TriggerDesc.fTrigger_Point = vecTrigger[i].fTrigger_Point;
		TriggerDesc.iIndex = vecTrigger[i].iIndex;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Trigger_Quest", L"Prototype_GameObject_Trigger", &TriggerDesc)))
		{
			MSGBOX("?????? ?????? ???????? ???? ?????? ????????????. Stage1.cpp Line 306");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_Trigger_Jump()
{
	// ???? ??????
	CJumpNode::DESC tJumpNodeDesc;
	tJumpNodeDesc.vPosition = { 25.f, 5.f, 84.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -25.f, 8.f, 98.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -176.f, 50.f, 335.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;

	// ???? ??????
	CJumpBox::DESC tJumpBoxrDesc;
	tJumpBoxrDesc.vPosition = { -42.5f, 4.5f, 82.f };
	tJumpBoxrDesc.vRotation = { 0.f, 0.f, 0.f };
	tJumpBoxrDesc.vScale = { 7.f, 2.f, 8.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_JumpBox", L"Proto_GameObject_JumpBox", &tJumpBoxrDesc)))
		return E_FAIL;
	tJumpBoxrDesc.vPosition = { -136.5f, 19.f, 231.5f };
	tJumpBoxrDesc.vRotation = { 0.f, 0.f, 0.f };
	tJumpBoxrDesc.vScale = { 7.f, 2.f, 6.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_JumpBox", L"Proto_GameObject_JumpBox", &tJumpBoxrDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CStage1::Ready_Treasure_Chest()
{
	vector<ENVIRONMENTLOADDATA> vecMapObjectData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecMapObjectData, L"../bin/SaveData/Treasure_Chest/DropBox.dat")))
		return E_FAIL;

	vector<_float4x4> vecObject;

	vector<CEnvironment::ENVIRONMENTDESC> tChestDesc;
	tChestDesc.resize(10);
	_uint iIndex = 0;

	for (auto& pData : vecMapObjectData)
	{
		vecObject.emplace_back(pData.WorldMat);
	}

	for (_int j = 0; j < vecObject.size(); ++j)
	{
		DROPBOXDESC Desc;

		Desc.WorldMat = vecObject[j];

		CDropBoxData* pDropboxdata = new CDropBoxData;

		Desc.itemData = pDropboxdata->GetItemData(j);

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_DropBox", L"Proto_GameObject_Treasure_Chest", &Desc)))
		{
			MSGBOX("Treasure_Chest ?????? ???????? ???? ?????? ????????????. Stage1.cpp Line 306");
			return E_FAIL;
		}

		m_pDumyDropData.push_back(pDropboxdata);
	}


	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_LootShield", L"Proto_GameObject_LootShield")))
		return E_FAIL;

	return S_OK;
}


//auto temp  = pDropboxdata->GetItemData(i);
		//std::vector<CItemData*> test;

		//for (auto& iter : temp)
		//{
		//	test.emplace_back(&iter);
		//}
		//MapObjectDesc.itemData.push_back(test);

HRESULT CStage1::Ready_Meteor()
{
	for (_uint i = 0; i < 10; i++)
	{
		CMeteor* pObj = nullptr;
		g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Meteor", L"Proto_GameObject_Weapon_Meteor", nullptr,(CGameObject**)&pObj);
		assert(pObj);
		pObj->setActive(false);
		Safe_AddRef(pObj);
		m_vecMeteor.emplace_back(pObj);
	}
	m_vecMeteorPos.resize(6);
	m_vecMeteorPos[0] = _float4(-90.f, -20.f, 96.f, 1.f);
	m_vecMeteorPos[1] = _float4(10.f, -20.f, 145.f, 1.f);
	m_vecMeteorPos[2] = _float4(-110.f, -5.f, 142.f, 1.f);
	m_vecMeteorPos[3] = _float4(-200.f, -20.f, 320.f, 1.f);
	m_vecMeteorPos[4] = _float4(-80.f, -20.f, 380.f, 1.f);
	m_vecMeteorPos[5] = _float4(-80, -20.f, 266.f, 1.f);

	m_fRandomMeteorSpawnTime = (_float)MathUtils::ReliableRandom((_double)10.f,(_double)25.f);

	return S_OK;
}

void CStage1::Shoot_Meteor(_double dDeltaTime)
{
	if (m_vecMeteor.empty())
		return;
	_uint iCount = 0;
	for (auto& pMeteor : m_vecMeteor)
	{
		if (!pMeteor->getActive())
			iCount++;
	}
	if (iCount == 0)
		return;
	if (iCount > 5)
		iCount = 5;

	m_fAccMeteorSpawn += (_float)dDeltaTime;
	if (m_fAccMeteorSpawn >= m_fRandomMeteorSpawnTime)
	{
		m_fAccMeteorSpawn = 0.f;
		m_fRandomMeteorSpawnTime = (_float)MathUtils::ReliableRandom(5.f,15.f);
		
		_uint iRandomShot = (_uint)MathUtils::ReliableRandom(1.0, (_double)iCount+1);
		for (_uint i = 0; i < iRandomShot; i++)
		{
			_vector vPos = XMVectorZero();
			_uint randomPos = (_uint)MathUtils::ReliableRandom(0, 6);
			vPos = XMLoadFloat4(&m_vecMeteorPos[randomPos]);

			_vector vPivot;
			_float fX = (_float)MathUtils::ReliableRandom(-10.0, 10.0);
			_float fZ = (_float)MathUtils::ReliableRandom(-10.0, 10.0);

			vPivot = XMVectorSet(fX, 0.f, fZ, 1.f);
			vPos += vPivot;
			CMeteor* pMeteor = Find_Meteor();
			if (!pMeteor)
				MSGBOX("Meteor Null!!");
			pMeteor->setActive(true);
			pMeteor->Move(vPos, randomPos);
		}
	}

}

CMeteor* CStage1::Find_Meteor()
{
	for (auto& pObj : m_vecMeteor)
	{
		if (!pObj->getActive())
			return pObj;
	}

	return nullptr;
}



CStage1* CStage1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CStage1* pInstance = new CStage1(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CStage1 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStage1::Free()
{
	CLevel::Free();

	if (m_pScenemaManager)
		Safe_Release(m_pScenemaManager);

	for (auto& iter : m_pDumyDropData)
		Safe_Delete(iter);
	m_pDumyDropData.clear();

	if (m_pIndicatorManager)
	{
		Safe_Release(m_pIndicatorManager);
		CIndicator_Manager::DestroyInstance();
	}

	if (g_pInteractManager)
		g_pInteractManager->Remove_Interactable();

	if (g_pDropManager)
		CDropManager::DestroyInstance();

	if(m_pTriggerSystem)
		Safe_Release(m_pTriggerSystem);

	for (auto& pObj : m_vecMeteor)
		Safe_Release(pObj);
	m_vecMeteor.clear();

	CWeaponGenerator::DestroyInstance();
	CVoiceManager::DestroyInstance();
}
