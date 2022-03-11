#include "pch.h"
#include "Monster_EarthAberrant.h"

CMonster_EarthAberrant::CMonster_EarthAberrant(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CActor(_pDevice, _pDeviceContext)
{
}

CMonster_EarthAberrant::CMonster_EarthAberrant(const CMonster_EarthAberrant& _rhs)
	:CActor(_rhs)
{
}

HRESULT CMonster_EarthAberrant::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_EarthAberrant::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	_vector Pos = { 3.f, 0.f, 0.f, 1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, Pos);
	return S_OK;
}

_int CMonster_EarthAberrant::Tick(_double _dDeltaTime)
{
	if (0 > __super::Tick(_dDeltaTime))
	{
		return -1;
	}

	m_pAnimControllerCom->Tick(_dDeltaTime);

	/*if (g_pGameInstance->getkeyDown(DIK_NUMPAD9))
	{
		++itest;
	}*/

	m_pAnimControllerCom->SetUp_NextAnimation(itest, true);
	m_pAnimControllerCom->Set_RootMotion(true, false);
	return 0;
}

_int CMonster_EarthAberrant::LateTick(_double _dDeltaTime)
{
	if (0 > __super::LateTick(_dDeltaTime))
	{
		return -1;
	}

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CMonster_EarthAberrant::Render()
{
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"MainCamera");

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
	{
		//m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModelCom->Render(i, 0);
	}

	return S_OK;
}

HRESULT CMonster_EarthAberrant::SetUp_Components()
{
	CTransform::TRANSFORMDESC Desc;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);
	m_pTransform->Set_TransformDesc(Desc);

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"Model_Monster_EarthAberrant", L"Model", (CComponent**)&m_pModelCom)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"AnimationController", L"Com_AnimationController", (CComponent**)&m_pAnimControllerCom)))
	{
		return E_FAIL;
	}

	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_EarthAberrant_Crystal"), 0);
	m_pModelCom->Add_Material(g_pGameInstance->Get_Material(L"Mtrl_EarthAberrant_Body"), 1);
	
	m_pAnimControllerCom->Set_GameObject(this);
	m_pAnimControllerCom->Set_Model(m_pModelCom);
	m_pAnimControllerCom->Set_Transform(m_pTransform);

	return S_OK;
}

CMonster_EarthAberrant* CMonster_EarthAberrant::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMonster_EarthAberrant* pInstance = new CMonster_EarthAberrant(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMonster_EarthAberrant");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonster_EarthAberrant::Clone(void* _pArg)
{
	CMonster_EarthAberrant* pInstance = new CMonster_EarthAberrant(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("Failed to Creating Clone CMonster_EarthAberrant");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_EarthAberrant::Free()
{
	Safe_Release(m_pModelCom);
	__super::Free();
}
