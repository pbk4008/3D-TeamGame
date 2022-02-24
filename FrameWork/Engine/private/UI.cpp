#include"UI.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"

CUI::CUI()
	: m_pBuffer(nullptr)
	, m_pTexture(nullptr)
{
}

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
{

}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
	, m_pBuffer(rhs.m_pBuffer)
	, m_pTexture(rhs.m_pTexture)
{
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pTexture);
}

HRESULT CUI::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pBuffer=g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"RectBuffer");

	if (!m_pBuffer)
		return E_FAIL;

	if (FAILED(SetUp_Components(L"RectBuffer", m_pBuffer)))
		return E_FAIL;

	m_pTexture=g_pGameInstance->Clone_Component<CTexture>(0, L"Texture");
	if (!m_pTexture)
		return E_FAIL;
	if (FAILED(SetUp_Components(L"Texture", m_pTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::NativeConstruct(void* pArg)
{
	if (FAILED(CGameObject::NativeConstruct(pArg)))
		return E_FAIL;

	wstring pTextureTag = (*(wstring*)pArg);

	if (FAILED(m_pTexture->Change_Texture(pTextureTag)))
		return E_FAIL;

	return S_OK;
}

_int CUI::Tick(_double TimeDelta)
{
	return _int();
}

_int CUI::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CUI::Render()
{
	return S_OK;
}

void CUI::Free()
{
	CGameObject::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}
