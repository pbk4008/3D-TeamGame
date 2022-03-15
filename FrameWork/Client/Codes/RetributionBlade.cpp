#include "pch.h"
#include "RetributionBlade.h"

#include "HierarchyNode.h"
#include "Silvermane.h"
#include "Monster_Bastion_2HSword.h"
#include "StateController.h"

CRetributionBlade::CRetributionBlade(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CWeapon(_pDevice, _pDeviceContext)
{
}

CRetributionBlade::CRetributionBlade(const CRetributionBlade& _rhs)
	: CWeapon(_rhs)
{
}

HRESULT CRetributionBlade::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = EType::Sword_2H;
	m_wstrName = L"RetributionBlade";

	return S_OK;
}

HRESULT CRetributionBlade::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (_pArg)
		m_pFixedBone = static_cast<CHierarchyNode*>(_pArg);

	//m_smatPivot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(-20.f), XMConvertToRadians(-67.f), XMConvertToRadians(0.f)) * XMMatrixTranslation(0.5f, 0.05f, -0.2f);

	return S_OK;
}

_int CRetributionBlade::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
		return -1;

	Attach_FixedBone(_dDeltaTime);
	Attach_Owner(_dDeltaTime);

	return _int();
}

_int CRetributionBlade::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
		return -1;

	if(m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CRetributionBlade::Render()
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

HRESULT CRetributionBlade::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 0.f;
	transformDesc.fRotationPerSec = 0.f;
	m_pTransform->Set_TransformDesc(transformDesc);
	m_pLocalTransform->Set_TransformDesc(transformDesc);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_YM, L"Model_RetributionBlade", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;

	return S_OK;
}

_int CRetributionBlade::Attach_FixedBone(const _double& _dDeltaTime)
{
	if (m_pFixedBone)
	{
		_matrix smatWorld = m_pFixedBone->Get_CombinedMatrix();

		smatWorld *= m_smatOwnerPivot;

		if (!m_isEquip)
		{
			smatWorld = m_smatPivot * smatWorld;
		}

		m_pLocalTransform->Set_WorldMatrix(smatWorld);
	}

	return NO_EVENT;
}

_int CRetributionBlade::Attach_Owner(const _double& _dDeltaTime)
{
	if (m_pOwner)
	{
		_matrix smatWorld = m_pLocalTransform->Get_WorldMatrix();
		_matrix smatOwerWorld = static_cast<CSilvermane*>(m_pOwner)->Get_Transform()->Get_CombinedMatrix();
		m_pTransform->Set_WorldMatrix(smatWorld * smatOwerWorld);
	}

	return _int();
}

CRetributionBlade* CRetributionBlade::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CRetributionBlade* pInstance = new CRetributionBlade(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CRetributionBlade Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CRetributionBlade::Clone(void* _pArg)
{
	CRetributionBlade* pInstance = new CRetributionBlade(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CRetributionBlade Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRetributionBlade::Free()
{

	__super::Free();
}
