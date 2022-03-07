#include "pch.h"
#include "ToolPoint.h"

CToolPoint::CToolPoint()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pTransform(nullptr)
	, m_pCollider(nullptr)
{
}

CToolPoint::CToolPoint(ID3D11Device* pDevcie, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevcie)
	, m_pDeviceContext(pDeviceContext)
	, m_pTransform(nullptr)
	, m_pCollider(nullptr)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CToolPoint::CToolPoint(const CToolPoint& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext)
	, m_pTransform(static_cast<CTransform*>(rhs.m_pTransform->Clone(nullptr)))
	, m_pCollider(static_cast<CCollider*>(rhs.m_pCollider->Clone(nullptr)))
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CToolPoint::Init_Point(_fvector vPos)
{
	_uint iIndex = 0;
	CTransform::TRANSDESC tTransDesc;
	tTransDesc.fRotatePerSec = XMConvertToRadians(10.f);
	tTransDesc.fSpeedPerSec = 5.f;
	m_pTransform=Init_ComDeForm<CTransform>(TOOLSCENEID::TOOL_DEFORM, COMPONENTID::TRANSFORM, &iIndex, &tTransDesc);

	if (!m_pTransform)
		return E_FAIL;

	m_pTransform->setState(CTransform::TRANS_STATE::STATE_POS, vPos);

	CCollider::COLLIDERDESC tColliderDesc;
	ZeroMemory(&tColliderDesc, sizeof(CCollider::COLLIDERDESC));
	tColliderDesc.eType = CCollider::COLLIDER_TYPE::TYPE_SPHERE;
	tColliderDesc.vScale = _float3(1.f, 1.f, 1.f);

	m_pCollider = Init_ComDeForm<CCollider>(TOOLSCENEID::TOOL_DEFORM, COMPONENTID::COLLIDER, &iIndex, &tColliderDesc);
	
	if (!m_pCollider)
		return E_FAIL;

	return S_OK;
}

_int CToolPoint::Update(_float fDeltaTime)
{
	m_pCollider->Update(m_pTransform->getWorldMatrix());

	return 0;
}

HRESULT CToolPoint::Render()
{
	m_pCollider->setUsingTool();
	m_pCollider->Render();

	return S_OK;
}

_fvector CToolPoint::getPos()
{
	return m_pTransform->getState(CTransform::TRANS_STATE::STATE_POS);;
}

_bool CToolPoint::Collision_Point(_fvector vRayPos, _fvector vRayDir)
{
	if (XMVector3Equal(vRayDir, XMVectorZero()))
		return false;

	return m_pCollider->CollisionRay_Sphere(vRayPos, vRayDir);
}

void CToolPoint::Change_Pos(_fvector vPos)
{
	m_pTransform->setState(CTransform::TRANS_STATE::STATE_POS, vPos);
}

CToolPoint* CToolPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _fvector vPos)
{
	CToolPoint* pInstance = new CToolPoint(pDevice, pDeviceContext);
	if (FAILED(pInstance->Init_Point(vPos)))
	{
		MSG_BOX(L"CToolPoint Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CToolPoint* CToolPoint::Clone()
{
	CToolPoint* pInstance = new CToolPoint(*this);
	
	return pInstance;
}

void CToolPoint::Click_Point()
{
	m_pCollider->setColor(XMVectorSet(0.f, 1.f, 0.f, 1.f));
}

void CToolPoint::Reset_Point()
{
	m_pCollider->setColor(XMVectorSet(0.f, 0.f, 0.f, 1.f));
}

void CToolPoint::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
}
