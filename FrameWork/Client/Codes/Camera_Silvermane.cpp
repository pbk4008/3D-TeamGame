#include "pch.h"
#include "Camera_Silvermane.h"

#include "Silvermane.h"
#include "Camera_Culling.h"
#include "CameraShake.h"

CCamera_Silvermane::CCamera_Silvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CCamera_Silvermane::CCamera_Silvermane(const CCamera_Silvermane& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCamera_Silvermane::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Prototype(L"Proto_GameObject_Camera_Culling", CCamera_Culling::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if(FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CameraShake", CCameraShake::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Silvermane::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	m_pSilvermane = static_cast<CSilvermane*>(g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Silvermane")->front());
	m_pSilvermane->Set_Camera(this);

	// 컬링용 카메라 따로생성
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Camera", L"Proto_GameObject_Camera_Culling", this)))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Silvermane::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//m_vLocalOriginPos = { 1.f, 5.f, -6.f };
	iProgress = Chase_Target(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	iProgress = Input_Key(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;
	m_pTransform->Set_WorldMatrix(m_pLocalTransform->Get_WorldMatrix() * m_pWorldTransform->Get_WorldMatrix());

	SpringArm();
	OnOffMonsterUI();

	if (m_pCameraShake)
	{
#pragma region 쉐이킹 테스트
		if (g_pGameInstance->getkeyDown(DIK_7))
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::TestX, _float3(0.f, 0.f, 0.f));
		}
		if (g_pGameInstance->getkeyDown(DIK_8))
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::TestY, _float3(0.f, 0.f, 0.f));
		}
		if (g_pGameInstance->getkeyDown(DIK_9))
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::TestZ, _float3(0.f, 0.f, 0.f));
		}
		if (g_pGameInstance->getkeyDown(DIK_4))
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::TestXY, _float3(0.f, 0.f, -2.f));
		}
		if (g_pGameInstance->getkeyDown(DIK_5))
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::TestXZ, _float3(0.f, 0.f, 2.f));
		}
		if (g_pGameInstance->getkeyDown(DIK_6))
		{
			g_pShakeManager->Shake(CShakeManager::ETemplate::TestYZ, _float3(0.f, 0.f, 0.f));
		}
		CCameraShake::SHAKEEVENT tShakeEvent;
		tShakeEvent.fDuration = 2.0f;
		tShakeEvent.fBlendInTime = 0.4f;
		tShakeEvent.fBlendOutTime = 0.4f;
		tShakeEvent.tWaveX.fAmplitude = 0.04f;
		tShakeEvent.tWaveX.fFrequency = 10.f;
		tShakeEvent.tWaveY.fAmplitude = 0.04f;
		tShakeEvent.tWaveY.fFrequency = 6.f;
		tShakeEvent.tWaveZ.fAmplitude = 0.04f;
		tShakeEvent.tWaveZ.fFrequency = 8.f;
		tShakeEvent.fBlendOutTime = 0.3f;
		if (g_pGameInstance->getkeyDown(DIK_RIGHT))
		{
			g_pShakeManager->Shake(tShakeEvent, _float3(0.f, 0.f, 0.f));
		}
#pragma endregion



		m_pCameraShake->Tick(this, _dDeltaTime);
		if (m_pCameraShake->IsShaking())
		{
			_vector svLocalTotalpos = XMVectorSetW(XMLoadFloat3(&m_vLocalOriginPos) + XMLoadFloat3(&m_vShakeAccPos), 1.f);
			m_pLocalTransform->Set_State(CTransform::STATE_POSITION, svLocalTotalpos);
		}
		else
		{
			_vector svShakeAccPos = XMVectorSetW(XMLoadFloat3(&m_vShakeAccPos), 1.f);
			_vector svLerp = XMVectorLerp(svShakeAccPos, XMVectorSet(0.f, 0.f, 0.f, 1.f), (_float)_dDeltaTime);
			_vector svLocalTotalpos = XMVectorSetW(XMLoadFloat3(&m_vLocalOriginPos) + svLerp, 1.f);
			m_pLocalTransform->Set_State(CTransform::STATE_POSITION, svLocalTotalpos);
		}
	}


	m_pCamera->Update_Matrix(m_pTransform->Get_WorldMatrix());
	return _int();
}

_int CCamera_Silvermane::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CCamera_Silvermane::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCamera_Silvermane::Ready_Components()
{
	CCamera::CAMERADESC cameraDesc;
	cameraDesc.pCameraTag = L"Camera_Silvermane";
	cameraDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	cameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	cameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	cameraDesc.vAxisY = _float4(0.f, 1.f, 0.f, 0.f);
	cameraDesc.fFovy = XMConvertToRadians(60.f);
	cameraDesc.fAspect = _float(g_iWinCx) / g_iWinCy;
	cameraDesc.fNear = 0.1f;
	cameraDesc.fFar = 300.f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Camera", L"Com_Camera", (CComponent**)&m_pCamera, &cameraDesc)))
		return E_FAIL;

	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Transform", L"Com_LocalTransform", (CComponent**)&m_pLocalTransform, &transformDesc)))
		return E_FAIL;
	m_vLocalOriginPos = { 1.f, 3.f, -2.f };
	m_pLocalTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vLocalOriginPos), 1.f));
	m_pLocalTransform->SetUp_Rotation(m_pLocalTransform->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(30.f));

	transformDesc.fRotationPerSec = XMConvertToRadians(120.f);
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Transform", L"Com_WorldTransform", (CComponent**)&m_pWorldTransform, &transformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CameraShake", L"Com_CameraShake", (CComponent**)&m_pCameraShake, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Silvermane::Set_ChaseTarget(const _bool _isChase)
{
	m_isChase = _isChase;
}

_int CCamera_Silvermane::Chase_Target(const _double& _dDeltaTime)
{
	if (!m_pSilvermane)
		return -1;
	if (!m_isChase)
		return 0;

	_vector svTargetPosition = m_pSilvermane->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_vector svPosition = m_pWorldTransform->Get_State(CTransform::STATE_POSITION);
	_vector svLerp = XMVectorLerp(svPosition, svTargetPosition, (_float)_dDeltaTime * 12.f);
	m_pWorldTransform->Set_State(CTransform::STATE_POSITION, svLerp);

	return _int();
}

_int CCamera_Silvermane::Input_Key(const _double& _dDeltaTime)
{
	_long   MouseMove = 0;
	MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X);
	if (MouseMove)
	{
		m_vRot.y += MouseMove * (_float)_dDeltaTime * 4.f;
		m_pWorldTransform->SetUp_Rotation(m_vRot);
	}

	MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y);
	if (MouseMove)
	{
		//m_pWorldTransform->Rotation_Axis(m_pWorldTransform->Get_State(CTransform::STATE_RIGHT), _dDeltaTime * MouseMove * 0.1f);
		m_vRot.x += MouseMove * (_float)_dDeltaTime * 4.f;
		if (20.f < m_vRot.x)
			m_vRot.x = 20.f;
		if (-70.f > m_vRot.x)
			m_vRot.x = -70.f;
		m_pWorldTransform->SetUp_Rotation(m_vRot);
	}

	return _int();
}

void CCamera_Silvermane::SpringArm()
{
	CTransform* pTargetTransform = m_pSilvermane->Get_Transform();

	_vector svPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector svTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	svTargetPos += XMVectorSet(0.5f, 1.f, 0.f, 0.f);


	RAYCASTDESC tRaycastDesc;
	XMStoreFloat3(&tRaycastDesc.vOrigin, svTargetPos);
	XMStoreFloat3(&tRaycastDesc.vDir, XMVector3Normalize(svPosition - svTargetPos));
	tRaycastDesc.fMaxDistance = 4.f;
	tRaycastDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC;
	CGameObject* pHitObject = nullptr;
	tRaycastDesc.ppOutHitObject = &pHitObject;
	if (g_pGameInstance->Raycast(tRaycastDesc))
	{
		if ((_uint)GAMEOBJECT::ENVIRONMENT == pHitObject->getTag())
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&tRaycastDesc.vHitPos), 1.f));
		}
	}
}

void CCamera_Silvermane::OnOffMonsterUI()
{
	/*_matrix smatView;
	smatView = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	smatView = XMMatrixInverse(nullptr, smatView);
	if (XMMatrixIsNaN(smatView))
	{
		MSGBOX("Error In  CCamera_Silvermane::OnOffMonsterUI()");
		return;
	}*/

	_matrix mat = m_pTransform->Get_WorldMatrix();

	_vector svRayPos, svRayDir;
	memcpy_s(&svRayDir, sizeof(_vector), &mat.r[2], sizeof(_vector));
	memcpy_s(&svRayPos, sizeof(_vector), &mat.r[3], sizeof(_vector));
	svRayDir = XMVector3Normalize(svRayDir);

	_float fOutDist = 0.f;
	_uint iObjectTag = -1;

	RAYCASTDESC tRaycastDesc;
	XMStoreFloat3(&tRaycastDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tRaycastDesc.vDir, svRayDir);
	tRaycastDesc.fMaxDistance = 15.f;
	tRaycastDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC;
	CGameObject* pHitObject = nullptr;
	tRaycastDesc.ppOutHitObject = &pHitObject;

	if (g_pGameInstance->Raycast(tRaycastDesc))
	{
		if ((_uint)GAMEOBJECT::MONSTER_CRYSTAL == pHitObject->getTag() ||
			(_uint)GAMEOBJECT::MONSTER_ABERRANT == pHitObject->getTag() ||
			(_uint)GAMEOBJECT::MONSTER_1H == pHitObject->getTag() ||
			(_uint)GAMEOBJECT::MONSTER_2H == pHitObject->getTag() ||
			(_uint)GAMEOBJECT::MONSTER_HEALER == pHitObject->getTag() ||
			(_uint)GAMEOBJECT::MONSTER_SHOOTER == pHitObject->getTag() ||
			(_uint)GAMEOBJECT::MONSTER_SPEAR == pHitObject->getTag()
			)
		{
			if (nullptr != pHitObject)
			{
				static_cast<CActor*>(pHitObject)->Set_UIShow(true);
				m_pTargetMonster = pHitObject;
			}
		}

		else
		{
			if (nullptr != pHitObject)
			{
				static_cast<CActor*>(pHitObject)->Set_UIShow(false);
			}
		}
	}
}

const _fvector CCamera_Silvermane::Get_Look() const
{
	return m_pWorldTransform->Get_State(CTransform::STATE_LOOK);
}

CCamera_Silvermane* CCamera_Silvermane::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCamera_Silvermane* pInstance = new CCamera_Silvermane(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCamera_Silvermane Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCamera_Silvermane::Clone(const _uint _iSceneID, void* _pArg)
{
	CCamera_Silvermane* pInstance = new CCamera_Silvermane(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CCamera_Silvermane Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Silvermane::Free()
{
	Safe_Release(m_pLocalTransform);
	Safe_Release(m_pWorldTransform);

	Safe_Release(m_pCamera);
	Safe_Release(m_pCameraShake);

	__super::Free();
}