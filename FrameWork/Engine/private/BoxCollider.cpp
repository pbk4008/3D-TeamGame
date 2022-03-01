#include "BoxCollider.h"

CBoxCollider::CBoxCollider()
	: m_pOriginAABB(nullptr)
	, m_pAABB(nullptr)
	, m_pOriginOBB(nullptr)
	, m_pOBB(nullptr)
	, m_eRotateType(ROTATE::ROT_END)
{
}

CBoxCollider::CBoxCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCollider(pDevice, pDeviceContext)
	, m_pOriginAABB(nullptr)
	, m_pAABB(nullptr)
	, m_pOriginOBB(nullptr)
	, m_pOBB(nullptr)
	, m_eRotateType(ROTATE::ROT_END)
{
}

CBoxCollider::CBoxCollider(const CBoxCollider& rhs)
	: CCollider(rhs)
	, m_pOriginAABB(rhs.m_pOriginAABB)
	, m_pAABB(rhs.m_pAABB)
	, m_pOriginOBB(rhs.m_pOriginOBB)
	, m_pOBB(rhs.m_pOBB)
	, m_eRotateType(rhs.m_eRotateType)
{
}

HRESULT CBoxCollider::NativeConstruct_Prototype()
{
	if (FAILED(CCollider::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoxCollider::NativeConstruct(void* pArg)
{
	BOXDESC tDesc = (*(BOXDESC*)pArg);

	m_eRotateType = tDesc.eRot;

	if (FAILED(CCollider::NativeConstruct(&tDesc.tColDesc.eType)))
		return E_FAIL;

	if (FAILED(Init_Shape(ToPxVector(XMLoadFloat3(&tDesc.fExtends)))))
		return E_FAIL;

	if (FAILED(Init_Collider(tDesc.tColDesc)))
		return E_FAIL;

	if (!m_pRigidBody)
		return E_FAIL;

	m_pRigidBody->userData = tDesc.pParent;

	if (FAILED(Init_Box()))
		return E_FAIL;



	return S_OK;
}

void CBoxCollider::Update(_fmatrix TransformMatrix)
{
	CCollider::Update(TransformMatrix);

	_matrix matTransform;
	ZeroMemory(&matTransform, sizeof(_matrix));

	PxBoxGeometry pxBox;
	m_pShape->getBoxGeometry(pxBox);
	_matrix matScale = XMMatrixScaling(pxBox.halfExtents.x, pxBox.halfExtents.y, pxBox.halfExtents.z);

	matTransform = matScale;

	PxTransform pxTransform = m_pRigidBody->getGlobalPose();
	if (m_eRotateType == CBoxCollider::ROTATE::ROT_OBB)
	{
		_vector vQuaternion=ToXMVector4(pxTransform.q);
		_matrix matRotate=XMMatrixRotationQuaternion(vQuaternion);
		matTransform *= matRotate;
	}
	_vector vPos = ToXMVector3(pxTransform.p);
	vPos = XMVectorSetW(vPos, 1.f);
	matTransform.r[3] = vPos;

	if (m_eRotateType == CBoxCollider::ROTATE::ROT_AABB)
		m_pOriginAABB->Transform(*m_pAABB, matTransform);
	else
		m_pOriginAABB->Transform(*m_pAABB, matTransform);
}

HRESULT CBoxCollider::Render(const wstring& pCameraTag)
{
	if (FAILED(CCollider::Render_Begin(pCameraTag)))
		return E_FAIL;

	if (m_eRotateType == CBoxCollider::ROTATE::ROT_AABB)
		DX::Draw(m_pBatch, *m_pAABB, XMLoadFloat4(&m_vColor));

	if (FAILED(CCollider::Render_End()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoxCollider::Init_Shape(PxVec3 pxExtends)
{
	CPhysicsXSystem* pInstance = GET_INSTANCE(CPhysicsXSystem);

	m_pShape = pInstance->Init_Shape(CPhysicsXSystem::COLLIDERTYPE::COL_BOX, pxExtends);

	RELEASE_INSTANCE(CPhysicsXSystem);

	return S_OK;
}

HRESULT CBoxCollider::Init_Box()
{
	PxBoxGeometry Box;
	m_pShape->getBoxGeometry(Box);
	PxTransform tr = m_pRigidBody->getGlobalPose();
	_vector vPos = ToXMVector3(tr.p);
	_float3 fPos, fExtends;
	XMStoreFloat3(&fPos, vPos);
	_vector vExtends = ToXMVector3(Box.halfExtents);
	XMStoreFloat3(&fExtends, vExtends);
	if (m_eRotateType == CBoxCollider::ROTATE::ROT_AABB)
	{
		m_pOriginAABB = new BoundingBox(fPos, fExtends);
		m_pAABB = new BoundingBox;
	}
	else
	{
		_vector vQuat = ToXMVector4(tr.q);
		_float4 fQuat;
		XMStoreFloat4(&fQuat, vQuat);
		m_pOriginOBB = new BoundingOrientedBox(fPos, fExtends, fQuat);
		m_pOBB = new BoundingOrientedBox;
	}

	m_vColor = _float4(1.f, 0.f, 0.f, 1.f);
	return S_OK;
}

CBoxCollider* CBoxCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CBoxCollider* pInstance = new CBoxCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBoxCollider Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CBoxCollider::Clone(void* pArg)
{
	CBoxCollider* pInstance = new CBoxCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBoxCollider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoxCollider::Free()
{
	CCollider::Free();
	Safe_Delete(m_pOriginAABB);
	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOriginOBB);
	Safe_Delete(m_pOBB);
}
