#include "pch.h"
#include "Cinema1_2.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "CinemaWeapon.h"
#include "ScenematicManager.h"

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

	CTransform* pGrayeHwakTr = m_pGrayeHwak->Get_Transform();
	pGrayeHwakTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(4.5f, -1.f, 16.f, 1.f));
	pGrayeHwakTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(300.f));

	CTransform* pPhoenixTr = m_pPhoenix->Get_Transform();
	pPhoenixTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.8f, -1.f, 16.f, 1.f));
	pPhoenixTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));

	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.8f, -1.f, 15.f, 1.f));
	pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));

	m_pGrayeHwak->AnimSpeed(1.25f);;
	m_pPhoenix->AnimSpeed(1.25f);;
	m_pSilvermane->AnimSpeed(1.25f);

	m_pSilvermane->Actor_AnimPlay(9);
	m_pGrayeHwak->Actor_AnimPlay(1);
	m_pPhoenix->Actor_AnimPlay(1);

	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(4.0f, -1.f, 16.f);
	m_pCam->Set_CameraMatrix(matPivot);
	m_pCam->Set_Fov(XMConvertToRadians(54.43f));

	CTransform* pScreeTr = m_pScree0->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.5f, -1.5f, 17.f, 1.f));

	pScreeTr = m_pScree1->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.5f, -1.f, 8.5f, 1.f));

	pScreeTr = m_pScree2->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.75f, -1.5f, 12.f, 1.f));

	pScreeTr = m_pScree3->Get_Transform();
	pScreeTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-30.f));
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(6.5f, -1.f, 15.f, 1.f));

	pScreeTr = m_pScree4->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(4.f, -1.8f, 13.f, 1.f));

	m_pGrayeHwakWeapon->Set_FixedBone(m_pSilvermane->Get_Bone("weapon_l"));
	m_pGrayeHwakWeapon->Set_OwnerPivotMatrix(m_pSilvermane->Get_Pivot());
	m_pGrayeHwakWeapon->set_OwerMatrix(m_pSilvermane->Get_Transform()->Get_WorldMatrix());

	CTransform* pWeaponTr = m_pPhoenixWeapon->Get_Transform();
	pWeaponTr->SetUp_Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	pWeaponTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.8f, -1.f, 16.f, 1.f));

	return S_OK;
}

_int CCinema1_2::Tick(_double dDeltaTime)
{
	m_pCam->Set_Fov(XMConvertToRadians(40.f));
	_uint iProgress=CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixTranslation(4.0f, -1.f, 15.f);
	m_pCam->Set_CameraMatrix(matPivot);

	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.8f, -1.f, 15.f, 1.f));
	pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));



	CTransform* pGrayeHwakTr = m_pGrayeHwak->Get_Transform();
	pGrayeHwakTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(4.27f, -1.f, 13.7f, 1.f));
	pGrayeHwakTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(300.f));

	CTransform* pScreeTr = m_pScree1->Get_Transform();
	pScreeTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.3f, -0.2f, 10.3f, 1.f));

	/*matPivot = XMMatrixRotationX(XMConvertToRadians(90.f))*XMMatrixTranslation(4.5f, 0.5f,14.f);
	m_pPhoenixWeapon->set_OwerMatrix(matPivot);*/

	if (m_pCam->Get_CamFrame() > 620.f)
	{
		pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.8f, -1.f, 14.f, 1.f));
		pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));

		m_pGrayeHwakWeapon->Set_FixedBone(m_pGrayeHwak->Get_Bone("weapon_r"));
		m_pGrayeHwakWeapon->Set_OwnerPivotMatrix(m_pGrayeHwak->Get_Pivot());
		m_pGrayeHwakWeapon->set_OwerMatrix(m_pGrayeHwak->Get_Transform()->Get_WorldMatrix());
	}
	if (m_pCam->Get_CamFrame() > 660.f)
	{
		m_pGrayeHwakWeapon->Set_FixedBone(m_pGrayeHwak->Get_Bone("weapon_l"));
		m_pSilvermane->AnimSpeed(1.43f);

		CTransform* pPhoenixTr = m_pPhoenix->Get_Transform();
		pPhoenixTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.9f, -1.f, 16.f, 1.f));
		pPhoenixTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));

		m_pPhoenix->Tick(dDeltaTime);
	}
	if (m_pCam->Get_CamFrame() > 900.f)
	{
		m_pSilvermane->AnimSpeed(1.08f);
		m_pGrayeHwak->AnimSpeed(1.15f);
		CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
		pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(4.0f, -1.f, 15.f, 1.f));
		pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));
		//pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(30.f));
		
	}

	if (m_pCam->Get_CamFrame() > 1000.f)
	{
		
		//m_pPhoenix->Acotr_AnimFrameSet();
		m_pPhoenixWeapon->Set_FixedBone(m_pPhoenix->Get_Bone("weapon_r"));
		m_pPhoenixWeapon->Set_OwnerPivotMatrix(m_pPhoenix->Get_Pivot());
		_matrix matPivot = m_pPhoenix->Get_Transform()->Get_WorldMatrix();
		//matPivot.
		m_pPhoenixWeapon->set_OwerMatrix(matPivot);
		m_pPhoenixWeapon->Tick(dDeltaTime);

		CTransform* pWeaponTr = m_pPhoenixWeapon->Get_Transform();
		_matrix matWorld = pWeaponTr->Get_WorldMatrix();
		matPivot = XMMatrixRotationX(XMConvertToRadians(-90.f))*matWorld;
		pWeaponTr->Set_WorldMatrix(matPivot);

	}
	else
	{
		m_pPhoenixWeapon->Tick(dDeltaTime);

		CTransform* pWeaponTr = m_pPhoenixWeapon->Get_Transform();
		matPivot = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixTranslation(4.45f, 0.5f, 13.8f);
		pWeaponTr->Set_WorldMatrix(matPivot);
	}
	if (m_pCam->Get_CamFrame() > 1300.f)
		pGrayeHwakTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(6.f, -1.f, 15.f, 1.f));
	

	if (m_pCam->Get_CamFrame() > 1300.f)
		m_pCam->Minus_Fov((_float)(10 * dDeltaTime), XMConvertToRadians(20.f));

	m_pGrayeHwak->Tick(dDeltaTime);
	m_pSilvermane->Tick(dDeltaTime);



	m_pGrayeHwakWeapon->Tick(dDeltaTime);

	m_pScree0->Tick(dDeltaTime);
	m_pScree1->Tick(dDeltaTime);
	m_pScree2->Tick(dDeltaTime);
	m_pScree3->Tick(dDeltaTime);
	m_pScree4->Tick(dDeltaTime);

	m_pCam->Tick(dDeltaTime);

	return _int();
}

_int CCinema1_2::LateTick(_double dDeltaTime)
{
	if (m_pCam->Get_CamMoveEnd())
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();
		CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA1_2);
		RELEASE_INSTANCE(CScenematicManager);
		return 0;
	}
	m_pGrayeHwak->LateTick(dDeltaTime);
	m_pPhoenix->LateTick(dDeltaTime);
	m_pSilvermane->LateTick(dDeltaTime);

	m_pScree0->LateTick(dDeltaTime);
	m_pScree1->LateTick(dDeltaTime);
	m_pScree2->LateTick(dDeltaTime);
	m_pScree3->LateTick(dDeltaTime);
	m_pScree4->LateTick(dDeltaTime);

	m_pPhoenixWeapon->LateTick(dDeltaTime);
	m_pGrayeHwakWeapon->LateTick(dDeltaTime);

	return _int();
}

void CCinema1_2::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);
	m_bActorAnimOn = false;

	m_pSilvermane->AnimSpeed(1.25f);
	m_pGrayeHwak->AnimSpeed(1.25f);;
	m_pPhoenix->AnimSpeed(1.f);
	m_pGrayeHwak->Actor_AnimReset();
	m_pPhoenix->Actor_AnimReset();
	m_pSilvermane->Actor_AnimReset();
	m_pCam->Reset_Camera();
	m_pPhoenix->Acotr_AnimFrameSet(360.0);
	m_pCam->Set_Fov(XMConvertToRadians(40.f));;

	CTransform* pPhoenixTr = m_pPhoenix->Get_Transform();
	pPhoenixTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.9f, -1.f, 18.f, 1.f));
	pPhoenixTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));


	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(3.8f, -1.f, 15.f, 1.f));
	pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));

	m_pGrayeHwakWeapon->Set_FixedBone(m_pSilvermane->Get_Bone("weapon_l"));
	m_pGrayeHwakWeapon->Set_OwnerPivotMatrix(m_pSilvermane->Get_Pivot());
	m_pGrayeHwakWeapon->set_OwerMatrix(m_pSilvermane->Get_Transform()->Get_WorldMatrix());

	m_pPhoenixWeapon->ResetBone();

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
	//ZeroMemory(&tCinemaDesc, sizeof(tCinemaDesc));

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
	if (FAILED(Ready_Actor(&m_pScree0, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree1, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree2, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree3, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;
	if (FAILED(Ready_Actor(&m_pScree4, (_uint)CINEMA_ACTOR::ACTOR_SCREE)))
		return E_FAIL;

	if (FAILED(Ready_Weapon(&m_pPhoenixWeapon, (_uint)CCinemaWeapon::WEAPONTYPE::TYPE_SWORD)))
		return E_FAIL;
	if (FAILED(Ready_Weapon(&m_pGrayeHwakWeapon, (_uint)CCinemaWeapon::WEAPONTYPE::TYPE_SPEAR)))
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

	Safe_Release(m_pScree0);
	Safe_Release(m_pScree1);
	Safe_Release(m_pScree2);
	Safe_Release(m_pScree3);
	Safe_Release(m_pScree4);

	Safe_Release(m_pPhoenixWeapon);
	Safe_Release(m_pGrayeHwakWeapon);
}
