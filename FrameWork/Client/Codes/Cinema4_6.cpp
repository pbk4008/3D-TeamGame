#include "pch.h"
#include "Cinema4_6.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "ScenematicManager.h"

CCinema4_6::CCinema4_6()
	: m_pCam(nullptr)
	, m_pBoss(nullptr)
{
}

CCinema4_6::CCinema4_6(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pBoss(nullptr)
{
}

HRESULT CCinema4_6::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pBoss->Actor_AnimPlay(8);
	CTransform* pBossTr = m_pBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(47.5f, -4.f, 143.f, 1.f));
	pBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));;

	m_pCam->Set_Fov(XMConvertToRadians(48.46f));
	//_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(48.f, -3.f, 150);
	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(47.5f, -4.1f, 143.f);
	m_pCam->Set_CameraMatrix(matPivot);;

	return S_OK;
}

_int CCinema4_6::Tick(_double dDeltaTime)
{
	_uint iProgress = CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	m_pBoss->Actor_AnimPlay(8);
	CTransform* pBossTr = m_pBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(47.5f, -4.f, 143.f, 1.f));
	pBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));;

	m_pCam->Set_Fov(XMConvertToRadians(48.46f));
	//_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(48.f, -3.f, 150);
	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(47.5f, -4.1f, 143.f);
	m_pCam->Set_CameraMatrix(matPivot);;


	m_pBoss->Tick(dDeltaTime);
	m_pCam->Tick(dDeltaTime);


	return _int();
}

_int CCinema4_6::LateTick(_double dDeltaTime)
{
	if (m_pCam->Get_CamMoveEnd())
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();

		list<CGameObject*>* pLayer = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE3, L"Layer_Boss");
		pLayer->front()->setActive(true);

		STOP_SOUND(CHANNEL::BGM);
		PLAY_SOUND(L"Stage3_BGM", CHANNEL::BGM);
		VOLUME_CHANGE(CHANNEL::BGM, 1.1f);

		return 0;
	}
	m_pBoss->LateTick(dDeltaTime);

	return _int();
}

void CCinema4_6::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);

	if (m_bActive)
	{
		m_pBoss->Actor_AnimReset();
		m_pCam->Reset_Camera();
		m_pCam->Change_CurrentCam();
	}
}

HRESULT CCinema4_6::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema4_6";
	tDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	tDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	tDesc.fFovy = XMConvertToRadians(90.f);
	tDesc.fFar = 300.f;
	tDesc.fNear = 0.1f;
	tDesc.fAspect = (_float)g_iWinCx / g_iWinCy;

	CCinemaCam::CINEMADESC tCinemaDesc;

	tCinemaDesc.tCameraDesc = tDesc;
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA4_6;

	m_pCam = g_pGameInstance->Clone_GameObject<CCinemaCam>((_uint)SCENEID::SCENE_STAGE3, L"Proto_GameObject_CinemaCamera", &tCinemaDesc);
	if (!m_pCam)
	{
		MSGBOX("Came Create Fail");
		return E_FAIL;
	}

	if (FAILED(Ready_Actor(&m_pBoss, (_uint)CINEMA_ACTOR::ACTOR_BOSS)))
		return E_FAIL;


	return S_OK;
}

CCinema4_6* CCinema4_6::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema4_6* pInstance = new CCinema4_6(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema4_6 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema4_6::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pBoss);
}
