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
	if (_pArg)
	{
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));
		m_pGameObject = m_tDesc.tColliderDesc.pGameObject;
	}

	if (FAILED(__super::NativeConstruct(&m_tDesc.tColliderDesc)))
		return E_FAIL;

	if (FAILED(m_pPhsyX->Create_Capsule(this)))
		return E_FAIL;

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

const CCapsuleCollider::DESC& CCapsuleCollider::getDesc() const
{
	return m_tDesc;
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
