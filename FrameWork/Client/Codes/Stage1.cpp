#include "pch.h"
#include "Loading.h"
#include "Stage1.h"
#include "Environment.h"
#include "SubEnvironment.h"

#include "Boss_Bastion_Judicator.h"

#include "Effect_DashDust.h"
#include "Effect_HitParticle.h"
#include "Effect_HitFloating.h"
#include "Effect_Env_Fire.h"
#include "UI_Ingame.h"
#include "UI_Player_HpBar.h"
#include "UI_Tuto_Base.h"
#include "UI_Tuto_Font.h"

#include "JumpNode.h"
#include "JumpBox.h"

CStage1::CStage1()
	: m_pTriggerSystem(nullptr)
	, m_bDebug(false)
	, m_iCountMonster(0)
	, m_bFirst(false)
{
}

CStage1::CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
	, m_pTriggerSystem(nullptr)
	, m_bDebug(false)
	, m_iCountMonster(0)
	, m_bFirst(false)
{
}

HRESULT CStage1::NativeConstruct()
{
	//몬스터 안잡고 진행하려면 true, 잡으면서 진행하려면 false (잡고갈때는 무조건 다 잡고가야됨)

	m_bDebug = false;//false로 바꾸면 무조건 몬스터 다잡고 가야됩니다.
#ifndef _DEBUG 
	m_bDebug = false;
#endif // DEBUG

	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Trigger_Jump()))
		return E_FAIL;

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
		return E_FAIL;

	if (FAILED(Ready_MapObject()))
		return E_FAIL;

	if (FAILED(Ready_TriggerSystem(L"../bin/SaveData/Trigger/MonsterSpawnTrigger.dat")))
		return E_FAIL;

	//if (FAILED(Ready_Boss(L"Layer_Boss")))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(Ready_Monster(L"Layer_Monster")))
	//{
	//	return E_FAIL;
	//}

	if (FAILED(Ready_Data_UI(L"../bin/SaveData/UI/UI.dat")))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Data_Effect()))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_UI(L"Layer_UI")))
	{
		return E_FAIL;
	}

	//if (FAILED(Ready_Trigger_Lod(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Light(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Monster(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Scene(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Quest(L"../bin/SaveData/Trigger/Stage1_QuestTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Treasure_Chest()))
	//	return E_FAIL;

	g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");


	return S_OK;
}

_int CStage1::Tick(_double TimeDelta)
{
#ifdef  _DEBUG
	_int iLevel = 0;
	if (g_pDebugSystem->Get_LevelMoveCheck(iLevel))
	{
		CLoading* pLoading = CLoading::Create(m_pDevice, m_pDeviceContext, (SCENEID)iLevel);
		if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, pLoading)))
			return -1;
		g_pDebugSystem->Set_LevelcMoveCheck(false);
		return 0;
	}
#endif //  _DEBUG

	if (nullptr != m_pTriggerSystem)
	{
		m_pTriggerSystem->Tick(TimeDelta);
	}
	if (m_iCountMonster == 0 && m_bFirst)
		m_pTriggerSystem->Check_Clear();

	/*CBoss_Bastion_Judicator* pBoss = (CBoss_Bastion_Judicator*)g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Boss")->front();
	if (nullptr != pBoss)
	{
		if (true == pBoss->Get_Dead())
		{
			if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE2))))
				return -1;
		}
	}*/

	return _int();
}

HRESULT CStage1::Render()
{
#ifdef _DEBUG
	if (nullptr != m_pTriggerSystem)
	{
		m_pTriggerSystem->Render();
	}
#endif

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
		if (pDesc.wstrInstaneTag == L"")
			break;
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Environment", L"Proto_GameObject_Environment", &pDesc)))
		{
			return E_FAIL;
		}
	}

	//------------------------------------------- Tree --------------------------------------------------------------------//

	/*vector<ENVIRONMENTLOADDATA> vecSubEnvData;
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
	}*/

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
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_SordTrail", L"Prototype_GameObject_SwordTral")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Plane", L"Proto_GameObject_Plane_Test",&wstrNaviFile)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;
	
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
	Desc.fAngle = 0.3f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Player_HpBar_Red", &Desc)))
		return E_FAIL;


	//Tuto Base
	CUI_Tuto_Base::UIACTIVEDESC Desc1;
	ZeroMemory(&Desc1, sizeof(CUI_Tuto_Base::UIACTIVEDESC));
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Tuto_Base");
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 1150.f, 360.f, 0.2f };
	Desc1.UIDesc.fSize = { 333.f , 105.f };
	Desc1.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Tuto_Base", &Desc1)))
		return E_FAIL;

	//Tuto Font
	CUI_Tuto_Font::UIACTIVEDESC Desc2;
	ZeroMemory(&Desc2, sizeof(CUI_Tuto_Font::UIACTIVEDESC));
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Tuto_Font");
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 1130.f, 360.f, 0.1f };
	Desc2.UIDesc.fSize = { 73.f , 73.f };
	Desc2.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;
	Desc2.iTextureNum = 0;
	
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Tuto_Font", &Desc2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Light()
{
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(0.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.mOrthinfo[0] = 50.f;

	if (FAILED(g_pGameInstance->CreateLightCam(m_pDevice, m_pDeviceContext, LightDesc))) return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	//LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	//LightDesc.vDirection = _float3(-1.f, -1.f, 1.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	//LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	//LightDesc.mOrthinfo[0] = 50.f;

	//if (FAILED(g_pGameInstance->CreateLightCam(m_pDevice, m_pDeviceContext, LightDesc))) return E_FAIL;

	//if (FAILED(g_pGameInstance->CreateLightCam(m_pDevice, m_pDeviceContext, LightDesc))) return E_FAIL;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Data_Effect()
{
	vector<CEffect_DashDust::EFFECTDESC> vecEffect;
	//Effect_Dash
	g_pGameInstance->LoadFile<CEffect_DashDust::EFFECTDESC>(vecEffect, L"../bin/SaveData/Effect/Effect_Player_Attack1.dat");
	
	for (int i = 0; i < vecEffect.size(); ++i)
	{
		wstring FullName = L"Proto_GameObject_Effect_DashDust";

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect", FullName, &vecEffect[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
			return E_FAIL;
		}
	}

	CEffect_Env_Fire::EFFECTDESC Desc;
	_tcscpy_s(Desc.TextureTag, L"Env_Fire");
	Desc.iRenderPassNum = 1;
	Desc.iImageCountX = 8;
	Desc.iImageCountY = 8;
	Desc.fFrame = 64.f;
	Desc.fEffectPlaySpeed = 1.f;
	Desc.fMyPos = { 0.f, 0.f, 0.f };

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Env_Fire", L"Proto_GameObject_Effect_Env_Fire", &Desc)))
	{
		MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//이펙트생성
	//vector<CEffect_HitParticle::EFFECTDESC> vecEffect;
	//g_pGameInstance->LoadFile<CEffect_HitParticle::EFFECTDESC>(vecEffect, L"../bin/SaveData/Effect/Effect_Player_Attack1.dat");

	for (int i = 0; i < vecEffect.size(); ++i)
	{
		wstring FullName = L"Proto_GameObject_Effect_Hit";
		//_tcscpy_s(vecEffect1[i].ShaderFullFilePath, L"../../Reference/ShaderFile/Shader_PointInstance.hlsl");
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hit", FullName, &vecEffect[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
			return E_FAIL;
		}
	}

	////이펙트생성
	vector<CEffect_HitFloating::EFFECTDESC> vecEffect1;
	g_pGameInstance->LoadFile<CEffect_HitFloating::EFFECTDESC>(vecEffect1, L"../bin/SaveData/Effect/Effect_Player_Attack2_Floating_2.dat");

	for (int i = 0; i < vecEffect1.size(); ++i)
	{
		wstring FullName = L"Proto_GameObject_Effect_Floating";
		//_tcscpy_s(vecEffect1[i].ShaderFullFilePath, L"../../Reference/ShaderFile/Shader_PointInstance.hlsl");
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Floating", FullName, &vecEffect1[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
			return E_FAIL;
		}
	}
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

	fp = &CStage1::Trgger_Function6;
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

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
	}
	m_iCountMonster = 3;
}

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
	}
	m_iCountMonster = 1;
}

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

		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);

		//한손검
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);

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
		iter++;
		(*iter)->setActive(true);
	}
	m_iCountMonster = 4;
}

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

		iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
	}
	m_iCountMonster = 3;
}

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

		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);
	}
	m_iCountMonster = 2;
}

void CStage1::Trgger_Function6()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Sword");
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

		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Crawler");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 8);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 9);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 10);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

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
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
	}
	m_iCountMonster = 7;
}

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

		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Healer");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Healer");
		if (!pLayer)
			return;
		pActor = static_cast<CActor*>((*iter));
		while (pActor->Get_HpRatio() == 0)
		{
			iter++;
			pActor->Set_Remove(true);
			pActor = static_cast<CActor*>((*iter));
		}
		iter = pLayer->begin();
		(*iter)->setActive(true);
	}
	m_iCountMonster = 3;
}

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

		iter = pLayer->begin();
		advance(iter, 9);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 10);
		(*iter)->setActive(true);

		//슈터
		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);

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
	}
	m_iCountMonster = 4;
}

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

		iter = pLayer->begin();
		advance(iter, 12);
		(*iter)->setActive(true);

		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Healer");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);


		pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Bastion_Shooter");
		if (!pLayer)
			return;

		iter = pLayer->begin();
		advance(iter, 1);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 2);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);

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
		iter++;
		(*iter)->setActive(true);
	}
	m_iCountMonster = 5;
}

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

		iter = pLayer->begin();
		advance(iter, 4);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 5);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 6);
		(*iter)->setActive(true);

		iter = pLayer->begin();
		advance(iter, 7);
		(*iter)->setActive(true);
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
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
		iter++;
		(*iter)->setActive(true);
	}
	m_iCountMonster = 5;
}

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
	}
	m_iCountMonster = 1;
}

//-175 51 422
void CStage1::Trgger_FunctionBoss()
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Boss");
	if (!pLayer)
		return;

	if (m_bDebug)
	{
		auto iter = pLayer->begin();
		advance(iter, 0);
		(*iter)->setActive(true);
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
	}
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
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecMapObjectData, L"../bin/SaveData/Treasure_Chest/Stage1_Treasure_Chest.dat")))
		return E_FAIL;

	vector<_float4x4> vecObject;

	vector<CEnvironment::ENVIRONMENTDESC> tChestDesc;
	tChestDesc.resize(10);
	_uint iIndex = 0;
	tChestDesc[iIndex].wstrInstaneTag = vecMapObjectData[0].FileName;

	for (auto& pData : vecMapObjectData)
	{
		vecObject. emplace_back(pData.WorldMat);
	}

	for (int i = 0; i < vecObject.size(); ++i)
	{
		MABOBJECT MapObjectDesc;

		MapObjectDesc.WorldMat = vecObject[i];

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Treasure_Chest", L"Proto_GameObject_Treasure_Chest", &MapObjectDesc)))
		{
			MSGBOX("Treasure_Chest 파일을 불러오는 도중 오류가 발생했습니다. Stage1.cpp Line 306");
			return E_FAIL;
		}
	}
	return S_OK;
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
	Safe_Release(m_pTriggerSystem);
}
