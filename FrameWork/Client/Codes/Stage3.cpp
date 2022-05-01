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
#include "UI_HpHeal_Num.h"
#include "UI_Shield_Meter.h"

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


	CTransform* pParticle =  g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE3, L"Layer_Effect_Stage3_Env_Respawn")->front()->Get_Transform();
	
	pParticle->Set_State(CTransform::STATE_POSITION, XMVectorSet(38.f, 1.f, 55.f, 1.f));


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
	_float3 vpos = { 48.f, 5.f, 146.f };
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

	//Player HpHeal Num
	CUI_HpHeal_Num::UIACTIVEDESC Desc0;
	ZeroMemory(&Desc0, sizeof(CUI_HpHeal_Num::UIACTIVEDESC));
	_tcscpy_s(Desc0.TextureTag, L"Texture_Monster_LevelNum");
	Desc0.bMinus = false;
	Desc0.fAngle = 0.f;
	Desc0.fPos = { 175.f, 638.f, 0.08f };
	Desc0.fSize = { 40.f , 40.f };
	Desc0.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_HpHeal_Num", &Desc0)))
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

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_Shield_Meter", &Desc1)))
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

	CEffect* pEffect = nullptr;
	wstring FullName = L"";

	vector<CEffect_DashDust::EF_PAR_DASH_DESC> vecDashEffect;
	vector<CEffect_HitParticle::EF_PAR_HIT_DESC> vecHitParticle;
	vector<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC> vecHitFloating;
	vector<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC> vecFloatingSpeed;
	vector<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC> vecFloatingUp;

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

	//이펙트매니저 안들어가는거 
	//Stage3 Env Respawn
	vecFloatingUp.clear();
	g_pGameInstance->LoadFile<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC>(vecFloatingUp, L"../bin/SaveData/Effect/Effect_Stage3_Env_Respawn.dat");

	vecFloatingUp[0].fParticleRandomPos = { 3.f, 2.f, 25.f };
	vecFloatingUp[0].ParticleColor = { 1.f,0.5f,0.3f, 1.f };
	vecFloatingUp[0].Power = 0.85f;
	vecFloatingUp[0].fRespawnPosY = 10.f;
	vecFloatingUp[0].bSmall = true;
	vecFloatingUp[0].fMyPos = { 38.f, 1.f, 55.f, 1.f };
	FullName = L"Proto_GameObject_Effect_FloatingUp";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Effect_Stage3_Env_Respawn", FullName, &vecFloatingUp[0], (CGameObject**)&pEffect)))
	{
		MSGBOX("Failed to Creating Effect_Stage3_Env_Respawn in CStage1::Ready_Effect()");
		return E_FAIL;
	}

	pEffect->setActive(true);
	//Stage3 Env Respawn
	vecFloatingUp[0].fMyPos = { 55.f, 1.f, 55.f, 1.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Effect_Stage3_Env_Respawn", FullName, &vecFloatingUp[0], (CGameObject**)&pEffect)))
		return E_FAIL;
	pEffect->setActive(true);

	return S_OK;
}
#pragma endregion


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
