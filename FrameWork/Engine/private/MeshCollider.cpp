#include "MeshCollider.h"

CMeshCollider::CMeshCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CCollider(_pDevice, _pDeviceContext)
{
}

CMeshCollider::CMeshCollider(const CMeshCollider& _rhs)
	: CCollider(_rhs)
{
}

HRESULT CMeshCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshCollider::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

const _int CMeshCollider::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

const _int CMeshCollider::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

CMeshCollider* CMeshCollider::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMeshCollider* pInstance = new CMeshCollider(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMeshCollider Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMeshCollider::Clone(void* _pArg)
{
	CMeshCollider* pInstance = new CMeshCollider(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CMeshCollider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeshCollider::Free()
{

	__super::Free();
}
