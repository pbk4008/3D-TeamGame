#include "BoxCollider.h"
#include "Gizmo.h"


CBoxCollider::CBoxCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CCollider(pDevice, pDeviceContext)
{
}

CBoxCollider::CBoxCollider(const CBoxCollider& rhs)
	: CCollider(rhs)
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

HRESULT CBoxCollider::Render(const wstring& pCameraTag)
{
	if (!m_pGizmo)
		return E_FAIL;

	m_pGizmo->DrawCube(XMLoadFloat4x4(&m_matWorldMatrix), pCameraTag, XMLoadFloat4(&m_vColor));
	
	return S_OK;
}

HRESULT CBoxCollider::Init_Shape(PxVec3 pxExtends)
{
	CPhysicsXSystem* pInstance = GET_INSTANCE(CPhysicsXSystem);

	m_pShape = pInstance->Init_Shape(CPhysicsXSystem::COLLIDERTYPE::COL_BOX, pxExtends);

	RELEASE_INSTANCE(CPhysicsXSystem);

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
}
