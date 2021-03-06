#include "CapsuleCollider.h"

CCapsuleCollider::CCapsuleCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CCollider(_pDevice, _pDeviceContext)
{
}

CCapsuleCollider::CCapsuleCollider(const CCapsuleCollider& _rhs)
	: CCollider(_rhs)
{
}

HRESULT CCapsuleCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCapsuleCollider::NativeConstruct(void* _pArg)
{
	DESC tDesc;
	if (_pArg)
	{
		memcpy_s(&tDesc, sizeof(DESC), _pArg, sizeof(DESC));
		m_fHeight = tDesc.fHeight;
		m_fRadius = tDesc.fRadius;
	}

	if (FAILED(__super::NativeConstruct(&tDesc.tColliderDesc)))
		return E_FAIL;

	if (FAILED(m_pPhsyX->Create_Capsule(this)))
		return E_FAIL;

	Update_PxTransform();
	return S_OK;
}

const _int CCapsuleCollider::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

const _int CCapsuleCollider::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

const _float CCapsuleCollider::getHeight() const
{
	return m_fHeight;
}

const _float CCapsuleCollider::getRadius() const
{
	return m_fRadius;
}

CCapsuleCollider* CCapsuleCollider::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCapsuleCollider* pInstance = new CCapsuleCollider(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCapsuleCollider Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CCapsuleCollider::Clone(void* _pArg)
{
	CCapsuleCollider* pInstance = new CCapsuleCollider(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CCapsuleCollider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCapsuleCollider::Free()
{

	__super::Free();
}
