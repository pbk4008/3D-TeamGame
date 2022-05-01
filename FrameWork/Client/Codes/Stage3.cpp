#include "pch.h"
#include "Stage3.h"
#include "Loading.h"
#include "Silvermane.h"
#include "Environment.h"

#include "TestObj.h"
#include "CapsuleObstacle.h"
#include "MeshEffect_Jupiter.h"
#include "UI_Player_HpBar.h"
#include "UI_Player_HpBar_Red.h"

#include "Effect_DashDust.h"
#include "Effect_HitParticle.h"
#include "Effect_HitFloating.h"
#include "Effect_Env_Fire.h"
#include "Effect_DeathParticle.h"
#include "Effect_Env_Floating.h"
#include "Effect_Guard.h"
#include "Effect_Falling_Leaf.h"
#include "Effect_Floating_Speed.h"
#include "Effect_Energy.h"


#include "UI.h"
#include "UI_Fill_CKey.h"
#include "Effect_FloatingUp.h"
#include "Effect_Dead_Spray.h"
#include "Explosion_Rock.h"
#include "Effect_Hammer_Dust.h"
#include "ScenematicManager.h"

#include "Cinema4_1.h"
#include "Cinema4_2.h"
#include "Cinema4_3.h"
#include "Cinema4_4.h"
#include "Cinema4_5.h"
#include "Cinema4_6.h"

CStage3::CStage3(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
	, m_pCinematicManager(nullptr)
{
}

HRESULT CStage3::NativeConstruct()
{
	g_pWeaponGenerator = CWeaponGenerator::GetInstance();

	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		MSGBOX("Failed To Creating Light");

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
		return E_FAIL;

	if (FAILED(Ready_MapObject()))
		return E_FAIL;

	if (FAILED(Ready_Boss(L"Layer_Boss")))
		return E_FAIL;

	if (FAILED(Ready_Data_UI(L"../bin/SaveData/UI/UI.dat")))
		return E_FAIL;

	if (FAILED(Ready_UI(L"Layer_UI")))
		return E_FAIL;

	if (FAILED(Ready_Data_Effect()))
		return E_FAIL;

	if (FAILED(Ready_GameManager()))
		return E_FAIL;

	g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");

	return S_OK;
}

_int CStage3::Tick(_double TimeDelta)
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
#endif //  _DEBUG

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
	if (m_pCinematicManager)
		m_pCinematicManager->Tick(TimeDelta);

	return _int();
}
_int CStage3::LateTick(_double TimeDelta)
{
	if (m_pCinematicManager)
		m_pCinematicManager->LateTick(TimeDelta);

	return _int();
}
HRESULT CStage3::Render()
{
	_vector vPos = g_pObserver->Get_PlayerPos();
	//cout << "현재 플레이어 위치 : " << XMVectorGetX(vPos) << ", " << XMVectorGetY(vPos) << ", " << XMVectorGetZ(vPos) << ", " << endl;

	return S_OK;
}

HRESULT CStage3::Ready_MapObject()
{
	vector<ENVIRONMENTLOADDATA> vecEnvironmentData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecEnvironmentData, L"../bin/SaveData/Stage_3.dat")))
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
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Environment", L"Proto_GameObject_Environment", &pDesc)))
		{
			int a = 10;
			//return E_FAIL;
		}
	}

	//Test
	//LIGHTDESC			LightDesc;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	//LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	//LightDesc.vDirection = _float3(0.f, -1.f, 1.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	//LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	//LightDesc.mOrthinfo[0] = 30.f;

	//if (FAILED(g_pGameInstance->CreateLightCam(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Creating DirectionLight Cam");

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	//LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	//LightDesc.vPosition = _float3(2.f, 15.f, 110.f);

	//if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Adding PointLight");

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
	//	return E_FAIL;

	////////////////////////////////////// 기둥들 못지나다니게 막기
	CCapsuleObstacle::DESC tObstacleDesc;
	tObstacleDesc.vPosition = { 35.9281349f, -4.35070515f, 131.292603f };
	tObstacleDesc.fHeight = 14.f;
	tObstacleDesc.fRadius = 1.2f;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_CapsuleObstacle", &tObstacleDesc)))
		return E_FAIL;
	tObstacleDesc.vPosition = { 36.7101517f, -4.35070515f, 154.704269f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_CapsuleObstacle", &tObstacleDesc)))
		return E_FAIL;
	tObstacleDesc.vPosition = { 48.3579216f, -4.35070515f, 159.164658f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_CapsuleObstacle", &tObstacleDesc)))
		return E_FAIL;
	tObstacleDesc.vPosition = { 59.9142151f, -4.35070515f, 154.434952f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_CapsuleObstacle", &tObstacleDesc)))
		return E_FAIL;
	tObstacleDesc.vPosition = { 59.7f, -4.5f, 130.1f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_CapsuleObstacle", &tObstacleDesc)))
		return E_FAIL;
	tObstacleDesc.vPosition = { 58.8f, -4.5f, 131.7f };
	tObstacleDesc.fHeight = 4.f;
	tObstacleDesc.fRadius = 1.f;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_CapsuleObstacle", &tObstacleDesc)))
		return E_FAIL;

	////////////////////////////////////// 기둥에 메쉬 이펙트 박아주기
	CMeshEffect_Jupiter::DESC tJupiterDesc;
	tJupiterDesc.vPos = { 35.9281349f, 0.3f, 131.292603f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_MeshEffect_Jupiter", &tJupiterDesc)))
		return E_FAIL;
	tJupiterDesc.vPos = { 36.7101517f, 0.3f, 154.704269f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_MeshEffect_Jupiter", &tJupiterDesc)))
		return E_FAIL;
	tJupiterDesc.vPos = { 48.3579216f, 0.3f, 159.164658f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_MeshEffect_Jupiter", &tJupiterDesc)))
		return E_FAIL;
	tJupiterDesc.vPos = { 59.9142151f, 0.3f, 154.434952f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Obstacle", L"Proto_GameObject_MeshEffect_Jupiter", &tJupiterDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage3::Ready_Player(const _tchar* LayerTag)
{
	CSilvermane::SCENEMOVEDATA tDesc = g_pObserver->Get_SceneMoveData();

	//스폰 하고자 하는 위치 지정
	tDesc.vPos = _float3(47.f, 5.f, 28.f);
	wstring wstrNaviFile = L"../Data/NavMesh/Stage_3_Nav.dat";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Plane", L"Proto_GameObject_Plane_Test", &wstrNaviFile)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_Silvermane",&tDesc)))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage3::Ready_Boss(const _tchar* LayerTag)
{
	_float3 vpos = { 48.f, 3.f, 146.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Boss", L"Proto_GameObject_Solaris", &vpos)))
		return E_FAIL;
	return S_OK;
}

HRESULT CStage3::Ready_UI(const _tchar* LayerTag)
{

	//Player HpBar Green
	CUI_Player_HpBar::UIDESC Desc;
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar");
	Desc.bMinus = true;
	Desc.fAngle = 0.3f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_UI_Green", L"Proto_GameObject_UI_Player_HpBar", &Desc)))
		return E_FAIL;

	//Player HpBar Red
	ZeroMemory(&Desc, sizeof(CUI_Player_HpBar::UIDESC));
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar_Red");
	Desc.bMinus = true;
	Desc.fAngle = 0.3f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_Player_HpBar_Red", &Desc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CStage3::Ready_Data_UI(const _tchar* pDataFilePath)
{
	//UI_Ingame_Static
	vector<CUI::UIDESC> vecUI;
	g_pGameInstance->LoadFile<CUI::UIDESC>(vecUI, pDataFilePath);

	for (int i = 0; i < vecUI.size(); ++i)
	{
		wstring Tag = vecUI[i].TextureTag;
		wstring FullName = L"Proto_GameObject_UI_" + Tag;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_UI", FullName, &vecUI[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_UI()");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage3::Ready_Light()
{
	g_pGameInstance->RemoveLight();

	LIGHTDESC			LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(-1.f, -1.f, -1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vPosition = _float3(57.f,150.f,243.f);
	LightDesc.mlookat = _float4(48.f,-4.f,141.f,1.f);
	LightDesc.mOrthinfo[0] = 50.f;
	LightDesc.bactive = true;
	if (FAILED(g_pGameInstance->CreateLightCam(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Creating DirectionLight Cam");

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	//LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	//LightDesc.vPosition = _float3(2.f, 15.f, 110.f);
	//LightDesc.bactive = true;

	//if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Adding PointLight");

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage3::Ready_Data_Effect()
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Foot_Dash", pEffect, 10)))
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGroundSmoke", pEffect, 5)))
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack1", pEffect, 10)))
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating", pEffect, 10)))
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating2", pEffect, 10)))
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Hit", pEffect, 5)))
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Guard", pEffect, 1)))
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Open_Box", pEffect, 1)))
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
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Effect_Item", FullName, &vecFloatingUp[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Item in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Item", pEffect, 1)))
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
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Effect_Box", FullName, &vecFloatingUp[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Box in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Box", pEffect, 1)))
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
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Eat_Item", pEffect, 1)))
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
	tDesc.fFrame = 8;
	tDesc.fEffectPlaySpeed = 20.f;
	tDesc.fAlpha = 1.f;
	tDesc.fWeight = 1.f;
	tDesc.fSize = 3.f;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Effect_HitImage", L"Proto_GameObject_Effect_HitImage", &tDesc, (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitImage", pEffect, 16)))
	{
		MSGBOX("Falild to Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	//Death smoke
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


	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Effect_DeadSmoke", L"Proto_GameObject_Effect_HitImage", &tDesc, (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_DeadSmoke", pEffect, 1)))
	{
		MSGBOX("Falild to Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
		return E_FAIL;
	}


	//Explosion Rock 
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

	//explosion rock left
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

	//explosion rock right
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

	//explosion rock
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

	//Hammer_Dust
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

#pragma endregion
	return S_OK;
}


HRESULT CStage3::Ready_Cinema()
{
	m_pCinematicManager = GET_INSTANCE(CScenematicManager);

	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema4_1::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
		return E_FAIL;
	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema4_2::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
		return E_FAIL;
	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema4_3::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
		return E_FAIL;
	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema4_4::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
		return E_FAIL;
	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema4_5::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
		return E_FAIL;
	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema4_6::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage3::Ready_GameManager(void)
{
	g_pVoiceManager = CVoiceManager::GetInstance();
	if (FAILED(g_pVoiceManager->NativeConstruct(SCENEID::SCENE_STAGE3)))
		return E_FAIL;

	return S_OK;
}

CStage3* CStage3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CStage3* pInstance = new CStage3(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CStage3 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStage3::Free()
{
	CLevel::Free();
	CWeaponGenerator::DestroyInstance();
	Safe_Release(m_pCinematicManager);
}
