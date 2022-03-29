#include "BoxCollider.h"

CBoxCollider::CBoxCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CCollider(_pDevice, _pDeviceContext)
{
}

CBoxCollider::CBoxCollider(const CBoxCollider& _rhs)
	: CCollider(_rhs)
{
}

HRESULT CBoxCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoxCollider::NativeConstruct(void* _pArg)
{
	DESC tDesc;
	if (_pArg)
	{
		memcpy_s(&tDesc, sizeof(DESC), _pArg, sizeof(DESC));
		m_pGameObject = tDesc.tColliderDesc.pGameObject;
		m_vScale = tDesc.vScale;
	}

	if (FAILED(__super::NativeConstruct(&tDesc.tColliderDesc)))
		return E_FAIL;

	if (FAILED(m_pPhsyX->Create_Box(this)))
		return E_FAIL;

	return S_OK;
}

const _int CBoxCollider::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

const _int CBoxCollider::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

const _float3& CBoxCollider::getScale() const
{
	return m_vScale;
}

CBoxCollider* CBoxCollider::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CBoxCollider* pInstance = new CBoxCollider(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBoxCollider Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CBoxCollider::Clone(void* _pArg)
{
	CBoxCollider* pInstance = new CBoxCollider(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CBoxCollider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoxCollider::Free()
{

	__super::Free();
}
