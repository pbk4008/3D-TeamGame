#include "pch.h"
#include "Cinema1_1.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "ScenematicManager.h"

CCinema1_1::CCinema1_1()
	: m_pCam(nullptr)
	, m_pGrayeHwak(nullptr)
	, m_pPhoenix(nullptr)
	, m_bActorAnimOn(false)
{
}

CCinema1_1::CCinema1_1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pGrayeHwak(nullptr)
	, m_pPhoenix(nullptr)
	, m_bActorAnimOn(false)
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

	m_pPhoenix->AnimSpeed(2.f);
	m_pPhoenix->Actor_AnimPlay(0);
	CTransform* pPhoenixTr = m_pPhoenix->Get_Transform();
	pPhoenixTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f, -1.f, 15.f, 1.f));


	m_pGrayeHwak->AnimSpeed(2.f);
	m_pGrayeHwak->Actor_AnimPlay(0);
	CTransform* pGrayeHwakTr = m_pGrayeHwak->Get_Transform();
	pGrayeHwakTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -1.f, 13.f, 1.f));

	return S_OK;
}

_int CCinema1_1::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	//CTransform* pPhoenixTr = m_pPhoenix->Get_Transform();
	//pPhoenixTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f, -1.f, 15.f, 1.f));

	//CTransform* pGrayeHwakTr = m_pGrayeHwak->Get_Transform();
	//pGrayeHwakTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, -1.f, 13.f, 1.f));

	m_pGrayeHwak->Tick(dDeltaTime);
	m_pPhoenix->Tick(dDeltaTime);


	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(3.f, -5.f, 13.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Tick(dDeltaTime);
	//m_bActorAnimOn = m_pCam->Get_Event(30.0);

	if (m_pCam->Get_CamFrame() > 850)
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();
		//CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		//pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA1_2);
		//RELEASE_INSTANCE(CScenematicManager);
	}

	return _int();
}

_int CCinema1_1::LateTick(_double dDeltaTime)
{
	m_pGrayeHwak->LateTick(dDeltaTime);
	m_pPhoenix->LateTick(dDeltaTime);

	return _int();
}

void CCinema1_1::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	m_pGrayeHwak->AnimSpeed(2.f);
	m_pPhoenix->AnimSpeed(2.f);
	m_pGrayeHwak->Actor_AnimReset();
	m_pPhoenix->Actor_AnimReset();
	if (m_bActive)
		m_pCam->Change_CurrentCam();
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
	ZeroMemory(&tCinemaDesc, sizeof(tCinemaDesc));

	tCinemaDesc.tCameraDesc = tDesc;
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA1_1;

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
}
