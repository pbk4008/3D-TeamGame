#include "UI_Texture.h"

UI_Texture::UI_Texture(void)
{
}

UI_Texture::~UI_Texture(void)
{
}

void UI_Texture::NativeConstruct(std::wstring _fg)
{
	__super::NativeConstruct(_fg);

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	assert("Failed to Create UI_Texture" && m_pDevice);
	
	_tchar szFullPath[MAX_PATH] = L"";
	_tchar szExt[MAX_PATH] = L"";
	DirectX::ScratchImage tImage;

	_wsplitpath_s(_fg.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 256);

	wsprintf(szFullPath, _fg.c_str(), 1);
	
	if (!lstrcmp(szExt, L".dds"))
	{
		LoadFromDDSFile(szFullPath, CP_FLAGS_NONE, nullptr, tImage);
	}
	else
		assert("DDS File Only");

	ID3D11Resource* pTextureResource = nullptr;
	CreateTexture(m_pDevice, tImage.GetImages(), tImage.GetImageCount(), tImage.GetMetadata(), &pTextureResource);
	assert("Failed to Create ID3D11Resource" && pTextureResource);

	ID3D11ShaderResourceView* pResourceView = nullptr;
	m_pDevice->CreateShaderResourceView(pTextureResource, nullptr, &pResourceView);
	assert("Failed to Create ID3D11ShaderResourceView" && pResourceView);


	Safe_Release(pTextureResource);
	tImage.Release();

	m_vecTextures.push_back(pResourceView);
}

void UI_Texture::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	for (auto& pTextureVec : m_vecTextures)
	{
		Safe_Release(pTextureVec);
	}
	m_vecTextures.clear();

}

void UI_Texture::Set_Texture(ID3D11ShaderResourceView* _image)
{
}

ID3D11ShaderResourceView* UI_Texture::Get_Texture(size_t _index)
{
	ID3D11ShaderResourceView* tex = nullptr;
	if (_index < m_vecTextures.size())
		tex = m_vecTextures[_index];
	else
		tex = m_vecTextures.back();

	return tex;
}
