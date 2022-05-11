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
#include "UI_Boss_HpBar_Red.h"
#include "UI_Boss_ShieldBar_Blue.h"
#include "UI_Skill_Meter_Back.h"
#include "UI_Skill_Meter_Gauge.h"
#include "UI_Skill_Meter_Gauge_Right.h"

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
#include "Cinema5_1.h"
#include "Cinema5_2.h"
#include "Cinema5_3.h"
 
#include "Boss_Solaris.h"

CStage3::CStage3(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
	, m_pCinematicManager(nullptr)
{
}

HRESULT CStage3::NativeConstruct()
{
	g_pWeaponGenerator = CWeaponGenerator::GetInstance();
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Hexgrid", L"../bin/Resources/Mesh/Effect/T_Hexgrid.dds")))
		MSGBOX("Failed To Add �޽�����Ʈ�� �ؽ�ó Tex");

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

	//if (FAILED(Ready_Cinema()))
	//	return E_FAIL;

	if (FAILED(Ready_Data_Effect()))
		return E_FAIL;

	if (FAILED(Ready_GameManager()))
		return E_FAIL;

	g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");


	g_pQuestManager->SetRender(true);
	g_pInvenUIManager->SetRender(true);

	//PLAY_SOUND(L"Stage3_BGM", CHANNEL::BGM);
	//VOLUME_CHANGE(CHANNEL::BGM, 1.5f);
	//g_pMainApp->Set_RenderBtn(CRenderer::RENDERBUTTON::FADEOUT, true);

	START_QUEST(EQuestHeaderType::Boss, L"T_HUD_KillAllMonster");

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
	{
		if (g_pGameInstance->getkeyDown(DIK_END))
			m_pCinematicManager->Active_Scenema(6);
		
		_float fDist = g_pObserver->Get_Dist(XMVectorSet(47.f, 1.5f, 99.f, 1.f));

		if (fDist < 3.f && !m_bCinemaStart)
		{
			m_bCinemaStart = true;
			//m_pCinematicManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA4_1);
			m_pCinematicManager->Active_Scenema(0);
		}
		m_pCinematicManager->Tick(TimeDelta);
	}

	if (g_pVoiceManager)
	{
		g_pVoiceManager->Tick(TimeDelta);
	}

	CTransform* pParticle =  g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE3, L"Layer_Effect_Stage3_Env_Respawn")->front()->Get_Transform();
	
	if(pParticle != nullptr)
		pParticle->Set_State(CTransform::STATE_POSITION, XMVectorSet(38.f, 1.f, 55.f, 1.f));

	if (m_pBoss->Get_Dead() && !m_bClear)
	{
		m_bClear = true;
		m_pCinematicManager->Active_Scenema((_uint)CINEMA_INDEX::CINEMA5_1);
	}

	//g_pInvenUIManager->Tick(TimeDelta);

	//if (g_pQuestManager)
		//g_pQuestManager->Tick(g_dImmutableTime);

	return _int();
}
_int CStage3::LateTick(_double TimeDelta)
{
	if (m_pCinematicManager)
	{
		_uint iProgress = m_pCinematicManager->LateTick(TimeDelta);
		if (iProgress == 1)
			return 0;
	}

	//if (g_pVoiceManager)
	//{
		//g_pVoiceManager->Late_Tick(TimeDelta);
	//}

	//if (g_pQuestManager)
		//g_pQuestManager->Late_Tick(TimeDelta);

	return _int();
}
HRESULT CStage3::Render()
{
	_vector vPos = g_pObserver->Get_PlayerPos();
	//cout << "���� �÷��̾� ��ġ : " << XMVectorGetX(vPos) << ", " << XMVectorGetY(vPos) << ", " << XMVectorGetZ(vPos) << ", " << endl;

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

	////////////////////////////////////// ��յ� �������ٴϰ� ����
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

	////////////////////////////////////// ��տ� �޽� ����Ʈ �ھ��ֱ�
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

	//���� �ϰ��� �ϴ� ��ġ ����
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
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Boss", L"Proto_GameObject_Solaris", &vpos, (CGameObject**)&m_pBoss)))
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
	Desc.fPos = { 333.f, 912.f, 0.f };
	Desc.fSize = { 265.f , 38.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_UI_Green", L"Proto_GameObject_UI_Player_HpBar", &Desc)))
		return E_FAIL;

	//Player HpBar Red
	ZeroMemory(&Desc, sizeof(CUI_Player_HpBar::UIDESC));
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar_Red");
	Desc.bMinus = true;
	Desc.fAngle = 0.3f;
	Desc.fPos = { 333.f, 912.f, 0.f };
	Desc.fSize = { 265.f , 38.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_Player_HpBar_Red", &Desc)))
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

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_HpHeal_Num", &Desc0)))
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

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_Shield_Meter", &Desc1)))
		return E_FAIL;

	//Player Skill_Meter_Back
	CUI_Skill_Meter_Back::UIDESC DescBack;
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Back");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1600.f, 880.f, 0.1f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Back", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Back2
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Back");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1710.f, 880.f, 0.1f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Back", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Gauge
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Gauge_Full");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1600.f, 880.f, 0.08f };
	DescBack.fSize = { 102.f , 13.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Gauge", &DescBack)))
		return E_FAIL;

	//Player Skill_Meter_Gauge
	_tcscpy_s(DescBack.TextureTag, L"Texture_Skill_Meter_Gauge_Fill");
	DescBack.bMinus = false;
	DescBack.fAngle = 0.3f;
	DescBack.fPos = { 1710.f, 882.f, 0.08f };
	DescBack.fSize = { 102.f , 17.f };
	DescBack.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, LayerTag, L"Proto_GameObject_UI_Player_Skill_Meter_Gauge_Right", &DescBack)))
		return E_FAIL;

	////Boss HpBar Red
	//CUI_Boss_HpBar_Red::UIDESC Desc2;
	//_tcscpy_s(Desc2.TextureTag, L"Texture_Boss_HpBar_Red");
	//Desc2.bMinus = false;
	//Desc2.fAngle = 0.46f;
	//Desc2.fPos = { 939.f, 38.f, 0.08f };
	//Desc2.fSize = { 510.f , 20.f };
	//Desc2.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_UI_Boss_HpBar", L"Proto_GameObject_UI_Boss_HpBar_Red", &Desc2)))
	//	return E_FAIL;

	////Boss ShieldBar Blue
	//_tcscpy_s(Desc2.TextureTag, L"Texture_Boss_ShieldBar_Blue");
	//Desc2.bMinus = false;
	//Desc2.fAngle = 0.46f;
	//Desc2.fPos = { 960.f, 59.f, 0.08f };
	//Desc2.fSize = { 510.f , 20.f };
	//Desc2.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_UI_Boss_ShieldBar", L"Proto_GameObject_UI_Boss_ShieldBar_Blue", &Desc2)))
	//	return E_FAIL;

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

	//���� ui
	/*vecUI.clear();
	g_pGameInstance->LoadFile<CUI::UIDESC>(vecUI, L"../bin/SaveData/UI/UI_Boss.dat");

	for (int i = 0; i < vecUI.size(); ++i)
	{
		wstring Tag = vecUI[i].TextureTag;
		wstring FullName = L"Proto_GameObject_UI_" + Tag;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_UI", FullName, &vecUI[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_UI()");
			return E_FAIL;
		}
	}*/
	return S_OK;
}

HRESULT CStage3::Ready_Light()
{
	g_pGameInstance->RemoveLight();

	LIGHTDESC			LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(0.f, -1.f, -1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vPosition = _float3(57.f,150.f,243.f);
	LightDesc.mlookat = _float4(48.f,-4.f,141.f,1.f);
	LightDesc.mOrthinfo[0] = 50.f;
	LightDesc.bactive = true;
	if (FAILED(g_pGameInstance->CreateLightCam(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Creating DirectionLight Cam");

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(0.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vPosition = _float3(57.f, 150.f, 143.f);
	LightDesc.mlookat = _float4(48.f, -4.f, 141.f, 1.f);
	LightDesc.mOrthinfo[0] = 50.f;
	LightDesc.bactive = true;

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc))) MSGBOX("Failed To Adding PointLight");

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage3::Ready_Data_Effect()
{
	//����Ʈ �Ŵ����� �����鼭 ����
	// ���� ����!! ������ ������ ENUM����
	//Manager�� ���� Effect;
#pragma region ����Ʈ�Ŵ����� ���°͵�, �������Ѽ� enum�� ������ߵ� 

//	////Player Dash
//	CEffect* pEffect = nullptr;
//	vector<CEffect_DashDust::EF_PAR_DASH_DESC> vecDashEffect;
//
//	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_Player_Foot_Dash.dat");
//	vecDashEffect[0].fAlpha = 0.05f;
//
//	wstring FullName = L"Proto_GameObject_Effect_DashDust";
//
//	////�������� ���� Effect���� �ֱ�
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Foot_Dash", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Foot_Dash in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//�Ŵ����� ����Ʈ �ֱ� (������ �Ű����� : ���� ����Ʈ �߰��� ���� ����)
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Foot_Dash", pEffect, 20)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Foot_Dash in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	////HitGroundSmoke
//	vecDashEffect.clear();
//	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_HitGround_Smoke.dat");
//
//	vecDashEffect[0].fAlpha = 0.1f;
//
//	FullName = L"Proto_GameObject_Effect_DashDust";
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGroundSmoke", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_HitGroundSmoke in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGroundSmoke", pEffect, 6)))
//	{
//		MSGBOX("Falild to Add_Effect_HitGroundSmoke in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Player Atttack1
//	vector<CEffect_HitParticle::EF_PAR_HIT_DESC> vecHitParticle;
//	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Player_Attack1.dat");
//
//	FullName = L"Proto_GameObject_Effect_Explosion";
//	vecHitParticle[0].ParticleColor = { 1.f , 0.6f, 0.3f ,1.f };
//	vecHitParticle[0].Power = 2.5f;
//
//	//�������� ���� Effect���� �ֱ�
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack1", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack1 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	//�Ŵ����� ����Ʈ �ֱ� (������ �Ű����� : ���� ����Ʈ �߰��� ���� ����)
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack1", pEffect, 20)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Attack1 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Player Atttack2 Floating
//	vector<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC> vecHitFloating;
//	g_pGameInstance->LoadFile<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC>(vecHitFloating, L"../bin/SaveData/Effect/Effect_Player_Attack2_Floating.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating";
//	vecHitFloating[0].ParticleColor = { 1.f , 0.8f, 0.5f ,1.f };
//	vecHitFloating[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating", FullName, &vecHitFloating[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack2_Floating in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating", pEffect, 20)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Attack2_Floating in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Player Atttack2 Floating2
//	vector<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC> vecFloatingSpeed;
//	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack2_Floating_2.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating_Speed";
//	vecFloatingSpeed[0].ParticleColor = { 1.f,0.5f,0.4f, 1.f };
//	vecFloatingSpeed[0].Power = 2.0f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating2", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack2_Floating2 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack2_Floating2", pEffect, 20)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Attack2_Floating2 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//MonsterDeath
//	vecHitParticle.clear();
//	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Monster_Death.dat");
//
//	FullName = L"Proto_GameObject_Effect_Explosion";
//	vecHitParticle[0].ParticleColor = { 0.5f, 1.0f, 0.1f ,1.f };
//	vecHitParticle[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Death", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Monster_Death in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Death", pEffect, 1)))
//	{
//		MSGBOX("Falild to Add_Effect_Monster_Death in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Player Hit
//	vecHitParticle.clear();
//	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Player_Hit.dat");
//
//	FullName = L"Proto_GameObject_Effect_Explosion";
//	vecHitParticle[0].ParticleColor = { 1.f , 0.6f, 0.3f ,1.f };
//	vecHitParticle[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Hit", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Hit in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Hit", pEffect, 8)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Hit in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Monster Guard
//	vecHitParticle.clear();
//	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Monster_Guard.dat");
//
//	FullName = L"Proto_GameObject_Effect_Explosion";
//	vecHitParticle[0].ParticleColor = { 0.3f, 0.3f, 1.f ,1.f };
//	vecHitParticle[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Guard", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Monster_Guard in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Monster_Guard", pEffect, 8)))
//	{
//		MSGBOX("Falild to Add_Effect_Monster_Guard in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//AttackLeft
//	vecFloatingSpeed.clear();
//	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack_Left.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating_Speed";
//
//	vecFloatingSpeed[0].ParticleColor = { 1.f, 0.9f, 0.5f, 1.f };
//	vecFloatingSpeed[0].Power = 2.0f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Left", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack_Left in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Left", pEffect, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Attack_Left in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//AttackRight
//	vecFloatingSpeed.clear();
//	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack_Right.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating_Speed";
//	vecFloatingSpeed[0].ParticleColor = { 1.f, 0.9f, 0.5f, 1.f };
//	vecFloatingSpeed[0].Power = 2.0f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Right", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack_Right in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Right", pEffect, 10)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Attack_Right in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//AttackRight_Last
//	vecFloatingSpeed.clear();
//	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack_Right_Last.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating_Speed";
//	vecFloatingSpeed[0].ParticleColor = { 1.f, 0.9f, 0.5f, 1.f };
//	vecFloatingSpeed[0].Power = 2.0f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Right_Last", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack_Right_Last in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Right_Last", pEffect, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Attack_Right_Last in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//HitGround
//	vecHitParticle.clear();
//	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Hit_Ground.dat");
//
//	FullName = L"Proto_GameObject_Effect_Explosion";
//	vecHitParticle[0].ParticleColor = { 0.9f,0.5f,0.2f, 1.f };
//	vecHitParticle[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hit_Ground", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Hit_Ground in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hit_Ground", pEffect, 6)))
//	{
//		MSGBOX("Falild to Add_Effect_Hit_Ground in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//
//	//OpenBox
//	vecFloatingSpeed.clear();
//	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Open_Box.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating_Speed";
//	vecFloatingSpeed[0].ParticleColor = { 0.3f,0.5f,1.f, 1.f };
//	vecFloatingSpeed[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Open_Box", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Open_Box in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Open_Box", pEffect, 3)))
//	{
//		MSGBOX("Falild to Add_Effect_Open_Box in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//PlayerAttackGround
//	vecFloatingSpeed.clear();
//	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Player_Attack_Ground.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating_Speed";
//	vecFloatingSpeed[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
//	vecFloatingSpeed[0].Power = 1.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Ground", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack_Ground in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Ground", pEffect, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Attack_Ground in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//PlayerAttackGround_2
//	vecHitFloating.clear();
//	g_pGameInstance->LoadFile<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC>(vecHitFloating, L"../bin/SaveData/Effect/Effect_Player_Attack_Ground_2.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating";
//	vecHitFloating[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
//	vecHitFloating[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Ground2", FullName, &vecHitFloating[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack_Ground2 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Player_Attack_Ground2", pEffect, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_Player_Attack_Ground2 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Item 
//	vector<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC> vecFloatingUp;
//	g_pGameInstance->LoadFile<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC>(vecFloatingUp, L"../bin/SaveData/Effect/Effect_Item.dat");
//
//	vecFloatingUp[0].ParticleColor = { 0.5f,1.0f,0.3f, 1.f };
//	vecFloatingUp[0].Power = 1.5f;
//	vecFloatingUp[0].fRespawnPosY = 5.f;
//	vecFloatingUp[0].bSmall = true;
//
//	FullName = L"Proto_GameObject_Effect_FloatingUp";
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Item", FullName, &vecFloatingUp[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Item in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Item", pEffect, 30)))
//	{
//		MSGBOX("Falild to Add_Effect_Item in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Box 
//	vecFloatingUp.clear();
//	g_pGameInstance->LoadFile<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC>(vecFloatingUp, L"../bin/SaveData/Effect/Effect_Box.dat");
//
//	vecFloatingUp[0].ParticleColor = { 0.5f,1.0f,0.7f, 1.f };
//	vecFloatingUp[0].Power = 1.5f;
//	vecFloatingUp[0].fRespawnPosY = 5.f;
//	vecFloatingUp[0].bSmall = true;
//
//	FullName = L"Proto_GameObject_Effect_FloatingUp";
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_Box", FullName, &vecFloatingUp[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Box in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Box", pEffect, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_Box in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//EatItem
//	vecFloatingSpeed.clear();
//	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Eat_Item.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating_Speed";
//	vecFloatingSpeed[0].ParticleColor = { 1.f,1.f,0.7f, 1.f };
//	vecFloatingSpeed[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Eat_Item", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Eat_Item in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Eat_Item", pEffect, 16)))
//	{
//		MSGBOX("Falild to Add_Effect_Eat_Item in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	////Hit_Image
//	CEffect_Dead_Spray::IMAGEEFFECTDESC tDesc;
//	ZeroMemory(&tDesc, sizeof(tDesc));
//
//	_tcscpy_s(tDesc.TextureTag, L"T_Hit");
//	tDesc.iRenderPassNum = 1;
//	tDesc.iImageCountX = 4;
//	tDesc.iImageCountY = 2;
//	tDesc.fFrame = 4;
//	tDesc.fEffectPlaySpeed = 20.f;
//	tDesc.fAlpha = 1.f;
//	tDesc.fWeight = 1.f;
//	tDesc.fSize = 3.f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_HitImage", L"Proto_GameObject_Effect_HitImage", &tDesc, (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitImage", pEffect, 16)))
//	{
//		MSGBOX("Falild to Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Death smoke image
//	ZeroMemory(&tDesc, sizeof(tDesc));
//	_tcscpy_s(tDesc.TextureTag, L"Smoke_4x4_1");
//	tDesc.iRenderPassNum = 1;
//	tDesc.iImageCountX = 4;
//	tDesc.iImageCountY = 4;
//	tDesc.fFrame = 16;
//	tDesc.fEffectPlaySpeed = 1.f;
//	tDesc.fAlpha = 0.8f;
//	tDesc.fWeight = 0.f;
//	tDesc.fSize = 6.f;
//
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect_DeadSmoke", L"Proto_GameObject_Effect_HitImage", &tDesc, (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_DeadSmoke", pEffect, 16)))
//	{
//		MSGBOX("Falild to Proto_GameObject_Effect_HitImage in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//
//	//Explosion Rock mesh
//	CExplosion_Rock* pObj = nullptr;
//	CExplosion_Rock::ROCKINFO DescRock;
//	FullName = L"Proto_GameObject_Explosion_Rock";
//	_tcscpy_s(DescRock.ModelTag, L"Model_Explosion_Rock_Up_Anim");
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Up", FullName, &DescRock, (CGameObject**)&pObj)))
//	{
//		MSGBOX("Failed to Creating Effect_Explosion Rock_Up in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Up", pObj, 10)))
//	{
//		MSGBOX("Falild to Add_Effect_Explosion Rock in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//explosion rock left mesh
//	_tcscpy_s(DescRock.ModelTag, L"Model_Explosion_Rock_Left_Anim");
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Left", FullName, &DescRock, (CGameObject**)&pObj)))
//	{
//		MSGBOX("Failed to Creating Effect_Explosion Rock_Left in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Left", pObj, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_Explosion Rock Left in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//explosion rock right mesh
//	_tcscpy_s(DescRock.ModelTag, L"Model_Explosion_Rock_Right_Anim");
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Right", FullName, &DescRock, (CGameObject**)&pObj)))
//	{
//		MSGBOX("Failed to Creating Effect_Explosion Rock_Right in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock_Right", pObj, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_Explosion ROck Right in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//explosion rock mesh
//	_tcscpy_s(DescRock.ModelTag, L"Model_Explosion_Rock_Anim");
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock", FullName, &DescRock, (CGameObject**)&pObj)))
//	{
//		MSGBOX("Failed to Creating Effect_Explosion Rock in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Explosion_Rock", pObj, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_Explosion ROck CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Hammer_Dust image
//	CEffect_Hammer_Dust* pHammer = nullptr;
//	CEffect_Hammer_Dust::EFFECTDESC Desc;
//	ZeroMemory(&Desc, sizeof(Desc));
//	_tcscpy_s(Desc.TextureTag, L"Hammer_Dust_2");
//	Desc.iRenderPassNum = 1;
//	Desc.iImageCountX = 8;
//	Desc.iImageCountY = 4;
//	Desc.fFrame = 32.f;
//	Desc.fEffectPlaySpeed = 1.f;
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hammer_Dust", L"Proto_GameObject_Effect_Hammer_Dust", &Desc, (CGameObject**)&pHammer)))
//	{
//		MSGBOX("Failed to Creating Effect_Hammer_Dust in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hammer_Dust", pHammer, 7)))
//	{
//		MSGBOX("Falild to Add_Effect_Explosion ROck in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Energy
//	vector<CEffect_Energy::EF_PAR_ENERGY_DESC> vecEnergyParticle;
//	g_pGameInstance->LoadFile<CEffect_Energy::EF_PAR_ENERGY_DESC>(vecEnergyParticle, L"../bin/SaveData/Effect/Effect_Energy.dat");
//
//	FullName = L"Proto_GameObject_Effect_Energy";
//	vecEnergyParticle[0].ParticleColor = { 1.f , 0.6f, 0.3f ,1.f };
//	vecEnergyParticle[0].Power = 2.5f;
//
//	//�������� ���� Effect���� �ֱ�
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Energy", FullName, &vecEnergyParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Player_Attack1 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Energy", pEffect, 2)))
//	{
//		MSGBOX("Falild to Add_Effect_Energy in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//EyeRazer 
//	vecHitParticle.clear();
//	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Boss_Razer.dat");
//
//	FullName = L"Proto_GameObject_Effect_Explosion";
//	vecHitParticle[0].ParticleColor = { 1.f , 0.4f, 0.3f ,1.f };
//	vecHitParticle[0].Power = 2.5f;
//
//	//�������� ���� Effect���� �ֱ�
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Razer", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Razer in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Razer", pEffect, 30)))
//	{
//		MSGBOX("Falild to Add Effect_Razer in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Boss_Attack_Ground
//	vecFloatingSpeed.clear();
//	g_pGameInstance->LoadFile<CEffect_Floating_Speed::EF_PAR_FLOATSPEED_DESC>(vecFloatingSpeed, L"../bin/SaveData/Effect/Effect_Boss_Attack_Ground.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating_Speed";
//	vecFloatingSpeed[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
//	vecFloatingSpeed[0].Power = 1.f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Attack_Ground", FullName, &vecFloatingSpeed[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Boss_Attack_Ground in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Attack_Ground", pEffect, 2)))
//	{
//		MSGBOX("Falild to Add_Effect_Boss_Attack_Ground in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Boss_AttackGround_2
//	vecHitFloating.clear();
//	g_pGameInstance->LoadFile<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC>(vecHitFloating, L"../bin/SaveData/Effect/Effect_Boss_Attack_Ground_2.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating";
//	vecHitFloating[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
//	vecHitFloating[0].Power = 0.8f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Attack_Ground_2", FullName, &vecHitFloating[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Boss_Attack_Ground_2 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Attack_Ground_2", pEffect, 3)))
//	{
//		MSGBOX("Falild to Add_Effect_Boss_Attack_Ground_2 in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	////HitGroundSmoke_Boss
//	vecDashEffect.clear();
//	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_HitGround_Smoke_Boss.dat");
//
//	vecDashEffect[0].fAlpha = 0.1f;
//
//	FullName = L"Proto_GameObject_Effect_DashDust";
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGround_Smoke_Boss", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_HitGround_Smoke_Boss in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_HitGround_Smoke_Boss", pEffect, 5)))
//	{
//		MSGBOX("Falild to Add_Effect_HitGround_Smoke_Boss in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	////Boss_Razer_Smoke
//	vecDashEffect.clear();
//	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_Boss_Razer_Smoke.dat");
//
//	vecDashEffect[0].fAlpha = 0.08f;
//
//	FullName = L"Proto_GameObject_Effect_DashDust";
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Razer_Smoke", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Boss_Razer_Smoke in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Razer_Smoke", pEffect, 30)))
//	{
//		MSGBOX("Falild to Add_Effect_Boss_Razer_Smoke in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//boss_HitGround
//	vecHitParticle.clear();
//	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Boss_Hit_Ground.dat");
//
//	FullName = L"Proto_GameObject_Effect_Explosion";
//	vecHitParticle[0].ParticleColor = { 1.f,0.5f,0.2f, 1.f };
//	vecHitParticle[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Hit_Ground", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Boss_Hit_Ground in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Hit_Ground", pEffect, 4)))
//	{
//		MSGBOX("Falild to Add_Effect_Boss_Hit_Ground in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	////Boss_Side_Attack_Smoke
//	vecDashEffect.clear();
//	g_pGameInstance->LoadFile<CEffect_DashDust::EF_PAR_DASH_DESC>(vecDashEffect, L"../bin/SaveData/Effect/Effect_Boss_Side_Attack_Smoke.dat");
//
//	vecDashEffect[0].fAlpha = 0.08f;
//
//	FullName = L"Proto_GameObject_Effect_DashDust";
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Side_Attack_Smoke", FullName, &vecDashEffect[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Boss_Side_Attack_Smoke in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Side_Attack_Smoke", pEffect, 6)))
//	{
//		MSGBOX("Falild to Add_Effect_Boss_Side_Attack_Smoke in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//Boss_Side_Attack
//	vecHitFloating.clear();
//	g_pGameInstance->LoadFile<CEffect_HitFloating::EF_PAR_HITFLOAT_DESC>(vecHitFloating, L"../bin/SaveData/Effect/Effect_Boss_Side_Attack.dat");
//
//	FullName = L"Proto_GameObject_Effect_Floating";
//	vecHitFloating[0].ParticleColor = { 1.f,0.3f,0.3f, 1.f };
//	vecHitFloating[0].Power = 0.7f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Side_Attack", FullName, &vecHitFloating[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Boss_Side_Attack in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Boss_Side_Attack", pEffect, 6)))
//	{
//		MSGBOX("Falild to Add_Effect_Boss_Side_Attack in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//	//BrokenPod
//	vecHitParticle.clear();
//	g_pGameInstance->LoadFile<CEffect_HitParticle::EF_PAR_HIT_DESC>(vecHitParticle, L"../bin/SaveData/Effect/Effect_Broken_Pod.dat");
//
//	FullName = L"Proto_GameObject_Effect_Explosion";
//	vecHitParticle[0].ParticleColor = { 1.f, 0.8f, 0.4f,1.f };
//	vecHitParticle[0].Power = 2.5f;
//
//	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Broken_Pod", FullName, &vecHitParticle[0], (CGameObject**)&pEffect)))
//	{
//		MSGBOX("Failed to Creating Effect_Broken_Pod in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//	if (FAILED(g_pGameInstance->Add_Effect((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Broken_Pod", pEffect, 7)))
//	{
//		MSGBOX("Falild to Add_Effect_Broken_Pod in CStage1::Ready_Effect()");
//		return E_FAIL;
//	}
//
//#pragma endregion
	//����Ʈ�Ŵ��� �ȵ��°� 
	//Stage3 Env Respawn
	CEffect* pEffect = nullptr;
	wstring FullName = L"";
	vector<CEffect_FloatingUp::EF_PAR_FLOATUP_DESC> vecFloatingUp;
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
	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema5_1::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
		return E_FAIL;
	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema5_2::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
		return E_FAIL;
	if (FAILED(m_pCinematicManager->Add_Scenema(CCinema5_3::Create(m_pDevice, m_pDeviceContext, (_uint)SCENEID::SCENE_STAGE3))))
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
