#include "TextureManager.h"

CTextureManager::CTextureManager()
{
}

HRESULT CTextureManager::Add_Texture(ID3D11Device* pDevice, const wstring& pTextureTag, const wstring& pFilePath, _uint iTextureCnt)
 {
	vector<ID3D11ShaderResourceView*>* pResource = Get_Texture(pTextureTag);
	if (pResource)
		return S_OK;

	return Init_Texture(pDevice, pTextureTag, pFilePath, iTextureCnt);
}

HRESULT CTextureManager::Delete_Texture()
{
	for (auto& pTextureVec : m_mapTexture)
	{
		for (auto& pTexture : pTextureVec.second)
			Safe_Release(pTexture);
		pTextureVec.second.clear();
		pTextureVec.second.shrink_to_fit();
	}
	m_mapTexture.clear();

	return S_OK;
}

HRESULT CTextureManager::Init_Texture(ID3D11Device* pDevice, const wstring& pTextureTag, const wstring& pFilePath, _uint iTextureCnt)
{
	if (!pDevice)
		return E_FAIL;

  	DirectX::ScratchImage tImage;

	_tchar szExt[MAX_PATH] = L"";
	_wsplitpath_s(pFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 256);

	_tchar szFullPath[MAX_PATH] = L"";
	HRESULT hr = 0;
	
	vector<ID3D11ShaderResourceView*> vecResource;
	vecResource.reserve(iTextureCnt);
	for (_uint i = 0; i < iTextureCnt; i++)
	{
		wsprintf(szFullPath, pFilePath.c_str(), i);

		if (!lstrcmp(szExt, L".dds"))
			hr = LoadFromDDSFile(szFullPath, CP_FLAGS_NONE, nullptr, tImage);
		else if (!lstrcmp(szExt, L".tga"))
		{
			hr = LoadFromTGAFile(szFullPath, nullptr, tImage);

			/*DirectX::ScratchImage MipChain;
			hr = DirectX::GenerateMipMaps(tImage.GetImages(), tImage.GetImageCount(), tImage.GetMetadata(), TEX_FILTER_FANT, 0, MipChain);
			if (FAILED(hr))
				return E_FAIL;
			else
			{
				ID3D11Resource* pTextureResource = nullptr;
			
				if (FAILED(DirectX::CreateTexture(pDevice, MipChain.GetImages(), MipChain.GetImageCount(), MipChain.GetMetadata(), &pTextureResource)))
					return E_FAIL;
			
				ID3D11ShaderResourceView* pShaderResourceView = nullptr;
			
				if (FAILED(pDevice->CreateShaderResourceView(pTextureResource, nullptr, &pShaderResourceView)))
					return E_FAIL;
			
				vecResource.emplace_back(pShaderResourceView);
			
				Safe_Release(pTextureResource);
				tImage.Release();
				MipChain.Release();
				continue;
			}*/
		}
		else
			hr = LoadFromWICFile(szFullPath, CP_FLAGS_NONE, nullptr, tImage);

		if (FAILED(hr))
			return E_FAIL;

		ID3D11Resource* pTextureResource = nullptr;
		if (FAILED(CreateTexture(pDevice, tImage.GetImages(), tImage.GetImageCount(), tImage.GetMetadata(), &pTextureResource)))
			return E_FAIL;

		ID3D11ShaderResourceView* pResourceView = nullptr;

		if (FAILED(pDevice->CreateShaderResourceView(pTextureResource, nullptr, &pResourceView)))
			return E_FAIL;

		vecResource.emplace_back(pResourceView);

		Safe_Release(pTextureResource);
		tImage.Release();          
	}
	m_mapTexture.emplace(pTextureTag, vecResource);
	return S_OK;
}

vector<ID3D11ShaderResourceView*>* CTextureManager::Get_Texture(const wstring& pTextureTag)
{
	auto iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),
		[&pTextureTag](pair<const wstring&, vector<ID3D11ShaderResourceView*>> Pair)
		->
		bool {
			if (pTextureTag == Pair.first)
				return true;
			else
				return false;
		}
	);
	if (iter == m_mapTexture.end())
		return nullptr;

	return &(iter->second);
}



void CTextureManager::Free()
{
	if (FAILED(Delete_Texture()))
		return;
}
