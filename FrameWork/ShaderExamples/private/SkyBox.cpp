#include "SkyBox.h"
#include "GameInstance.h"

CSkyBox::CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice,pDeviceContext)
{
}

CSkyBox::CSkyBox(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyBox::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyBox::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	
	return S_OK;
}

_int CSkyBox::Tick(_double TimeDelta)
{
	return _int();
}

_int CSkyBox::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		if (m_pRenderer->Get_Shadow())
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);

		if (m_pRenderer->Get_PBR())
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PBR, this);

		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	}

	m_pTransform->Set_State(CTransform::STATE_POSITION, g_pGameInstance->Get_CamPosition(L"MainCamera"));
	return _int();
}

HRESULT CSkyBox::Render()
{
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture->Get_ShaderResourceView(3));

	m_pVIBufferCom->Render(0);
	return S_OK;
}

HRESULT CSkyBox::Render_Shadow()
{
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture->Get_ShaderResourceView(3));

	m_pVIBufferCom->Render(0);
	return S_OK;
}

HRESULT CSkyBox::Render_ShadeShadow(ID3D11ShaderResourceView* ShaodwMap)
{
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture->Get_ShaderResourceView(3));

	m_pVIBufferCom->Render(0);
	return S_OK;
}

HRESULT CSkyBox::Render_PBR()
{
	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture->Get_ShaderResourceView(3));

	m_pVIBufferCom->Render(1);
	return S_OK;
}

HRESULT CSkyBox::SetUp_Components()
{
	CTransform::TRANSFORMDESC TransDesc;

	TransDesc.fRotationPerSec = XMConvertToRadians(90.f);
	TransDesc.fSpeedPerSec = 20.f;

	m_pTransform->Set_TransformDesc(TransDesc);

	m_pTexture = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");

	m_pTexture->Change_Texture(L"SkyBox");

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Components((_uint)SCENEID::SCENE_STAGE1, L"PrototypeCubeVIBuffer", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

CSkyBox* CSkyBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CSkyBox* pInstance = new CSkyBox(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyBox::Clone(void* pArg)
{
	CSkyBox* pInstance = new CSkyBox(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBufferCom);
}
