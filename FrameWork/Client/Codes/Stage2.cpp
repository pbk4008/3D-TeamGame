#include "pch.h"
#include "..\Headers\Stage2.h"
#include "Loading.h"
#include "Silvermane.h"
#include "Environment.h"

#include "JumpNode.h"

#include "UI_Player_HpBar.h"
#include "UI_Player_HpBar_Red.h"
#include "UI_Blank_CKey.h"
#include "UI_Fill_CKey.h"

CStage2::CStage2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
	, m_pTriggerSystem(nullptr)
	, m_bDebug(false)
	,m_bFirst(false)
	,m_iCountMonster(0)
{
}

HRESULT CStage2::NativeConstruct()
{
	m_bDebug = false;
#ifndef _DEBUG
	m_bDebug = false;
#endif
	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_NaviMesh()))
		return E_FAIL;

	if (FAILED(Ready_MapObject()))
		return E_FAIL;

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
		return E_FAIL;

	if (FAILED(Ready_UI(L"Layer_UI")))
		return E_FAIL;

	if (FAILED(Ready_Data_UI(L"../bin/SaveData/UI/UI.dat")))
	{
		return E_FAIL;
	}

	//if (FAILED(Ready_TriggerSystem(L"../bin/SaveData/Trigger/MonsterSpawnTrigger2.dat")))
	//	return E_FAIL;

	return S_OK;
}

_int CStage2::Tick(_double TimeDelta)
{
#ifdef  _DEBUG
	_int iLevel = 0;
	if (g_pDebugSystem->Get_LevelMoveCheck(iLevel))
	{
		CLoading* pLoading = CLoading::Create(m_pDevice, m_pDeviceContext, (SCENEID)iLevel);
		if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, pLoading)))
			return -1;
		g_pDebugSystem->Set_LevelcMoveCheck(false);
	}
	//list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Crawler");

#endif //  _DEBUG
	if (nullptr != m_pTriggerSystem)
	{
		if (g_pGameInstance->getkeyDown(DIK_BACKSPACE))
		{
			m_pTriggerSystem->CurrentTriggerMonsterAllDelete();
			m_iCountMonster = 0;
		}
		m_pTriggerSystem->Tick(TimeDelta);

		if (m_iCountMonster == 0 && m_bFirst)
			m_pTriggerSystem->Check_Clear();
	}

	

	return _int();
}

HRESULT CStage2::Render()
{
#ifdef _DEBUG
	if (nullptr != m_pTriggerSystem)
	{
		m_pTriggerSystem->Render();
	}
#endif
	return S_OK;
}

HRESULT CStage2::Ready_NaviMesh()
{
	wstring wstrNaviFile = L"../Data/NavMesh/Stage_2_Nav.dat";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_Plane", L"Proto_GameObject_Plane_Test", &wstrNaviFile)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage2::Ready_MapObject()
{
	vector<ENVIRONMENTLOADDATA> vecEnvironmentData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecEnvironmentData, L"../bin/SaveData/Stage_2.dat")))
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
		if (pDesc.wstrInstaneTag == L"")
			break;
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_Environment", L"Proto_GameObject_Environment", &pDesc)))
			return E_FAIL;
	}

	///////////////////// 스테이지 2용
	CJumpNode::DESC tJumpNodeDesc;
	tJumpNodeDesc.vPosition = { 30.f , 23.f, 202.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { 27.f, 18.f, 228.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -2.f, 15.f, 235.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { 39.f, 17.f, 268.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { 33.f, 15.f, 278.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage2::Ready_Player(const _tchar* LayerTag)
{
	//이전 씬에 있는 플레이어의 체력 및 기타 변수들 가져오기
	CSilvermane::SCENEMOVEDATA tDesc = g_pObserver->Get_SceneMoveData();
	
	//스폰 하고자 하는 위치 지정
	tDesc.vPos = _float3(70.f, 3.f, 5.f);

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_SordTrail", L"Prototype_GameObject_SwordTral")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, LayerTag, L"Proto_GameObject_Silvermane", &tDesc)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage2::Ready_UI(const _tchar* LayerTag)
{
	//Player HpBar Green
	CUI_Player_HpBar::UIDESC Desc;
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar");
	Desc.bMinus = true;
	Desc.fAngle = 0.3f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI_Green", L"Proto_GameObject_UI_Player_HpBar", &Desc)))
		return E_FAIL;

	//Player HpBar Red
	ZeroMemory(&Desc, sizeof(CUI_Player_HpBar::UIDESC));
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar_Red");
	Desc.bMinus = true;
	Desc.fAngle = 0.3f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI", L"Proto_GameObject_UI_Player_HpBar_Red", &Desc)))
		return E_FAIL;

	//Blank_Ckey
	CUI_Blank_CKey::UIACTIVEDESC Desc3;
	ZeroMemory(&Desc3, sizeof(CUI_Blank_CKey::UIACTIVEDESC));
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Blank_Ckey");
	Desc3.UIDesc.bMinus = false;
	Desc3.UIDesc.fAngle = 0.f;
	Desc3.UIDesc.fPos = { 700.f, 390.f, 0.1f };
	Desc3.UIDesc.fSize = { 60.f , 60.f };
	Desc3.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI_BlankC", L"Proto_GameObject_UI_Blank_CKey", &Desc3)))
		return E_FAIL;

	//Fill_Ckey
	CUI_Fill_Ckey::UIACTIVEDESC Desc4;
	ZeroMemory(&Desc4, sizeof(CUI_Fill_Ckey::UIACTIVEDESC));
	_tcscpy_s(Desc4.UIDesc.TextureTag, L"Texture_Fill_Ckey");
	Desc4.UIDesc.bMinus = false;
	Desc4.UIDesc.fAngle = 0.f;
	Desc4.UIDesc.fPos = { 700.f, 390.f, 0.09f };
	Desc4.UIDesc.fSize = { 60.f , 60.f };
	Desc4.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI_FillC", L"Proto_GameObject_UI_Fill_CKey", &Desc4)))
		return E_FAIL;

	return S_OK;
}


HRESULT CStage2::Ready_Data_UI(const _tchar* pDataFilePath)
{
	//UI_Ingame_Static
	vector<CUI::UIDESC> vecUI;
	g_pGameInstance->LoadFile<CUI::UIDESC>(vecUI, pDataFilePath);

	for (int i = 0; i < vecUI.size(); ++i)
	{
		wstring Tag = vecUI[i].TextureTag;
		wstring FullName = L"Proto_GameObject_UI_" + Tag;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI", FullName, &vecUI[i])))
		{
			MSGBOX("Failed to Creating in CStage2::Ready_UI()");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage2::Ready_JumpTrigger()
{
	// 점프 노드들
	CJumpNode::DESC tJumpNodeDesc;
	tJumpNodeDesc.vPosition = { 30.f , 23.f, 202.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { 27.f, 18.f, 228.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -2.f, 15.f, 235.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { 39.f, 15.f, 268.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage2::Ready_TriggerSystem(const _tchar* pTriggerFile)
{
	m_pTriggerSystem = CTriggerSystem<Client::CStage2>::Create(m_pDevice, m_pDeviceContext, pTriggerFile, this);

	if (!m_pTriggerSystem)
		return E_FAIL;
	
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_2H, L"../bin/SaveData/MonsterSpawn2Stage/2H.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_SWORD, L"../bin/SaveData/MonsterSpawn2Stage/Sword.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_CRYSTAL, L"../bin/SaveData/MonsterSpawn2Stage/Crystal.dat")))
		return E_FAIL;																																
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_HEAL, L"../bin/SaveData/MonsterSpawn2Stage/Healer.dat")))
		return E_FAIL;																															
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_SHOOTER, L"../bin/SaveData/MonsterSpawn2Stage/Shooter.dat")))
		return E_FAIL;																														
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_MID, L"../bin/SaveData/MonsterSpawn2Stage/MidBoss.dat")))
		return E_FAIL;																													
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_SPEAR, L"../bin/SaveData/MonsterSpawn2Stage/Spear.dat")))
		return E_FAIL;																												
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_BRONZE, L"../bin/SaveData/MonsterSpawn2Stage/Bronze.dat")))
		return E_FAIL;

	if (FAILED(Ready_TriggerFunctionSetting()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage2::Ready_TriggerFunctionSetting()
{
	void(Client::CStage2:: * fp)();

	fp = &CStage2::Trgger_Function1;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_Function2;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_Function3;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_Function4;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_Function5;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_Function6;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_Function7;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_Function8;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_Function9;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	fp = &CStage2::Trgger_FunctionBoss;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	return S_OK;
}

void CStage2::Trgger_Function1()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Crawler");

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
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

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
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

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
	m_iCountMonster = 9;
}

void CStage2::Trgger_Function2()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 3);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

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
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

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
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

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

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

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
	m_iCountMonster = 5;
}

void CStage2::Trgger_Function3()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 3);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 5);
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
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

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
}

void CStage2::Trgger_Function4()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Crawler");

	if (!pLayer)
		return;

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 8);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 9);
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
	}
	m_iCountMonster = 4;
}

void CStage2::Trgger_Function6()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

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

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	else
	{
		auto iter = pLayer->begin();
		CActor* pActor = static_cast<CActor*>((*iter));
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

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

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

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
	m_iCountMonster = 5;
}

void CStage2::Trgger_Function7()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bronze");
	if (!pLayer)
		return;

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
		(*iter)->Get_Transform()->Rotation_Axis(CTransform::STATE_UP, XMConvertToRadians(-90.f));
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;

		(*iter)->setActive(true);
		(*iter)->Get_Transform()->Rotation_Axis(CTransform::STATE_UP, XMConvertToRadians(90.f));
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
	}
	m_iCountMonster = 2;
}

void CStage2::Trgger_Function8()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Spear");
	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
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
	}
	m_iCountMonster = 1;
}

void CStage2::Trgger_Function9()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");
	if (!pLayer)
		return;

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 10);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 11);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 12);
		(*iter)->setActive(true);
		iter = pLayer->begin();
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		advance(iter, 13);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter = pLayer->begin();
		advance(iter, 8);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_2HSword");
		if (!pLayer)
			return;

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
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
		iter++;
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");
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

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_2HSword");
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
	m_iCountMonster = 7;
}

void CStage2::Trgger_Function5()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Spear");
	if (!pLayer)
		return;
	if(m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_2HSword");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
		m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

		if (!pLayer)
			return;

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

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_2HSword");

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

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

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

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

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

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

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
	m_iCountMonster = 6;
}

void CStage2::Trgger_FunctionBoss()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Boss");
	if (!pLayer)
		return;
	
	auto iter = pLayer->begin();
	(*iter)->setActive(true);
	m_pTriggerSystem->Add_CurrentTriggerMonster((*iter));
}

CStage2* CStage2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CStage2* pInstance = new CStage2(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CStage2 Crate Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStage2::Free()
{
	CLevel::Free();
	Safe_Release(m_pTriggerSystem);
}
