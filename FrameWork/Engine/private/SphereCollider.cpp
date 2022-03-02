#include "SphereCollider.h"
#include "Gizmo.h"

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


	PxVec3 pxExtends = Calcul_Extends(XMLoadFloat4x4(&tDesc.matTransform));
	if (FAILED(Init_Shape(pxExtends)))
		return E_FAIL;

	tDesc.tColDesc.fPos = _float3(tDesc.matTransform._41, tDesc.matTransform._42, tDesc.matTransform._43);
	if (FAILED(Init_Collider(tDesc.tColDesc)))
		return E_FAIL;

	if (!m_pRigidBody)
		return E_FAIL;

	m_pRigidBody->userData = tDesc.pParent;

	m_matLoaclMatrix = tDesc.matTransform;

	return S_OK;
}

HRESULT CSphereCollider::Render(const wstring& pCameraTag)
{
	if (!m_pGizmo)
		return E_FAIL;

	m_pGizmo->DrawSphere(XMLoadFloat4x4(&m_matWorldMatrix), pCameraTag, XMLoadFloat4(&m_vColor));
	return S_OK;
}

HRESULT CSphereCollider::Init_Shape(PxVec3 pxExtends)
{
	CPhysicsXSystem* pInstance = GET_INSTANCE(CPhysicsXSystem);

	m_pShape = pInstance->Init_Shape(CPhysicsXSystem::COLLIDERTYPE::COL_SPHERE, pxExtends);

	RELEASE_INSTANCE(CPhysicsXSystem);

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
