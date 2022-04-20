#include "pch.h"
#include "Cinema2_1.h"
#include "CinemaCam.h"
#include "CinemaActor.h"

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

	//_matrix matPivot = XMMatrixRotationX(XMConvertToRadians(270.f)) * XMMatrixRotationZ(XMConvertToRadians(270.f)) * XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(3.f, -1.f, 11.f);
	//m_pCam->Set_CameraMatrix(matPivot);

	m_pSilvermane->Actor_AnimPlay(2);
	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	//pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.f, -1.f, 3.f, 1.f));

	m_pMidBoss->Actor_AnimPlay(0);
	CTransform* pMidBossTr = m_pMidBoss->Get_Transform();
	//pMidBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.f, -1.f, 3.f, 1.f));

	return S_OK;
}

_int CCinema2_1::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	/*CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, vTmp2);*/
	m_pSilvermane->Tick(dDeltaTime);

	//CTransform* pBossTr = m_pMidBoss->Get_Transform();
	//_vector vPos = XMVectorSet(0.f, 0.f, 10.f, 1.f);
	//pBossTr->Set_State(CTransform::STATE_POSITION, vPos);
	m_pMidBoss->Tick(dDeltaTime);

	_matrix matPivot = XMMatrixTranslation(-180.f,45.f,430.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Tick(dDeltaTime);


	if (m_pCam->Get_CamMoveEnd())
		m_bCinemaEnd = true;

	return _int();
}

_int CCinema2_1::LateTick(_double dDeltaTime)
{
	m_pSilvermane->LateTick(dDeltaTime);
	m_pMidBoss->LateTick(dDeltaTime);

	return _int();
}

void CCinema2_1::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	m_pSilvermane->Actor_AnimReset();
	m_pMidBoss->Actor_AnimReset();
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
	ZeroMemory(&tCinemaDesc, sizeof(tCinemaDesc));

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
}
