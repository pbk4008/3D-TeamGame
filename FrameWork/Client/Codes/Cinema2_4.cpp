#include "pch.h"
#include "Cinema2_4.h"
#include "CinemaCam.h"
#include "CinemaActor.h"

CCinema2_4::CCinema2_4()
	: m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_bActorAnimOn(false)
{
}

CCinema2_4::CCinema2_4(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_bActorAnimOn(false)
{
}

HRESULT CCinema2_4::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(-191.f, 52.f, 410.f);
	m_pCam->Set_CameraMatrix(matPivot);

	m_pMidBoss->Actor_AnimPlay(8);
	CTransform* pMidBossTr = m_pMidBoss->Get_Transform();
	pMidBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-172.f, 57.f, 441.5f, 1.f));

	return S_OK;
}

_int CCinema2_4::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	/*CTransform* pBossTr = m_pMidBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-172.f, 57.f, 441.5f, 1.f));*/
	m_pMidBoss->Tick(dDeltaTime);

	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(-191.f, 52.f, 410.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Tick(dDeltaTime);


	if (m_pCam->Get_CamMoveEnd())
		m_bCinemaEnd = true;

	return _int();
}

_int CCinema2_4::LateTick(_double dDeltaTime)
{
	m_pMidBoss->LateTick(dDeltaTime);

	return _int();
}

void CCinema2_4::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	m_pMidBoss->Actor_AnimReset();
	if (m_bActive)
		m_pCam->Change_CurrentCam();
}

HRESULT CCinema2_4::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema2_4";
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
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA2_4;

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

CCinema2_4* CCinema2_4::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema2_4* pInstance = new CCinema2_4(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema1_2 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema2_4::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pMidBoss);
}
