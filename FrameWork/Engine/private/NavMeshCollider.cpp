#include "NavMeshCollider.h"

CNavMeshCollider::CNavMeshCollider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CCollider(_pDevice, _pDeviceContext)
{
}

CNavMeshCollider::CNavMeshCollider(const CNavMeshCollider& _rhs)
	: CCollider(_rhs)
{
}

HRESULT CNavMeshCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavMeshCollider::NativeConstruct(void* _pArg)
{
	DESC tDesc;
	if (_pArg)
	{
		//memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));
		// vector 컨테이너가 있어서 무지성 memcpy 금지!!!!!!!!!!!!!
		tDesc = *(static_cast<DESC*>(_pArg));
		m_pGameObject = tDesc.tColliderDesc.pGameObject;
		m_vecPoints = tDesc.vecPoints;
	}

	if (FAILED(__super::NativeConstruct(&tDesc.tColliderDesc)))
		return E_FAIL;

	if (FAILED(m_pPhsyX->Create_NavMesh(this)))
		return E_FAIL;

	return S_OK;
}

const _int CNavMeshCollider::Tick(const _double& _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

const _int CNavMeshCollider::LateTick(const _double& _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

vector<_float3*>& CNavMeshCollider::getPoints()
{
	return m_vecPoints;
}

CNavMeshCollider* CNavMeshCollider::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CNavMeshCollider* pInstance = new CNavMeshCollider(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CNavMeshCollider Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CNavMeshCollider::Clone(void* _pArg)
{
	CNavMeshCollider* pInstance = new CNavMeshCollider(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CNavMeshCollider Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavMeshCollider::Free()
{

	__super::Free();
}
