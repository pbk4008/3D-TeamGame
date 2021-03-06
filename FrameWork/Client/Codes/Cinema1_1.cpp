#include "pch.h"
#include "Cinema1_1.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "CinemaWeapon.h"
#include "Wall.h"
#include "Subtitles.h"

#include "ScenematicManager.h"

CCinema1_1::CCinema1_1()
	: m_pCam(nullptr)
	, m_pGrayeHwak(nullptr)
	, m_pPhoenix(nullptr)
	, m_bActorAnimOn(false)
	,m_pScree0(nullptr)
	,m_pScree1(nullptr)
	,m_pScree2(nullptr)
	,m_pScree3(nullptr)
	,m_pScree4(nullptr)
	, m_pGrayHwakSpear(nullptr)
{
}

CCinema1_1::CCinema1_1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pGrayeHwak(nullptr)
	, m_pPhoenix(nullptr)
	, m_bActorAnimOn(false)
	, m_pScree0(nullptr)
	, m_pScree1(nullptr)
	, m_pScree2(nullptr)
	, m_pScree3(nullptr)
	, m_pScree4(nullptr)
	, m_pGrayHwakSpear(nullptr)
{
}

HRESULT CCinema1_1::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(3.f, -5.f, 13.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Set_Fov(XMConvertToRadians(71.51f));

	m_pPhoenix->Actor_AnimPlay(0);
	CTransform* pPhoenixTr = m_pPhoenix->Get_Transform();
	pPhoenixTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(2.f, -1.f, 15.f, 1.f));
	pPhoenixTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));


	m_pGrayeHwak->Actor_AnimPlay(0);
	CTransform* pGrayeHwakTr = m_pGrayeHwak->Get_Transform();
	pGrayeHwakTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -1.5f, 13.f, 1.f));

	m_pGrayeHwak->AnimSpeed(2.f);
	m_pPhoenix->AnimSpeed(2.f);

	m_pScree0->Actor_AnimPlay(0);
	m_pScree1->Actor_AnimPlay(1);
	m_pScree2->Actor_AnimPlay(2);
	m_pScree3->Actor_AnimPlay(3);
	m_pScree4->Actor_AnimPlay(4);

	matPivot = XMMatrixIdentity();
	m_pGrayHwakSpear->set_OwerMatrix(matPivot);

	CTransform* pScreeTr = m_pScree0->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.5f, -1.5f, 17.f, 1.f));

	pScreeTr = m_pScree1->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.5f, -1.f, 8.5f, 1.f));

	pScreeTr = m_pScree2->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.75f, -1.5f, 12.f, 1.f));

	pScreeTr = m_pScree3->Get_Transform();
	pScreeTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-30.f));
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(6.5f, -1.f, 15.f, 1.f));

	pScreeTr = m_pScree4->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(4.f, -1.8f, 13.f, 1.f));
	
	return S_OK;
}

_int CCinema1_1::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	m_pGrayeHwak->Tick(dDeltaTime);
	m_pPhoenix->Tick(dDeltaTime);

	m_pScree0->Tick(dDeltaTime);
	m_pScree1->Tick(dDeltaTime);
	m_pScree2->Tick(dDeltaTime);
	m_pScree3->Tick(dDeltaTime);;
	m_pScree4->Tick(dDeltaTime);

	m_pCam->Tick(dDeltaTime);;

	m_pGrayHwakSpear->Tick(dDeltaTime);

	CTransform* pWeaponTr = m_pGrayHwakSpear->Get_Transform();

	pWeaponTr->SetUp_Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	pWeaponTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(2.f, -0.5f, 10.5f, 1.f));
	
	if (m_pCam->Get_CamFrame() > 200.f && m_iSubTitleSequence == 0)
	{
		m_iSubTitleSequence = 1;
		m_pSubTitleGrayeHwak->setActive(true);
		g_pGameInstance->Play_Shot(L"ThisWontBe", CHANNEL::Cinema);

	}
	if(!IS_PLAYING(CHANNEL::Cinema)&&m_iSubTitleSequence == 1)
	{
		m_iSubTitleSequence = 2;
		g_pGameInstance->StopSound(CHANNEL::Cinema);
		m_pSubTitleGrayeHwak->setActive(false);
	}
	if (m_pCam->Get_CamFrame() > 630.f && m_iSubTitleSequence==2)
	{
		m_iSubTitleSequence = 3;
		m_pSubTitlePhoenix->setActive(true);
		g_pGameInstance->Play_Shot(L"IfAnyoneCan", CHANNEL::Cinema);
	}

	return _int();
}

_int CCinema1_1::LateTick(_double dDeltaTime)
{
	cout << m_pCam->Get_CamFrame() << endl;
	if (m_pCam->Get_CamFrame() > 650)
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();

		CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA1_2);
		RELEASE_INSTANCE(CScenematicManager);
		return 0;

	}
	m_pGrayeHwak->LateTick(dDeltaTime);
	m_pPhoenix->LateTick(dDeltaTime);

	m_pScree0->LateTick(dDeltaTime);
	m_pScree1->LateTick(dDeltaTime);
	m_pScree2->LateTick(dDeltaTime);
	m_pScree3->LateTick(dDeltaTime);
	m_pScree4->LateTick(dDeltaTime);

	m_pGrayHwakSpear->LateTick(dDeltaTime);

	return _int();
}

void CCinema1_1::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	
	m_pGrayeHwak->Actor_AnimReset();
	m_pPhoenix->Actor_AnimReset();
	m_pCam->Reset_Camera();

	m_iSubTitleSequence = 0;
	if (m_bActive)
	{
		g_pGameInstance->StopSound(CHANNEL::Cinema);
		m_pSubTitleGrayeHwak->setActive(false);
		m_pSubTitlePhoenix->setActive(false);
		m_pCam->Change_CurrentCam();
	}
		//m_pCam->Change_CurrentCam();
}

HRESULT CCinema1_1::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema1_1";
	tDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	tDesc.fFovy = XMConvertToRadians(90.f);
	tDesc.fFar = 300.f;
	tDesc.fNear = 0.1f;
	tDesc.fAspect = (_float)g_iWinCx / g_iWinCy;

	CCinemaCam::CINEMADESC tCinemaDesc;
	//ZeroMemory(&tCinemaDesc, sizeof(tCinemaDesc));

	tCinemaDesc.tCameraDesc = tDesc;
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA1_1;

	m_pCam = g_pGameInstance->Clone_GameObject<CCinemaCam>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_CinemaCamera", &tCinemaDesc);

	if (!m_pCam)
	{
		MSGBOX("Came Create Fail");
		return E_FAIL;
	}

	if (FAILED(Ready_Actor(&m_pGrayeHwak, (_uint)CINEMA_ACTOR::ACTOR_GRAYEHAWK)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pPhoenix, (_uint)CINEMA_ACTOR::ACTOR_PHOENIX)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree0, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree1, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree2, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree3, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree4, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;

	if (FAILED(Ready_Weapon(&m_pGrayHwakSpear, 0)))
		return E_FAIL;

	
	CSubtitles::Desc tTitleDesc;
	tTitleDesc.bUsingCinema = true;

	m_pSubTitlePhoenix=g_pGameInstance->Clone_GameObject<CSubtitles>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_UI_Subtitle", &tTitleDesc);
	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_SubTitle", m_pSubTitlePhoenix);
	Safe_AddRef(m_pSubTitlePhoenix);
	m_pSubTitlePhoenix->SetImage(L"Revena_1");

	m_pSubTitleGrayeHwak = g_pGameInstance->Clone_GameObject<CSubtitles>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_UI_Subtitle", &tTitleDesc);
	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_SubTitle", m_pSubTitleGrayeHwak);
	Safe_AddRef(m_pSubTitleGrayeHwak);
	m_pSubTitleGrayeHwak->SetImage(L"Soras_1");

	return S_OK;
}

CCinema1_1* CCinema1_1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema1_1* pInstance = new CCinema1_1(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema1_1 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema1_1::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pGrayeHwak);
	Safe_Release(m_pPhoenix);

	Safe_Release(m_pScree0);
	Safe_Release(m_pScree1);
	Safe_Release(m_pScree2);
	Safe_Release(m_pScree3);
	Safe_Release(m_pScree4);

	Safe_Release(m_pGrayHwakSpear);

	Safe_Release(m_pSubTitleGrayeHwak);
	Safe_Release(m_pSubTitlePhoenix);
}
