#include "pch.h"
#include "Cinema3_4.h"
#include "CinemaCam.h"
#include "CinemaActor.h"
#include "CinemaWeapon.h"

#include "ScenematicManager.h"

CCinema3_4::CCinema3_4()
	: m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_pSilvermane(nullptr)
	, m_pMidWeapon(nullptr)
	, m_pFloor(nullptr)
{
}

CCinema3_4::CCinema3_4(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CScenematic(pDevice, pDeviceContext)
	, m_pCam(nullptr)
	, m_pMidBoss(nullptr)
	, m_pSilvermane(nullptr)
	, m_pMidWeapon(nullptr)
	, m_pFloor(nullptr)
{
}

HRESULT CCinema3_4::NativeContruct(_uint iSceneID)
{
	if (FAILED(CScenematic::NativeContruct(iSceneID)))
		return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pMidBoss->Actor_AnimPlay(3);
	m_pSilvermane->Actor_AnimPlay(3);
	m_pFloor->Actor_AnimPlay(1);

	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-175.3f, 52.f, 425.f, 1.f));
	pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(230.f));

	CTransform* pMidBossTr = m_pMidBoss->Get_Transform();
	pMidBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-174.5f, 52.f, 425.f, 1.f));
	pMidBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(240.f));;
	
	m_pCam->Set_Fov(XMConvertToRadians(54.43f));
	_matrix matPivot = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixTranslation(-177.f, 60.f, 413.f);
	m_pCam->Set_CameraMatrix(matPivot);

	m_pMidWeapon->Set_FixedBone(m_pMidBoss->Get_Bone("weapon_1"));
	m_pMidWeapon->Set_OwnerPivotMatrix(m_pMidBoss->Get_Pivot());
	m_pMidWeapon->set_OwerMatrix(m_pMidBoss->Get_Transform()->Get_WorldMatrix());

	return S_OK;
}

_int CCinema3_4::Tick(_double dDeltaTime)
{
	_uint iProgress = CScenematic::Tick(dDeltaTime);
	if (iProgress == 1)
		return 0;

	CTransform* pFloorTr = m_pFloor->Get_Transform();
	pFloorTr->Scale_One();
	pFloorTr->Scaling(XMVectorSet(0.7f, 0.7f, 0.7f, 0.f));
	pFloorTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-130.f));

	_matrix matPivot = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixTranslation(-175.f, 53.f, 413.5f);
	m_pCam->Set_CameraMatrix(matPivot);

	CTransform* pMidBossTr = m_pMidBoss->Get_Transform();
	pMidBossTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-174.5f, 52.f, 428.f, 1.f));
	pMidBossTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(240.f));;

	CTransform* pSilvermaneTr = m_pSilvermane->Get_Transform();
	pSilvermaneTr->Set_State(CTransform::STATE_POSITION, XMVectorSet(-175.3f, 52.f, 428.f, 1.f));
	pSilvermaneTr->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(230.f));

	m_pMidWeapon->set_OwerMatrix(m_pMidBoss->Get_Transform()->Get_WorldMatrix());

	m_pSilvermane->Tick(dDeltaTime);
	m_pMidBoss->Tick(dDeltaTime);
	m_pCam->Tick(dDeltaTime);
	m_pMidWeapon->Tick(dDeltaTime);
	m_pFloor->Tick(dDeltaTime);

	return _int();
}

_int CCinema3_4::LateTick(_double dDeltaTime)
{
	if (m_pCam->Get_CamMoveEnd())
	{
		m_bCinemaEnd = true;
		m_pCam->Reset_Camera();
		CScenematicManager* pInstance = GET_INSTANCE(CScenematicManager);
		pInstance->Change_Cinema((_uint)CINEMA_INDEX::CINEMA3_5);
		RELEASE_INSTANCE(CScenematicManager);

		/*list<CGameObject*>* objList = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Floor");

		auto iter = objList->begin();
		advance(iter, 4);
		(*iter)->setActive(true);*/

		return 0;
	}
	m_pMidBoss->LateTick(dDeltaTime);
	m_pSilvermane->LateTick(dDeltaTime);
	m_pMidWeapon->LateTick(dDeltaTime);
	m_pFloor->LateTick(dDeltaTime);

	return _int();
}

void CCinema3_4::Set_Active(_bool bCheck)
{
	CScenematic::Set_Active(bCheck);

	m_pMidBoss->Actor_AnimReset();
	m_pSilvermane->Actor_AnimReset();
	m_pCam->Reset_Camera();

	m_pFloor->Actor_AnimReset();
	if (bCheck)
	{
		list<CGameObject*>* objList = g_pGameInstance->getObjectList((_uint)SCENEID::SCENE_STAGE1, L"Layer_Floor");

		auto iter = objList->begin();
		advance(iter, 4);

		CGameObject* pFloor = (*iter);

		_matrix matWorld = pFloor->Get_Transform()->Get_WorldMatrix();
		CTransform* pFloorTr = m_pFloor->Get_Transform();
		pFloorTr->Set_WorldMatrix(matWorld);

		pFloor->setActive(false);
	}
	if (m_bActive)
		m_pCam->Change_CurrentCam();
}

HRESULT CCinema3_4::Ready_Components()
{
	CCamera::CAMERADESC tDesc;
	tDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	tDesc.pCameraTag = L"Cienema3_4";
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
	tCinemaDesc.iShotTag = (_uint)CINEMA_INDEX::CINEMA3_4;

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
	if (FAILED(Ready_Actor(&m_pFloor, (_uint)CINEMA_ACTOR::ACTOR_FLOOR)))
		return E_FAIL;
	if (FAILED(Ready_Weapon(&m_pMidWeapon, 2)))
		return E_FAIL;


	return S_OK;
}

CCinema3_4* CCinema3_4::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID)
{
	CCinema3_4* pInstance = new CCinema3_4(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeContruct(iSceneID)))
	{
		MSGBOX("Cinema3_4 Crate Faile");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCinema3_4::Free()
{
	CScenematic::Free();
	Safe_Release(m_pCam);
	Safe_Release(m_pSilvermane);
	Safe_Release(m_pMidBoss);
	Safe_Release(m_pMidWeapon);
	Safe_Release(m_pFloor);
}
