#include "framework.h"
#include "pch.h"
#include "MFCTerrain.h"
#include "GameInstance.h"

CMFCTerrain::CMFCTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMFCTerrain::CMFCTerrain(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMFCTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMFCTerrain::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CMFCTerrain::Tick(_double TimeDelta)
{
	return _int();
}

_int CMFCTerrain::LateTick(_double TimeDelta)
{

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_PRIORITY, this);
	}
	return _int();
}

HRESULT CMFCTerrain::Render()
{

	SetUp_Shader();

	m_pVIBufferCom->Render(0);
	
	return S_OK;
}

void CMFCTerrain::SetUp_Shader()
{
	/* l-value 오류때문에 다 선언해줌. */
	_matrix World = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix View = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MFCCamera_Proj", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix Proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MFCCamera_Proj", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"MFCCamera_Proj");

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &World, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &View, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &Proj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureCom, 0);

	m_pVIBufferCom->SetUp_ValueOnShader("g_vCamPosition", (void*)&CamPos, sizeof(_vector));
}

HRESULT CMFCTerrain::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components(0, L"Prototype_Component_VIBuffer_Terrain", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

CMFCTerrain* CMFCTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMFCTerrain* pInstance = new CMFCTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMFCTerrain::Clone(void* pArg)
{
	CMFCTerrain* pInstance = new CMFCTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMFCTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
