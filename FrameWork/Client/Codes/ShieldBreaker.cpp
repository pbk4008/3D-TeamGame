#include "pch.h"
#include "ShieldBreaker.h"

#include "Boss_Bastion_Judicator.h"
#include "HierarchyNode.h"

#include "Material.h"

CShieldBreaker::CShieldBreaker(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{
}

CShieldBreaker::CShieldBreaker(const CShieldBreaker& rhs)
	: CWeapon(rhs)
	, m_pCollider(rhs.m_pCollider)
{
	Safe_AddRef(m_pCollider);
}

HRESULT CShieldBreaker::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_matPivot, XMMatrixIdentity());
	
	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_MIDBOSS;
	m_eType = EType::Hammer_2H;
	m_wstrName = L"ShieldBreaker";

	CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/ShieldBreaker/T_2H_hammer_Shieldbreaker_D.dds", 1)))
		return E_FAIL;
	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"2H_hammer_Shieldbreaker", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	if (FAILED(pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0)))
		return E_FAIL;

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/ShieldBreaker/T_2H_hammer_Shieldbreaker_N.dds", 1)))
		return E_FAIL;
	if (FAILED(pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0)))
		return E_FAIL;

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/ShieldBreaker/T_2H_hammer_Shieldbreaker_CEO.dds", 1)))
		return E_FAIL;
	if (FAILED(pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0)))
		return E_FAIL;

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/ShieldBreaker/T_2H_hammer_Shieldbreaker_MRA.dds", 1)))
		return E_FAIL;
	if (FAILED(pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0)))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_Material(L"MI_2H_hammer_Shieldbreaker", pMtrl)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShieldBreaker::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID,pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(pArg);

	m_fDamage = 7.f;

	return S_OK;
}

_int CShieldBreaker::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	Attach_FixedBone(TimeDelta);
	Attach_Owner(TimeDelta);
	
	if (true == m_bHitGround)
	{
		OnHitGroundEffect();
		m_bHitGround = false;
	}
	
	if (nullptr != m_pCollider)
	{
		m_pCollider->Tick(TimeDelta);
	}

	return _int();
}

_int CShieldBreaker::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CShieldBreaker::Render()
{
	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.color = _float4(1.f, 1.f, 0.f, 1.f);
	desc.empower = 1.f;
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag,&desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CShieldBreaker::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CShieldBreaker::OnTriggerEnter(CCollision& collision)
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

void CShieldBreaker::setActive(_bool bActive)
{
	CGameObject::setActive(bActive);

	switch (bActive)
	{
	case true:
		if (m_pCollider)
			m_pCollider->Add_ActorToScene();
		break;
	case false:
		if (m_pCollider)
			m_pCollider->Remove_ActorFromScene();
		break;
	}
}

HRESULT CShieldBreaker::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_ShieldBreaker", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"MI_2H_hammer_Shieldbreaker"), 0);


	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.pGameObject = this;

	CCapsuleCollider::DESC tCapsuleColliderDesc;
	tCapsuleColliderDesc.tColliderDesc = tColliderDesc;
	tCapsuleColliderDesc.fHeight = 2.f;
	tCapsuleColliderDesc.fRadius = 0.5f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider, &tCapsuleColliderDesc)))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.f, 0.8f);
	m_pCollider->setPivotMatrix(matPivot);
	m_pCollider->setShapeLayer((_uint)ELayer::MonsterWeapon);

	return S_OK;
}

_int CShieldBreaker::Attach_FixedBone(const _double& TimeDelta)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return S_OK;
}

_int CShieldBreaker::Attach_Owner(const _double& TimeDelta)
{
	if (m_pOwner)
	{
		_matrix matWorld = m_pLocalTransform->Get_WorldMatrix();
		CTransform* BossTransform = (CTransform*)static_cast<CBoss_Bastion_Judicator*>(m_pOwner)->Get_Component(L"Com_Transform");
		_matrix OwnerMat = BossTransform->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(matWorld * OwnerMat);
	}

	return _int();
}

void CShieldBreaker::OnHitGroundEffect()
{
	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	_vector NewPos = svLook * 2.9f;
	NewPos = XMVectorSetY(NewPos, XMVectorGetY(NewPos) + 1.5f);
	XMStoreFloat3(&m_vEndPos, svPos + svLook);

	m_pOwner->Active_Effect((_uint)EFFECT::HIT_GROUND, svLook * 2.6f);
	m_pOwner->Active_Effect((_uint)EFFECT::EXPLOSION_ROCK_UP, svLook * 2.6f);
	m_pOwner->Active_Effect((_uint)EFFECT::HAMMER_DUST, NewPos);
}

CShieldBreaker* CShieldBreaker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CShieldBreaker* pInstance = new CShieldBreaker(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CShieldBreaker Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CShieldBreaker::Clone(const _uint _iSceneID, void* pArg)
{
	CShieldBreaker* pInstance = new CShieldBreaker(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CShieldBreaker Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShieldBreaker::Free()
{
	CWeapon::Free();
	Safe_Release(m_pCollider);
}
