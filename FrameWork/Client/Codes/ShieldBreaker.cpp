#include "pch.h"
#include "ShieldBreaker.h"

#include "Boss_Bastion_Judicator.h"
#include "HierarchyNode.h"


CShieldBreaker::CShieldBreaker(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{
}

CShieldBreaker::CShieldBreaker(const CShieldBreaker& rhs)
	: CWeapon(rhs)
{
}

HRESULT CShieldBreaker::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Hammer_2H;
	m_wstrName = L"ShieldBreaker";

	return S_OK;
}

HRESULT CShieldBreaker::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(pArg);

	return S_OK;
}

_int CShieldBreaker::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	Attach_FixedBone(TimeDelta);
	Attach_Owner(TimeDelta);
	
	return _int();
}

_int CShieldBreaker::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CShieldBreaker::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		m_pModel->Render(i, 0);
	}
}

HRESULT CShieldBreaker::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Weapon_ShieldBreaker", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	return S_OK;
}

_int CShieldBreaker::Attach_FixedBone(const _double& TimeDelta)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= m_smatOwnerPivot;

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
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

CGameObject* CShieldBreaker::Clone(void* pArg)
{
	CShieldBreaker* pInstance = new CShieldBreaker(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CShieldBreaker Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShieldBreaker::Free()
{

	__super::Free();
}
