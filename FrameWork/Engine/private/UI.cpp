#include"UI.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"

CUI::CUI()
	: m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pTrapziumBuffer(nullptr)
	, m_bFirstShow(false)
	, m_bShow(false)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pTrapziumBuffer(nullptr)
	, m_bFirstShow(false)
	, m_bShow(false)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
	, m_pBuffer(rhs.m_pBuffer)
	, m_pTexture(rhs.m_pTexture)
	, m_pTrapziumBuffer(rhs.m_pTrapziumBuffer)
	, m_bFirstShow(rhs.m_bFirstShow)
	, m_bShow(rhs.m_bShow)
	, m_Desc(rhs.m_Desc)
{
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pTrapziumBuffer);
}

HRESULT CUI::NativeConstruct_Prototype()
{
	if (FAILED(CGameObject::NativeConstruct_Prototype()))
		return E_FAIL;

	/*m_pBuffer=g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_RectBuffer");

	if (!m_pBuffer)
		return E_FAIL;

	if (FAILED(SetUp_Components(L"Com_RectBuffer", m_pBuffer)))
		return E_FAIL;*/

	m_pTexture=g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (!m_pTexture)
		return E_FAIL;
	if (FAILED(SetUp_Components(L"Com_Texture", m_pTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
		return E_FAIL;

	/*if (!pArg)
		return S_OK;

	wstring pTextureTag = (*(wstring*)pArg);

	if (FAILED(m_pTexture->Change_Texture(pTextureTag)))
		return E_FAIL;*/

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
	__super::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTrapziumBuffer);
}
