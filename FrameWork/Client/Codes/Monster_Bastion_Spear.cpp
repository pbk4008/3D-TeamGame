#include "pch.h"
#include "Monster_Bastion_Spear.h"

CMonster_Bastion_Spear::CMonster_Bastion_Spear(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CActor(_pDevice, _pDeviceContext)
{
}

CMonster_Bastion_Spear::CMonster_Bastion_Spear(const CMonster_Bastion_Spear& _rhs)
	: CActor(_rhs)
{
}

HRESULT CMonster_Bastion_Spear::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype())) return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Bastion_Spear::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg))) return E_FAIL;

	if (FAILED(Ready_Components())) return E_FAIL;

	return S_OK;
}

_int CMonster_Bastion_Spear::Tick(_double _dDeltaTime)
{
	_int iProgress = __super::Tick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	m_pAnimationController->Tick(_dDeltaTime);

	return _int();
}

_int CMonster_Bastion_Spear::LateTick(_double _dDeltaTime)
{
	_int iProgress = __super::LateTick(_dDeltaTime);
	if (NO_EVENT != iProgress) return iProgress;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this))) return -1;

	return _int();
}

HRESULT CMonster_Bastion_Spear::Render()
{
	if (FAILED(__super::Render())) return E_FAIL;

	_matrix smatWorld, smatView, smatProj;
	smatWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	smatView = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	smatProj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (FAILED(m_pModel->SetUp_ValueOnShader("g_WorldMatrix", &smatWorld, sizeof(_matrix)))) return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ViewMatrix", &smatView, sizeof(_matrix)))) return E_FAIL;
	if (FAILED(m_pModel->SetUp_ValueOnShader("g_ProjMatrix", &smatProj, sizeof(_matrix)))) return E_FAIL;

	for (_uint i = 0; i < m_pModel->Get_NumMeshContainer(); ++i)
	{
		//if (FAILED(m_pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE))) return E_FAIL;

		if (FAILED(m_pModel->Render(i, 0))) return E_FAIL;
	}

	return S_OK;
}

HRESULT CMonster_Bastion_Spear::Ready_Components()
{
	CTransform::TRANSFORMDESC transformDesc;
	transformDesc.fSpeedPerSec = 10.f;
	transformDesc.fRotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_TransformDesc(transformDesc);
	_float4 vPosition = { -3.f, 0.f, 3.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_JS, L"Model_Bastion_Spear", L"Model", (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Bastion_Tier1_Down"), 0);
	m_pModel->Add_Material(g_pGameInstance->Get_Material(L"Bastion_Tier1_Top"), 1);

	if (FAILED(SetUp_Components((_uint)SCENEID::SCENE_TEST_JS, L"Component_AnimationController", L"AnimationController", (CComponent**)&m_pAnimationController)))
		return E_FAIL;
	m_pAnimationController->Set_GameObject(this);
	m_pAnimationController->Set_Model(m_pModel);
	m_pAnimationController->Set_Transform(m_pTransform);

	return S_OK;
}

CMonster_Bastion_Spear* CMonster_Bastion_Spear::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_Bastion_Spear* pInstance = new CMonster_Bastion_Spear(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMonster_Bastion_Spear Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_Bastion_Spear::Clone(void* _pArg)
{
	CMonster_Bastion_Spear* pInstance = new CMonster_Bastion_Spear(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("CMonster_Bastion_Spear Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Bastion_Spear::Free()
{
	Safe_Release(m_pAnimationController);
	Safe_Release(m_pModel);

	__super::Free();
}
