#include "pch.h"
#include "Needle.h"

#include "HierarchyNode.h"
#include "Silvermane.h"
#include "StateController.h"

#include "Material.h"
#include "SwordTrail.h"
#include "TrailEffect.h"


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

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON;
	m_eType = EType::Sword_1H;
	m_wstrName = L"Needle";

	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Needle", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Needle/T_1h_Sword_Needle_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Needle/T_1h_Sword_Needle_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Needle/T_1h_Sword_Needle_MRA.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, L"../Bin/Resources/Mesh/Needle/T_1h_Sword_Needle_CEO.dds", 1);
	pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_Needle", pMtrl);

	return S_OK;
}

HRESULT CNeedle::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	XMStoreFloat4x4(&m_matPivot, XMMatrixRotationRollPitchYaw(XMConvertToRadians(-20.f), XMConvertToRadians(-67.f), XMConvertToRadians(0.f)) * XMMatrixTranslation(0.5f, 0.05f, -0.2f));

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect", m_pTransform, (CGameObject**)&m_pTrailEffect)))
		MSGBOX(L"트레일 이펙트 생성 실패. from Needle");

	//Light
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	LightDesc.fRange = 7.f;
	LightDesc.vDiffuse = _float4(1.f, 0.7f, 0.5f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	XMStoreFloat3(&LightDesc.vPosition,m_pTransform->Get_State(CTransform::STATE_POSITION));

	if (nullptr == m_pLight)
	{
		if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc, &m_pLight)))
			MSGBOX("Failed To Adding PointLight");
	}
	
	m_pLight->Set_Show(false);
	m_fLightRange = LightDesc.fRange;

	m_fDamage = 3;

	m_bActive = false;
	m_pCollider->Remove_ActorFromScene();
	return S_OK;
}

_int CNeedle::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);
	_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_vector svLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
	XMStoreFloat3(&m_vEndPos, svPos + svLook);

	if (m_pCollider)
		m_pCollider->Tick(_dDeltaTime);


	if (m_bLight && 0.f <= m_fLightRange)
	{
		m_fLightRange -= (_float)_dDeltaTime * 10.f;
		m_pLight->Set_Range(m_fLightRange);
	}

	if (0.f >= m_fLightRange)
	{
		m_fLightRange = 0.f;
		m_pLight->Set_Show(false);
		m_bLight = false;
	}

	return _int();
}

_int CNeedle::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if (m_isTrail)
	{
		m_pTrailEffect->Record_Points(_dDeltaTime);
		m_pTrailEffect->Set_IsRender(true);
		m_pRenderer->SetRenderButton(CRenderer::DISTORTION, true);
	}
	else
	{
		m_pTrailEffect->Clear_Points();
		m_pTrailEffect->Set_IsRender(false);
		m_pRenderer->SetRenderButton(CRenderer::DISTORTION, false);
	}

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);


	//if (g_pObserver->IsAttack())
	//	m_bTrailOnOff = true;
	//else
	//	m_bTrailOnOff = false;

	//if (m_bTrailOnOff == true)
	//{
	//	_vector top, bottom, look;
	//	_matrix world = m_pTransform->Get_WorldMatrix();
	//	
	//	look = world.r[2];
	//	top = world.r[3];
	//	bottom = world.r[3];
	//	/*look = XMVector3Normalize(look);*/
	//	top += look * 2.5f;
	//	bottom += look * 0.5f;

	//	m_pTrail->AddVertex(top, bottom);
	//	m_pTrail->Tick(_dDeltaTime);
	//}
	//else
	//	m_pTrail->Clear_Vertex();

	return _int();
}

HRESULT CNeedle::Render()
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

HRESULT CNeedle::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CNeedle::OnTriggerEnter(CCollision& collision)
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

		if (nullptr != m_pLight)
		{
			m_pLight->Set_Pos(static_cast<CActor*>(collision.pGameObject)->Get_Transform()->Get_State(CTransform::STATE_POSITION));
			m_pLight->Set_Show(true);
			_vector vColor = { 1.f, 0.7f, 0.5f, 1.f };
			m_pLight->Set_Color(vColor);
			m_fLightRange = 4.f;
			m_pLight->Set_Range(m_fLightRange);
			m_bLight = true;
		}
		
		break;
	}
}

void CNeedle::OnTriggerExit(CCollision& collision)
{
	/*if (nullptr != m_pLight)
	{
		m_pLight->Set_Show(false);
	}*/
}

void CNeedle::RangeAttack()
{
	OVERLAPDESC tOverlapDesc;
	tOverlapDesc.geometry = PxSphereGeometry(4.f);
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
				ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
				tAttackDesc.fDamage += m_fDamage * 0.5f;
				tAttackDesc.iLevel = 2;
				tAttackDesc.pHitObject = this;
				pActor->Hit(tAttackDesc);
				break;
			}
		}
	}
}

HRESULT CNeedle::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Needle", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_Needle"), 0);

	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.pGameObject = this;
	CCapsuleCollider::DESC tCapsuleColliderDesc;
	tCapsuleColliderDesc.tColliderDesc = tColliderDesc;
	tCapsuleColliderDesc.fHeight = 1.2f;
	tCapsuleColliderDesc.fRadius = 0.2f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider, &tCapsuleColliderDesc)))
		return E_FAIL;

	_matrix smatPviot = XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.f, 0.8f);
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

		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);;

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

void CNeedle::Set_Equip(const _bool _isEquip, void* _pArg)
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
	CWeapon::Free();

	Safe_Release(m_pCollider);
}
