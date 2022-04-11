#include "pch.h"
#include "FlyingShield.h"
#include "Material.h"

CFlyingShield::CFlyingShield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}

CFlyingShield::CFlyingShield(const CFlyingShield& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CFlyingShield::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	CMaterial* pMtrl = nullptr;
	CTexture* pTexture = nullptr;
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Mtrl_FlyingShield", L"../../Reference/ShaderFile/Shader_Weapon.hlsl", CMaterial::EType::Static);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_D.dds", 1);
	pMtrl->Set_Texture("g_DiffuseTexture", TEXTURETYPE::TEX_DIFFUSE, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_N.dds", 1);
	pMtrl->Set_Texture("g_BiNormalTexture", TEXTURETYPE::TEX_NORMAL, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_MRA.dds", 1);
	pMtrl->Set_Texture("g_MRATexture", TEXTURETYPE::TEX_MRA, pTexture, 0);
	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Shield/T_ShieldBase_CEO.dds", 1);
	pMtrl->Set_Texture("g_CEOTexture", TEXTURETYPE::TEX_CEO, pTexture, 0);
	g_pGameInstance->Add_Material(L"Mtrl_FlyingShield", pMtrl);

	return S_OK;
}

HRESULT CFlyingShield::NativeConstruct(const _uint _iSceneID, void* _pArg)
{
	if (_pArg)
	{
		memcpy_s(&m_tDesc, sizeof(DESC), _pArg, sizeof(DESC));
	}

	if (FAILED(__super::NativeConstruct(_iSceneID, _pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_vector svDir = XMVectorSetW(XMLoadFloat3(&m_tDesc.vTargetPos), 1.f) - m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_fDis = XMVectorGetX(XMVector3Length(svDir));
	svDir = XMVector3Normalize(svDir);
	XMStoreFloat3(&m_vDir, svDir);
	m_fLiveTime = 1.6f;

	//m_fSpeed = m_fDis / (m_fLiveTime * 0.5f);
	m_fSpeed = 50.f;

	m_fDamage = 3.f;
	m_isAttack = true;

	g_pObserver->Set_IsThrownObject(true);
	return S_OK;
}

_int CFlyingShield::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (!m_isReturn)
	{
		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector svVelocity = XMLoadFloat3(&m_vDir) * m_fSpeed * (_float)_dDeltaTime;
		svPos += svVelocity;
		m_fAccDis += XMVectorGetX(XMVector3Length(svVelocity));
		m_pTransform->Set_State(CTransform::STATE_POSITION, svPos);

		if (m_fDis < m_fAccDis)
		{
			m_isReturn = true;
			static_cast<CSilvermane*>(m_pOwner)->Return_Shield();
		}
	}
	else
	{
		_vector svPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		svPos -= XMLoadFloat3(&m_vDir) * m_fSpeed * (_float)_dDeltaTime;
		m_pTransform->Set_State(CTransform::STATE_POSITION, svPos);
	}

	m_pCollider->Tick(_dDeltaTime);
	return _int();
}

_int CFlyingShield::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress)
		return iProgress;

	if (m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CFlyingShield::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SCB desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.color = _float4(0.f, 0.2862f, 0.5490f, 1.f);
	desc.empower = 0.8f;

	CWeapon::BindConstantBuffer(L"Camera_Silvermane", &desc);
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 0);

	return S_OK;
}

HRESULT CFlyingShield::Render_Shadow()
{
	CWeapon::BindConstantBuffer(L"Camera_Silvermane");
	CWeapon::BindLightBuffer();
	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
		m_pModel->Render(i, 1);

	return S_OK;
}

void CFlyingShield::OnTriggerEnter(CCollision& collision)
{
	if (m_isReturn)
	{
		if (collision.pGameObject == m_pOwner)
		{
			setActive(false);
			m_bRemove = true;
			static_cast<CSilvermane*>(m_pOwner)->End_ThrowShield();
		}
		return;
	}

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

		g_pObserver->Set_IsThrownObject(false);
		m_isAttack = false;
		m_isReturn = true;
		static_cast<CSilvermane*>(m_pOwner)->Return_Shield();
		break;
	}
}

HRESULT CFlyingShield::Ready_Components()
{
	m_pTransform->Set_WorldMatrix(m_tDesc.pOriginTransform->Get_WorldMatrix());

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Model_FlyingShield", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationRollPitchYaw(XMConvertToRadians(-90.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	m_pModel->Set_PivotMatrix(matPivot);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_FlyingShield"), 0);


	CCollider::DESC tColliderDesc;
	tColliderDesc.isTrigger = true;
	tColliderDesc.pGameObject = this;
	CBoxCollider::DESC tBoxColliderDesc;
	tBoxColliderDesc.tColliderDesc = tColliderDesc;
	tBoxColliderDesc.vScale = { 1.f, 0.2f, 1.f };
	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_BoxCollider", L"Collider", (CComponent**)&m_pCollider, &tBoxColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CFlyingShield* CFlyingShield::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CFlyingShield* pInstance = new CFlyingShield(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CFlyingShield Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CFlyingShield::Clone(_uint _iSceneID, void* _pArg)
{
	CFlyingShield* pInstance = new CFlyingShield(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, _pArg)))
	{
		MSGBOX("CFlyingShield Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFlyingShield::Free()
{
	__super::Free();

	Safe_Release(m_pCollider);
	Safe_Release(m_pModel);
}
