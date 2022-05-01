#include "pch.h"
#include "Player_Weapon.h"

#include "HierarchyNode.h"
#include "Silvermane.h"
#include "Material.h"
#include "TrailEffect_Normal.h"
#include "TrailEffect_Distortion.h"
#include "Pot.h"
#include "Light.h"

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

		CMaterial* pMtrl = nullptr;
		CTexture* pTexture = nullptr;

		pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, m_szMIname, m_szMIFilePath, CMaterial::EType::Static);

		for (_uint i = 0; i < desc.NumMat; ++i)
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
	
	if(FAILED(Ready_TrailEffects(_desc)))
		return E_FAIL;

	if(EType::Hammer_2H == m_eType)
		XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(-21.5f), XMConvertToRadians(-118.f), XMConvertToRadians(20.f)) * XMMatrixTranslation(0.28f, 0.11f, 0.05f));
	else
		XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(-20.f), XMConvertToRadians(-67.f), XMConvertToRadians(0.f)) * XMMatrixTranslation(0.5f, 0.05f, -0.2f));

	m_bActive = false;

	if(CWeapon::EType::Sword_1H	 == m_eType)
		m_pCapsuleCollider->Remove_ActorFromScene();
	else
		m_pBoxCollider->Remove_ActorFromScene();

	LIGHTDESC LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.fRange = 5.f;
	LightDesc.vDiffuse = _float4(0.3686f, 04941.f, 0.60784f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.bactive = false;
	LightDesc.vPosition = _float3(0, 0, 0);

	m_LightRange = LightDesc.fRange;
	m_OrigLightRange = LightDesc.fRange;

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc, &m_pActiveLight))) MSGBOX("Failed To Adding PointLight");


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


	if ((_uint)SCENEID::SCENE_STAGE3 != m_iSceneID)
	{
		LightOnOff(m_HitPosition, XMVectorSet(1.f, 1.f, 1.f, 1.f), 20.f);
	}

	else if ((_uint)SCENEID::SCENE_STAGE3 == m_iSceneID)
	{
		LightOnOff(m_HitPosition, XMVectorSet(1.f, 1.f, 1.f, 1.f), 1.f);
	}
	return _int();
}

_int CPlayer_Weapon::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if (m_isTrail)
	{
		if (m_pTrailEffect_Normal)
		{
			m_pTrailEffect_Normal->Record_Points(_dDeltaTime);
			m_pTrailEffect_Normal->Set_IsRender(true);
		}
		if (m_pTrailEffect_Distortion)
		{
			m_pTrailEffect_Distortion->Record_Points(_dDeltaTime);
			m_pTrailEffect_Distortion->Set_IsRender(true);
			m_pRenderer->SetRenderButton(CRenderer::DISTORTION, true);
		}
	}
	else
	{
		if (m_pTrailEffect_Normal)
		{
			m_pTrailEffect_Normal->Clear_Points();
			m_pTrailEffect_Normal->Set_IsRender(false);
		}
		if (m_pTrailEffect_Distortion)
		{
			m_pTrailEffect_Distortion->Clear_Points();
			m_pTrailEffect_Distortion->Set_IsRender(false);
			//m_pRenderer->SetRenderButton(CRenderer::DISTORTION, false);
		}
	}

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

	RIM rimdesc;
	ZeroMemory(&rimdesc, sizeof(rimdesc));

	if (m_rimcheck == true)
	{
		CWeapon::RimIntensity(g_fDeltaTime * -1.f);

		rimdesc.rimcheck = m_rimcheck;
		rimdesc.rimintensity = m_rimintensity - 3.f;
		rimdesc.rimcol = m_rimcol;
		XMStoreFloat4(&rimdesc.camdir, XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_POSITION) - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane")));
		if (FAILED(m_pModel->SetUp_ValueOnShader("g_rimtimer", &m_rimtime, sizeof(_float)))) MSGBOX("Failed to Apply RimTime Value");
	}

	CWeapon::BindConstantBuffer(wstrCamTag,&desc, &rimdesc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CPlayer_Weapon::Render_Shadow()
{
	CWeapon::Render_Shadow();

	return S_OK;
}

HRESULT CPlayer_Weapon::Render_Velocity()
{
	CWeapon::Render_Velocity();

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

		if (m_pActiveLight != nullptr)
		{
			m_bLightCheck = true;
			m_pActiveLight->Set_Active(true);
			m_LightRange = m_OrigLightRange;
			m_HitPosition = static_cast<CActor*>(collision.pGameObject)->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		}


		break;
	}

}

void CPlayer_Weapon::RangeAttack(const _float _fForce)
{
	OVERLAPDESC tOverlapDesc;
	switch (m_eType)
	{
	case CWeapon::EType::Sword_1H:
		tOverlapDesc.geometry = PxSphereGeometry(4.f);
		break;
	case CWeapon::EType::Hammer_2H:
		tOverlapDesc.geometry = PxSphereGeometry(6.f);
		break;
	}
	XMStoreFloat3(&tOverlapDesc.vOrigin, m_pTransform->Get_State(CTransform::STATE_POSITION));
	CGameObject* pHitObject = nullptr;
	tOverlapDesc.ppOutHitObject = &pHitObject;
	//tOverlapDesc.filterData.flags = PxQueryFlag::eDYNAMIC;
	tOverlapDesc.layerMask = (1 << (_uint)ELayer::Monster) + (1 << (_uint)ELayer::Pot);
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
			{
				ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
				tAttackDesc.fDamage += m_fDamage * _fForce * 0.8f;
				tAttackDesc.iLevel = 2;
				tAttackDesc.pHitObject = this;
				pActor->Hit(tAttackDesc);
			}
				break;
			
			case (_uint)GAMEOBJECT::POT:
				CCollision Collision;
				Collision.pGameObject = this;
				static_cast<CPot*>(tOverlapDesc.vecHitObjects[i])->OnTriggerEnter(Collision);
				break;
			}
		}
	}
}

void CPlayer_Weapon::LightOnOff(_fvector pos, _fvector color, _float deltaspeed)
{
	if (m_bLightCheck == true)
	{
		m_LightRange += g_fDeltaTime * -deltaspeed;

		m_pActiveLight->Set_Range(m_LightRange);
		m_pActiveLight->Set_Pos(pos);
		m_pActiveLight->Set_Color(color);

		if (m_LightRange <= 0.f)
		{
			m_LightRange = m_OrigLightRange;
			m_bLightCheck = false;
			m_pActiveLight->Set_Active(false);
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

	if (CWeapon::EType::Hammer_2H == m_eType)
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

HRESULT CPlayer_Weapon::Ready_TrailEffects(const Desc& _tDesc)
{
	switch (_tDesc.EWeaponType)
	{
	case CWeapon::EType::Sword_1H:
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.fLength = 1.f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.5f));
		tTrailDesc.wstrTextureTag = L"TrailBase";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Distortion", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Distortion)))
			MSGBOX(L"디스토션 트레일 생성 실패. from Needle");
		Safe_AddRef(m_pTrailEffect_Distortion);
	}
		break;
	case CWeapon::EType::Hammer_2H:
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.fLength = 0.6f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.5f));
		tTrailDesc.wstrTextureTag = L"TrailBase";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Distortion", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Distortion)))
			MSGBOX(L"디스토션 트레일 생성 실패. from Needle");
		Safe_AddRef(m_pTrailEffect_Distortion);
	}
		break;
	}

	if (L"Needle" == _tDesc.weaponName)
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.fLength = 0.4f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 2.f));
		tTrailDesc.wstrTextureTag = L"Fire_02";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
			MSGBOX(L"노말 트레일 생성 실패. from Needle");
		Safe_AddRef(m_pTrailEffect_Normal);
	}
	else if (L"Eclipse" == _tDesc.weaponName)
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.fLength = 0.4f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 2.f));
		tTrailDesc.wstrTextureTag = L"Fire_02";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
			MSGBOX(L"노말 트레일 생성 실패. from Eclipse");
		Safe_AddRef(m_pTrailEffect_Normal);
	}
	else if (L"Justice" == _tDesc.weaponName)
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.fLength = 0.4f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 2.f));
		tTrailDesc.wstrTextureTag = L"EnergyPlasma_Tile_H_01";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
			MSGBOX(L"노말 트레일 생성 실패. from Justice");
		Safe_AddRef(m_pTrailEffect_Normal);
	}
	else if (L"Dragon" == _tDesc.weaponName)
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.fLength = 0.4f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 2.f));
		tTrailDesc.wstrTextureTag = L"waterfall02";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
			MSGBOX(L"노말 트레일 생성 실패. from Dragon");
		Safe_AddRef(m_pTrailEffect_Normal);
	}
	////////////////////////////////////////////////// 해머
	else if (L"Fury" == _tDesc.weaponName)
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.fLength = 0.4f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.9f));
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.wstrTextureTag = L"EnergyPlasma_Tile_H_01";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
			MSGBOX(L"노말 트레일 생성 실패. from Fury");
		Safe_AddRef(m_pTrailEffect_Normal);
	}
	else if (L"Ironstone" == _tDesc.weaponName)
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.fLength = 0.2f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.9f));
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.wstrTextureTag = L"SpeedTrails_Packed";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
			MSGBOX(L"노말 트레일 생성 실패. from Ironstone");
		Safe_AddRef(m_pTrailEffect_Normal);
	}
	else if (L"Legend1" == _tDesc.weaponName)
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.fLength = 0.2f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.9f));
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.wstrTextureTag = L"ice02";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
			MSGBOX(L"노말 트레일 생성 실패. from Legend1");
		Safe_AddRef(m_pTrailEffect_Normal);
	}
	else if (L"Legend5" == _tDesc.weaponName)
	{
		CTrailEffect::DESC tTrailDesc;
		tTrailDesc.fLength = 0.2f;
		XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.9f));
		tTrailDesc.pOwnerTransform = m_pTransform;
		tTrailDesc.wstrTextureTag = L"PerlinElectric_Packed";
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
			MSGBOX(L"노말 트레일 생성 실패. from Legend5");
		Safe_AddRef(m_pTrailEffect_Normal);
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

	Safe_Release(m_pTrailEffect_Distortion);
	Safe_Release(m_pTrailEffect_Normal);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pCapsuleCollider);
}
