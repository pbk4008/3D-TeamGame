#include "CapsuleCollider.h"

CCapsuleCollider::CCapsuleCollider()
	: m_pOriginUpSphere(nullptr)
	,m_pOriginDownSphere(nullptr)
	,m_pUpSphere(nullptr)
	,m_pDownSphere(nullptr)
	, m_pOriginCapsuleBody(nullptr)
	,m_pCapsuleBody(nullptr)
{
}

CCapsuleCollider::CCapsuleCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCollider(pDevice, pDeviceContext)
	, m_pOriginUpSphere(nullptr)
	, m_pOriginDownSphere(nullptr)
	, m_pUpSphere(nullptr)
	, m_pDownSphere(nullptr)
	, m_pOriginCapsuleBody(nullptr)
	, m_pCapsuleBody(nullptr)
{
}

CCapsuleCollider::CCapsuleCollider(const CCapsuleCollider& rhs)
	: CCollider(rhs)
	, m_pOriginUpSphere(rhs.m_pOriginUpSphere)
	, m_pOriginDownSphere(rhs.m_pOriginDownSphere)
	, m_pUpSphere(rhs.m_pUpSphere)
	, m_pDownSphere(rhs.m_pDownSphere)
	, m_pOriginCapsuleBody(rhs.m_pOriginCapsuleBody)
	, m_pCapsuleBody(rhs.m_pCapsuleBody)
{
}

HRESULT CCapsuleCollider::NativeConstruct_Prototype()
{
	if (FAILED(CCollider::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCapsuleCollider::NativeConstruct(void* pArg)
{
	CAPSULEDESC tDesc = (*(CAPSULEDESC*)pArg);

	if (FAILED(CCollider::NativeConstruct(&tDesc.tColDesc.eType)))
		return E_FAIL;

	if (FAILED(Init_Shape(ToPxVector(XMLoadFloat3(&tDesc.fExtends)))))
		return E_FAIL;

	if (FAILED(Init_Collider(tDesc.tColDesc)))
		return E_FAIL;

	if (!m_pRigidBody)
		return E_FAIL;

	m_pRigidBody->userData = tDesc.pParent;

	if (FAILED(Init_Capsule()))
		return E_FAIL;

	return S_OK;
}

void CCapsuleCollider::Update(_fmatrix TransformMatrix)
{
	CCollider::Update(TransformMatrix);

	_matrix matUpSphereTransform, matDownSphereTransform, matBodyTransform;
	ZeroMemory(&matUpSphereTransform, sizeof(_matrix));
	ZeroMemory(&matDownSphereTransform, sizeof(_matrix));
	ZeroMemory(&matBodyTransform, sizeof(_matrix));

	PxCapsuleGeometry pxCapsule;
	m_pShape->getCapsuleGeometry(pxCapsule);
	PxTransform pxTransform = m_pRigidBody->getGlobalPose();

	///Body
	_matrix matBodyScale = XMMatrixScaling(pxCapsule.radius, pxCapsule.halfHeight*2.f, pxCapsule.radius);

	matBodyTransform = matBodyScale;

	_vector vQuternion = ToXMVector4(pxTransform.q);
	_matrix matBodyRoate = XMMatrixRotationQuaternion(vQuternion);
	matBodyTransform *= matBodyRoate;

	_vector vPos = ToXMVector3(pxTransform.p);
	vPos = XMVectorSetW(vPos,1.f);
	matBodyTransform.r[3] = vPos;

	///UpSphere

	matUpSphereTransform = XMMatrixIdentity();
	
	PxVec3 tmpVec = PxVec3(0.f, pxCapsule.halfHeight, 0.f);
	_vector vUpSpherePos = ToXMVector3(tmpVec);
	vUpSpherePos = XMVectorSetW(vUpSpherePos, 1.f);
	matUpSphereTransform.r[3] = vUpSpherePos;

	matUpSphereTransform *= matBodyTransform;

	///DownSphere
	matDownSphereTransform = XMMatrixIdentity();
	tmpVec = PxVec3(0.f, -pxCapsule.halfHeight, 0.f);
	_vector vDownSpherePos = ToXMVector3(tmpVec);
	vDownSpherePos = XMVectorSetW(vDownSpherePos, 1.f);
	matDownSphereTransform.r[3] = vDownSpherePos;

	matDownSphereTransform *= matBodyTransform;
	
	m_pOriginCapsuleBody->Transform(*m_pCapsuleBody, matBodyTransform);
	m_pOriginDownSphere->Transform(*m_pDownSphere, matDownSphereTransform);
	m_pOriginUpSphere->Transform(*m_pUpSphere, matUpSphereTransform);

}

HRESULT CCapsuleCollider::Render(const wstring& pCameraTag)
{
	if (FAILED(CCollider::Render_Begin(pCameraTag)))
		return E_FAIL;
	DX::Draw(m_pBatch, *m_pUpSphere, XMLoadFloat4(&m_vColor));
	DX::Draw(m_pBatch, *m_pCapsuleBody, XMLoadFloat4(&m_vColor));
	DX::Draw(m_pBatch, *m_pDownSphere, XMLoadFloat4(&m_vColor));
	if (FAILED(CCollider::Render_End()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCapsuleCollider::Init_Shape(PxVec3 pxExtends)
{
	CPhysicsXSystem* pInstance = GET_INSTANCE(CPhysicsXSystem);

	m_pShape = pInstance->Init_Shape(CPhysicsXSystem::COLLIDERTYPE::COL_CAP, pxExtends);

	RELEASE_INSTANCE(CPhysicsXSystem);

	return S_OK;
}

HRESULT CCapsuleCollider::Init_Capsule()
{
	PxCapsuleGeometry pxCapsule;
	m_pShape->getCapsuleGeometry(pxCapsule);

	PxTransform pxTransform = m_pRigidBody->getGlobalPose();
	_vector vPos = ToXMVector3(pxTransform.p);
	_float3 fPos, fExtends;
	XMStoreFloat3(&fPos, vPos);
	fExtends = _float3(pxCapsule.radius*2.f, pxCapsule.halfHeight * 2.f, pxCapsule.radius*2.f);

	/////Body
	_vector vQuat = ToXMVector4(pxTransform.q);
	_float4 fQuat;
	XMStoreFloat4(&fQuat, vQuat);
	m_pOriginCapsuleBody = new BoundingOrientedBox(fPos, fExtends,fQuat);
	m_pCapsuleBody = new BoundingOrientedBox;
	//UpSphere
	_vector vSpherePos = vPos;
	vSpherePos=XMVectorSetY(vSpherePos, fPos.y + pxCapsule.halfHeight);
	_float3 fSpherePos;
	XMStoreFloat3(&fSpherePos, vSpherePos);
	m_pOriginUpSphere = new BoundingSphere(fSpherePos, pxCapsule.radius);
	m_pUpSphere = new BoundingSphere;

	//DownSphere
	vSpherePos = XMVectorSetY(vSpherePos, fPos.y - pxCapsule.halfHeight);
	XMStoreFloat3(&fSpherePos, vSpherePos);
	m_pOriginDownSphere = new BoundingSphere(fSpherePos, pxCapsule.radius);
	m_pDownSphere = new BoundingSphere;


	m_vColor = _float4(1.f, 0.f, 0.f, 1.f);
	return S_OK;
}

CCapsuleCollider* CCapsuleCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CCapsuleCollider* pInstance = new CCapsuleCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCapsuleCollider Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCapsuleCollider::Clone(void* pArg)
{
	CCapsuleCollider* pInstance = new CCapsuleCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCapsuleCollider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCapsuleCollider::Free()
{
	CCollider::Free();

	Safe_Delete(m_pOriginUpSphere);
	Safe_Delete(m_pOriginDownSphere);
	Safe_Delete(m_pUpSphere);
	Safe_Delete(m_pDownSphere);
	Safe_Delete(m_pOriginCapsuleBody);
	Safe_Delete(m_pCapsuleBody);
}
