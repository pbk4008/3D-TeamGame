#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{	
private:
	explicit CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	ID3D11ShaderResourceView* Get_ShaderResourceView(_uint iIndex) {
		return m_Textures[iIndex];	
	}
public:
	virtual HRESULT NativeConstruct_Prototype(const wstring& pTextureFilePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	HRESULT Change_Texture(const wstring& pTextureTag);
private:	
	vector<ID3D11ShaderResourceView*>			m_Textures;
	typedef vector<ID3D11ShaderResourceView*>	TEXTURES;
public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pTextureFilePath, _uint iNumTextures = 1);
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END