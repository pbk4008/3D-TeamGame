#include "pch.h"
#include "BackGround.h"

CBackGround::CBackGround()
	:m_pRcTex(nullptr)
	,m_pTexture(nullptr)
{
}

CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) 
	: CGameObject(pDevice, pDeviceContext)
	, m_pRcTex(nullptr)
	, m_pTexture(nullptr)
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	: CGameObject(rhs)
	,m_pRcTex(nullptr)
	, m_pTexture(nullptr)
{
}

HRESULT CBackGround::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::NativeConstruct(void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBackGround::Tick(_double fDeltaTime)
{
	if (FAILED(CGameObject::Tick(fDeltaTime)))
		return -1;

	return 0;
}

_int CBackGround::LateTick(_double fDeltaTime)
{
	if (FAILED(CGameObject::LateTick(fDeltaTime)))
		return -1;
	if (!m_pRenderer)
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return 0;
}

HRESULT CBackGround::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	if (!m_pRcTex)
		return E_FAIL;
	
	_matrix matWorld, matView, matProj;
	matWorld = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());

	_matrix Identity = XMMatrixIdentity();
	m_pRcTex->SetUp_ValueOnShader("g_WorldMatrix", &matWorld, sizeof(_matrix));
	m_pRcTex->SetUp_ValueOnShader("g_ViewMatrix", &Identity, sizeof(_matrix));
	m_pRcTex->SetUp_ValueOnShader("g_ProjMatrix", &Identity, sizeof(_matrix));
	m_pRcTex->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pRcTex->Render(0);

	return S_OK;
}

CGameObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstance = new CBackGround(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("BackGround Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CBackGround* pInstance = new CBackGround(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("BackGround Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

HRESULT CBackGround::Ready_GameObject()
{

	return S_OK;
}

void CBackGround::Free()
{
	CGameObject::Free();
	Safe_Release(m_pRcTex);
	Safe_Release(m_pTexture);
}
