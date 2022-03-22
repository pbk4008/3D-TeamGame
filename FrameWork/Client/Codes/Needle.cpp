#include "pch.h"
#include "Needle.h"

#include "HierarchyNode.h"
#include "Silvermane.h"
#include "StateController.h"

CNeedle::CNeedle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}

CNeedle::CNeedle(const CNeedle& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CNeedle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Sword_1H;
	m_wstrName = L"Needle";

	return S_OK;
}

HRESULT CNeedle::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);

	XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(-20.f), XMConvertToRadians(-67.f), XMConvertToRadians(0.f)) * XMMatrixTranslation(0.5f, 0.05f, -0.2f));

	return S_OK;
}

_int CNeedle::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);

	if (m_pCollider)
		m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CNeedle::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CNeedle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		m_pModel->Render(i, 0);
	}

	return S_OK;
}

HRESULT CNeedle::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components(m_iSceneID, L"Model_Needle", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.pGameObject = this;

	CCapsuleCollider::DESC tCapsuleColliderDesc;
	tCapsuleColliderDesc.tColliderDesc = tColliderDesc;
	tCapsuleColliderDesc.fHeight = 1.f;
	tCapsuleColliderDesc.fRadius = 0.1f;
	if (FAILED(SetUp_Components(m_iSceneID, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider, &tCapsuleColliderDesc)))
		return E_FAIL;

	_matrix smatPviot = XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.f, 1.f);
	m_pCollider->setPivotMatrix(smatPviot);

	return S_OK;
}

_int CNeedle::Attach_FixedBone(const _double& _dDeltaTime)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();
		if (XMMatrixIsNaN(smatWorld))
			smatWorld = XMMatrixIdentity();

		smatWorld *= XMLoadFloat4x4(&m_smatOwnerPivot);;

		if (!m_isEquip)
			smatWorld = XMLoadFloat4x4(&m_matPivot) * smatWorld;

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CNeedle::Attach_Owner(const _double& _dDeltaTime)
{
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CSilvermane*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CNeedle* CNeedle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CNeedle* pInstance = new CNeedle(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CNeedle Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CNeedle::Clone(const _uint _iSceneID, void* _pArg)
{
	CNeedle* pInstance = new CNeedle(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CNeedle Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNeedle::Free()
{
	Safe_Release(m_pCollider);

	__super::Free();
}
