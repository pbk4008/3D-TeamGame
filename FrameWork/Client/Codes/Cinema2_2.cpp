#include "pch.h"
#include "Cinema2_2.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "CinemaWeapon.h"

#include "ScenematicManager.h"

CCinema2_2::CCinema2_2()
	: m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_bActorAnimOn(false)
	, m_pMidWeapon(nullptr)
{
}

CCinema2_2::CCinema2_2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_bActorAnimOn(false)
	, m_pMidWeapon(nullptr)
{
}

HRESULT CCinema2_2::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pMidBoss->Actor_AnimPlay(6);
	CTransform* pBossTr = m_pMidBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-176.5f, 46.7f, 411.8f, 1.f));;
	pBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(240.f));
	pBossTr->Scaling(XMVectorSet(0.8f, 0.8f, 0.8f, 0.f));

	m_pMidWeapon->Set_FixedBone(m_pMidBoss->Get_Bone("weapon_r"));
	m_pMidWeapon->Set_OwnerPivotMatrix(m_pMidBoss->Get_Pivot());
	m_pMidWeapon->set_OwerMatrix(m_pMidBoss->Get_Transform()->Get_WorldMatrix());

	_matrix matPivot = XMMatrixTranslation(-171.f, 45.8f, 411.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Set_Fov(XMConvertToRadians(60.f));

	return S_OK;
}

_int CCinema2_2::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;
	m_pCam->Set_Fov(XMConvertToRadians(33.4f));
	_matrix matPivot = XMMatrixTranslation(-171.8f, 45.f, 409.f);
	m_pCam->Set_CameraMatrix(matPivot);

	CTransform* pBossTr = m_pMidBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-176.5f, 46.7f, 412.f, 1.f));;

	m_pMidWeapon->set_OwerMatrix(m_pMidBoss->Get_Transform()->Get_WorldMatrix());

	m_pMidBoss->Tick(dDeltaTime);
	m_pCam->Tick(dDeltaTime);
	m_pMidWeapon->Tick(dDeltaTime);

	return _int();
}

_int CCinema2_2::LateTick(_double dDeltaTime)
{
	if (m_pCam->Get_CamMoveEnd())
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();
		CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA2_3);
		RELEASE_INSTANCE(CScenematicManager);
		return 0;
	}
	m_pMidBoss->LateTick(dDeltaTime);
	m_pMidWeapon->LateTick(dDeltaTime);
	return _int();
}

void CCinema2_2::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	m_pMidBoss->Actor_AnimReset();
	m_pMidBoss->AnimSpeed(1.2f);
	m_pCam->Reset_Camera();
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
	if (FAILED(Ready_Weapon(&m_pMidWeapon, 2)))
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
	Safe_Release(m_pMidWeapon);
}
