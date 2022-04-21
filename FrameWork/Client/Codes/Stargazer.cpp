#include "pch.h"
#include "Stargazer.h"
#include "HierarchyNode.h"
#include "Monster_Bastion_Sword.h"
#include "Material.h"

CStargazer::CStargazer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
	, m_pCollider(nullptr)
{
}
CStargazer::CStargazer(const CStargazer& _rhs)
	: CWeapon(_rhs)
	, m_pCollider(_rhs.m_pCollider)
{
	Safe_AddRef(m_pCollider);
}

HRESULT CStargazer::NativeConstruct_Prototype()
{
	if (FAILED(CWeapon::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Monster_Sword1H;
	m_iObectTag = (_uint)GAMEOBJECT::WEAPON_1HSWORD;

	return S_OK;
}

HRESULT CStargazer::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (FAILED(CWeapon::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_fDamage = 3;

	return S_OK;
}

_int CStargazer::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	//뼈행렬 업데이트
	Attach_FixedBone(_dDeltaTime);
	//무기 가지고 있는 객체의 월드 업데이트
	Attach_Owner(_dDeltaTime);

	if(nullptr != m_pCollider)
		m_pCollider->Tick(_dDeltaTime);

	return _int();
}

_int CStargazer::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA,this);

	return _int();
}

HRESULT CStargazer::Render()
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

HRESULT CStargazer::Render_Shadow()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	CWeapon::BindConstantBuffer(wstrCamTag);
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CStargazer::setActive(_bool bActive)
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

void CStargazer::Set_OwnerPivotMatrix(const _fmatrix& _smatPivot)
{
	XMStoreFloat4x4(&m_matOwnerPivot, _smatPivot);
}

HRESULT CStargazer::SetUp_Component()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_Weapon_Stargazer", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	CCapsuleCollider::DESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));

	tDesc.fHeight = 1.5f;
	tDesc.fRadius = 0.25f;
	tDesc.tColliderDesc.eRigidType = ERigidType::Dynamic;
	tDesc.tColliderDesc.fDynamicFriction = 0.f;
	tDesc.tColliderDesc.fStaticFriction = 0.f;
	tDesc.tColliderDesc.fRestitution = 0.f;
	tDesc.tColliderDesc.isGravity = false;
	tDesc.tColliderDesc.isKinematic = false;
	tDesc.tColliderDesc.isVisualization = true;
	tDesc.tColliderDesc.isSceneQuery = false;
	tDesc.tColliderDesc.isTrigger = true;
	tDesc.tColliderDesc.pGameObject = this;

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_CapsuleCollider", L"Collider", (CComponent**)&m_pCollider,&tDesc)))
		return E_FAIL;

	_matrix matPivot = /*XMMatrixScaling(10.f, 10.f, 10.f) * */XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f)) * XMMatrixTranslation(0.f, 0.f, 0.8f);
	m_pCollider->setPivotMatrix(matPivot);
	m_pCollider->setShapeLayer((_uint)ELayer::MonsterWeapon);

	if (FAILED(SetUp_Material()))
		return E_FAIL;

	return S_OK;
}

_int CStargazer::Attach_FixedBone(const _double& _dDeltaTime)
{
	//뼈 노드가 있을때
	if (m_pFixedBone)
	{
		//뼈노드가 가지고 있는 Combine행렬 가져옴
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();
		//무기 가지고 있는 객체의 피벗 곱해줌
		smatWorld *= XMLoadFloat4x4(&m_matOwnerPivot);
		//무기 로컬 트랜스 폼 갱신
		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}
	return _int();
}

_int CStargazer::Attach_Owner(const _double& _dDeltaTime)
{
	//무기 가지고 있는 객체가 있을때
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CMonster_Bastion_Sword*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}
	return _int();
}

HRESULT CStargazer::SetUp_Material()
{
	if (!m_pModel)
		return E_FAIL;
	CMaterial* pMtrl = g_pGameInstance->Get_Material(L"Mtrl_Stargazer");
	if (!pMtrl)
	{
		CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Stargazer/T_1h_Sword_Stargazer_D.dds", 1);
		pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_Stargazer", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
		pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Stargazer/T_1h_Sword_Stargazer_N.dds", 1);
		pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Stargazer/T_1h_Sword_Stargazer_MRA.dds", 1);
		pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);

		pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
		pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Stargazer/T_1h_Sword_Stargazer_CEO.dds", 1);
		pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);

		g_pGameInstance->Add_Material(L"Mtrl_Stargazer", pMtrl);

	}
	m_pModel->Add_Material(pMtrl, 0);

	return S_OK;
}

void CStargazer::Check_Attack()
{
	/*if (!m_pOwner)
		return;

	if (m_pOwner->IsAttack())
		m_isAttack = true;
	else
		m_isAttack = false;*/
}

void CStargazer::OnTriggerEnter(CCollision& collision)
{
	if (collision.pGameObject->getTag() == (_uint)GAMEOBJECT::PLAYER)
	{
		if (!m_isAttack)
			return;

		ATTACKDESC tAttackDesc = m_pOwner->Get_AttackDesc();
		tAttackDesc.fDamage += m_fDamage;
		static_cast<CActor*>(collision.pGameObject)->Hit(tAttackDesc);
	}
}

void CStargazer::OnTriggerExit(CCollision& collision)
{
	/*m_isAttack = false;
	m_pOwner->Set_IsAttack(false);*/
}


CStargazer* CStargazer::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CStargazer* pInstance = new CStargazer(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CStargazer Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CStargazer::Clone(const _uint _iSceneID, void* pArg)
{
	CStargazer* pInstance = new CStargazer(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("CStargazer Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStargazer::Free()
{
	CWeapon::Free();
	Safe_Release(m_pCollider);
}
