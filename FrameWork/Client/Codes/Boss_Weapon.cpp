#include "pch.h"
#include "Boss_Weapon.h"

#include "Boss_Solaris.h"
#include "HierarchyNode.h"

#include "Material.h"
#include "TrailEffect_Distortion.h"


CBoss_Weapon::CBoss_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{
}

CBoss_Weapon::CBoss_Weapon(const CBoss_Weapon& rhs)
	: CWeapon(rhs)
	, m_pCollider(rhs.m_pCollider)
{
	Safe_AddRef(m_pCollider);
}

HRESULT CBoss_Weapon::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_matPivot, XMMatrixIdentity());

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_BOSS;
	m_eType = EType::BossWeapon;
	m_wstrName = L"Boss_Weapon";

	return S_OK;
}

HRESULT CBoss_Weapon::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID,pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(pArg);

	setActive(true);
	m_fDamage = 7.f;

	CTrailEffect::DESC tTrailDesc;
	tTrailDesc.pOwnerTransform = m_pTransform;
	tTrailDesc.fLength = 2.f;
	XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 5.f));
	tTrailDesc.wstrTextureTag = L"TrailBase";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Distortion", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Distortion)))
		MSGBOX(L"디스토션 트레일 생성 실패. from Needle");
	Safe_AddRef(m_pTrailEffect_Distortion);

	return S_OK;
}

_int CBoss_Weapon::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;


	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 1.6f, 0.0f);
	m_pCollider->setPivotMatrix(matPivot);


	Attach_FixedBone(TimeDelta);
	Attach_Owner(TimeDelta);
	
	if (nullptr != m_pCollider)
	{
		m_pCollider->Tick(TimeDelta);
	}

	return _int();
}

_int CBoss_Weapon::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (m_isAttack)
	{
		if (m_pTrailEffect_Distortion)
		{
			m_pTrailEffect_Distortion->Record_Points(TimeDelta);
			m_pTrailEffect_Distortion->Set_IsRender(true);
			m_pRenderer->SetRenderButton(CRenderer::DISTORTION, true);
		}
	}
	else
	{
		if (m_pTrailEffect_Distortion)
		{
			m_pTrailEffect_Distortion->Clear_Points();
			m_pTrailEffect_Distortion->Set_IsRender(false);
			//m_pRenderer->SetRenderButton(CRenderer::DISTORTION, false);
		}
	}

	return _int();
}

HRESULT CBoss_Weapon::Render()
{
	return S_OK;
}

HRESULT CBoss_Weapon::Render_Shadow()
{
	return S_OK;
}

void CBoss_Weapon::OnTriggerEnter(CCollision& collision)
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

void CBoss_Weapon::setActive(_bool bActive)
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

HRESULT CBoss_Weapon::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.pGameObject = this;

	CCapsuleCollider::DESC tCapsuleColliderDesc;
	tCapsuleColliderDesc.tColliderDesc = tColliderDesc;
	tCapsuleColliderDesc.fHeight = 4.f;
	tCapsuleColliderDesc.fRadius = 0.8f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider, &tCapsuleColliderDesc)))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 2.f, 0.0f);
	m_pCollider->setPivotMatrix(matPivot);
	m_pCollider->setShapeLayer((_uint)ELayer::MonsterWeapon);

	return S_OK;
}

_int CBoss_Weapon::Attach_FixedBone(const _double& TimeDelta)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CBoss_Weapon::Attach_Owner(const _double& TimeDelta)
{
	if (m_pOwner)
	{
		_matrix matWorld = m_pLocalTransform->Get_WorldMatrix();
		CTransform* BossTransform = (CTransform*)static_cast<CBoss_Solaris*>(m_pOwner)->Get_Component(L"Com_Transform");
		_matrix OwnerMat = BossTransform->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(matWorld * OwnerMat);
	}

	return _int();
}

CBoss_Weapon* CBoss_Weapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CBoss_Weapon* pInstance = new CBoss_Weapon(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBoss_Weapon Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBoss_Weapon::Clone(const _uint _iSceneID, void* pArg)
{
	CBoss_Weapon* pInstance = new CBoss_Weapon(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CBoss_Weapon Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Weapon::Free()
{
	CWeapon::Free();
	Safe_Release(m_pCollider);
	Safe_Release(m_pTrailEffect_Distortion);
}
