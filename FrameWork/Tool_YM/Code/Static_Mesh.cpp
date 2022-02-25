#include "pch.h"
#include "Static_Mesh.h"
#include "GameInstance.h"

CStatic_Mesh::CStatic_Mesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CGameObject(_pDevice, _pDeviceContext)
{
}

CStatic_Mesh::CStatic_Mesh(const CStatic_Mesh& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CStatic_Mesh::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStatic_Mesh::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_ModelDesc = (MODELDESC*)pArg;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CStatic_Mesh::Tick(_double TimeDelta)
{
	return _int();
}

_int CStatic_Mesh::LateTick(_double TimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return _int();
}

HRESULT CStatic_Mesh::Render()
{
	Set_WVPMatrix();

	if (nullptr != m_pModelCom)
	{
		if (FAILED(m_pModelCom->Bind_Buffers()))
			return E_FAIL;

		for (_uint i = 0; i < m_pModelCom->Get_NumMeshContainer(); ++i)
		{
			m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE);
			m_pModelCom->Render(i, 0);
		}
	}

	return S_OK;
}

HRESULT CStatic_Mesh::SetUp_Components()
{
	/* Com_Model */
	if (FAILED(__super::SetUp_Components(TAB_MAP, m_ModelDesc->strFileName, L"Com_Model", (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CStatic_Mesh::Set_WVPMatrix(void)
{
	_fmatrix matWorld = m_pTransform->Get_WorldMatrix();
	auto vCamPos = g_pGameInstance->Get_CamPosition(L"Camera");
	 
	auto matWorldInvers = XMMatrixTranspose(matWorld);
	auto matViewInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	auto matProjInvers = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));


	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &matWorldInvers, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &matViewInvers, sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &matProjInvers, sizeof(_matrix));
	
}

CGameObject* CStatic_Mesh::Clone(void* pArg)
{
	CStatic_Mesh* pInstance = new CStatic_Mesh(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CStatic_Mesh!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CStatic_Mesh* CStatic_Mesh::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CStatic_Mesh* pInstance = new CStatic_Mesh(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CStatic_Mesh!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatic_Mesh::Free(void)
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
