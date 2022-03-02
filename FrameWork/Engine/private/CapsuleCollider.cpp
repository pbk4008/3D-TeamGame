#include "CapsuleCollider.h"
#include "Gizmo.h"

CCapsuleCollider::CCapsuleCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCollider(pDevice, pDeviceContext)
{
}

CCapsuleCollider::CCapsuleCollider(const CCapsuleCollider& rhs)
	: CCollider(rhs)
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

HRESULT CCapsuleCollider::Render(const wstring& pCameraTag)
{
	if (!m_pGizmo)
		return E_FAIL;

	m_pGizmo->DrawCapsule(XMLoadFloat4x4(&m_matWorldMatrix), pCameraTag, XMLoadFloat4(&m_vColor));
	
	return S_OK;
}

HRESULT CCapsuleCollider::Init_Shape(PxVec3 pxExtends)
{
	CPhysicsXSystem* pInstance = GET_INSTANCE(CPhysicsXSystem);

	m_pShape = pInstance->Init_Shape(CPhysicsXSystem::COLLIDERTYPE::COL_CAP, pxExtends);

	RELEASE_INSTANCE(CPhysicsXSystem);

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
}
