#include "pch.h"
#include "Cinema2_1.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "CinemaWeapon.h"
#include "Subtitles.h"

#include "ScenematicManager.h"

CCinema2_1::CCinema2_1()
	: m_pCam(nullptr)
	, m_pSilvermane(nullptr)
	, m_pMidBoss(nullptr)
	, m_bActorAnimOn(false)
{
}

CCinema2_1::CCinema2_1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pSilvermane(nullptr)
	, m_pMidBoss(nullptr)
	, m_bActorAnimOn(false)
{
}

HRESULT CCinema2_1::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pSilvermane->Actor_AnimPlay(6);
	m_pMidBoss->Actor_AnimPlay(5);

	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-177.f, 53.f, 425.f, 1.f));
	pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(230.f));

	CTransform* pBossTr = m_pMidBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-175.7f, 51.5f, 419.8f, 1.f));
	pBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(240.f));
	pBossTr->Scale_One();
	pBossTr->Scaling(XMVectorSet(0.5f, 0.5f, 0.5f, 0.f));

	_matrix matPivot = XMMatrixTranslation(-172.f, 42.5f, 404.3f);
	m_pCam->Set_CameraMatrix(matPivot);

	m_pMidWeapon->Set_FixedBone(m_pMidBoss->Get_Bone("weapon_r"));
	m_pMidWeapon->Set_OwnerPivotMatrix(m_pMidBoss->Get_Pivot());
	m_pMidWeapon->set_OwerMatrix(m_pMidBoss->Get_Transform()->Get_WorldMatrix());
	m_pMidWeapon->Tick(g_fDeltaTime);


	return S_OK;
}

_int CCinema2_1::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;
	
	m_pCam->Set_Fov(XMConvertToRadians(90.f));
	_matrix matPivot = XMMatrixTranslation(-172.f, 42.5f, 404.3f);
	m_pCam->Set_CameraMatrix(matPivot);


	cout << m_pCam->Get_CamFrame() << endl;

	if (m_pCam->Get_CamFrame() >= 480.f)
	{
		m_pMidBoss->Tick(dDeltaTime);
		m_pMidWeapon->Set_OwnerPivotMatrix(m_pMidBoss->Get_Pivot());
		m_pMidWeapon->set_OwerMatrix(m_pMidBoss->Get_Transform()->Get_WorldMatrix());
		m_pMidWeapon->Tick(dDeltaTime);
	}
	m_pSilvermane->Tick(dDeltaTime);
	m_pCam->Tick(dDeltaTime);

	cout << "Cam : " << m_pCam->Get_CamFrame() << endl;
	if (m_pCam->Get_CamFrame() > 610.f && m_iSubTitleSequence == 0)
	{
		m_iSubTitleSequence = 1;
		g_pGameInstance->StopSound(CHANNEL::Cinema);
		m_pSubTitleMidBoss->setActive(true);
		g_pGameInstance->Play_Shot(L"NoManShall",CHANNEL::Cinema);
	}
	//if (m_pCam->Get_CamFrame() > 100.f && m_iSubTitleSequence==2)
	//	m_iSubTitleSequence = 3;

	return _int();
}

_int CCinema2_1::LateTick(_double dDeltaTime)
{
	if (m_pCam->Get_CamMoveEnd())
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();
		CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA2_2);
		RELEASE_INSTANCE(CScenematicManager);
		return 0;
	}
	m_pSilvermane->LateTick(dDeltaTime);
	m_pMidBoss->LateTick(dDeltaTime);
	m_pMidWeapon->LateTick(dDeltaTime);

	return _int();
}

void CCinema2_1::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	m_pSilvermane->Actor_AnimReset();
	m_pMidBoss->Actor_AnimReset();
	m_pCam->Reset_Camera();
	m_pMidBoss->Acotr_AnimFrameSet(260.0);
	m_pCam->Set_Fov(XMConvertToRadians(90.f));
	m_iSubTitleSequence = 0;
	if (m_bActive)
		m_pCam->Change_CurrentCam();
}

HRESULT CCinema2_1::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema2_1";
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
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA2_1;

	m_pCam=g_pGameInstance->Clone_GameObject<CCinemaCam>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_CinemaCamera", &tCinemaDesc);
	if (!m_pCam)
	{
		MSGBOX("Came Create Fail");
		return E_FAIL;
	}

	if (FAILED(Ready_Actor(&m_pMidBoss, (_uint)CINEMA_ACTOR::ACTOR_MIDBOSS)))
		return E_FAIL;

	if (FAILED(Ready_Actor(&m_pSilvermane, (_uint)CINEMA_ACTOR::ACTOR_SILVERMANE)))
		return E_FAIL;

	if (FAILED(Ready_Weapon(&m_pMidWeapon, 2)))
		return E_FAIL;

	CSubtitles::Desc tTitleDesc;
	tTitleDesc.bUsingCinema = true;

	m_pSubTitleMidBoss = g_pGameInstance->Clone_GameObject<CSubtitles>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_UI_Subtitle", &tTitleDesc);
	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_SubTitle", m_pSubTitleMidBoss);
	Safe_AddRef(m_pSubTitleMidBoss);
	m_pSubTitleMidBoss->SetImage(L"MidBoss_1");

	return S_OK;
}

CCinema2_1* CCinema2_1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema2_1* pInstance = new CCinema2_1(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema1_2 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema2_1::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pSilvermane);
	Safe_Release(m_pMidBoss);
	Safe_Release(m_pMidWeapon);

	Safe_Release(m_pSubTitleMidBoss);
}
