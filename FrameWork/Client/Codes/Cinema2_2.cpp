#include "pch.h"
#include "Cinema2_2.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "ScenematicManager.h"

CCinema2_2::CCinema2_2()
	: m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_bActorAnimOn(false)
{
}

CCinema2_2::CCinema2_2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_bActorAnimOn(false)
{
}

HRESULT CCinema2_2::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(-191.f, 41.7f, 408.f);
	m_pCam->Set_CameraMatrix(matPivot);

	m_pMidBoss->Actor_AnimPlay(6);
	CTransform* pMidBossTr = m_pMidBoss->Get_Transform();
	pMidBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-172.f, 57.f, 441.5f, 1.f));

	return S_OK;
}

_int CCinema2_2::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	CTransform* pBossTr = m_pMidBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-176.5f, 46.7f, 411.8f, 1.f));;
	pBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(240.f));
	pBossTr->Scaling(XMVectorSet(0.8f, 0.8f, 0.8f, 0.f));
	//pBossTr->Scale_One();
	m_pMidBoss->Tick(dDeltaTime);

	_matrix matPivot = XMMatrixTranslation(-171.f, 45.8f, 411.f);
	//_matrix matPivot = XMMatrixTranslation(-170.f, 46.f, 380.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Set_Fov(XMConvertToRadians(60.f));
	m_pCam->Tick(dDeltaTime);

	

	return _int();
}

_int CCinema2_2::LateTick(_double dDeltaTime)
{
	m_pMidBoss->LateTick(dDeltaTime);

	if (m_pCam->Get_CamMoveEnd())
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();
		CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA2_3);
		RELEASE_INSTANCE(CScenematicManager);
	}
	return _int();
}

void CCinema2_2::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	m_pMidBoss->Actor_AnimReset();
	if (m_bActive)
		m_pCam->Change_CurrentCam();
}

HRESULT CCinema2_2::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema2_2";
	tDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	tDesc.fFovy = XMConvertToRadians(90.f);
	tDesc.fFar = 300.f;
	tDesc.fNear = 0.1f;
	tDesc.fAspect = (_float)g_iWinCx / g_iWinCy;

	CCinemaCam::CINEMADESC tCinemaDesc;
	ZeroMemory(&tCinemaDesc, sizeof(tCinemaDesc));

	tCinemaDesc.tCameraDesc = tDesc;
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA2_2;

	m_pCam=g_pGameInstance->Clone_GameObject<CCinemaCam>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_CinemaCamera", &tCinemaDesc);
	if (!m_pCam)
	{
		MSGBOX("Came Create Fail");
		return E_FAIL;
	}

	if (FAILED(Ready_Actor(&m_pMidBoss, (_uint)CINEMA_ACTOR::ACTOR_MIDBOSS)))
		return E_FAIL;

	return S_OK;
}

CCinema2_2* CCinema2_2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema2_2* pInstance = new CCinema2_2(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema2_2 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema2_2::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pMidBoss);
}
