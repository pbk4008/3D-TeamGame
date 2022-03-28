#include "pch.h"
#include "..\Headers\Stage2.h"
#include "Loading.h"
#include "Silvermane.h"
#include "Environment.h"

#include "JumpNode.h"

CStage2::CStage2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
	, m_bDebug(false)
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

	if (FAILED(Ready_TriggerSystem(L"../bin/SaveData/Trigger/MonsterSpawnTrigger2.dat")))
		return E_FAIL;

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
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Crawler");

#endif //  _DEBUG
	m_pTriggerSystem->Tick(TimeDelta);

	return _int();
}

HRESULT CStage2::Render()
{
#ifdef _DEBUG
	m_pTriggerSystem->Render();
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
		{
			int a=10;
		}
	}

	return S_OK;
}

HRESULT CStage2::Ready_Player(const _tchar* LayerTag)
{
	//이전 씬에 있는 플레이어의 체력 및 기타 변수들 가져오기
	CSilvermane::SCENEMOVEDATA tDesc = g_pObserver->Get_SceneMoveData();
	
	//스폰 하고자 하는 위치 지정
	tDesc.vPos = _float3(70.f, 3.f, 5.f);

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, LayerTag, L"Proto_GameObject_Silvermane", &tDesc)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;

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
	
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_SWORD, L"../bin/SaveData/MonsterSpawn2Stage/Sword.dat")))
		return E_FAIL;
	if (FAILED(m_pTriggerSystem->Load_MonsterSpawnPoint((_uint)SCENEID::SCENE_STAGE2, CTriggerSystem<CStage2>::MONSTER::MON_2H, L"../bin/SaveData/MonsterSpawn2Stage/2H.dat")))
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

	fp = &CStage2::Trgger_FunctionBoss;;
	m_pTriggerSystem->Add_TriggerFuntion(fp);

	return S_OK;
}

void CStage2::Trgger_Function1()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Crawler");

	if (!pLayer)
		return;
	//땅벌레
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 3);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

	}
}

void CStage2::Trgger_Function2()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

		if (!pLayer)
			return;

		 iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);
	}
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
		iter = pLayer->begin();
		advance(iter, 3);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 3);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
	}

}

void CStage2::Trgger_Function4()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Crawler");

	if (!pLayer)
		return;

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);
		iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);
		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
		iter = pLayer->begin();
		advance(iter, 8);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Spear");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_2HSword");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Spear");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_2HSword");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

		if (!pLayer)
			return;
		iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);
	}
}

void CStage2::Trgger_Function5()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");

	if (!pLayer)
		return;

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);
		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Healer");

		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);
	}
}

void CStage2::Trgger_Function6()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bronze");
	if (!pLayer)
		return;

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
	}
}

void CStage2::Trgger_Function7()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Spear");
	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
	}
}

void CStage2::Trgger_Function8()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Sword");
	if (!pLayer)
		return;
	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 8);
		(*iter)->setActive(true);
		iter = pLayer->begin();
		advance(iter, 9);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;
		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;
		iter = pLayer->begin();
		(*iter)->setActive(true);
	}

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
		iter = pLayer->begin();
		advance(iter, 11);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 8);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_2HSword");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
	}
	else
	{
		auto iter = pLayer->begin();
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Bastion_2HSword");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		(*iter)->setActive(true);
	}
}

void CStage2::Trgger_FunctionBoss()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Boss");
	if (!pLayer)
		return;
	
	auto iter = pLayer->begin();
	(*iter)->setActive(true);
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
