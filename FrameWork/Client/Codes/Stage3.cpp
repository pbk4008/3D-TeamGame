#include "pch.h"
#include "Stage3.h"
#include "Loading.h"
#include "Silvermane.h"
#include "Environment.h"
#include "TestObj.h"
#include "CapsuleObstacle.h"
#include "MeshEffect_Jupiter.h"
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

	if (FAILED(Ready_MapObject()))
		return E_FAIL;

	//if (FAILED(Ready_Boss(L"Layer_Boss")))
	//	return E_FAIL;

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
		return E_FAIL;

	g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Test", L"Proto_GameObject_TestObject")))
	//	return E_FAIL;

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
	tDesc.vPos = _float3(47.f, 3.f, 28.f);
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
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE3, L"Layer_Boss", L"Proto_GameObject_Solaris")))
		return E_FAIL;
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
