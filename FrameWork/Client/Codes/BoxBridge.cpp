#include "pch.h"
#include "BoxBridge.h"

CBoxBridge::CBoxBridge(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CBoxBridge::CBoxBridge(const CBoxBridge& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CBoxBridge::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::ENVIRONMENT;

	return S_OK;
}

HRESULT CBoxBridge::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (_pArg)
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CBoxBridge::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CBoxBridge::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CBoxBridge::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG

#endif // _DEBUG

	return S_OK;
}

HRESULT CBoxBridge::Ready_Components()
{
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_tDesc.vPosition), 1.f));
	m_pTransform->SetUp_Rotation(m_tDesc.vRotation);

	CCollider::DESC tColliderDesc;
	tColliderDesc.eRigidType = ERigidType::Static;
	tColliderDesc.isSceneQuery = true;
	tColliderDesc.pGameObject = this;
	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { m_tDesc.vScale.x, m_tDesc.vScale.y, m_tDesc.vScale.z };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	_matrix smatPivot = XMMatrixTranslation(0.f, -tBoxColliderDesc.vScale.y * 0.5f, 0.f);
	m_pCollider->setPivotMatrix(smatPivot);
	m_pCollider->Update_PxTransform();

	return S_OK;
}

CBoxBridge* CBoxBridge::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CBoxBridge* pInstance = new CBoxBridge(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBoxBridge Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBoxBridge::Clone(const _uint _iSceneID, void* _pArg)
{
	CBoxBridge* pInstance = new CBoxBridge(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CBoxBridge Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoxBridge::Free()
{
	__super::Free();
	Safe_Release(m_pCollider);
}
