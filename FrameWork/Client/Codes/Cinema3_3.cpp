#include "pch.h"
#include "Cinema3_3.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "ScenematicManager.h"

CCinema3_3::CCinema3_3()
	: m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_pSilvermane(nullptr)
{
}

CCinema3_3::CCinema3_3(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_pSilvermane(nullptr)
{
}

HRESULT CCinema3_3::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	_matrix matPivot = XMMatrixTranslation(-177.f, 55.f, 425.f);
	m_pCam->Set_CameraMatrix(matPivot);

	m_pSilvermane->AnimSpeed(2.f);
	m_pSilvermane->Actor_AnimPlay(2);
	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f, -1.f, 15.f, 1.f));


	m_pMidBoss->AnimSpeed(2.f);
	m_pMidBoss->Actor_AnimPlay(2);
	CTransform* pMidBossTr = m_pMidBoss->Get_Transform();
	pMidBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -1.f, 13.f, 1.f));

	return S_OK;
}

_int CCinema3_3::Tick(_double dDeltaTime)
{
	_uint iProgress = CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-175.3f, 52.f, 425.f, 1.f));
	pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(230.f));

	CTransform* pMidBossTr = m_pMidBoss->Get_Transform();
	pMidBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-174.5f, 52.f, 425.f, 1.f));
	pMidBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(240.f));;

	m_pSilvermane->Tick(dDeltaTime);
	m_pMidBoss->Tick(dDeltaTime);


	m_pCam->Set_Fov(XMConvertToRadians(43.6f));
	_matrix matPivot = XMMatrixTranslation(-175.f, 52.f, 425.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Tick(dDeltaTime);
	//m_bActorAnimOn = m_pCam->Get_Event(30.0);

	

	return _int();
}

_int CCinema3_3::LateTick(_double dDeltaTime)
{
	m_pMidBoss->LateTick(dDeltaTime);
	m_pSilvermane->LateTick(dDeltaTime);

	if (m_pCam->Get_CamMoveEnd())
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();
		CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA3_4);
		RELEASE_INSTANCE(CScenematicManager);
	}
	return _int();
}

void CCinema3_3::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_pMidBoss->AnimSpeed(1.f);
	m_pSilvermane->AnimSpeed(1.f);

	m_pMidBoss->Actor_AnimPlay(2);
	m_pSilvermane->Actor_AnimPlay(2);

	m_pMidBoss->Actor_AnimReset();
	m_pSilvermane->Actor_AnimReset();

	if (m_bActive)
		m_pCam->Change_CurrentCam();
}

HRESULT CCinema3_3::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema3_3";
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
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA3_3;

	m_pCam = g_pGameInstance->Clone_GameObject<CCinemaCam>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_CinemaCamera", &tCinemaDesc);
	if (!m_pCam)
	{
		MSGBOX("Came Create Fail");
		return E_FAIL;
	}

	if (FAILED(Ready_Actor(&m_pSilvermane, (_uint)CINEMA_ACTOR::ACTOR_SILVERMANE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pMidBoss, (_uint)CINEMA_ACTOR::ACTOR_MIDBOSS)))
		return E_FAIL;


	return S_OK;
}

CCinema3_3* CCinema3_3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema3_3* pInstance = new CCinema3_3(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema3_3 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema3_3::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pSilvermane);
	Safe_Release(m_pMidBoss);
}
