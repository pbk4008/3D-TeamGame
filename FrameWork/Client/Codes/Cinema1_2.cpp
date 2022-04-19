#include "pch.h"
#include "Cinema1_2.h"
#include "CinemaCam.h"
#include "CinemaActor.h"

CCinema1_2::CCinema1_2()
	: m_pCam(nullptr)
	, m_pGrayeHwak(nullptr)
	, m_pPhoenix(nullptr)
	, m_pSilvermane(nullptr)
	, m_bActorAnimOn(false)
{
}

CCinema1_2::CCinema1_2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pGrayeHwak(nullptr)
	, m_pPhoenix(nullptr)
	, m_pSilvermane(nullptr)
	, m_bActorAnimOn(false)
{
}

HRESULT CCinema1_2::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationX(XMConvertToRadians(270.f)) * XMMatrixRotationZ(XMConvertToRadians(270.f)) * XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(3.f, -1.f, 11.f);
	m_pCam->Set_CameraMatrix(matPivot);

	m_pPhoenix->Actor_AnimPlay(1);
	CTransform* pPhoenixTr = m_pPhoenix->Get_Transform();
	pPhoenixTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.f, -1.f, 8.f, 1.f));


	m_pGrayeHwak->Actor_AnimPlay(1);
	CTransform* pGrayeHwakTr = m_pGrayeHwak->Get_Transform();
	pGrayeHwakTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.f, -1.f, 8.f, 1.f));

	m_pSilvermane->Actor_AnimPlay(0);
	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.f, -1.f, 3.f, 1.f));

	return S_OK;
}

_int CCinema1_2::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	_vector vTmp1 = XMVectorSet(3.f, -1.5f, 8.f,1.f);
	_vector vTmp2 = XMVectorSet(3.f,  0.f, 1.f,1.f);
	CTransform* pGrayHwakTr = m_pGrayeHwak->Get_Transform();
	pGrayHwakTr->Set_State(CTransform::STATE_POSITION, vTmp1);

	CTransform* pPhoenixTr = m_pPhoenix->Get_Transform();
	pPhoenixTr->Set_State(CTransform::STATE_POSITION, vTmp1);

	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, vTmp2);

	m_pGrayeHwak->Tick(dDeltaTime);
	m_pPhoenix->Tick(dDeltaTime);
	m_pSilvermane->Tick(dDeltaTime);

	/*_matrix matPivot = XMMatrixRotationX(XMConvertToRadians(270.f)) * XMMatrixRotationZ(XMConvertToRadians(270.f)) * XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(3.f, -1.f, 11.f);
	m_pCam->Set_CameraMatrix(matPivot);*/
	m_pCam->Tick(dDeltaTime);


	//m_bActorAnimOn = m_pCam->Get_Event(30.0);

	if (m_pCam->Get_CamMoveEnd())
		m_bCinemaEnd = true;

	return _int();
}

_int CCinema1_2::LateTick(_double dDeltaTime)
{
	m_pGrayeHwak->LateTick(dDeltaTime);
	m_pPhoenix->LateTick(dDeltaTime);
	m_pSilvermane->LateTick(dDeltaTime);

	return _int();
}

void CCinema1_2::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	m_pGrayeHwak->Actor_AnimReset();
	m_pPhoenix->Actor_AnimReset();
	m_pSilvermane->Actor_AnimReset();
	if (m_bActive)
		m_pCam->Change_CurrentCam();
}

HRESULT CCinema1_2::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema1_2";
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
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA1_2;

	m_pCam=g_pGameInstance->Clone_GameObject<CCinemaCam>((_uint)SCENEID::SCENE_STAGE1, L"Proto_GameObject_CinemaCamera", &tCinemaDesc);
	if (!m_pCam)
	{
		MSGBOX("Came Create Fail");
		return E_FAIL;
	}

	if (FAILED(Ready_Actor(&m_pGrayeHwak, (_uint)CINEMA_ACTOR::ACTOR_GRAYEHAWK)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pPhoenix, (_uint)CINEMA_ACTOR::ACTOR_PHOENIX)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pSilvermane, (_uint)CINEMA_ACTOR::ACTOR_SILVERMANE)))
		return E_FAIL;

	return S_OK;
}

CCinema1_2* CCinema1_2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema1_2* pInstance = new CCinema1_2(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema1_2 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema1_2::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pGrayeHwak);
	Safe_Release(m_pPhoenix);
	Safe_Release(m_pSilvermane);
}
