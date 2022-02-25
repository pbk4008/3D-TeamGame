#include "Floor.h"
#include "Floor.h"
#include "GameInstance.h"

CFloor::CFloor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice,pDeviceContext)
{
}

CFloor::CFloor(const CGameObject& rhs)
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
	//m_pVIBufferCom->Culling(m_pTransformCom->Get_WorldMatrixInverse());

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CFloor::Render()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix world, view, proj;
	world = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	view = XMMatrixTranspose(pGameInstance->Get_Transform(L"Dynamic", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	proj = XMMatrixTranspose(pGameInstance->Get_Transform(L"Dynamic", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &world, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &view, sizeof(_matrix));
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &proj, sizeof(_matrix));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseSourTexture", m_pTextureCom, 0);
	
	//m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseDestTexture", m_pTextureCom, 1);
	//m_pVIBufferCom->SetUp_TextureOnShader("g_FilterTexture", m_pFilterTexCom[0]);
	//m_pVIBufferCom->SetUp_TextureOnShader("g_BrushTexture", m_pFilterTexCom[1]);

	m_pVIBufferCom->Render(0);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CFloor::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_STATIC, TEXT("PrototypeTerrainVIBuffer"), TEXT("TerrainCom"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	//if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_STATIC, L"Texture", L"Com_Texture", (CComponent**)&m_pTextureCom, &wstr)))
	//	return E_FAIL;

	wstring wstr = L"TerrainBase";
	m_pTextureCom = g_pGameInstance->Clone_Component<CTexture>(0, L"Texture", &wstr);

	if (m_pTextureCom == nullptr)
		return E_FAIL;

	/*if(FAILED(__super::SetUp_Components(L"TerrainBase")))*/

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

	//Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
