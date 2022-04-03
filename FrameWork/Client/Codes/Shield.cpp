#include "pch.h"
#include "Shield.h"

#include "HierarchyNode.h"
#include "Silvermane.h"
#include "Material.h"

CShield::CShield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}

CShield::CShield(const CShield& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CShield::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON;
	m_wstrName = L"Shield";
	m_eType = EType::Shield;

	XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)) * XMMatrixTranslation(0.f, 0.f, 0.f));


	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Shield", L"../../Reference/ShaderFile/Shader_Shield.hlsl", CMaterial::EType::Anim);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_MRA.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_CEO.dds", 1);
	pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Shield", pMtrl);


	return S_OK;
}

HRESULT CShield::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if(_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);

	return S_OK;
}

_int CShield::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);

	m_pAnimationController->Tick(_dDeltaTime);

	m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CShield::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CShield::Render()
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

HRESULT CShield::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Shield", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationRollPitchYaw(XMConvertToRadians(-90.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	m_pModel->Set_PivotMatrix(matPivot);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Shield"), 0);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);
	m_pAnimationController->Set_PlaySpeed(0.55f);


	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.pGameObject = this;
	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { 1.f, 0.2f, 1.f};
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	//_matrix smatPivot = XMMatrixTranslation(0.f, 0.f, 0.0f);
	//m_pCollider->setPivotMatrix(smatPivot);

	return S_OK;
}

_int CShield::Attach_FixedBone(const _double& _dDeltaTime)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();
		smatWorld *= XMLoadFloat4x4(&m_smatOwnerPivot);;

		if (!m_isEquip)
			smatWorld = XMLoadFloat4x4(&m_matPivot) * smatWorld;

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CShield::Attach_Owner(const _double& _dDeltaTime)
{
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CSilvermane*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CShield::Set_Equip(const _bool _isEquip, void* _pArg)
{
	__super::Set_Equip(_isEquip, _pArg);
	m_bActive = _isEquip;
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

void CShield::Set_EquipAnim(const _bool _isEquip)
{
	switch (_isEquip)
	{
	case true:
		m_pAnimationController->SetUp_NextAnimation("SK_shieldBase.ao|A_Spectral_Shield_Block_Start_Weapon", false);
		break;
	case false:
		m_pAnimationController->SetUp_NextAnimation("SK_shieldBase.ao|A_Spectral_Shield_Block_End_Weapon", false);
		break;
	}
}

CShield* CShield::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CShield* pInstance = new CShield(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CShield Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CShield::Clone(const _uint _iSceneID, void* _pArg)
{
	CShield* pInstance = new CShield(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CShield Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShield::Free()
{
	CWeapon::Free();

	Safe_Release(m_pAnimationController);
	Safe_Release(m_pCollider);
}
