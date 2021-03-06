#include "pch.h"
#include "EarthAberrant_Pick.h"

#include "Monster_EarthAberrant.h"
#include "HierarchyNode.h"

#include "Material.h"
#include "TrailEffect_Distortion.h"


CEarthAberrant_Pick::CEarthAberrant_Pick(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{
}

CEarthAberrant_Pick::CEarthAberrant_Pick(const CEarthAberrant_Pick& rhs)
	: CWeapon(rhs)
	, m_pCollider(rhs.m_pCollider)
{
	Safe_AddRef(m_pCollider);
}

HRESULT CEarthAberrant_Pick::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_matPivot, XMMatrixIdentity());

	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_EARTH;
	m_eType = EType::Earth_Pick;
	m_wstrName = L"EarthAberrant_Pick";

	CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Earth_Aberrant_Pick/T_IcePick_D.dds", 1)))
		return E_FAIL;
	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Earth_Aberrant_Pick", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	if (FAILED(pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0)))
		return E_FAIL;

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Earth_Aberrant_Pick/T_IcePick_N.dds", 1)))
		return E_FAIL;
	if (FAILED(pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0)))
		return E_FAIL;

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Earth_Aberrant_Pick/T_IcePick_MRA.dds", 1)))
		return E_FAIL;
	if (FAILED(pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0)))
		return E_FAIL;

	//TODO : ?̰? MASK ???̴???? ???? ?ݾƳ??? 
	/*pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	if (FAILED(pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Earth_Aberrant_Pick/T_IcePick_MASK.dds", 1)))
		return E_FAIL;
	if (FAILED(pMtrl->Set_Texture("g_MASKTexture", TEXTURETYPE::TEX_MASK, pTexture, 0)))
		return E_FAIL;*/


	if (FAILED(g_pGameInstance->Add_Material(L"MI_Earth_Aberrant_Pick", pMtrl)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEarthAberrant_Pick::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID,pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(pArg);

	m_fDamage = 3.f;

	CTrailEffect::DESC tTrailDesc;
	tTrailDesc.pOwnerTransform = m_pTransform;
	tTrailDesc.fLength = 0.6f;
	XMStoreFloat4x4(&tTrailDesc.matPivot, XMMatrixTranslation(0.f, 0.f, 1.f));
	tTrailDesc.wstrTextureTag = L"TrailBase";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(m_iSceneID, L"Layer_Effect", L"Proto_GameObject_TrailEffect_Distortion", &tTrailDesc, (CGameObject**)&m_pTrailEffect_Distortion)))
		MSGBOX(L"???????? Ʈ???? ???? ????. from Needle");
	Safe_AddRef(m_pTrailEffect_Distortion);

	return S_OK;
}

_int CEarthAberrant_Pick::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	Attach_FixedBone(TimeDelta);
	Attach_Owner(TimeDelta);
	
	if (nullptr != m_pCollider)
	{
		m_pCollider->Tick(TimeDelta);
	}

	

	return _int();
}

_int CEarthAberrant_Pick::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if (!g_pGameInstance->isIn_WorldFrustum(m_pTransform->Get_State(CTransform::STATE_POSITION), 3.f))
		return 0;
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

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CEarthAberrant_Pick::Render()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CEarthAberrant_Pick::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CEarthAberrant_Pick::OnTriggerEnter(CCollision& collision)
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

void CEarthAberrant_Pick::setActive(_bool bActive)
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

HRESULT CEarthAberrant_Pick::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_EarthAberrant_Pick", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"MI_Earth_Aberrant_Pick"), 0);

	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.eRigidType = ERigidType::Dynamic;
	tColliderDesc.pGameObject = this;

	CCapsuleCollider::DESC tCapsuleColliderDesc;
	tCapsuleColliderDesc.tColliderDesc = tColliderDesc;
	tCapsuleColliderDesc.fHeight = 0.4f;
	tCapsuleColliderDesc.fRadius = 0.25f;
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider, &tCapsuleColliderDesc)))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.f, -0.6f);
	m_pCollider->setPivotMatrix(matPivot);
	m_pCollider->setShapeLayer((_uint)ELayer::MonsterWeapon);

	return S_OK;
}

_int CEarthAberrant_Pick::Attach_FixedBone(const _double& TimeDelta)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CEarthAberrant_Pick::Attach_Owner(const _double& TimeDelta)
{
	if (m_pOwner)
	{
		_matrix matWorld = m_pLocalTransform->Get_WorldMatrix();
		CTransform* BossTransform = (CTransform*)static_cast<CMonster_EarthAberrant*>(m_pOwner)->Get_Component(L"Com_Transform");
		_matrix OwnerMat = BossTransform->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(matWorld * OwnerMat);
	}

	return _int();
}

CEarthAberrant_Pick* CEarthAberrant_Pick::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CEarthAberrant_Pick* pInstance = new CEarthAberrant_Pick(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CEarthAberrant_Pick Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEarthAberrant_Pick::Clone(const _uint _iSceneID, void* pArg)
{
	CEarthAberrant_Pick* pInstance = new CEarthAberrant_Pick(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CEarthAberrant_Pick Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEarthAberrant_Pick::Free()
{
	CWeapon::Free();

	if(m_pTrailEffect_Distortion != nullptr)
		m_pTrailEffect_Distortion->Set_Remove(true);

	Safe_Release(m_pCollider);
	Safe_Release(m_pTrailEffect_Distortion);
}
