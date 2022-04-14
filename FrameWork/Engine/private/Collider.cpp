#include "Collider.h"

#include "Transform.h"
#include "GameObject.h"
#include "GameInstance.h"

CCollider::CCollider(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext)
	: CComponent(_pDevice, _pDeviceContext)
	, m_pPhsyX(CPhysicsXSystem::GetInstance())
{
	Safe_AddRef(m_pPhsyX);
	XMStoreFloat4x4(&m_matPivot, XMMatrixIdentity());
}

CCollider::CCollider(const CCollider& _rhs)
	: CComponent(_rhs)
	, m_pPhsyX(_rhs.m_pPhsyX)
	, m_matPivot(_rhs.m_matPivot)
{
	Safe_AddRef(m_pPhsyX);
}

void CCollider::Remove_ActorFromScene()
{
	m_pPhsyX->Remove_Actor(m_pRigidActor);
}

void CCollider::Add_ActorToScene()
{
	m_pPhsyX->Add_Actor(m_pRigidActor);
}

void CCollider::Sleep()
{
	if (ERigidType::Dynamic == m_tDesc.eRigidType)
		static_cast<PxRigidDynamic*>(m_pRigidActor)->putToSleep();
}

void CCollider::WakeUp()
{
	if (ERigidType::Dynamic == m_tDesc.eRigidType)
		static_cast<PxRigidDynamic*>(m_pRigidActor)->wakeUp();
}

void CCollider::Add_Force(_fvector vPow)
{
	_float4 vPos;
	XMStoreFloat4(&vPos, vPow);
	PxVec3 vPower = ToPxVec3(vPos);
	if (m_tDesc.eRigidType == ERigidType::Dynamic)
	{
		static_cast<PxRigidDynamic*>(m_pRigidActor)->addForce(vPower, PxForceMode::eACCELERATION);
	}
}

HRESULT CCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * _pArg)
{
	if (_pArg)
	{
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));
		m_pGameObject = m_tDesc.pGameObject;
	}

	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

const _int CCollider::Tick(const _double& _dDeltaTime)
{
	Update_PxTransform();

	return _int();
}

const _int CCollider::LateTick(const _double& _dDeltaTime)
{
	Update_Transform();

	return _int();
}

const CCollider::DESC& CCollider::getDesc() const
{
	return m_tDesc;
}

CGameObject* CCollider::getGameObject()
{
	return m_pGameObject;
}

void CCollider::setRigidActor(PxRigidActor* _pRigidbody)
{
	m_pRigidActor = _pRigidbody;
}

void CCollider::setShape(PxShape* _pShape)
{
	m_pShape = _pShape;
}

void CCollider::setMaterial(PxMaterial* _pMaterial)
{
	m_pMaterial = _pMaterial;
}

void CCollider::setMass(const _float _fValue)
{
	if (0.f > _fValue || ERigidType::Dynamic != m_tDesc.eRigidType)
		return;

	static_cast<PxRigidBody*>(m_pRigidActor)->setMass(_fValue);
	m_tDesc.fMass = _fValue;
}

void CCollider::setPivotMatrix(const _fmatrix& _smatPivot)
{
	XMStoreFloat4x4(&m_matPivot, _smatPivot);
}

void CCollider::setGravity(const _bool _isGravity)
{
	m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !_isGravity);
	m_tDesc.isGravity = _isGravity;
}

void CCollider::setKinematic(const _bool _isKinematic)
{
	static_cast<PxRigidBody*>(m_pRigidActor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, _isKinematic);
	m_tDesc.isKinematic = _isKinematic;
}

void CCollider::setTrigger(const _bool _isTrigger)
{
	switch (_isTrigger)
	{
	case true:
		m_pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		m_pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		break;
	case false:
		m_pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
		m_pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		break;
	}
	m_tDesc.isTrigger = _isTrigger;
}

void CCollider::setSceneQuery(const _bool _isSceneQuery)
{
	m_pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, _isSceneQuery);
	m_tDesc.isSceneQuery = _isSceneQuery;
}

void CCollider::setShapeLayer(const _uint _iLayer)
{
	if (m_pPhsyX)
		m_pPhsyX->Set_ShapeLayer(m_pShape, _iLayer);
}

_int CCollider::Update_Transform()
{
	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	CTransform* pTransform = m_pGameObject->Get_Transform();
	_float3 vPosition = FromPxVec3(pxTransform.p);
	//_float4 vQuat = FromPxQuat(pxTransform.q);
	//_float3 vRotation = QuaternionToEuler(vQuat);

	pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

	return _int();
}

_int CCollider::Update_PxTransform()
{
	CTransform* pTransform = m_pGameObject->Get_Transform();
	_matrix smatWorld = pTransform->Get_WorldMatrix();
	smatWorld = XMLoadFloat4x4(&m_matPivot) * smatWorld;

	_vector svScale, svQuat, svPos;
	XMMatrixDecompose(&svScale, &svQuat, &svPos, smatWorld);

	// Pos
	_float3 vPosition;
	XMStoreFloat3(&vPosition, svPos);

	// Rot
	_float4 vQuat;
	XMStoreFloat4(&vQuat, svQuat);
	
	PxTransform transform = m_pShape->getLocalPose();
	transform.p = ToPxVec3(vPosition);
	transform.q = ToPxQuat(vQuat);
	m_pRigidActor->setGlobalPose(transform);

	return _int();
}

void CCollider::Free()
{
	if (m_pShape)
		m_pShape->getActor()->detachShape(*m_pShape);
	m_pPhsyX->Remove_Actor(m_pRigidActor);

	Safe_PxRelease(m_pShape);
	Safe_PxRelease(m_pRigidActor);
	Safe_PxRelease(m_pMaterial);

	Safe_Release(m_pPhsyX);
	__super::Free();
}
