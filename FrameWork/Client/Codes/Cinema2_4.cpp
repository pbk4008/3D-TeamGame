#include "pch.h"
#include "Cinema2_4.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "CinemaWeapon.h"
#include "ScenematicManager.h"

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

	m_pMidBoss->Actor_AnimPlay(8);

	CTransform* pBossTr = m_pMidBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-177.f, 52.2f, 410.8f, 1.f));
	pBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(240.f));;

	_matrix matPivot = XMMatrixTranslation(-173.f, 52.f, 410.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Set_Fov(XMConvertToRadians(30.f));

	m_pMidWeapon->Set_FixedBone(m_pMidBoss->Get_Bone("weapon_r"));
	m_pMidWeapon->Set_OwnerPivotMatrix(m_pMidBoss->Get_Pivot());
	m_pMidWeapon->set_OwerMatrix(m_pMidBoss->Get_Transform()->Get_WorldMatrix());

	return S_OK;
}

_int CCinema2_4::Tick(_double dDeltaTime)
{
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	m_pCam->Set_Fov(XMConvertToRadians(33.4f));
	_matrix matPivot = XMMatrixTranslation(-172.9f, 52.2f, 411.1f);
	m_pCam->Set_CameraMatrix(matPivot);

	CTransform* pBossTr = m_pMidBoss->Get_Transform();
	pBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-172.7f, 52.2f, 410.8f, 1.f));;
	pBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(230.f));

	m_pMidWeapon->set_OwerMatrix(m_pMidBoss->Get_Transform()->Get_WorldMatrix());

	m_pMidBoss->Tick(dDeltaTime);
	m_pCam->Tick(dDeltaTime);
	m_pMidWeapon->Tick(dDeltaTime);

	if (!g_pGameInstance->IsPlaying(CHANNEL::Cinema))
	{
		list<CGameObject*>* pList = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_SubTitle");
		for (auto& pSubTitle : *pList)
			pSubTitle->setActive(false);
		g_pGameInstance->StopSound(CHANNEL::Cinema);
	}

	return _int();
}

_int CCinema2_4::LateTick(_double dDeltaTime)
{
	if (m_pCam->Get_CamMoveEnd())
	{
		m_bCinemaEnd = true;
		CGameObject* pBoss = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Boss")->front();
		if (nullptr != pBoss)
		{
			pBoss->setActive(true);
			SHOW_MAPINFO(TRUE, 1); /* 중간보스 HUD */
		}
		/*CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA3_1);
		RELEASE_INSTANCE(CScenematicManager);*/
		return 0;
	}
	m_pMidBoss->LateTick(dDeltaTime);
	m_pCam->LateTick(dDeltaTime);
	m_pMidWeapon->LateTick(dDeltaTime);
	

	return _int();
}

void CCinema2_4::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;
	m_pMidBoss->Actor_AnimReset();
	m_pCam->Reset_Camera();
	m_pMidBoss->AnimSpeed(1.f);;
	m_pCam->Set_AnimSpeed(0.55f);
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
	//ZeroMemory(&tCinemaDesc, sizeof(tCinemaDesc));

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
	if (FAILED(Ready_Weapon(&m_pMidWeapon, 2)))
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
	Safe_Release(m_pMidWeapon);
}
