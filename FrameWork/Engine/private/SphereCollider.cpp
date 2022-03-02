#include "SphereCollider.h"

CSphereCollider::CSphereCollider()
	: m_pOriginSphere(nullptr)
	, m_pSphere(nullptr)
{
}

CSphereCollider::CSphereCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCollider(pDevice, pDeviceContext)
	, m_pOriginSphere(nullptr)
	, m_pSphere(nullptr)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
	: CCollider(rhs)
	, m_pOriginSphere(rhs.m_pOriginSphere)
	, m_pSphere(rhs.m_pSphere)
{
}

HRESULT CSphereCollider::NativeConstruct_Prototype()
{
	if (FAILED(CCollider::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSphereCollider::NativeConstruct(void* pArg)
{
	SPHEREDESC tDesc = (*(SPHEREDESC*)pArg);

	if (FAILED(CCollider::NativeConstruct(&tDesc.tColDesc.eType)))
		return E_FAIL;

	if (FAILED(Init_Shape(PxVec3(tDesc.fRadius, tDesc.fRadius, tDesc.fRadius))))
		return E_FAIL;

	if (FAILED(Init_Collider(tDesc.tColDesc)))
		return E_FAIL;

	if (!m_pRigidBody)
		return E_FAIL;

	m_pRigidBody->userData = tDesc.pParent;

	if (FAILED(Init_Sphere()))
		return E_FAIL;
	return S_OK;
}

void CSphereCollider::Update(_fmatrix TransformMatrix)
{
	CCollider::Update(TransformMatrix);

	_matrix matTransform;
	ZeroMemory(&matTransform, sizeof(_matrix));

	PxSphereGeometry pxSphere;
	m_pShape->getSphereGeometry(pxSphere);
	_matrix matScale = XMMatrixScaling(pxSphere.radius, pxSphere.radius, pxSphere.radius);

	matTransform = matScale;

	PxTransform pxTransform= m_pRigidBody->getGlobalPose();
	_vector vPos = ToXMVector3(pxTransform.p);
	matTransform.r[3] = vPos;

	m_pOriginSphere->Transform(*m_pSphere, matTransform);
}

HRESULT CSphereCollider::Render(const wstring& pCameraTag)
{
	if (FAILED(CCollider::Render_Begin(pCameraTag)))
		return E_FAIL;

	DX::Draw(m_pBatch, *m_pSphere, XMLoadFloat4(&m_vColor));

	if (FAILED(CCollider::Render_End()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSphereCollider::Init_Shape(PxVec3 pxExtends)
{
	CPhysicsXSystem* pInstance = GET_INSTANCE(CPhysicsXSystem);

	m_pShape = pInstance->Init_Shape(CPhysicsXSystem::COLLIDERTYPE::COL_SPHERE, pxExtends);

	RELEASE_INSTANCE(CPhysicsXSystem);

	return S_OK;
}

HRESULT CSphereCollider::Init_Sphere()
{
	PxSphereGeometry pxSphere;
	m_pShape->getSphereGeometry(pxSphere);
	PxTransform pxTransform = m_pRigidBody->getGlobalPose();
	_vector vPos = ToXMVector3(pxTransform.p);
	_float3 fPos;
	XMStoreFloat3(&fPos, vPos);
	_float fRadius = pxSphere.radius;
	m_pOriginSphere = new BoundingSphere(fPos, fRadius);
	m_pSphere = new BoundingSphere;

	m_vColor = _float4(1.f, 0.f, 0.f, 1.f);

	return S_OK;
}

CSphereCollider* CSphereCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CSphereCollider* pInstance = new CSphereCollider(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CSphere Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CSphereCollider::Clone(void* pArg)
{
	CSphereCollider* pInstance = new CSphereCollider(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CSphere Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSphereCollider::Free()
{
	CCollider::Free();
	Safe_Delete(m_pOriginSphere);
	Safe_Delete(m_pSphere);
}
