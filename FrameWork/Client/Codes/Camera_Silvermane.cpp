#include "pch.h"
#include "Camera_Silvermane.h"

#include "Silvermane.h"
#include "Camera_Culling.h"
#include "CameraShake.h"
#include "HierarchyNode.h"

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
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CameraShake", CCameraShake::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Silvermane::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
	{
		return E_FAIL;
	}

	m_pSilvermane = static_cast<CSilvermane*>(g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Silvermane")->front());
	m_pSilvermane->Set_Camera(this);

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	// 컬링용 카메라 따로생성
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Camera", L"Proto_GameObject_Camera_Culling", this)))
		return E_FAIL;

	g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");
	m_pRenderer->SetCameraTag(g_pGameInstance->Get_BaseCameraTag());

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

	if (m_isExecution)
	{
		m_fExecutionChangeTime += (_float)_dDeltaTime;

		// Eye
		_matrix smatLocal = m_pEyeBone->Get_CombinedMatrix();
		_matrix smatOwerPivot = m_pSilvermane->Get_Model()->Get_PivotMatrix();
		_vector svEye = (smatLocal * smatOwerPivot).r[3];
		// At
		smatLocal = m_pAtBone->Get_CombinedMatrix();
		_vector svAt = (smatLocal * smatOwerPivot).r[3];
		// Right, Up, Look
		_vector svLook, svRight, svUp;
		svLook = XMVector3Normalize(svAt - svEye);
		svRight = XMVector3Normalize(XMVector3Cross(_vector{ 0.f, 1.f, 0.f, 0.f }, svLook));
		svUp = XMVector3Normalize(XMVector3Cross(svLook, svRight));

		// SetLocalTransform
		m_pLocalTransform->Set_State(CTransform::STATE_RIGHT, svRight);
		m_pLocalTransform->Set_State(CTransform::STATE_UP, svUp);
		m_pLocalTransform->Set_State(CTransform::STATE_LOOK, svLook);
		m_pLocalTransform->Set_State(CTransform::STATE_POSITION, svEye);

		// SetWorldTransform
		_matrix smatOwner = m_pSilvermane->Get_Transform()->Get_WorldMatrix();
		_matrix smatResult = m_pLocalTransform->Get_WorldMatrix()* smatOwner;

		// LerpMatrix
		_float fRatio = m_fExecutionChangeTime / 1.f;
		if (1.f < fRatio)
			fRatio = 1.f;
		_matrix smatWorld = m_pTransform->Get_WorldMatrix();
		smatWorld.r[0] = XMVectorLerp(smatWorld.r[0], smatResult.r[0], fRatio);
		smatWorld.r[1] = XMVectorLerp(smatWorld.r[1], smatResult.r[1], fRatio);
		smatWorld.r[2] = XMVectorLerp(smatWorld.r[2], smatResult.r[2], fRatio);
		smatWorld.r[3] = XMVectorLerp(smatWorld.r[3], smatResult.r[3], fRatio);

		m_pTransform->Set_WorldMatrix(smatWorld);
	}
	else
	{
		if (false == g_pMainApp->IsDeltaTimeZero())
		{
			iProgress = Input_Key(_dDeltaTime);
			if (NO_EVENT != iProgress)
				return iProgress;
		}

		m_fExecutionChangeTime += (_float)_dDeltaTime * 0.2f;
		_float fRatio = m_fExecutionChangeTime / 1.f;
		if (1.f >= fRatio)
		{
			_matrix smatResult = m_pLocalTransform->Get_WorldMatrix() * m_pWorldTransform->Get_WorldMatrix();
			_matrix smatWorld = m_pTransform->Get_WorldMatrix();
			smatWorld.r[0] = XMVectorLerp(smatWorld.r[0], smatResult.r[0], fRatio);
			smatWorld.r[1] = XMVectorLerp(smatWorld.r[1], smatResult.r[1], fRatio);
			smatWorld.r[2] = XMVectorLerp(smatWorld.r[2], smatResult.r[2], fRatio);
			smatWorld.r[3] = XMVectorLerp(smatWorld.r[3], smatResult.r[3], fRatio);

			m_pTransform->Set_WorldMatrix(smatWorld);
		}
		else
			m_pTransform->Set_WorldMatrix(m_pLocalTransform->Get_WorldMatrix() * m_pWorldTransform->Get_WorldMatrix());

		SpringArm();
	}

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
			_float fDeltaTime = (_float)_dDeltaTime;
			if (1.f < fDeltaTime)
				fDeltaTime = 1.f;
			_vector svLerp = XMVectorLerp(svShakeAccPos, XMVectorSet(0.f, 0.f, 0.f, 1.f), fDeltaTime);
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
	m_pLocalTransform->SetUp_Rotation(_vector{ 1.f, 0.f, 0.f, 0.f }, XMConvertToRadians(30.f));

	transformDesc.fRotationPerSec = XMConvertToRadians(120.f);
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Transform", L"Com_WorldTransform", (CComponent**)&m_pWorldTransform, &transformDesc)))
		return E_FAIL;
	m_pWorldTransform->Set_State(CTransform::STATE_POSITION, m_pSilvermane->Get_Transform()->Get_State(CTransform::STATE_POSITION));

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CameraShake", L"Com_CameraShake", (CComponent**)&m_pCameraShake, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Silvermane::Set_ChaseTarget(const _bool _isChase)
{
	m_isChase = _isChase;
}

void CCamera_Silvermane::Set_Execution(const _bool _isExecution, CHierarchyNode* _pEyeBone, CHierarchyNode* _pAtBone)
{
	if (_isExecution != m_isExecution)
	{
		if (_pEyeBone)
			m_pEyeBone = _pEyeBone;
		if (_pAtBone)
			m_pAtBone = _pAtBone;

		switch (_isExecution)
		{
		case true:
			m_fExecutionChangeTime = 0.f;
			break;
		case false:
			m_pLocalTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_vLocalOriginPos), 1.f));
			m_pLocalTransform->SetUp_Rotation(_vector{ 1.f, 0.f, 0.f, 0.f }, XMConvertToRadians(30.f));
			m_fExecutionChangeTime = 0.f;
			break;
		}

		m_isExecution = _isExecution;
	}
}

void CCamera_Silvermane::Respawn()
{
	m_vRot = { 0.f, 0.f, 0.f };
	m_pWorldTransform->SetUp_Rotation(m_vRot);
}

_int CCamera_Silvermane::Chase_Target(const _double& _dDeltaTime)
{
	if (!m_pSilvermane)
		return -1;
	if (!m_isChase)
		return 0;

	_vector svTargetPosition = m_pSilvermane->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_vector svPosition = m_pWorldTransform->Get_State(CTransform::STATE_POSITION);
	_float fRatio = (_float)_dDeltaTime * 12.f;
	if (1.f < fRatio)
		fRatio = 1.f;
	_vector svLerp = XMVectorLerp(svPosition, svTargetPosition, fRatio);
	m_pWorldTransform->Set_State(CTransform::STATE_POSITION, svLerp);

	return _int();
}

_int CCamera_Silvermane::Input_Key(const _double& _dDeltaTime)
{
	_long   MouseMove = 0;
	MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_X);
	if (MouseMove)
	{
		m_vRot.y += MouseMove * 0.05f;// * (_float)_dDeltaTime * 4.f;
		m_pWorldTransform->SetUp_Rotation(m_vRot);
	}

	MouseMove = g_pGameInstance->getMouseMoveState(CInputDev::MOUSEMOVESTATE::MM_Y);
	if (MouseMove)
	{
		//m_pWorldTransform->Rotation_Axis(m_pWorldTransform->Get_State(CTransform::STATE_RIGHT), _dDeltaTime * MouseMove * 0.1f);
		m_vRot.x += MouseMove * 0.05f;// *(_float)_dDeltaTime * 4.f;
		if (20.f < m_vRot.x)
			m_vRot.x = 20.f;
		if (-75.f > m_vRot.x)
			m_vRot.x = -75.f;
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
	tRaycastDesc.layerMask = (1 << (_uint)ELayer::Enviroment);
	if (g_pGameInstance->Raycast(tRaycastDesc))
	{
		for (_uint i = 0; i < tRaycastDesc.iHitNum; ++i)
		{
			if ((_uint)GAMEOBJECT::ENVIRONMENT == tRaycastDesc.vecHitObjects[i]->getTag())
			{
				m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&tRaycastDesc.vecHitPositions[i]), 1.f));
				return;
			}
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

	svRayPos += svRayDir * 6.f;
	SWEEPDESC tSweepDesc;
	tSweepDesc.geometry = PxSphereGeometry(1.f);
	XMStoreFloat3(&tSweepDesc.vOrigin, svRayPos);
	XMStoreFloat3(&tSweepDesc.vDir, svRayDir);
	tSweepDesc.fMaxDistance = 15.f;
	tSweepDesc.filterData.flags = PxQueryFlag::eANY_HIT | PxQueryFlag::eDYNAMIC;
	tSweepDesc.layerMask = (1 << (_uint)ELayer::Monster)/*+ (1 << (_uint)ELayer::Player)*/;
	CGameObject* pHitObject = nullptr;
	tSweepDesc.ppOutHitObject = &pHitObject;

	if (g_pGameInstance->Sweep(tSweepDesc))
	{
		_uint iTag = pHitObject->getTag();
		switch (iTag)
		{
		case (_uint)GAMEOBJECT::MONSTER_CRYSTAL:
		case (_uint)GAMEOBJECT::MONSTER_ABERRANT:
		case (_uint)GAMEOBJECT::MONSTER_1H:
		case (_uint)GAMEOBJECT::MONSTER_2H:
		case (_uint)GAMEOBJECT::MONSTER_HEALER:
		case (_uint)GAMEOBJECT::MONSTER_SHOOTER:
		case (_uint)GAMEOBJECT::MONSTER_SPEAR:
		case (_uint)GAMEOBJECT::MONSTER_ANIMUS:
			if (nullptr != pHitObject)
			{
				m_pTargetMonster = pHitObject;
				static_cast<CActor*>(m_pTargetMonster)->Set_UIShow(true);
			}
			break;
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