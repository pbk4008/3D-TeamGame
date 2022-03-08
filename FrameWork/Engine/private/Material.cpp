#include "Material.h"

#include "Texture.h"

CMaterial::CMaterial(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: m_pDevice(_pDevice)
	, m_pDeviceContext(_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	m_vecTextures.resize(AI_TEXTURE_TYPE_MAX);
}

HRESULT CMaterial::Native_Construct(const wstring& _wstrName, const wstring& _wstrShaderFilePath, const EType _eType)
{
	m_wstrName = _wstrName;

	switch (_eType)
	{
	case EType::Static:
	{
		D3D11_INPUT_ELEMENT_DESC ElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		if (FAILED(Compile_ShaderFiles(_wstrShaderFilePath, ElementDescs, 5)))
			return E_FAIL;
	}
		break;
	case EType::Anim:
	{
		D3D11_INPUT_ELEMENT_DESC ElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		if (FAILED(Compile_ShaderFiles(_wstrShaderFilePath, ElementDescs, 7)))
			return E_FAIL;
	}
		break;
	case EType::Instance_Static:
	{
		D3D11_INPUT_ELEMENT_DESC ElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};
		if (FAILED(Compile_ShaderFiles(_wstrShaderFilePath, ElementDescs, 9)))
			return E_FAIL;
	}
	break;
	case EType::Instance_Anim:
	{
		D3D11_INPUT_ELEMENT_DESC ElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};
		if (FAILED(Compile_ShaderFiles(_wstrShaderFilePath, ElementDescs, 10)))
			return E_FAIL;
	}
	break;
	}

	m_wstrShaderPath = _wstrShaderFilePath;
	m_eType = _eType;
	return S_OK;
}

HRESULT CMaterial::Render(const _uint _iPassIndex)
{
	if (FAILED(m_vecEffectDescs[_iPassIndex]->pPass->Apply(0, m_pDeviceContext)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMaterial::Compile_ShaderFiles(const wstring& _wstrShaderFilePath, D3D11_INPUT_ELEMENT_DESC* _pElementDesc, const _uint _iNumElements)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob* pShaderByteCodes = nullptr;
	ID3DBlob* pCompileErrorMessage = nullptr;

	if (FAILED(D3DCompileFromFile(_wstrShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pShaderByteCodes, &pCompileErrorMessage)))
		return E_FAIL;

	if (FAILED(D3DX11CreateEffectFromMemory(pShaderByteCodes->GetBufferPointer(), pShaderByteCodes->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC			EffectDesc;

	if (FAILED(pTechnique->GetDesc(&EffectDesc)))
		return E_FAIL;

	m_vecEffectDescs.reserve(EffectDesc.Passes);

	for (_uint i = 0; i < EffectDesc.Passes; ++i)
	{
		EFFECTDESC* pEffectDesc = new EFFECTDESC;
		ZeroMemory(pEffectDesc, sizeof(EFFECTDESC));

		pEffectDesc->pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC			PassDesc;

		pEffectDesc->pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(_pElementDesc, _iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pEffectDesc->pInputLayout)))
			return E_FAIL;

		m_vecEffectDescs.push_back(pEffectDesc);
	}

	return S_OK;
}

HRESULT CMaterial::SetUp_ValueOnShader(const string _strConstantName, void* _pData, const _uint _iSize)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_strConstantName.c_str());
	if (nullptr == pVariable)
		return E_FAIL;

	if (FAILED(pVariable->SetRawValue(_pData, 0, _iSize)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMaterial::SetUp_TextureOnShader(const string _strConstantName, const aiTextureType _eTextureType, _uint _iTextureIndex)
{
	if (m_vecTextures[_eTextureType])
	{
		ID3D11ShaderResourceView* pShaderResourceView = m_vecTextures[_eTextureType]->Get_ShaderResourceView(_iTextureIndex);
		if (pShaderResourceView)
		{
			ID3DX11EffectShaderResourceVariable* pVariable = m_pEffect->GetVariableByName(_strConstantName.c_str())->AsShaderResource();
			if (nullptr == pVariable)
				return E_FAIL;

			return pVariable->SetResource(pShaderResourceView);
		}
	}

	return S_OK;
}

const wstring& CMaterial::Get_Name() const
{
	return m_wstrName;
}

HRESULT CMaterial::Set_Texture(const string& _strConstantName, const aiTextureType _eTextureType, CTexture* _pTexture, const _uint _iTextureIndex)
{
	if (m_vecTextures[_eTextureType])
	{
		Safe_Release(m_vecTextures[_eTextureType]);
	}
	m_vecTextures[_eTextureType] = _pTexture;

	SetUp_TextureOnShader(_strConstantName.c_str(), _eTextureType, _iTextureIndex);
	return S_OK;
}

void CMaterial::Set_InputLayout(_uint iPassIndex)
{
	m_pDeviceContext->IASetInputLayout(m_vecEffectDescs[iPassIndex]->pInputLayout);
}

CMaterial* CMaterial::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const wstring& _wstrName, const wstring& _wstrShaderFilePath, const EType _eType)
{
	CMaterial* pInstance = new CMaterial(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->Native_Construct(_wstrName, _wstrShaderFilePath, _eType)))
	{
		MSGBOX("CMaterial Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMaterial::Free()
{
	for (auto& pEffectDesc : m_vecEffectDescs)
	{
		//Safe_Release(pEffectDesc->pPass);
		Safe_Release(pEffectDesc->pInputLayout);
		Safe_Delete(pEffectDesc);
	}
	m_vecEffectDescs.clear();

	Safe_Release(m_pEffect);

	for (auto& pMtrl : m_vecTextures)
		Safe_Release(pMtrl);
	m_vecTextures.clear();

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
