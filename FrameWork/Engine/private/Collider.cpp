#include "Collider.h"
#include "Transform.h"
#include "PipeLine.h"
#include "GameInstance.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pGizmo(nullptr)
{
	ZeroMemory(&m_matLoaclMatrix, sizeof(_float4x4));
	ZeroMemory(&m_matWorldMatrix, sizeof(_float4x4));
	ZeroMemory(&m_vColor, sizeof(_float4));
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	/*, m_pEffect(rhs.m_pEffect)
	, m_pBatch(rhs.m_pBatch)
	, m_pInputLayout(rhs.m_pInputLayout)*/
	, m_pGizmo(rhs.m_pGizmo)
	, m_vColor(rhs.m_vColor)
	, m_pRigidBody(rhs.m_pRigidBody)
	, m_pShape(rhs.m_pShape)
	, m_eType(rhs.m_eType)
	, m_matLoaclMatrix(rhs.m_matLoaclMatrix)
	, m_matWorldMatrix(rhs.m_matWorldMatrix)
{
	Safe_AddRef(m_pGizmo);
	PX_ADDREF(m_pShape)
}

HRESULT CCollider::NativeConstruct_Prototype()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	m_pGizmo = pInstance->Clone_Component<CGizmo>(0,L"Gizmo");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	m_eType = (*(CPhysicsXSystem::ACTORTYPE*)pArg);

	m_vColor = _float4(1.f, 0.f, 0.f, 1.f);
	return S_OK;
}

void CCollider::Update(_fmatrix TransformMatrix)
{
	_matrix matTransform = XMLoadFloat4x4(&m_matLoaclMatrix);

	matTransform *= TransformMatrix;
	_matrix matScale=Update_Scale(matTransform);

	_matrix matRotate=Update_Rotate(matTransform);

	matScale *= matRotate;

	if (m_eType == CPhysicsXSystem::ACTORTYPE::ACTOR_DYNAMIC)
	{
		PxVec3 pxPos = PxVec3(ToPxVector(matTransform.r[3]));
		_matrix matPos = Update_Position(pxPos);
		matScale *= matPos;
	}
	XMStoreFloat4x4(&m_matWorldMatrix, matScale);
	/*_vector vTransformPos = TransformMatrix.r[3];
	_vector vTransformQuat = getQuaternion(TransformMatrix);

	PxQuat q(XMVectorGetX(vTransformQuat), XMVectorGetY(vTransformQuat), XMVectorGetZ(vTransformQuat), XMVectorGetW(vTransformQuat));
	PxVec3 p(XMVectorGetX(vTransformPos), XMVectorGetY(vTransformPos), XMVectorGetZ(vTransformPos));

	PxTransform pRigidTr = m_pRigidBody->getGlobalPose();
	pRigidTr.q* q;
	pRigidTr.p += p;
	if (m_eType == CPhysicsXSystem::ACTORTYPE::ACTOR_DYNAMIC)
		static_cast<PxRigidDynamic*>(m_pRigidBody)->setGlobalPose(pRigidTr);*/
}

HRESULT CCollider::Render(const wstring& pCameraTag)
{
	return S_OK;
}

void CCollider::Collider()
{
	if (!m_bActive)
	{
		m_isCollision = !m_isCollision;
		if (!m_isCollision)
			m_vColor = _float4(1.f, 0.f, 0.f, 1.f);
		else
			m_vColor = _float4(0.f, 1.f, 0.f, 1.f);
	}
}


HRESULT CCollider::Init_Collider(const CPhysicsXSystem::COLDESC& tDesc)
{
	CPhysicsXSystem* pInstance = GET_INSTANCE(CPhysicsXSystem);

	if (FAILED(pInstance->Init_RigidActor(m_pShape, tDesc, &m_pRigidBody)))
	{
		RELEASE_INSTANCE(CPhysicsXSystem);
		return E_FAIL;
	}
	RELEASE_INSTANCE(CPhysicsXSystem);

	return S_OK;
}

const PxVec3 CCollider::Calcul_Extends(_fmatrix matTransform)
{
	_float fSizeX, fSizeY, fSizeZ;

	fSizeX = XMVectorGetX(XMVector3Length(matTransform.r[0]));
	fSizeY = XMVectorGetY(XMVector3Length(matTransform.r[1]));
	fSizeZ = XMVectorGetZ(XMVector3Length(matTransform.r[2]));

	PxVec3 result = PxVec3(fSizeX, fSizeY, fSizeZ);

	return result;
}

_fvector CCollider::getQuaternion(_fmatrix matTransform)
{
	_vector vRight, vUp, vLook;
	vRight = matTransform.r[0];
	vUp = matTransform.r[1];
	vLook = matTransform.r[2];

	vRight = XMVector3Normalize(vRight);
	vUp = XMVector3Normalize(vUp);
	vLook = XMVector3Normalize(vLook);

	_matrix matRotation;
	ZeroMemory(&matRotation, sizeof(_matrix));

	matRotation.r[0] = vRight;
	matRotation.r[1] = vUp;
	matRotation.r[2] = vLook;

	_vector vQuaternion = XMQuaternionRotationMatrix(matRotation);

	return vQuaternion;
}

_fvector CCollider::ToXMVector3(const PxVec3 pxvec)
{
	_fvector vVector = XMVectorSet(pxvec.x, pxvec.y, pxvec.z,0.f);

	return vVector;
}

_fvector CCollider::ToXMVector4(const PxQuat pxquat)
{
	_fvector vVector = XMVectorSet(pxquat.x, pxquat.y, pxquat.z, pxquat.w);

	return vVector;
}

const PxVec3 CCollider::ToPxVector(_fvector xmvec)
{
	PxVec3 vVector = PxVec3(XMVectorGetX(xmvec), XMVectorGetY(xmvec), XMVectorGetZ(xmvec));

	return vVector;
}

const PxQuat CCollider::ToQuat(_fvector xmvec)
{
	_float4 tmpQuat;
	XMStoreFloat4(&tmpQuat, xmvec);
	PxQuat pxQuat = PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);

	return pxQuat;
}

const _float3 CCollider::QuaternionToEuler(const _float4& _q)
{
	_float3 euler;

	float unit = (_q.x * _q.x) + (_q.y * _q.y) + (_q.z * _q.z) + (_q.w * _q.w);

	float test = _q.x * _q.w - _q.y * _q.z;

	if (test > 0.49999f * unit)
	{
		euler.x = M_PI / 2;
		euler.y = 2.0f * atan2f(_q.y, _q.x);
		euler.z = 0;
	}
	else if (test < -0.49999f * unit)
	{
		euler.x = -M_PI / 2;
		euler.y = -2.0f * atan2f(_q.y, _q.x);
		euler.z = 0;
	}
	else
	{
		euler.x = asinf(2.0f * (_q.w * _q.x - _q.y * _q.z));
		euler.y = atan2f(2.0f * _q.w * _q.y + 2.0f * _q.z * _q.x, 1 - 2.0f * (_q.x * _q.x + _q.y * _q.y));
		euler.z = atan2f(2.0f * _q.w * _q.z + 2.0f * _q.x * _q.y, 1 - 2.0f * (_q.z * _q.z + _q.x * _q.x));
	}

	euler.x = XMConvertToDegrees(euler.x);
	euler.y = XMConvertToDegrees(euler.y);
	euler.z = XMConvertToDegrees(euler.z);

	euler.x = fmodf(euler.x, 360.0f);
	euler.y = fmodf(euler.y, 360.0f);
	euler.z = fmodf(euler.z, 360.0f);

	return euler;
}

_fmatrix CCollider::Update_Scale(_fmatrix matTransform)
{
	_float fSizeX = XMVectorGetX(XMVector3Length(matTransform.r[0]));
	_float fSizeY = XMVectorGetX(XMVector3Length(matTransform.r[1]));
	_float fSizeZ = XMVectorGetX(XMVector3Length(matTransform.r[2]));

	PxGeometryHolder tHolder = m_pShape->getGeometry();
	if (tHolder.getType() == PxGeometryType::eBOX)
	{
		PxBoxGeometry pBox = tHolder.box();
		pBox.halfExtents = PxVec3(fSizeX * 0.5f, fSizeY * 0.5f, fSizeZ * 0.5f);
		m_pShape->setGeometry(pBox);
	}
	else if (tHolder.getType() == PxGeometryType::eCAPSULE)
	{
		tHolder.capsule().halfHeight = fSizeY * 0.5f;
		//	tHolder.capsule().radius = fSizeX * 0.5f;
		//	m_pShape->setGeometry(tHolder.capsule());
	}
	else if (tHolder.getType() == PxGeometryType::eSPHERE)
	{
		tHolder.sphere().radius = fSizeX * 0.5f;
		//	m_pShape->setGeometry(tHolder.sphere());
	}

	_matrix matScale = XMMatrixScaling(fSizeX, fSizeY, fSizeZ);

	return matScale;
	//switch (tHolder.getType())
	//{
	//case PxGeometryType::eBOX :
	//	pGeometry = &tHolder.box();
	//	static_cast<PxBoxGeometry*>(pGeometry)->halfExtents = PxVec3(fSizeX * 0.5f, fSizeY * 0.5f, fSizeZ * 0.5f);
	//	m_pShape->setGeometry(*pGeometry);
	//	break;
	//case PxGeometryType::eCAPSULE:
	//	tHolder.capsule().halfHeight = fSizeY * 0.5f;
	//	tHolder.capsule().radius = fSizeX * 0.5f;
	//	m_pShape->setGeometry(tHolder.capsule());
	//	break;
	//case PxGeometryType::eSPHERE:
	//	tHolder.sphere().radius = fSizeX * 0.5f;
	//	m_pShape->setGeometry(tHolder.sphere());
	//	break;
	//default:
	//	break;
	//}
}

_fmatrix CCollider::Update_Rotate(_fmatrix matTransform)
{
	_vector vTransformQuat = getQuaternion(matTransform);
	PxVec3 pxTransformQuat = ToPxVector(vTransformQuat);
	PxTransform pRigidTr = m_pRigidBody->getGlobalPose();
	pRigidTr.rotate(pxTransformQuat);

	_vector vQuat = ToXMVector4(pRigidTr.q);
	_matrix matRotate = XMMatrixRotationQuaternion(vQuat);

	return matRotate;
}

_fmatrix CCollider::Update_Position(PxVec3 vPos)
{
	_matrix matPos = XMMatrixIdentity();
	PxTransform pRigidTr = m_pRigidBody->getGlobalPose();
	pRigidTr.p= vPos;

	_vector vMatPos = XMVectorSet(pRigidTr.p.x, pRigidTr.p.y, pRigidTr.p.z, 1.f);

	static_cast<PxRigidDynamic*>(m_pRigidBody)->setGlobalPose(pRigidTr);
	matPos.r[3] = vMatPos;
	return matPos;
}

void CCollider::Free()
{
	__super::Free();

	PX_RELEASE(m_pShape);
	if (false == m_isCloned)
		PX_RELEASE(m_pRigidBody);

	Safe_Release(m_pGizmo);
}
