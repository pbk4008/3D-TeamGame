#include "Material.h"
#include "Texture.h"
#include "TextureManager.h"

CMaterial::CMaterial(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: m_pDevice(_pDevice)
	, m_pDeviceContext(_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	m_vecTextures.resize((_uint)TEXTURETYPE::TEX_END);
}

HRESULT CMaterial::Native_Construct(const wstring& _wstrName, const wstring& _wstrShaderFilePath, const EType _eType)
{
	m_wstrName = _wstrName;
	m_eType = _eType;

	switch (m_eType)
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

		   { "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		   { "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		   { "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		   { "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
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
		   { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		   { "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		   { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		   { "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		   { "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		   { "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		   { "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};
		if (FAILED(Compile_ShaderFiles(_wstrShaderFilePath, ElementDescs, 11)))
			return E_FAIL;
	}
	break;
	}

	m_wstrShaderPath = _wstrShaderFilePath;
	m_vecTextures.resize((_uint)TEXTURETYPE::TEX_END);

	return S_OK;
}

HRESULT CMaterial::Render(const _uint _iPassIndex)
{
	if (FAILED(m_vecEffectDescs[_iPassIndex]->pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

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

HRESULT CMaterial::SetUp_TextureOnShader(const string _strConstantName, TEXTURETYPE _eTextureType, _uint _iTextureIndex)
{
	if (m_vecTextures[(_uint)_eTextureType])
	{
		ID3D11ShaderResourceView* pShaderResourceView = m_vecTextures[(_uint)_eTextureType]->Get_ShaderResourceView(_iTextureIndex);
		if (pShaderResourceView)
		{
			ID3DX11EffectShaderResourceVariable* pVariable = m_pEffect->GetVariableByName(_strConstantName.c_str())->AsShaderResource();
			if (nullptr == pVariable)
				return E_FAIL;

			HRESULT hr = pVariable->SetResource(pShaderResourceView);

			return hr;
		}
	}

	return S_OK;
}

HRESULT CMaterial::SetUp_TextureOnShader(const string _strConstantName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectShaderResourceVariable* pVariable = m_pEffect->GetVariableByName(_strConstantName.c_str())->AsShaderResource();

	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetResource(pSRV);
}

const wstring& CMaterial::Get_Name() const
{
	return m_wstrName;
}

HRESULT CMaterial::Set_Texture(const string& _strConstantName, TEXTURETYPE _eTextureType, CTexture* _pTexture, const _uint _iTextureIndex)
{
	if (m_vecTextures[(_uint)_eTextureType])
		Safe_Release(m_vecTextures[(_uint)_eTextureType]);
	m_vecTextures[(_uint)_eTextureType] = _pTexture;

	SetUp_TextureOnShader(_strConstantName.c_str(), _eTextureType, _iTextureIndex);
	return S_OK;
}

HRESULT CMaterial::Set_Texture(TEXTURETYPE _eTextureType, const wstring& _pTextureTag, const wstring& _pTexturePath, _uint _iTextureIndex)
{
	if (m_vecTextures[(_uint)_eTextureType])
	{
		Safe_Release(m_vecTextures[(_uint)_eTextureType]);
	}


	CTextureManager* pTextureMgr = GET_INSTANCE(CTextureManager);

	if (FAILED(pTextureMgr->Add_Texture(m_pDevice, _pTextureTag, _pTexturePath)))
		return E_FAIL;

	RELEASE_INSTANCE(CTextureManager);

	CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);

	if (!pTexture)
		return E_FAIL;

	if (FAILED(pTexture->Change_Texture(_pTextureTag)))
		return E_FAIL;

	m_vecTextures[(_uint)_eTextureType] = pTexture;
	
	return S_OK;
}

void CMaterial::Set_InputLayout(_uint iPassIndex)
{
	m_pDeviceContext->IASetInputLayout(m_vecEffectDescs[iPassIndex]->pInputLayout);
}

void CMaterial::Using_Tool()
{
	if (m_eType == CMaterial::EType::Instance_Static || m_eType == CMaterial::EType::Instance_Anim)
	{
		_bool bUsingTool = true;
		SetUp_ValueOnShader("g_bUsingTool", &bUsingTool, sizeof(_bool));
	}
}

HRESULT CMaterial::Change_Material()
{
	for (auto& pEffectDesc : m_vecEffectDescs)
	{
		Safe_Release(pEffectDesc->pInputLayout);
		Safe_Delete(pEffectDesc);
	}
	m_vecEffectDescs.clear();

	Safe_Release(m_pEffect);

	vector<CTexture*> vecTexturs = m_vecTextures;

	if (FAILED(Native_Construct(m_wstrName,m_wstrShaderPath, m_eType)))
		return E_FAIL;
	
	m_vecTextures = vecTexturs;

	return S_OK;
}

CSaveManager::MTRLDATA CMaterial::SetMaterialSaveData()
{
	CSaveManager::MTRLDATA tMtrlData;
	ZeroMemory(&tMtrlData, sizeof(tMtrlData));

	tMtrlData.iMtrlType = (_uint)(m_eType);
	lstrcpy(tMtrlData.pMtrlName, m_wstrName.c_str());
	lstrcpy(tMtrlData.pShader_Path, m_wstrShaderPath.c_str());

	_uint iTextureCnt = 0;
	for (_uint i=0; i< (_uint)TEXTURETYPE::TEX_END; i++)
	{
		if (m_vecTextures[i])
		{
			CSaveManager::TEXTUREDATA tTextureData;
			ZeroMemory(&tTextureData, sizeof(tTextureData));

			tTextureData.iType = i;
			lstrcpy(tTextureData.pTextureName, m_vecTextures[i]->getTextureTag().c_str());
			tMtrlData.vecTextureData.emplace_back(tTextureData);
			iTextureCnt++;
		}
	}
	tMtrlData.iTextureCnt = iTextureCnt;

	return tMtrlData;
}

list<wstring> CMaterial::Get_TextureName()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	list<wstring> pNameList;

	for (auto& pTexture : m_vecTextures)
	{
		if(!pTexture)
			pNameList.emplace_back(L"");
		else
			pNameList.emplace_back(pTexture->getTextureTag());
	}

	return pNameList;
}

HRESULT CMaterial::SetUp_TextureOnShader()
{
	for (_uint i = 0; i < (_uint)TEXTURETYPE::TEX_END; i++)
	{
		string str = "";
		if (!m_vecTextures[i])
			continue;
		switch ((TEXTURETYPE)i)
		{
		case TEXTURETYPE::TEX_DIFFUSE:
			str = "g_DiffuseTexture";
			break;
		case TEXTURETYPE::TEX_NORMAL:
			str = "g_BiNormalTexture";
			break;
		case TEXTURETYPE::TEX_METALIC:
			/*str = "g_Metallic;";*/
			break;
		case TEXTURETYPE::TEX_OCCLUSION:
			/*str = "g_AO";*/
			break;
		case TEXTURETYPE::TEX_ROUGHNESS:
			/*str = "g_Roughness";*/
			break;
		case TEXTURETYPE::TEX_ORM:
			str = "g_ORMTexture";
			break;
		case TEXTURETYPE::TEX_OMER:
			str = "g_OMERTexture";
			break;
		case TEXTURETYPE::TEX_CEO:
			str = "g_CEOTexture";
			break;
		case TEXTURETYPE::TEX_MRA:
			str = "g_MRATexture";
			break;
		case TEXTURETYPE::TEX_HEIGHT:
			str = "g_HeightTexture";
			break;
		case TEXTURETYPE::TEX_TINT:
			break;
		case TEXTURETYPE::TEX_MT:
			break;
		case TEXTURETYPE::TEX_ORS:
			str = "g_ORSTexture";
			break;
		case TEXTURETYPE::TEX_NM:
			break;
		case TEXTURETYPE::TEX_RM:
			break;
		case TEXTURETYPE::TEX_NRM:
			break;
		case TEXTURETYPE::TEX_MASK:
			str = "g_MaskTexture";
			break;
		case TEXTURETYPE::TEX_DR:
			break;
		case TEXTURETYPE::TEX_NOH:
			break;
		case TEXTURETYPE::TEX_ORH:
			break;
		case TEXTURETYPE::TEX_COEFF: // 20
			break;
		case TEXTURETYPE::TEX_ND: // 21
			break;
		case TEXTURETYPE::TEX_OMR: // 22
			str = "g_OMRTexture";
			break;
		case TEXTURETYPE::TEX_NORMAL2: // 23
			break;
		case TEXTURETYPE::TEX_ORME: // 24
			str = "g_ORMETexture";
			break;
		case TEXTURETYPE::TEX_END:
			break;
		default:
			break;
		}
		SetUp_TextureOnShader(str, (TEXTURETYPE)i);
	}
	return S_OK;
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
