#include "Floor.h"
#include "Floor.h"
#include "GameInstance.h"

CFloor::CFloor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice,pDeviceContext)
{
}

CFloor::CFloor(const CFloor& rhs)
	: CGameObject(rhs)
{
}

HRESULT CFloor::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CFloor::Tick(_double TimeDelta)
{
	return _int();
}

_int CFloor::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CFloor::Render()
{
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseSourTexture", m_pTexture->Get_ShaderResourceView(0));
	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseDestTexture", m_pTexture->Get_ShaderResourceView(1));
	/*m_pVIBufferCom->SetUp_TextureOnShader("g_FilterTexture", m_pFilterTexture->Get_ShaderResourceView(0));
	m_pVIBufferCom->SetUp_TextureOnShader("g_BrushTexture", m_pBrushTexture->Get_ShaderResourceView(0));*/

	m_pVIBufferCom->Render(0);


	return S_OK;
}

HRESULT CFloor::Render_Shadow()
{
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(m_LightDesc->mLightView);
	proj = XMMatrixTranspose(m_LightDesc->mLightProj);

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_LightView", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_LightProj", &proj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseSourTexture", m_pTexture->Get_ShaderResourceView(0));
	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseDestTexture", m_pTexture->Get_ShaderResourceView(1));
	//m_pVIBufferCom->SetUp_TextureOnShader("g_FilterTexture", m_pFilterTexture->Get_ShaderResourceView(0));
	//m_pVIBufferCom->SetUp_TextureOnShader("g_BrushTexture", m_pBrushTexture->Get_ShaderResourceView(0));

	m_pVIBufferCom->Render(1);
	return S_OK;
}

HRESULT CFloor::Render_ShadeShadow(ID3D11ShaderResourceView* ShadowMap)
{
	_matrix world, view, proj, lightview, lightproj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	lightview = XMMatrixTranspose(m_LightDesc->mLightView);
	lightproj = XMMatrixTranspose(m_LightDesc->mLightProj);

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_LightView", &lightview, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_LightProj", &lightproj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseSourTexture", m_pTexture->Get_ShaderResourceView(0));
	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseDestTexture", m_pTexture->Get_ShaderResourceView(1));
	//m_pVIBufferCom->SetUp_TextureOnShader("g_FilterTexture", m_pFilterTexture->Get_ShaderResourceView(0));
	//m_pVIBufferCom->SetUp_TextureOnShader("g_BrushTexture", m_pBrushTexture->Get_ShaderResourceView(0));
	m_pVIBufferCom->SetUp_TextureOnShader("g_ShadowTexture", ShadowMap);

	m_pVIBufferCom->Render(2);
	return S_OK;
}

HRESULT CFloor::Render_PBR()
{
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));

	m_pVIBufferCom->Render(3);
	return S_OK;
}

HRESULT CFloor::SetUp_Components()
{
	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Texture");
	m_pFilterTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Texture");
	m_pBrushTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Texture");

	m_pTexture->Change_Texture(L"FloorBase");
	//m_pFilterTexture->Change_Texture(L"FloorFilter");
	//m_pBrushTexture->Change_Texture(L"FloorBrush");

	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"PrototypeTerrainVIBuffer", L"TerrainCom", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	m_LightDesc = g_pGameInstance->Get_LightDesc(0);

	return S_OK;
}

CFloor* CFloor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CFloor* pInstance = new CFloor(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CFloor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFloor::Clone(void* pArg)
{
	CFloor* pInstance = new CFloor(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating  Clone CFloor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFloor::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pBrushTexture);
	Safe_Release(m_pVIBufferCom);
}
