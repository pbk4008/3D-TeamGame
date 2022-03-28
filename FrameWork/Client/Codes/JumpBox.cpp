#include "pch.h"
#include "JumpBox.h"

CJumpBox::CJumpBox(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CJumpBox::CJumpBox(const CJumpBox& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CJumpBox::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::JUMP_BOX;

	return S_OK;
}

HRESULT CJumpBox::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (_pArg)
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

_int CJumpBox::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_isDisable)
	{
		m_fDisableTime += (_float)_dDeltaTime;

		if (1.5f < m_fDisableTime)
		{
			m_pCollider->setSceneQuery(true);
			m_isDisable = false;
			m_fDisableTime = 0.f;
		}
	}

	m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CJumpBox::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;
	return _int();
}

HRESULT CJumpBox::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG

#endif // _DEBUG

	return S_OK;
}

HRESULT CJumpBox::Ready_Components()
{
	CTransform::TRANSFORMDESC tTransformDesc;
	tTransformDesc.fSpeedPerSec = 0.f;
	tTransformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_tDesc.vPosition), 1.f));
	m_pTransform->SetUp_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tDesc.vRotation.y));


	CCollider::DESC tColliderDesc;
	tColliderDesc.eRigidType = ERigidType::Static;
	tColliderDesc.isSceneQuery = true;
	//tColliderDesc.isKinematic = true;
	tColliderDesc.isTrigger = false;
	tColliderDesc.pGameObject = this;
	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { m_tDesc.vScale.x, m_tDesc.vScale.y, m_tDesc.vScale.z };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	_matrix smatPivot = XMMatrixTranslation(0.f, tBoxColliderDesc.vScale.y * 0.5f, 0.f);
	m_pCollider->setPivotMatrix(smatPivot);

	return S_OK;
}

void CJumpBox::DisableCollision()
{
	m_pCollider->setSceneQuery(false);
	m_isDisable = true;
}


CJumpBox* CJumpBox::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CJumpBox* pInstance = new CJumpBox(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CJumpBox Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CJumpBox::Clone(const _uint _iSceneID, void* _pArg)
{
	CJumpBox* pInstance = new CJumpBox(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CJumpBox Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJumpBox::Free()
{
	Safe_Release(m_pCollider);

	__super::Free();
}
