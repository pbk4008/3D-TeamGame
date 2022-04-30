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
	//몬스터 안잡고 진행하려면 true, 잡으면서 진행하려면 false (잡고갈때는 무조건 다 잡고가야됨)

	m_bDebug = false;//false로 바꾸면 무조건 몬스터 다잡고 가야됩니다.
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

	//if (FAILED(Ready_Trigger_Jump()))
	//{
	//	MSGBOX("Stage1 Jump");
	//	return E_FAIL;
	//}

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
	{
		MSGBOX("Stage1 Player");
		return E_FAIL;
	}

	//if (FAILED(Ready_MapObject()))
	//{
	//	MSGBOX("Stage1 MapObject");
	//	return E_FAIL;
	//}

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

	//if (FAILED(Ready_Meteor()))
	//{
	//	MSGBOX("Meteor");
	//	return E_FAIL;
	//}
	
	if (FAILED(Ready_Indicator()))
	{
		MSGBOX("Indicator");
		return E_FAIL;
	}

	//if (FAILED(Ready_Portal()))
	//{
	//	MSGBOX("Portal");
	//	return E_FAIL;
	//}

	if (FAILED(Ready_Wall()))
	{
		MSGBOX("Wall");
		return E_FAIL;
	}

	//if (FAILED(Ready_Pot()))
	//	return E_FAIL;

	//if (FAILED(Ready_Cinema()))
	//{
	//	MSGBOX("Cinema");
	//	return E_FAIL;
	//}

	g_pGameInstance->PlayBGM(L"Stage1_BGM");
	
	//m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA1_1);

	return S_OK;
}

_int CStage1::Tick(_double TimeDelta)
{
	//_float3 fPos = { 0.f,5.f,20.f };

	//m_pPot->Tick(TimeDelta);
	/*_vector vTmp = g_pObserver->Get_PlayerPos();
	cout << XMVectorGetX(vTmp) << ", " << XMVectorGetY(vTmp) << ", " << XMVectorGetZ(vTmp) << endl;*/

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


		//if (m_iCountMonster == 0 && m_bFirst)//트리거 몬스터 다 잡힘
		//{
		//	//포탈 위치 체크 
		//	if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 4)
		//	{
		//		if (m_iPortalCount == 0)
		//		{
		//			m_pTriggerSystem->Trigger_Clear();

		//			m_iPortalCount = 4;
		//			Open_Potal(XMVectorSet(-58.f, 15.f, 213.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			Open_Potal(XMVectorSet(-64.f, 15.f, 230.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			Open_Potal(XMVectorSet(-77.f, 15.f, 220.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			Open_Potal(XMVectorSet(-62.f, 15.f, 206.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		//			m_iCountMonster += 4;
		//		}
		//		else if (m_iPortalCount == 4)
		//		{
		//			Open_Potal(XMVectorSet(-80.f, 15.f, 220.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			Open_Potal(XMVectorSet(-56.f, 18.f, 222.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		//			m_iCountMonster += 2;
		//			m_iPortalCount += 2;
		//			m_bPortalClear = true;
		//		}
		//		else if (m_bPortalClear)
		//		{
		//			CLEAR_QUEST(L"T_HUD_KillAllMonster");
		//			m_pTriggerSystem->Next_TriggerOn();
		//			m_bPortalClear = false;
		//		}
		//	}
		//	else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 5)
		//	{
		//		if (m_iPortalCount == 6)
		//		{
		//			m_pTriggerSystem->Trigger_Clear();
		//			Open_Potal(XMVectorSet(-130.f, 20.f, 216.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		//			m_iPortalCount++;
		//			m_iCountMonster++;
		//		}
		//		else if (m_iPortalCount == 7)
		//		{
		//			Open_Potal(XMVectorSet(-126.f, 20.f, 220.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			Open_Potal(XMVectorSet(-127.f, 20.f, 214.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		//			m_iCountMonster += 2;
		//			m_iPortalCount += 2;
		//			m_bPortalClear = true;
		//		}
		//		else if (m_bPortalClear)
		//		{
		//			CLEAR_QUEST(L"T_HUD_KillAllMonster");
		//			m_bPortalClear = false;
		//			m_pTriggerSystem->Next_TriggerOn();
		//		}
		//	}
		//	else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 6)
		//	{
		//		if (m_iPortalCount == 9)
		//		{
		//			m_pTriggerSystem->Trigger_Clear();
		//			Open_Potal(XMVectorSet(-135.f, 18.f, 255, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		//			Open_Potal(XMVectorSet(-129.f, 18.f, 257.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		//			Open_Potal(XMVectorSet(-138, 18.f, 266.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			m_iPortalCount+=3;
		//			m_iCountMonster+=3;
		//			m_bPortalClear = true;
		//		}
		//		else if (m_bPortalClear)
		//		{
		//			CLEAR_QUEST(L"T_HUD_KillAllMonster");
		//			m_bPortalClear = false;
		//			m_pTriggerSystem->Next_TriggerOn();
		//		}
		//	}
		//	else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 7)
		//	{
		//		if (m_iPortalCount == 12)
		//		{
		//			m_pTriggerSystem->Trigger_Clear();
		//			Open_Potal(XMVectorSet(-177.f, 29.f, 300.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			Open_Potal(XMVectorSet(-172.f, 29.f, 316.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			Open_Potal(XMVectorSet(-168.f, 29.f, 306.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		//			Open_Potal(XMVectorSet(-173.f, 29.f, 300.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		//			m_iCountMonster += 4;
		//			m_iPortalCount += 4;
		//			m_bPortalClear = true;
		//		}
		//		else if (m_bPortalClear)
		//		{
		//			CLEAR_QUEST(L"T_HUD_KillAllMonster");
		//			m_pTriggerSystem->Next_TriggerOn();
		//			m_bPortalClear = false;
		//		}
		//	}
		//	else
		//		m_pTriggerSystem->Check_Clear();

		//	CLEAR_QUEST(L"T_HUD_KillAllMonster"); /* 포탈로 등장하는 몬스터가 아닌 경우에 몹이 다 잡힌 경우 */
		//}

		//CBoss_Bastion_Judicator* pBoss = (CBoss_Bastion_Judicator*)g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Boss")->front();
		//if (nullptr != pBoss)
		//{
		//	if (true == pBoss->Get_Dead())
		//	{
		//		if (!m_bBossClear)
		//		{
		//			m_bBossClear = true;
		//			m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA3_1);
		//		}
		//		else
		//		{
		//			if (m_pScenemaManager->Get_EventCinema((_uint)CINEMA_INDEX::CINEMA3_5))
		//			{
		//				m_pScenemaManager->ResetCinema();
		//				g_pInvenUIManager->SetRender(true);
		//				g_pQuestManager->SetRender(true);
		//				if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE2))))
		//					return -1;
		//
		//				return 0;
		//			}
		//		}
		//	}
		//}
	}

#pragma region Using Debug
	_float3 fPos = { 0.f,5.f,20.f };

	////////////// 레이저 이펙트
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD0))
	//{
	//	CMeshEffect_Razer* pRazer = nullptr;
	//	CGameObject* pPlayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Silvermane")->front();
	//	XMStoreFloat3(&fPos, pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION) - _vector{ 0.f, 2.f, 0.f, 0.f });
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_MeshEffect_Razer", &fPos, (CGameObject**)&pRazer)))
	//		return -1;
	//	//pRazer->setActive(true);
	//}
	//if (g_pGameInstance->getkeyDown(DIK_NUMPAD0))
	//{
	//	CBoss_Bastion_Judicator* pMidBoss = nullptr;
	//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Test", L"Proto_GameObject_Boss_Bastion", &fPos, (CGameObject**)&pMidBoss)))
	//		return -1;
	//	pMidBoss->setActive(true);
	//}
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
		if (g_pGameInstance->getkeyDown(DIK_END))
			m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA1_1);

		m_pScenemaManager->Tick(TimeDelta);

		if (m_pScenemaManager->Get_EventCinema((_uint)CINEMA_INDEX::CINEMA2_4))
		{
			CBoss_Bastion_Judicator* pBoss = (CBoss_Bastion_Judicator*)g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Boss")->front();
			if (nullptr != pBoss)
			{
				pBoss->setActive(true);
				m_pScenemaManager->ResetCinema();
			}
		}
	}

	/*for Meteor*/
	m_fAccMeteorStartTime += (_float)TimeDelta;
	if (m_fAccMeteorStartTime > 15.f)
		Shoot_Meteor(TimeDelta);

	if(g_pQuestManager)
		g_pQuestManager->Tick(g_dImmutableTime);
	
	if (g_pGuideManager)
		g_pGuideManager->Tick(g_dImmutableTime);

	Open_Wall();


	return _int();
}

_int CStage1::LateTick(_double TimeDelta)
{

	if(m_pScenemaManager)
		m_pScenemaManager->LateTick(TimeDelta);

	if(g_pQuestManager)
		g_pQuestManager->Late_Tick(TimeDelta);

	if (g_pGuideManager)
		g_pGuideManager->Late_Tick(g_dImmutableTime);

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
		if (pDesc.wstrInstaneTag == L"Brazier_Large_01.fbx")
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
		{
			return E_FAIL;
		}
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
	//// 네비메쉬
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
	//	MSGBOX(L"메쉬 이펙트 테스트2 생성 실패");

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
	Desc.fAngle = 0.3f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI_Green", L"Proto_GameObject_UI_Player_HpBar", &Desc)))
		return E_FAIL;

	//Player HpBar Red
	ZeroMemory(&Desc, sizeof(CUI_Player_HpBar::UIDESC));
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar_Red");
	Desc.bMinus = true;
	Desc.fAngle = 0.f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Player_HpBar_Red", &Desc)))
		return E_FAIL;

	//Player HpHeal Num
	CUI_HpHeal_Num::UIACTIVEDESC Desc0;
	ZeroMemory(&Desc0, sizeof(CUI_HpHeal_Num::UIACTIVEDESC));
	_tcscpy_s(Desc0.TextureTag, L"Texture_Monster_LevelNum");
	Desc0.bMinus = false;
	Desc0.fAngle = 0.f;
	Desc0.fPos = { 175.f, 638.f, 0.08f };
	Desc0.fSize = { 40.f , 40.f };
	Desc0.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_HpHeal_Num", &Desc0)))
		return E_FAIL;

	//Shield_Meter
	CUI_Shield_Meter::UIACTIVEDESC Desc1;
	ZeroMemory(&Desc1, sizeof(CUI_Shield_Meter::UIACTIVEDESC));
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Shield_Meter");
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 100.f, 620.f, 0.06f };
	Desc1.UIDesc.fSize = { 50.f, 50.f };
	Desc1.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Shield_Meter", &Desc1)))
		return E_FAIL;

	////Tuto Base
	//CUI_Tuto_Base::UIACTIVEDESC Desc1;
	//ZeroMemory(&Desc1, sizeof(CUI_Tuto_Base::UIACTIVEDESC));
	//_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Tuto_Base");
	//Desc1.UIDesc.bMinus = false;
	//Desc1.UIDesc.fAngle = 0.f;
	//Desc1.UIDesc.fPos = { 1150.f, 360.f, 0.2f };
	//Desc1.UIDesc.fSize = { 333.f , 105.f };
	//Desc1.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Tuto_Base", &Desc1)))
	//	return E_FAIL;

	////Tuto Font
	//CUI_Tuto_Font::UIACTIVEDESC Desc2;
	//ZeroMemory(&Desc2, sizeof(CUI_Tuto_Font::UIACTIVEDESC));
	//_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Tuto_Font");
	//Desc2.UIDesc.bMinus = false;
	//Desc2.UIDesc.fAngle = 0.f;
	//Desc2.UIDesc.fPos = { 1130.f, 360.f, 0.1f };
	//Desc2.UIDesc.fSize = { 73.f , 73.f };
	//Desc2.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;
	//Desc2.iTextureNum = 0;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_UI_Tuto_Font", &Desc2)))
	//	return E_FAIL;

	//Blank_Ckey
	CUI_Blank_CKey::UIACTIVEDESC Desc3;
	ZeroMemory(&Desc3, sizeof(CUI_Blank_CKey::UIACTIVEDESC));
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Fill_Ckey");
	Desc3.UIDesc.bMinus = false;
	Desc3.UIDesc.fAngle = 0.f;
	Desc3.UIDesc.fPos = { 700.f, 390.f, 0.1f };
	Desc3.UIDesc.fSize = { 40.f , 40.f };
	Desc3.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI_BlankC", L"Proto_GameObject_UI_Blank_CKey", &Desc3)))
		return E_FAIL;

	//Fill_Ckey
	CUI_Fill_Ckey::UIACTIVEDESC Desc4;
	ZeroMemory(&Desc4, sizeof(CUI_Fill_Ckey::UIACTIVEDESC));
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Blank_Ckey");
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 700.f, 390.f, 0.09f };
	Desc4.UIDesc.fSize = { 40.f , 40.f };
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

	return S_OK;
}

HRESULT CStage1::Ready_Data_Effect()
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
	vecHitParticle[0].ParticleColor = { 1.f , 0.6f, 0.3f ,1.f };
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

#pragma endregion

#pragma region 이펙트매니저에 안들어가는것들
	////공중에떠있는환경파티클
	//Env floating
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
	if (FAILED(m_pIndicatorManager->Add_Indicator((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI_Indicator", 10)))
	{
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

	//1번Trigger와 함수 연결
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

HRESULT CStage1::Ready_Portal()
{
	for (_uint i = 1; i <= 73; i++)
	{
		if (i <= 19)//19개
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Sword")))
				return E_FAIL;
		}
		else if(i>=20 && i<=38)//19
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Shooter")))
				return E_FAIL;
		}
		else if(i>=39 && i<=54)//16개
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Healer")))
				return E_FAIL;
		}
		else if (i >= 55 && i <=63)
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Crawler")))
				return E_FAIL;
		}
		else if (i >= 64 && i <=73)
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_PortalMonster", L"Proto_GameObject_Monster_EarthAberrant")))
				return E_FAIL;
		}
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Portal", L"Proto_GameObject_Portal")))
			return E_FAIL;
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
	if (m_iCountMonster == 2 && m_iPortalCount == 0)
	{
		m_iPortalCount = 1;
		Open_Potal(XMVectorSet(-46.f, -3.f, 79.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-38.f, -3.f, 87.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-33.f, -3.f, 87.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-37.f, -3.f, 79.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-37.f, -3.f, 83.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 5;
	}
	else if (m_iCountMonster == 5 && m_iPortalCount == 1)
	{
		m_iPortalCount = 2;
		Open_Potal(XMVectorSet(-46.f, -3.f, 79.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-46.f, -3.f, 79.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 2;
	}
	else if (m_iCountMonster == 0 && m_iPortalCount == 2)
		m_pTriggerSystem->Check_Clear();
}

void CStage1::Portal_Spot2()
{
	if (m_iCountMonster == 3 && m_iPortalCount == 2)
	{
		m_iPortalCount = 3;
		Open_Potal(XMVectorSet(-84.f, 15.f, 215.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-78.f, 15.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-70.f, 15.f, 228.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-61.f, 15.f, 227.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-60.f, 15.f, 217.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-62.f, 15.f, 206.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-71.f, 15.f, 203.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 7;
	}
	else if (m_iCountMonster == 5 && m_iPortalCount == 3)
	{
		m_iPortalCount = 4;
		Open_Potal(XMVectorSet(-81.f, 15.f, 208.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-74.f, 15.f, 207.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-70.f, 15.f, 202.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-73.f, 15.f, 205.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		m_iCountMonster += 4;
	}
	else if (m_iCountMonster == 3 && m_iPortalCount == 4)
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
	else if (m_iCountMonster == 5 && m_iPortalCount == 5)
	{
		m_iPortalCount = 6;
		Open_Potal(XMVectorSet(-59.f, 15.f, 223.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-71.f, 15.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-76.f, 15.f, 233.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster == 0 && m_iPortalCount == 6)
	{
		CLEAR_QUEST(L"T_HUD_KillAllMonster");
		m_pTriggerSystem->Check_Clear();
	}
}

void CStage1::Portal_Spot3()
{
	if (m_iPortalCount == 6 && m_iCountMonster == 1)
	{
		m_iPortalCount = 7;
		Open_Potal(XMVectorSet(-131.f, 19.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-130.f, 19.f, 214.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-133.f, 19.f, 219.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 3;
	}
	else if (m_iPortalCount == 7 && m_iCountMonster == 1)
	{
		m_iPortalCount = 8;
		Open_Potal(XMVectorSet(-141.f, 19.f, 214.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-141.f, 19.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 2;
	}
	else if (m_iPortalCount == 8 && m_iCountMonster == 2)
	{
		m_iPortalCount = 9;
		Open_Potal(XMVectorSet(-141.f, 19.f, 214.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-139.f, 19.f, 221.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-133.f, 19.f, 220.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-129.f, 19.f, 214.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 4;
	}
	else if (m_iPortalCount == 9 && m_iCountMonster == 0)
	{
		CLEAR_QUEST(L"T_HUD_KillAllMonster");
		m_pTriggerSystem->Check_Clear();
	}
}

void CStage1::Portal_Spot4()
{
	if (m_iPortalCount == 9 && m_iCountMonster == 3)
	{
		m_iPortalCount = 10;
		Open_Potal(XMVectorSet(-130.f, 18.f, 255.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-139.f, 18.f, 255.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-136.f, 18.f, 262.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-126.f, 18.f, 267.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-135.f, 18.f, 273.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 5;
	}
	else if (m_iPortalCount == 10 && m_iCountMonster == 3)
	{
		m_iPortalCount = 11;
		Open_Potal(XMVectorSet(-130.f, 18.f, 263.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-136.f, 18.f, 263.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-142.f, 18.f, 261.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		m_iCountMonster += 3;
	}
	else if (m_iPortalCount == 11 && m_iCountMonster == 4)
	{
		m_iPortalCount = 12;
		Open_Potal(XMVectorSet(-131.f, 18.f, 256.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-141.f, 18.f, 258.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-136.f, 18.f, 261.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-125.f, 18.f, 271.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-139.f, 18.f, 274.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 5;
	}
	else if (m_iPortalCount == 12 && m_iCountMonster == 0)
	{
		CLEAR_QUEST(L"T_HUD_KillAllMonster");
		m_pTriggerSystem->Check_Clear();
	}
}

void CStage1::Portal_Spot5()
{
	if (m_iPortalCount == 12 && m_iCountMonster == 3)
	{
		m_iPortalCount = 13;
		Open_Potal(XMVectorSet(-178.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-178.f, 29.f, 309.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-171.f, 29.f, 310.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-171.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-165.f, 29.f, 309.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 5;
	}
	else if (m_iPortalCount == 13 && m_iCountMonster == 3)
	{
		m_iPortalCount = 14;
		Open_Potal(XMVectorSet(-176.f, 29.f, 304.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-177.f, 29.f, 317.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		Open_Potal(XMVectorSet(-174.f, 30.f, 320.f, 1.f), (_uint)GAMEOBJECT::MONSTER_ABERRANT);
		m_iCountMonster += 3;
	}
	else if (m_iPortalCount == 14 && m_iCountMonster == 2)
	{
		m_iPortalCount = 15;
		Open_Potal(XMVectorSet(-170.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-176.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-181.f, 29.f, 307.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-175.f, 29.f, 314.f, 1.f), (_uint)GAMEOBJECT::MONSTER_CRYSTAL);
		Open_Potal(XMVectorSet(-178.f, 30.f, 320.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-173.f, 30.f, 323.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 6;
	}
	else if (m_iPortalCount == 15 && m_iCountMonster == 6)
	{
		m_iPortalCount = 16;
		Open_Potal(XMVectorSet(-171.f, 29.f, 300.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-181.f, 29.f, 301.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-183.f, 29.f, 310.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 3;
	}
	else if (m_iPortalCount == 16 && m_iCountMonster == 0)
	{
		CLEAR_QUEST(L"T_HUD_KillAllMonster");
		m_pTriggerSystem->Check_Clear();
	}
}

void CStage1::Portal_Spot6()
{
	list<CGameObject*>* vecObj = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_2HSword");
	auto iter = vecObj->begin();
	CActor* pActor = static_cast<CActor*>(*iter);
	if (pActor->Get_HpRatio() < 0.3f && m_iPortalCount ==16)
	{
		m_iPortalCount = 17;
		Open_Potal(XMVectorSet(-180.f, 52.f, 393.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-172.f, 52.f, 393.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(-175.f, 52.f, 388.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 3;
	}
	else if (m_iPortalCount == 17 && m_iCountMonster == 3)
	{
		m_iPortalCount = 18;
		Open_Potal(XMVectorSet(-178.f, 52.f, 391.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-170.f, 52.f, 390.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-171.f, 52.f, 383.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
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
		if (m_iCountMonster == 0)
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


//땅강아지 3마리
void CStage1::Trgger_Function1()
{
	//TriggerSystem에서 저장된 몬스터 위치를 가져온다(MonsterType)
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");
	//몬스터 위치를 통해서 클론한다
	m_bFirst = true;
	if (!pLayer)
		return;
	//땅벌레
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
}

//대지 1마리
void CStage1::Trgger_Function2()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_EarthAberrant");
	//몬스터 위치를 통해서 클론한다
	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		//대지
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		//대지
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
//땅강아지 2마리 소드 2마리
void CStage1::Trgger_Function3()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");

	if (!pLayer)
		return;
	//땅벌레

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
		//한손검
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
		//한손검
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

	START_QUEST(EQuestHeaderType::FirestStep, L"T_HUD_Find_DropBox");
	START_QUEST(EQuestHeaderType::FirestStep, L"T_HUD_EquipNewWeapon");
}
//땅강아지 3마리
void CStage1::Trgger_Function4()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");
	if (!pLayer)
		return;
	//땅벌레

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
//중간에 많은데(포탈3개)
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
//힐러1마리 소드 2마리(포탈 3개)
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
// 소드3 총1
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
		//슈터
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

		//슈터
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
//힐러1 총2 소드2(포탈 4개)
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
//대지 5마리
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
//투핸드 1마리
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
//보스
void CStage1::Trgger_FunctionBoss()
{
	g_pGameInstance->StopSound(CSoundMgr::CHANNELID::BGM);
	g_pGameInstance->PlayBGM(L"Stage1_Boss_BGM");

	m_pScenemaManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA2_1);

	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Boss");
	if (!pLayer)
		return;
	/*if (m_bDebug)
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
	}*/
}

HRESULT CStage1::Ready_Trigger_Lod(const _tchar* pDataFilePath)
{
	//트리거를 벡터로 받는다
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
			MSGBOX("트리거 파일을 불러오는 도중 오류가 발생했습니다. Stage1.cpp Line 204");
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
			MSGBOX("트리거 파일을 불러오는 도중 오류가 발생했습니다. Stage1.cpp Line 306");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_Trigger_Jump()
{
	// 점프 노드들
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

	// 점프 박스들
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
			MSGBOX("Treasure_Chest 파일을 불러오는 도중 오류가 발생했습니다. Stage1.cpp Line 306");
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

}
