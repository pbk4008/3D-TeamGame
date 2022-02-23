#pragma once
#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__
#include "SingleTon.h"
BEGIN(Engine)
class CTextureManager final : public CSingleTon<CTextureManager>
{
	friend CSingleTon;
private:
	NO_COPY(CTextureManager);
	explicit CTextureManager();
	virtual ~CTextureManager() = default;
public:
	HRESULT Add_Texture(ID3D11Device* pDevice, const wstring& pTextureTag, const wstring& pFilePath, _uint iTextureCnt = 1);
	HRESULT Delete_Texture();
	vector<ID3D11ShaderResourceView*>* Get_Texture(const wstring& pTextureTag);
private:
	HRESULT Init_Texture(ID3D11Device* pDevice,  const wstring& pTextureTag, const wstring& pFilePath, _uint iTextureCnt);
private:
	virtual void Free();
private:
	map<wstring, vector<ID3D11ShaderResourceView*>> 	m_mapTexture;
};
END
#endif