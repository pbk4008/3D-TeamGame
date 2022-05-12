#include "pch.h"
#include "..\Headers\Stage2.h"
#include "Loading.h"
#include "Silvermane.h"
#include "Environment.h"

#include "JumpNode.h"
#include "DropBoxData.h"
#include "BoxBridge.h"

//UI
#include "UI_Player_HpBar.h"
#include "UI_Player_HpBar_Red.h"
#include "UI_Blank_CKey.h"
#include "UI_Fill_CKey.h"
#include "UI_HpHeal_Num.h"
#include "UI_Shield_Meter.h"
#include "UI_Skill_Meter_Back.h"
#include "UI_Skill_Meter_Gauge.h"
#include "UI_Skill_Meter_Gauge_Right.h"

//Effect
#include "Effect_Env_Fire.h"
#include "Effect_Env_Floating.h"

#include "DropManager.h"
#include "Potal.h"
#include <Boss_Bastion_Judicator.h>
#include "Pot.h"
#include "Indicator_Manager.h"

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
	g_pWeaponGenerator = CWeaponGenerator::GetInstance();

	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Light())) MSGBOX("Failed To Creating Light");

	if (FAILED(Ready_NaviMesh()))
		return E_FAIL;

	if (FAILED(Ready_MapObject()))
	{
		MSGBOX("MapObject");
		return E_FAIL;
	}

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
	{
		MSGBOX("Player");

		return E_FAIL;
	}

	if (FAILED(Ready_UI(L"Layer_UI")))
	{
		MSGBOX("UI");

		return E_FAIL;
	}

	if (FAILED(Ready_Data_UI(L"../bin/SaveData/UI/UI.dat")))
	{
		MSGBOX("UIData");
		return E_FAIL;
	}

	/* 랜더타겟 지우지 못하는 버그있음요 */
	if (FAILED(Ready_TriggerSystem(L"../bin/SaveData/Trigger/MonsterSpawnTrigger2.dat")))
		return E_FAIL;

	if (FAILED(Ready_Treasure_Chest()))
	{
		MSGBOX("Chest");
		return E_FAIL;
	}
	if (FAILED(Ready_Portal()))
		return E_FAIL;

	if (FAILED(Ready_GameManager()))
		return E_FAIL;

	if (FAILED(Ready_Pot()))
		return E_FAIL;

	m_pIndicatorManager = GET_INSTANCE(CIndicator_Manager);
	if (FAILED(m_pIndicatorManager->Add_Indicator((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI_Indicator", 30)))
	{
		MSGBOX("Failed to Create Indicator");
		return E_FAIL;
	}

	//g_pGameInstance->StopSound(CSoundMgr::CHANNELID::BGM);
	//g_pGameInstance->PlayBGM(L"Stage2_BGM");
	//VOLUME_CHANGE(CHANNEL::BGM, 3.f);

	return S_OK;
}

_int CStage2::Tick(_double TimeDelta)
{
	//cout << "monster count : " << m_iCountMonster << endl;
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

		if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 0)
			Portal_Spot1();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 2)
			Portal_Spot2();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 4)
			Portal_Spot3();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 5)
			Portal_Spot4();
		else if (m_pTriggerSystem->Get_CurrentTriggerNumber() == 8)
			Portal_Spot5();
		else
		{
			if (m_iCountMonster == 0 && m_pTriggerSystem->Get_CurrentTriggerNumber() != -1)
				m_pTriggerSystem->Check_Clear();
		}


		CBoss_Bastion_Judicator* pBoss = (CBoss_Bastion_Judicator*)g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Boss")->front();
		if (nullptr != pBoss)
		{
			if (m_iCountMonster >= 10000)
				m_iCountMonster = 0;

			if (m_iCountMonster == 0 && pBoss->Get_ChangeLevel())
			{
				g_pMainApp->Set_RenderBtn(CRenderer::RENDERBUTTON::FADEIN, true);
				if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE3))))
					return -1;

				return 0;
			}
		}
	}
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
			g_pInvenUIManager->OpenModal();
			g_pMainApp->Set_DeltaTimeZero(true);
		}
	}

	if(g_pInteractManager)
		g_pInteractManager->Tick(TimeDelta);

	if(g_pDropManager)
		g_pDropManager->Tick();

	if (g_pQuestManager)
		g_pQuestManager->Tick(TimeDelta);

	if (g_pVoiceManager && 4 == g_pGameInstance->getCurrentLevel())
		g_pVoiceManager->Tick(TimeDelta);

	if (m_pIndicatorManager)
		m_pIndicatorManager->Active_Indicator();

	return _int();
}

_int CStage2::LateTick(_double TimeDelta)
{
	if (g_pQuestManager)
		g_pQuestManager->Late_Tick(TimeDelta);

	if (g_pVoiceManager)
		g_pVoiceManager->Late_Tick(TimeDelta);

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

HRESULT CStage2::Ready_Light()
{
	g_pGameInstance->RemoveLight();

	LIGHTDESC			LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(0.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);\
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.bactive = true;
	LightDesc.mOrthinfo[0] = 30.f;

	if (FAILED(g_pGameInstance->CreateLightCam(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Creating DirectionLight Cam");

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.fRange = 150.f;
	LightDesc.vDiffuse = _float4(0.6549f, 0.3411f, 0.3411f, 1.f);
	LightDesc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vPosition = _float3(71.f, 35.f, 81.f);
	LightDesc.bactive = true;
	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Adding PointLight");

	LightDesc.fRange = 30.f;
	LightDesc.vDiffuse = _float4(1.0f, 0.34509f, 0.1333f, 1.f);
	LightDesc.vPosition = _float3(55.f, 16.f, 237.f);
	LightDesc.bactive = true;
	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Adding PointLight");

	LightDesc.fRange = 50.f;
	LightDesc.vDiffuse = _float4(0.8784f, 0.2607f, 0.0725f, 1.f);
	LightDesc.vPosition = _float3(7.5f, 29.f, 331.f);
	LightDesc.bactive = true;
	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Adding PointLight");
	
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
		return E_FAIL;
	
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
		if (pDesc.wstrInstaneTag == L"Brazier_03_Lod0.fbx")
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
				Desc.bUsingGravity = false;
				Desc.IDTag = 4;
				if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_NoisFire", L"Proto_GameObject_Effect_Env_Fire", &Desc)))
					MSGBOX("Failed To Clone NoisFire");
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
				Desc.IDTag = 4;
				if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_NoisFire", L"Proto_GameObject_Effect_Env_Fire", &Desc)))
					MSGBOX("Failed To Clone NoisFire");
			}
		}

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
	//tJumpNodeDesc.vPosition = { -2.f, 15.f, 235.f };
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
	//	return E_FAIL;
	tJumpNodeDesc.vPosition = { 39.f, 18.f, 268.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	//tJumpNodeDesc.vPosition = { 33.f, 15.f, 278.f };
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
	//	return E_FAIL;
	// 네비메쉬 끊어진곳 점프노드 삭제하고 박스 콜라이더로 대체
	CBoxBridge::DESC tBoxBridgeDesc;
	tBoxBridgeDesc.vPosition = { 34.5f , 11.5f, 276.f };
	tBoxBridgeDesc.vRotation = { -10.f, -30.f, 0.f };
	tBoxBridgeDesc.vScale = { 10.f, 1.f, 4.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_BoxBridge", L"Proto_GameObject_BoxBridge", &tBoxBridgeDesc)))
		return E_FAIL;

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

HRESULT CStage2::Ready_UI(const _tchar* LayerTag)
{
	//Player HpBar Green
	CUI_Player_HpBar::UIDESC Desc;
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar");
	Desc.bMinus = true;
	Desc.fAngle = 0.35f;
	Desc.fPos = { 333.f, 912.f, 0.f };
	Desc.fSize = { 265.f , 38.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI_Green", L"Proto_GameObject_UI_Player_HpBar", &Desc)))
		return E_FAIL;

	//Player HpBar Red
	ZeroMemory(&Desc, sizeof(CUI_Player_HpBar::UIDESC));
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar_Red");
	Desc.bMinus = true;
	Desc.fAngle = 0.35f;
	Desc.fPos = { 333.f, 912.f, 0.f };
	Desc.fSize = { 265.f , 38.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI", L"Proto_GameObject_UI_Player_HpBar_Red", &Desc)))
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

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, LayerTag, L"Proto_GameObject_UI_HpHeal_Num", &Desc0)))
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

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, LayerTag, L"Proto_GameObject_UI_Shield_Meter", &Desc1)))
		return E_FAIL;

	//Player Skill_Meter_Back
	CUI_Skill_Meter_Back::UIDESC DescBack;
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Back");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1600.f, 880.f, 0.1f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Back", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Back2
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Back");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1710.f, 880.f, 0.1f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Back", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Gauge
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Gauge_Full");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1600.f, 880.f, 0.08f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Gauge", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Gauge
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Gauge_Fill");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1710.f, 882.f, 0.08f };
	DescBack.fSize = { 102.f , 17.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Gauge_Right", &DescBack)))
		return E_FAIL;

	//Blank_Ckey
	CUI_Blank_CKey::UIACTIVEDESC Desc3;
	ZeroMemory(&Desc3, sizeof(CUI_Blank_CKey::UIACTIVEDESC));
	_tcscpy_s(Desc3.UIDesc.TextureTag, L"Texture_Fill_Ckey");
	Desc3.UIDesc.bMinus = false;
	Desc3.UIDesc.fAngle = 0.f;
	Desc3.UIDesc.fPos = { 700.f, 390.f, 0.1f };
	Desc3.UIDesc.fSize = { 40.f , 40.f };
	Desc3.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_UI_BlankC", L"Proto_GameObject_UI_Blank_CKey", &Desc3)))
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
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { 27.f, 18.f, 228.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -2.f, 15.f, 235.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	//tJumpNodeDesc.vPosition = { 39.f, 15.f, 268.f };
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStage2::Ready_Portal()
{
	for (_uint i = 0; i < 75; i++)
	{
		if (i < 18)//18
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Sword")))
				return E_FAIL;
		}
		else if (i >=18 && i < 36)//18
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Shooter")))
				return E_FAIL;
		}
		else if(i>=36 && i<51)//15
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_2HSword")))
				return E_FAIL;
		}
		else if (i >= 51 && i < 61)//10
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Spear")))
				return E_FAIL;
		}
		else if (i >= 61 && i < 75)//14
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_PortalMonster", L"Proto_GameObject_Monster_Bastion_Healer")))
				return E_FAIL;
		}

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_Portal", L"Proto_GameObject_Portal")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStage2::Ready_Data_Effect(const _tchar* pDataFilePath)
{
#pragma region 이펙트매니저에 안들어가는것들
	////공중에떠있는환경파티클
	//Env floating
	vector<CEffect_Env_Floating::EFFECTDESC> vecEnvFloating;
	g_pGameInstance->LoadFile<CEffect_Env_Floating::EFFECTDESC>(vecEnvFloating, L"../bin/SaveData/Effect/Effect_Env_Floating_1.dat");

	wstring FullName = L"Proto_GameObject_Effect_Env_Floating";

	vecEnvFloating[0].fMyPos = { -5.f, 1.f, 20.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_Effect_Env_Floating_1", FullName, &vecEnvFloating[0])))
	{
		MSGBOX("Failed to Creating Effect_Env_Floating_1 in CStage1::Ready_Effect()");
		return E_FAIL;
	}
#pragma endregion

	return S_OK;
}

HRESULT CStage2::Ready_GameManager(void)
{
	g_pDropManager = CDropManager::GetInstance();
	if (FAILED(g_pDropManager->NativeConstruct((SCENEID::SCENE_STAGE2))))
		return E_FAIL;

	g_pVoiceManager = CVoiceManager::GetInstance();
	if (FAILED(g_pVoiceManager->NativeConstruct(SCENEID::SCENE_STAGE2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage2::Ready_Pot()
{
	vector<ENVIRONMENTLOADDATA> vecEnvironmentData;
	g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecEnvironmentData, L"../bin/SaveData/Stage2_Pot.dat");

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
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_Pot", L"Proto_GameObject_Pot", &tDesc)))
			return E_FAIL;
	}
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
	CLEAR_ALLQUEST();

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
	
	START_QUEST(EQuestHeaderType::Sunforge, L"T_HUD_GotoDungeon");
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

	PLAY_SOUND(L"Monster_Battle", CHANNEL::BATTLE);
	VOLUME_CHANGE(CHANNEL::BGM, 0.5f);

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

	VOLUME_CHANGE(CHANNEL::BGM, 1.f);

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

	PLAY_SOUND(L"Monster_Battle", CHANNEL::BATTLE);
	VOLUME_CHANGE(CHANNEL::BGM, 0.5f);

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
	m_iCountMonster += 1;
}

void CStage2::Portal_Spot1()
{
	if (m_iCountMonster <= 5 && m_iPortalCount == 0)
	{
		m_iPortalCount = 1;
		Open_Potal(XMVectorSet(47.f, 0.f, 6.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(53.f, 0.f, 26.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(72.f, 0.f, 32.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(93.f, 0.f, 21.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		m_iCountMonster += 4;
	}
	else if (m_iCountMonster <= 5 && m_iPortalCount == 1)
	{
		m_iPortalCount = 2;
		Open_Potal(XMVectorSet(71.f, 0.f, 8.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(66.f, 0.f, 5.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(57.f, 0.f, 4.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster <= 3 && m_iPortalCount == 2)
	{
		m_iPortalCount = 4;
		Open_Potal(XMVectorSet(58.f, 7.f, 57.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(97.f, 7.f, 47.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(76.f, 7.f, 62.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(46.f, 7.f, 48.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 4;
	}
	else if (m_iCountMonster <= 0 && m_iPortalCount == 4)
		m_pTriggerSystem->Check_Clear();
}

void CStage2::Portal_Spot2()
{
	if (m_iCountMonster <= 2 && m_iPortalCount == 4)
	{
		m_iPortalCount = 5;
		Open_Potal(XMVectorSet(51.f, 13.f, 246.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(63.f, 13.f, 247.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(58.f, 13.f, 227.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(52.f, 13.f, 243.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 4;
	}
	else if (m_iCountMonster <= 3 && m_iPortalCount == 5)
	{
		m_iPortalCount = 6;
		Open_Potal(XMVectorSet(28.f, 15.f, 224.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(27.f, 15.f, 222.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(37.f, 14.f, 230.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster <= 2  && m_iPortalCount == 6)
	{
		m_iPortalCount = 7;
		Open_Potal(XMVectorSet(42.f, 13.f, 240.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(54.f, 13.f, 246.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(56.f, 13.f, 227.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(44.f, 13.f, 232.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		m_iCountMonster += 4;
	}
	else if (m_iCountMonster <= 0 && m_iPortalCount == 7)
		m_pTriggerSystem->Check_Clear();
}

void CStage2::Portal_Spot3()
{

	if (m_iCountMonster <= 4 && m_iPortalCount == 7)
	{
		PLAY_SOUND(L"Monster_Battle", CHANNEL::BATTLE);
		VOLUME_CHANGE(CHANNEL::BGM, 1.5f);

		m_iPortalCount = 8;
		Open_Potal(XMVectorSet(8.f, 10.f, 206.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		Open_Potal(XMVectorSet(4.f, 10.f, 213.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(8.f, 10.f, 219.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster <= 3 && m_iPortalCount == 8)
	{
		m_iPortalCount = 9;
		Open_Potal(XMVectorSet(-5.f, 11.f, 239.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(-5.f, 11.f, 239.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		m_iCountMonster += 2;
	}
	else if (m_iCountMonster <= 2 && m_iPortalCount == 9)
	{
		m_iPortalCount = 10;
		Open_Potal(XMVectorSet(-1.f, 11.f, 263.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(-2.f, 11.f, 270.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(9.f, 11.f, 261.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(17.f, 11.f, 283.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		m_iCountMonster += 4;
	}
	else if (m_iCountMonster <= 3 && m_iPortalCount == 10)
	{
		m_iPortalCount = 11;
		Open_Potal(XMVectorSet(24.f, 11.f, 293.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(35.f, 11.f, 293.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(28.f, 11.f, 281.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster == 1 || m_iCountMonster == 0)
	{
		if (m_iPortalCount == 11)
		{
			m_iCountMonster = 0;
			m_pTriggerSystem->Check_Clear();
		}
	}
}

void CStage2::Portal_Spot4()
{
	if (m_iCountMonster <= 3 && m_iPortalCount == 11)
	{
		PLAY_SOUND(L"Monster_Battle_2", CHANNEL::BATTLE);
		VOLUME_CHANGE(CHANNEL::BGM, 1.5f);

		m_iPortalCount = 12;
		Open_Potal(XMVectorSet(5.f, 17.f, 353.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(16.f, 17.f, 332.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(11.f, 17.f, 332.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(1.f, 17.f, 330.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(4.f, 17.f, 350.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		m_iCountMonster += 5;
	}
	else if (m_iCountMonster <= 4 && m_iPortalCount == 12)
	{
		m_iPortalCount = 13;
		Open_Potal(XMVectorSet(16.f, 17.f, 328.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		Open_Potal(XMVectorSet(11.f, 17.f, 321.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(4.f, 17.f, 333.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(0.f, 17.f, 314, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(17.f, 17.f, 315, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		m_iCountMonster += 5;
	}
	else if (m_iCountMonster <= 6 && m_iPortalCount == 13)
	{
		m_iPortalCount = 14;
		Open_Potal(XMVectorSet(-1.f, 17.f, 339.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(4.f, 17.f, 331.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		Open_Potal(XMVectorSet(15.f, 17.f, 333.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster == 1 || m_iCountMonster == 0)
	{
		if (m_iPortalCount == 14)
		{
			m_iCountMonster = 0;
			m_pTriggerSystem->Check_Clear();
		}
	}
}

void CStage2::Portal_Spot5()
{
	if (m_iCountMonster <= 4 && m_iPortalCount == 14)
	{
		m_iPortalCount = 15;
		Open_Potal(XMVectorSet(72.f, 32.f, 448.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(65.f, 32.f, 439.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		Open_Potal(XMVectorSet(57.f, 32.f, 444.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		Open_Potal(XMVectorSet(56.f, 32.f, 454.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(65.f, 32.f, 461.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(66.f, 32.f, 451.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(59.f, 32.f, 448.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 7;
	}
	else if (m_iCountMonster <= 5 && m_iPortalCount == 15)
	{
		m_iPortalCount = 16;
		Open_Potal(XMVectorSet(57.f, 32.f, 458.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		Open_Potal(XMVectorSet(57.f, 32.f, 447.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(64.f, 32.f, 443.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(75.f, 32.f, 448.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(67, 32.f, 441, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 5;
	}
	else if (m_iCountMonster <= 6 && m_iPortalCount == 16)
	{
		m_iPortalCount = 17;
		Open_Potal(XMVectorSet(75.f, 32.f, 356.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(72.f, 32.f, 448.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(65.f, 32.f, 439.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(57.f, 32.f, 444.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(56.f, 32.f, 454.f, 1.f), (_uint)GAMEOBJECT::MONSTER_2H);
		Open_Potal(XMVectorSet(65.f, 32.f, 461.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(66.f, 32.f, 451.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(59.f, 32.f, 448.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		m_iCountMonster += 8;
	}
	else if (m_iCountMonster <= 7 && m_iPortalCount == 17)
	{
		m_iPortalCount = 18;
		Open_Potal(XMVectorSet(57.f, 32.f, 458.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		Open_Potal(XMVectorSet(57.f, 32.f, 447.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(64.f, 32.f, 443.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(75.f, 32.f, 448.f, 1.f), (_uint)GAMEOBJECT::MONSTER_1H);
		Open_Potal(XMVectorSet(67, 32.f, 441, 1.f), (_uint)GAMEOBJECT::MONSTER_SPEAR);
		m_iCountMonster += 5;
	}
	else if (m_iCountMonster <= 5 && m_iPortalCount == 18)
	{
		m_iPortalCount = 19;
		Open_Potal(XMVectorSet(47.f, 32.f, 446.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(60.f, 32.f, 453.f, 1.f), (_uint)GAMEOBJECT::MONSTER_SHOOTER);
		Open_Potal(XMVectorSet(66.f, 32.f, 444.f, 1.f), (_uint)GAMEOBJECT::MONSTER_HEALER);
		m_iCountMonster += 3;
	}
	else if (m_iCountMonster == 0&& m_iPortalCount == 19)
	{
		m_pTriggerSystem->Check_Clear();
		m_pTriggerSystem->setAllTriggerClear(true);
	}
}



HRESULT CStage2::Ready_Treasure_Chest()
{
	vector<ENVIRONMENTLOADDATA> vecMapObjectData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecMapObjectData, L"../bin/SaveData/Treasure_Chest/DropBox2.dat")))
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

		Desc.itemData = pDropboxdata->GetStage2ItemData(j);

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE2, L"Layer_DropBox", L"Proto_GameObject_Treasure_Chest", &Desc)))
		{
			MSGBOX("Stage2 Treasure_Chest 파일을 불러오는 도중 오류가 발생했습니다.");
			return E_FAIL;
		}
		m_pDumyDropData.push_back(pDropboxdata);
	}

	return S_OK;
}

void CStage2::Open_Potal(_fvector vPos, _uint iMonTag)
{
	list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE2, L"Layer_Portal");
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

	if(m_pTriggerSystem)
		Safe_Release(m_pTriggerSystem);

	if (m_pIndicatorManager)
	{
		Safe_Release(m_pIndicatorManager);
		CIndicator_Manager::DestroyInstance();
	}

	if(g_pInteractManager)
		g_pInteractManager->Remove_Interactable();

	if (g_pDropManager)
		CDropManager::DestroyInstance();

	if (g_pVoiceManager)
		CVoiceManager::DestroyInstance();

	for (auto& iter : m_pDumyDropData)
		Safe_Delete(iter);
	m_pDumyDropData.clear();

	CWeaponGenerator::DestroyInstance();
}
