#include "pch.h"
#include "Fury.h"

#include "HierarchyNode.h"
#include "Silvermane.h"
#include "Material.h"

#include "TrailEffect_Normal.h"

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
	if (_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);
	
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// 트레일 이펙트 달기
	CTrailEffect::DESC tTrailDesc;
	tTrailDesc.fLength = 0.2f;
	XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.5f));
	tTrailDesc.pOwnerTransform = m_pTransform;
	tTrailDesc.wstrTextureTag = L"Wisp_01";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Normal", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Normal)))
		MSGBOX(L"노말 트레일 생성 실패. from Needle");

	XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(-21.5f), XMConvertToRadians(-118.f), XMConvertToRadians(20.f)) * XMMatrixTranslation(0.28f, 0.11f, 0.05f));

	m_fDamage = 5;

	m_bActive = false;
	m_pCollider->Remove_ActorFromScene();
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

	if (m_isTrail)
	{
		m_pTrailEffect_Normal->Record_Points(_dDeltaTime);
		m_pTrailEffect_Normal->Set_IsRender(true);
		//m_pTrailEffect_Distortion->Record_Points(_dDeltaTime);
		//m_pTrailEffect_Distortion->Set_IsRender(true);
		//m_pRenderer->SetRenderButton(CRenderer::DISTORTION, true);
	}
	else
	{
		m_pTrailEffect_Normal->Clear_Points();
		m_pTrailEffect_Normal->Set_IsRender(false);
		//m_pTrailEffect_Distortion->Clear_Points();
		//m_pTrailEffect_Distortion->Set_IsRender(false);
		//m_pRenderer->SetRenderButton(CRenderer::DISTORTION, false);
	}

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CFury::Render()
{
	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	desc.color = _float4(0.f, 0.552f, 0.384f, 1.f);
	desc.empower = 0.6f;

	CWeapon::BindConstantBuffer(wstrCamTag,&desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CFury::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CFury::OnTriggerEnter(CCollision& collision)
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

void CFury::RangeAttack()
{
	OVERLAPDESC tOverlapDesc;
	tOverlapDesc.geometry = PxSphereGeometry(6.f);
	XMStoreFloat3(&tOverlapDesc.vOrigin, m_pTransform->Get_State(CTransform::STATE_POSITION));
	CGameObject* pHitObject = nullptr;
	tOverlapDesc.ppOutHitObject = &pHitObject;
	tOverlapDesc.filterData.flags = PxQueryFlag::eDYNAMIC;
	tOverlapDesc.layerMask = (1 << (_uint)ELayer::Monster);
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
	tBoxColliderDesc.vScale = { 0.8f, 0.5f, 1.8f };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;
	_matrix smatPivot = XMMatrixTranslation(0.f, 0.f, 0.4f);
	m_pCollider->setPivotMatrix(smatPivot);

	return S_OK;
}

_int CFury::Attach_FixedBone(const _double& _dDeltaTime)
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
	if (m_isEquip != _isEquip)
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
	CWeapon::Free();

	Safe_Release(m_pCollider);
	m_pTrailEffect_Normal->Set_Remove(true);
}
