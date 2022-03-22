#include "pch.h"
#include "EarthAberrant_Pick.h"

#include "Monster_EarthAberrant.h"
#include "HierarchyNode.h"


CEarthAberrant_Pick::CEarthAberrant_Pick(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CWeapon(pDevice, pDeviceContext)
{
}

CEarthAberrant_Pick::CEarthAberrant_Pick(const CEarthAberrant_Pick& rhs)
	: CWeapon(rhs)
{
}

HRESULT CEarthAberrant_Pick::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Earth_Pick;
	m_wstrName = L"EarthAberrant_Pick";

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

	return S_OK;
}

_int CEarthAberrant_Pick::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	Attach_FixedBone(TimeDelta);
	Attach_Owner(TimeDelta);
	
	return _int();
}

_int CEarthAberrant_Pick::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CEarthAberrant_Pick::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix));
	m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix));

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		m_pModel->Render(i, 0);
	}
}

HRESULT CEarthAberrant_Pick::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Weapon_EarthAberrant_Pick", L"Com_Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"MI_Earth_Aberrant_Pick"), 0);

	return S_OK;
}

_int CEarthAberrant_Pick::Attach_FixedBone(const _double& TimeDelta)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= XMLoadFloat4x4(&m_smatOwnerPivot);

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

	__super::Free();
}
