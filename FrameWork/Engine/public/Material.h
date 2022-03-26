#pragma once

#include "Base.h"
#include "SaveManager.h"
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
	HRESULT SetUp_TextureOnShader(const string _strConstantName, TEXTURETYPE eType, _uint _iTextureIndex = 0);
	HRESULT SetUp_TextureOnShader(const string _strConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT SetUp_TextureOnShader();

	const wstring& Get_Name() const;

	HRESULT Set_Texture(const string& _strConstantName, TEXTURETYPE _eTextureType, CTexture* _pTexture, const _uint _iTextureIndex = 0);
	HRESULT Set_Texture(TEXTURETYPE _eTextureType, const wstring& _pTextureTag, const wstring& _pTexturePath, _uint _iTextureIndex = 0);
	void Set_InputLayout(_uint iPassIndex);
	void Using_Tool();

	HRESULT Change_Material();
	CSaveManager::MTRLDATA SetMaterialSaveData();
public:
	list<wstring> Get_TextureName();
	const wstring Get_ShaderFileName() { return m_wstrShaderPath; }
	const _uint Get_Type() { return (_uint)m_eType; }
public:
	void Set_ShaderFileName(const wstring& pFileName) { m_wstrShaderPath = pFileName; }
	void Set_Type(_uint iType) { m_eType = (EType)iType; }
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