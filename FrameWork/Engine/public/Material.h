#pragma once

#include "Base.h"

BEGIN(Engine)

class CTexture;
class ENGINE_DLL CMaterial final : public CBase 
{
public:
	enum class EType {Static, Anim, Instance_Static, Instance_Anim, Max};
private:
	explicit CMaterial(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual~CMaterial() = default;

public:
	virtual HRESULT Native_Construct(const wstring& _wstrName, const wstring& _wstrShaderFilePath, const EType _eType);
	virtual HRESULT Render(const _uint _iPassIndex = 0);

	virtual HRESULT Compile_ShaderFiles(const wstring& _wstrShaderFilePath, D3D11_INPUT_ELEMENT_DESC* _pElementDesc, const _uint _iNumElements);
	HRESULT SetUp_ValueOnShader(const string _strConstantName, void* _pData, const _uint _iSize);
	HRESULT SetUp_TextureOnShader(const string _strConstantName, const aiTextureType eType, _uint _iTextureIndex = 0);

	const wstring& Get_Name() const;

	HRESULT Set_Texture(const string& _strConstantName, const aiTextureType _eTextureType, CTexture* _pTexture, const _uint _iTextureIndex = 0);
	void Set_InputLayout(_uint iPassIndex);

private:
	vector<CTexture*> m_vecTextures;
	wstring m_wstrShaderPath = L"";
	wstring m_wstrName = L"";
	EType m_eType = EType::Static;

	ID3DX11Effect* m_pEffect = nullptr;
	vector<EFFECTDESC*>	m_vecEffectDescs;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

public:
	static CMaterial* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const wstring& _wstrName, const wstring& _wstrShaderFilePath, const EType _eType);
	virtual void Free() override;
};

END