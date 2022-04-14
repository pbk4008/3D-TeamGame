#include "pch.h"
#include "Player_Weapon.h"

#include "HierarchyNode.h"
#include "Silvermane.h"
#include "Material.h"

CPlayer_Weapon::CPlayer_Weapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}

CPlayer_Weapon::CPlayer_Weapon(const CPlayer_Weapon& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CPlayer_Weapon::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON;

	return S_OK;
}

HRESULT CPlayer_Weapon::NativeConstruct(const _uint _iSceneID, Desc _desc)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, &_desc)))
		return E_FAIL;

	CPlayer_Weapon::Desc desc = _desc;
	{
		m_pFixedBone = (desc.pHierarchyNode);
		m_fDamage = desc.damage;
		m_eType = desc.EWeaponType;
		m_wstrName = desc.weaponName;
		m_szMIname += m_wstrName;
		m_szModelName = desc.modelName;
		m_fWeaponColor = desc.weaponColor;
		m_EWeaponType = desc.EWeaponType;

		CMaterial* pMtrl = nullptr;
		CTexture* pTexture = nullptr;

		pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, m_szMIname, m_szMIFilePath, CMaterial::EType::Static);

		for (_int i = 0; i < desc.NumMat; ++i)
		{
			std::wstring szMiTexPath = L"../bin/Resources/Mesh/";
			szMiTexPath = m_szMiTexPath + m_wstrName + L"/";
			szMiTexPath += desc.arrMaterial[i];
			pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, szMiTexPath, 1);
			pMtrl->Set_Texture(g_arrMI_Tex[i], g_arrMI_Texture[i], pTexture, 0);
		}
		g_pGameInstance->Add_Material(m_szMIname, pMtrl);
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	if(EType::Hammer_2H == m_eType)
		XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(-21.5f), XMConvertToRadians(-118.f), XMConvertToRadians(20.f)) * XMMatrixTranslation(0.28f, 0.11f, 0.05f));
	else
		XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(-20.f), XMConvertToRadians(-67.f), XMConvertToRadians(0.f)) * XMMatrixTranslation(0.5f, 0.05f, -0.2f));

	m_bActive = false;

	if(CWeapon::EType::Sword_1H	 == m_EWeaponType)
		m_pCapsuleCollider->Remove_ActorFromScene();
	else
		m_pBoxCollider->Remove_ActorFromScene();

	return S_OK;
}

_int CPlayer_Weapon::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);

	if(nullptr != m_pBoxCollider)
		m_pBoxCollider->Tick(_dDeltaTime);
	if(nullptr != m_pCapsuleCollider)
		m_pCapsuleCollider->Tick(_dDeltaTime);

	return _int();
}

_int CPlayer_Weapon::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CPlayer_Weapon::Render()
{
	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	desc.color = m_fWeaponColor;
	desc.empower = 0.6f;

	CWeapon::BindConstantBuffer(wstrCamTag,&desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CPlayer_Weapon::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CPlayer_Weapon::OnTriggerEnter(CCollision& collision)
{
	_uint iTag = collision.pGameObject->getTag();
	switch (iTag)
	{
	case (_uint)GAMEOBJECT::MONSTER_CRYSTAL:
	case (_uint)GAMEOBJECT::MONSTER_ABERRANT:
	case (_uint)GAMEOBJECT::MONSTER_1H:
	case (_uint)GAMEOBJECT::MONSTER_2H:
	case (_uint)GAMEOBJECT::MONSTER_HEALER:
	case (_uint)GAMEOBJECT::MONSTER_SHOOTER:
	case (_uint)GAMEOBJECT::MONSTER_SPEAR:
	case (_uint)GAMEOBJECT::MONSTER_ANIMUS:
	case (_uint)GAMEOBJECT::MIDDLE_BOSS:
	case (_uint)GAMEOBJECT::BOSS:
		if (!m_isAttack)
			return;

		ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
		tAttackDesc.fDamage += m_fDamage;
		tAttackDesc.pHitObject = this;
		static_cast<CActor*>(collision.pGameObject)->Hit(tAttackDesc);
		break;
	}
}

void CPlayer_Weapon::RangeAttack()
{
	OVERLAPDESC tOverlapDesc;
	tOverlapDesc.geometry = PxSphereGeometry(6.f);
	XMStoreFloat3(&tOverlapDesc.vOrigin, m_pTransform->Get_State(CTransform::STATE_POSITION));
	CGameObject* pHitObject = nullptr;
	tOverlapDesc.ppOutHitObject = &pHitObject;
	tOverlapDesc.filterData.flags = PxQueryFlag::eDYNAMIC;
	if (g_pGameInstance->Overlap(tOverlapDesc))
	{
		_uint iSize = (_uint)tOverlapDesc.vecHitObjects.size();
		for (_uint i = 0; i < iSize; ++i)
		{
			CActor* pActor = static_cast<CActor*>(tOverlapDesc.vecHitObjects[i]);
			_uint iTag = tOverlapDesc.vecHitObjects[i]->getTag();
			switch (iTag)
			{
			case (_uint)GAMEOBJECT::MONSTER_CRYSTAL:
			case (_uint)GAMEOBJECT::MONSTER_ABERRANT:
			case (_uint)GAMEOBJECT::MONSTER_1H:
			case (_uint)GAMEOBJECT::MONSTER_2H:
			case (_uint)GAMEOBJECT::MONSTER_HEALER:
			case (_uint)GAMEOBJECT::MONSTER_SHOOTER:
			case (_uint)GAMEOBJECT::MONSTER_SPEAR:
			case (_uint)GAMEOBJECT::MONSTER_ANIMUS:
			case (_uint)GAMEOBJECT::MIDDLE_BOSS:
			case (_uint)GAMEOBJECT::BOSS:

				ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
				tAttackDesc.fDamage += m_fDamage * 0.8f;
				tAttackDesc.iLevel = 2;
				tAttackDesc.pHitObject = this;
				pActor->Hit(tAttackDesc);
				break;
			}
		}
	}
}

HRESULT CPlayer_Weapon::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, m_szModelName, L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Add_Material(g_pGameInstance->Get_Material(m_szMIname), 0);

	
	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.pGameObject = this;

	if (CWeapon::EType::Hammer_2H == m_EWeaponType)
	{
		CBoxCollider::DESC tBoxColliderDesc;
		tBoxColliderDesc.tColliderDesc = tColliderDesc;
		tBoxColliderDesc.vScale = { 0.8f, 0.5f, 1.8f };
		if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pBoxCollider, &tBoxColliderDesc)))
			return E_FAIL;
		_matrix smatPivot = XMMatrixTranslation(0.f, 0.f, 0.4f);
		m_pBoxCollider->setPivotMatrix(smatPivot);
		m_pBoxCollider->setShapeLayer((_uint)ELayer::Weapon);
	}
	else
	{
		CCapsuleCollider::DESC tCapsuleColliderDesc;
		tCapsuleColliderDesc.tColliderDesc = tColliderDesc;
		tCapsuleColliderDesc.fHeight = 1.2f;
		tCapsuleColliderDesc.fRadius = 0.2f;
		if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCapsuleCollider, &tCapsuleColliderDesc)))
			return E_FAIL;

		_matrix smatPviot = XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.f, 0.8f);
		m_pCapsuleCollider->setPivotMatrix(smatPviot);
		m_pCapsuleCollider->setShapeLayer((_uint)ELayer::Weapon);
	}


	return S_OK;
}

_int CPlayer_Weapon::Attach_FixedBone(const _double& _dDeltaTime)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);

		if (!m_isEquip)
		{
			smatWorld = XMLoadFloat4x4(&m_matPivot) * smatWorld;
		}

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CPlayer_Weapon::Attach_Owner(const _double& _dDeltaTime)
{
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CSilvermane*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

void CPlayer_Weapon::Set_Equip(const _bool _isEquip, void* _pArg)
{
	if (m_isEquip != _isEquip)
	{
		__super::Set_Equip(_isEquip, _pArg);
		switch (_isEquip)
		{
		case true:
		{
			if (m_pBoxCollider)
				m_pBoxCollider->Add_ActorToScene();
			else
				m_pCapsuleCollider->Add_ActorToScene();
		}
			break;
		case false:
		{
			if (m_pBoxCollider)
				m_pBoxCollider->Remove_ActorFromScene();
			else
				m_pCapsuleCollider->Remove_ActorFromScene();
		}
			break;
		}
	}
}

CPlayer_Weapon* CPlayer_Weapon::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CPlayer_Weapon* pInstance = new CPlayer_Weapon(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CPlayer_Weapon Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Weapon::Clone(const _uint _iSceneID, void* _pArg)
{
	return nullptr;
}

void CPlayer_Weapon::Free()
{
	CWeapon::Free();

	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pCapsuleCollider);
}
