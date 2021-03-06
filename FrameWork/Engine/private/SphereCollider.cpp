#include "SphereCollider.h"

CSphereCollider::CSphereCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CCollider(_pDevice, _pDeviceContext)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider& _rhs)
	: CCollider(_rhs)
{
}


HRESULT CSphereCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSphereCollider::NativeConstruct(void* _pArg)
{
	DESC tDesc;
	if (_pArg)
	{
		memcpy_s(&tDesc, sizeof(DESC), _pArg, sizeof(DESC));
		m_fRadius = tDesc.fRadius;
	}

	if (FAILED(__super::NativeConstruct(&tDesc.tColliderDesc)))
		return E_FAIL;

	if (FAILED(m_pPhsyX->Create_Sphere(this)))
		return E_FAIL;

	Update_PxTransform();
	return S_OK;
}

const _int CSphereCollider::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

const _int CSphereCollider::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

const _float CSphereCollider::getRadius() const
{
	return m_fRadius;
}

CSphereCollider* CSphereCollider::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CSphereCollider* pInstance = new CSphereCollider(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CSphereCollider Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CSphereCollider::Clone(void* _pArg)
{
	CSphereCollider* pInstance = new CSphereCollider(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CSphereCollider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSphereCollider::Free()
{

	__super::Free();
}
