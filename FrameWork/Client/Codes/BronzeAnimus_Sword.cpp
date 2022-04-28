#include "pch.h"
#include "BronzeAnimus_Sword.h"

#include "HierarchyNode.h"
#include "Monster_BronzeAnimus.h"
#include "Material.h"
#include "StateController.h"
#include "TrailEffect_Distortion.h"

CBronzeAnimus_Sword::CBronzeAnimus_Sword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
	, m_pCollider(nullptr)
{
	Safe_AddRef(m_pCollider);
}

CBronzeAnimus_Sword::CBronzeAnimus_Sword(const CBronzeAnimus_Sword& _rhs)
	: CWeapon(_rhs)
	, m_pCollider(_rhs.m_pCollider)
{
}

HRESULT CBronzeAnimus_Sword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::BronzeAnimus_Sword;
	m_wstrName = L"BronzeAnimus_Sword";
	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_BRONZE;

	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_BronzeAnimus_Sword", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/BronzeAnimus_Sword/T_BronzeAnimus_Sword_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/BronzeAnimus_Sword/T_BronzeAnimus_Sword_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/BronzeAnimus_Sword/T_BronzeAnimus_Sword_OMER.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_OMER, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_BronzeAnimus_Sword", pMtrl);

	return S_OK;
}

HRESULT CBronzeAnimus_Sword::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);

	m_fDamage = 5.f;

	CTrailEffect::DESC tTrailDesc;
	tTrailDesc.pOwnerTransform = m_pTransform;
	tTrailDesc.fLength = 1.f;
	XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.5f));
	tTrailDesc.wstrTextureTag = L"TrailBase";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Distortion", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Distortion)))
		MSGBOX(L"디스토션 트레일 생성 실패. from Needle");
	Safe_AddRef(m_pTrailEffect_Distortion);

	return S_OK;
}

_int CBronzeAnimus_Sword::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);

	if (m_pCollider)
		m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CBronzeAnimus_Sword::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if (m_isAttack)
	{
		if (m_pTrailEffect_Distortion)
		{
			m_pTrailEffect_Distortion->Record_Points(_dDeltaTime);
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
			m_pRenderer->SetRenderButton(CRenderer::DISTORTION, false);
		}
	}

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CBronzeAnimus_Sword::Render()
{
	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	desc.color = _float4(1.f, 0.f, 0.f, 1.f);
	desc.empower = 0.6f;

	CWeapon::BindConstantBuffer(wstrCamTag,&desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CBronzeAnimus_Sword::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CBronzeAnimus_Sword::OnTriggerEnter(CCollision& collision)
{
	_uint iTag = collision.pGameObject->getTag();
	if ((_uint)GAMEOBJECT::PLAYER == iTag)
	{
		if (!m_isAttack)
			return;

		ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
		tAttackDesc.fDamage += m_fDamage;
		tAttackDesc.iLevel = 2;
		static_cast<CActor*>(collision.pGameObject)->Hit(tAttackDesc);
	}
}

void CBronzeAnimus_Sword::setActive(_bool bActive)
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

HRESULT CBronzeAnimus_Sword::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_BronzeAnimus_Sword", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_BronzeAnimus_Sword"), 0);

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
	m_pCollider->setShapeLayer((_uint)ELayer::MonsterWeapon);

	CTrailEffect::DESC tTrailDesc;
	tTrailDesc.pOwnerTransform = m_pTransform;
	tTrailDesc.fLength = 0.6f;
	XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.f));
	tTrailDesc.wstrTextureTag = L"TrailBase";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Distortion", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Distortion)))
		MSGBOX(L"디스토션 트레일 생성 실패. from Needle");
	Safe_AddRef(m_pTrailEffect_Distortion);

	return S_OK;
}

_int CBronzeAnimus_Sword::Attach_FixedBone(const _double& _dDeltaTime)
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

_int CBronzeAnimus_Sword::Attach_Owner(const _double& _dDeltaTime)
{
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CSilvermane*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CBronzeAnimus_Sword* CBronzeAnimus_Sword::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CBronzeAnimus_Sword* pInstance = new CBronzeAnimus_Sword(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBronzeAnimus_Sword Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBronzeAnimus_Sword::Clone(const _uint _iSceneID, void* _pArg)
{
	CBronzeAnimus_Sword* pInstance = new CBronzeAnimus_Sword(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CBronzeAnimus_Sword Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBronzeAnimus_Sword::Free()
{
	__super::Free();

	Safe_Release(m_pCollider);
	Safe_Release(m_pTrailEffect_Distortion);
}
