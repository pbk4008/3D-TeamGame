#include "..\Public\Font_Manager.h"
#include "CustumFont.h"


CFont_Manager::CFont_Manager()
{
}

HRESULT CFont_Manager::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const wstring& pFontTag, const wstring& pFontPath)
{
	CCustomFont*		pFont = CCustomFont::Create(pDevice, pDeviceContext, pFontPath);

	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.insert(FONT::value_type(pFontTag, pFont));

	return S_OK;
}

HRESULT CFont_Manager::Render_Font(const wstring& pFontTag, _fvector vColor, const wstring& pString, const _float2& _vPos, const _float2& _vScale)
{
	auto	iter = find_if(m_Fonts.begin(), m_Fonts.end(), CTag_Finder(pFontTag));

	if (iter == m_Fonts.end())
		return E_FAIL;

	return iter->second->Render_CustomFont(vColor, pString, _vPos, _vScale);
}

void CFont_Manager::Free()
{
	for (auto& pFont : m_Fonts)
		Safe_Release(pFont.second);

	m_Fonts.clear();
}

