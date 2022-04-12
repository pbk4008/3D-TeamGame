#include "pch.h"
#include "RetributionBlade.h"

#include "HierarchyNode.h"
#include "Monster_Bastion_2HSword.h"
#include "Material.h"
#include "StateController.h"

CRetributionBlade::CRetributionBlade(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
	, m_pCollider(nullptr)
{
}

CRetributionBlade::CRetributionBlade(const CRetributionBlade& _rhs)
	: CWeapon(_rhs)
	, m_pCollider(_rhs.m_pCollider)
{
	Safe_AddRef(m_pCollider);
}

HRESULT CRetributionBlade::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Sword_2H;
	m_wstrName = L"RetributionBlade";
	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_2HSword;

	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_RetributionBlade", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/RetributionBlade/T_2h_Sword_RetributionBlade_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/RetributionBlade/T_2h_Sword_RetributionBlade_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/RetributionBlade/T_2h_Sword_RetributionBlade_MRA.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/RetributionBlade/T_2h_Sword_RetributionBlade_CEO.dds", 1);
	pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_RetributionBlade", pMtrl);

	return S_OK;
}

HRESULT CRetributionBlade::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);

	//m_smatPivot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(-20.f), XMConvertToRadians(-67.f), XMConvertToRadians(0.f)) * XMMatrixTranslation(0.5f, 0.05f, -0.2f);

	m_fDamage = 7.f;

	return S_OK;
}

_int CRetributionBlade::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);

	if (m_pCollider)
		m_pCollider->Tick(_dDeltaTime);




	return _int();
}

_int CRetributionBlade::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CRetributionBlade::Render()
{
	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	desc.color = _float4(0.7529f, 0.7529f, 0.7529f, 1.f);
	desc.empower = 0.7f;

	CWeapon::BindConstantBuffer(wstrCamTag, &desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CRetributionBlade::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CRetributionBlade::OnTriggerEnter(CCollision& collision)
{
	_uint iTag = collision.pGameObject->getTag();
	if ((_uint)GAMEOBJECT::PLAYER == iTag)
	{
		if (!m_isAttack)
			return;

		ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
		tAttackDesc.fDamage += m_fDamage;
		static_cast<CActor*>(collision.pGameObject)->Hit(tAttackDesc);
	}
}


HRESULT CRetributionBlade::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_RetributionBlade", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_RetributionBlade"), 0);

	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.pGameObject = this;

	CCapsuleCollider::DESC tCapsuleColliderDesc;
	tCapsuleColliderDesc.tColliderDesc = tColliderDesc;
	tCapsuleColliderDesc.fHeight = 2.f;
	tCapsuleColliderDesc.fRadius = 0.2f;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider, &tCapsuleColliderDesc)))
		return E_FAIL;

	_matrix smatPviot = XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.f, 0.8f);
	m_pCollider->setPivotMatrix(smatPviot);

	return S_OK;
}

_int CRetributionBlade::Attach_FixedBone(const _double& _dDeltaTime)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);

		if (!m_isEquip)
		{
			smatWorld = m_smatPivot * smatWorld;
		}

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CRetributionBlade::Attach_Owner(const _double& _dDeltaTime)
{
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CSilvermane*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CRetributionBlade* CRetributionBlade::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CRetributionBlade* pInstance = new CRetributionBlade(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CRetributionBlade Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CRetributionBlade::Clone(const _uint _iSceneID, void* _pArg)
{
	CRetributionBlade* pInstance = new CRetributionBlade(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CRetributionBlade Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRetributionBlade::Free()
{
	CWeapon::Free();

	Safe_Release(m_pCollider);
}
