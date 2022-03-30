#include "pch.h"
#include "Fury.h"

#include "HierarchyNode.h"
#include "Silvermane.h"
#include "Material.h"

CFury::CFury(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}

CFury::CFury(const CFury& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CFury::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON;
	m_eType = EType::Hammer_2H;
	m_wstrName = L"Fury";


	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Fury", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Fury/T_2h_Hammer_Fury_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Fury/T_2h_Hammer_Fury_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Fury/T_2h_Hammer_Fury_MRA.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Fury/T_2h_Hammer_Fury_CEO.dds", 1);
	pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Fury", pMtrl);

	return S_OK;
}

HRESULT CFury::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);

	XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(-21.5f), XMConvertToRadians(-118.f), XMConvertToRadians(20.f)) * XMMatrixTranslation(0.28f, 0.11f, 0.05f));

	return S_OK;
}

_int CFury::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);

	m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CFury::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CFury::Render()
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
		//m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		m_pModel->Render(i, 0);
	}

	return S_OK;
}

HRESULT CFury::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Fury", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Fury"), 0);


	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.pGameObject = this;
	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { 0.8f, 0.5f, 1.f };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	_matrix smatPivot = XMMatrixTranslation(0.f, 0.f, 0.8f);
	m_pCollider->setPivotMatrix(smatPivot);

	return S_OK;
}

_int CFury::Attach_FixedBone(const _double& _dDeltaTime)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= XMLoadFloat4x4(&m_smatOwnerPivot);

		if (!m_isEquip)
		{
			smatWorld = XMLoadFloat4x4(&m_matPivot) * smatWorld;
		}

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CFury::Attach_Owner(const _double& _dDeltaTime)
{
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CSilvermane*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CFury::Set_Equip(const _bool _isEquip, void* _pArg)
{
	__super::Set_Equip(_isEquip, _pArg);
	switch (_isEquip)
	{
	case true:
		m_pCollider->Add_ActorToScene();
		break;
	case false:
		m_pCollider->Remove_ActorFromScene();
		break;
	}

}

CFury* CFury::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CFury* pInstance = new CFury(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CFury Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CFury::Clone(const _uint _iSceneID, void* _pArg)
{
	CFury* pInstance = new CFury(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CFury Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFury::Free()
{
	Safe_Release(m_pCollider);

	__super::Free();
}
