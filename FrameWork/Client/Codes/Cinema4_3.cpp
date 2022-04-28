#include "pch.h"
#include "Cinema4_3.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "ScenematicManager.h"

CCinema4_3::CCinema4_3()
	: m_pCam(nullptr)
	, m_pBoss(nullptr)
	, m_pSilvermane(nullptr)
{
}

CCinema4_3::CCinema4_3(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pBoss(nullptr)
	, m_pSilvermane(nullptr)
{
}

HRESULT CCinema4_3::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pSilvermane->AnimSpeed(1.2f);

	m_pBoss->Actor_AnimPlay(4);
	m_pSilvermane->Actor_AnimPlay(4);

	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-180.3f, 52.f, 425.f, 1.f));
	pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(230.f));

	CTransform* pMidBossTr = m_pBoss->Get_Transform();
	pMidBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-180.f, 52.f, 425.f, 1.f));
	pMidBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(240.f));;

	m_pCam->Set_Fov(XMConvertToRadians(54.43f));
	_matrix matPivot = XMMatrixTranslation(-180.f, 52.f, 425.f);
	m_pCam->Set_CameraMatrix(matPivot);

	return S_OK;
}

_int CCinema4_3::Tick(_double dDeltaTime)
{
	_uint iProgress = CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	m_pSilvermane->Tick(dDeltaTime);
	m_pBoss->Tick(dDeltaTime);
	m_pCam->Tick(dDeltaTime);


	return _int();
}

_int CCinema4_3::LateTick(_double dDeltaTime)
{
	if (m_pCam->Get_CamFrame() > 213.f)
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();
	}
	m_pBoss->LateTick(dDeltaTime);
	m_pSilvermane->LateTick(dDeltaTime);
	

	return _int();
}

void CCinema4_3::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);

	m_pBoss->Actor_AnimReset();
	m_pSilvermane->Actor_AnimReset();
	m_pCam->Reset_Camera();

	if (m_bActive)
		m_pCam->Change_CurrentCam();
}

HRESULT CCinema4_3::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema4_3";
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
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA4_3;

	m_pCam = g_pGameInstance->Clone_GameObject<CCinemaCam>((_uint)SCENEID::SCENE_STAGE3, L"Proto_GameObject_CinemaCamera", &tCinemaDesc);
	if (!m_pCam)
	{
		MSGBOX("Came Create Fail");
		return E_FAIL;
	}

	if (FAILED(Ready_Actor(&m_pSilvermane, (_uint)CINEMA_ACTOR::ACTOR_SILVERMANE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pBoss, (_uint)CINEMA_ACTOR::ACTOR_BOSS)))
		return E_FAIL;


	return S_OK;
}

CCinema4_3* CCinema4_3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema4_3* pInstance = new CCinema4_3(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema4_3 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema4_3::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pSilvermane);
	Safe_Release(m_pBoss);
}
