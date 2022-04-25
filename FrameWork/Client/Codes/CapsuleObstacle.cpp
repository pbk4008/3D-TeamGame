#include "pch.h"
#include "CapsuleObstacle.h"

CCapsuleObstacle::CCapsuleObstacle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CCapsuleObstacle::CCapsuleObstacle(const CCapsuleObstacle& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CCapsuleObstacle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::ENVIRONMENT;

	return S_OK;
}

HRESULT CCapsuleObstacle::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (_pArg)
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CCapsuleObstacle::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	//m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 59.7f, -4.5f, 130.1f, 1.f });
	//m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 58.8f, -4.5f, 131.7f, 1.f });
	//m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CCapsuleObstacle::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	return _int();
}

HRESULT CCapsuleObstacle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG

#endif // _DEBUG

	return S_OK;
}

HRESULT CCapsuleObstacle::Ready_Components()
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
	CCapsuleCollider::DESC tCapsuleColliderDesc;
	tCapsuleColliderDesc.tColliderDesc = tColliderDesc;
	tCapsuleColliderDesc.fHeight = m_tDesc.fHeight;
	tCapsuleColliderDesc.fRadius = m_tDesc.fRadius;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider, &tCapsuleColliderDesc)))
		return E_FAIL;
	_matrix smatPivot = XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, tCapsuleColliderDesc.fHeight * 0.5f, 0.f);
	m_pCollider->setPivotMatrix(smatPivot);
	m_pCollider->Update_PxTransform();

	return S_OK;
}

CCapsuleObstacle* CCapsuleObstacle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCapsuleObstacle* pInstance = new CCapsuleObstacle(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCapsuleObstacle Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCapsuleObstacle::Clone(const _uint _iSceneID, void* _pArg)
{
	CCapsuleObstacle* pInstance = new CCapsuleObstacle(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CCapsuleObstacle Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCapsuleObstacle::Free()
{
	__super::Free();
	Safe_Release(m_pCollider);
}
