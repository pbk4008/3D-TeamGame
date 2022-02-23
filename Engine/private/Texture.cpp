#include "..\public\Texture.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
	
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
{
	for (auto& pShaderResourceView : m_Textures)
		Safe_AddRef(pShaderResourceView);
}

HRESULT CTexture::NativeConstruct_Prototype(const _tchar * pTextureFilePath, _uint iNumTextures)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	_tchar		szFullPath[MAX_PATH] = TEXT("");

	
	DirectX::ScratchImage		ScratchImage;

	/*DirectX::SaveToWICFile();*/

	//

	//LPDIRECT3DTEXTURE9		pTexture;

	//pTexture->LockRect();

	//D3DXCreateTexture

	//	LockRect

	//D3DXSaveTextureToFile(, D3D)

	HRESULT hr = 0;

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		_tchar		szExt[MAX_PATH] = TEXT("");
		_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, 256);
		
		wsprintf(szFullPath, pTextureFilePath, i);		

		if (0 == lstrcmp(szExt, TEXT(".dds")))		
			hr = DirectX::LoadFromDDSFile(szFullPath, DirectX::CP_FLAGS_NONE, nullptr, ScratchImage);

		else if(0 == lstrcmp(szExt, TEXT(".tga")))
			hr = DirectX::LoadFromTGAFile(szFullPath, nullptr, ScratchImage);

		else
			hr = DirectX::LoadFromWICFile(szFullPath, DirectX::CP_FLAGS_NONE, nullptr, ScratchImage);

		if (FAILED(hr))
			return E_FAIL;

		ID3D11Resource*		pTextureResource = nullptr;

		if (FAILED(DirectX::CreateTexture(m_pDevice, ScratchImage.GetImages(), ScratchImage.GetImageCount(), ScratchImage.GetMetadata(), &pTextureResource)))
			return E_FAIL;

		ID3D11ShaderResourceView*		pShaderResourceView = nullptr;

		if (FAILED(m_pDevice->CreateShaderResourceView(pTextureResource, nullptr, &pShaderResourceView)))
			return E_FAIL;

		m_Textures.push_back(pShaderResourceView);

		Safe_Release(pTextureResource);
		ScratchImage.Release();
	}



	
	

	




	

	return S_OK;
}

HRESULT CTexture::NativeConstruct(void * pArg)
{
	return S_OK;
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pShaderResourceView : m_Textures)
		Safe_Release(pShaderResourceView);
}
