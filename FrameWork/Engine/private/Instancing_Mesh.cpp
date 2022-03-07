#include "Instancing_Mesh.h"
#include "MeshContainer.h"
#include "Material.h"
#include "TextureManager.h"
#include "Component_Manager.h"
#include "Texture.h"

CInstancing_Mesh::CInstancing_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pVBInstance(nullptr)
	, m_iInstNumVertices(0)
	, m_iInstStride(0)
{
	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&m_matPivot, sizeof(_float4x4));
}

CInstancing_Mesh::CInstancing_Mesh(const CInstancing_Mesh& rhs)
	: CComponent(rhs)
	, m_vecMaterialDesc(rhs.m_vecMaterialDesc)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_VBInstSubResourceData(rhs.m_VBInstSubResourceData)
	, m_iInstStride(rhs.m_iInstStride)
	, m_iInstNumVertices(rhs.m_iInstNumVertices)
	, m_eType(rhs.m_eType)
	, m_pEffect(rhs.m_pEffect)
	, m_PassDesc(rhs.m_PassDesc)
	, m_matPivot(rhs.m_matPivot)
{
	Safe_AddRef(m_pVBInstance);

	m_vecMeshContainers.resize((_uint)rhs.m_vecMeshContainers.size());
	for (auto& MtrlMeshContainer : rhs.m_vecMeshContainers)
	{
		for (auto& pPrototypeMeshContainer : MtrlMeshContainer)
		{
			CMeshContainer* pMeshContainer = (CMeshContainer*)pPrototypeMeshContainer->Clone(nullptr);
			if (nullptr == pMeshContainer)
				return;

			m_vecMeshContainers[pMeshContainer->getMaterialIndex()].push_back(pMeshContainer);
		}
	}
	for (auto& pMaterial : m_vecMaterialDesc)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(pMaterial->pMeshTexture[i]);
	}
	for (auto& pPassDesc : m_PassDesc)
	{
		Safe_AddRef(pPassDesc->pInputLayout);
		Safe_AddRef(pPassDesc->pPass);
	}
	Safe_AddRef(m_pEffect);
	//for (auto& pMaterial : m_vecMaterials)
	//{
	//	for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	//		Safe_AddRef(pMaterial->pMeshTexture[i]);
	//}
}

HRESULT CInstancing_Mesh::NativeConstruct_Prototype(const wstring& pMeshFilePath, const wstring& pShaderFile, INSTANCE_TYPE eType)
{
	m_eType = eType;

	if (m_eType == INSTANCE_TYPE::STATIC)
	{
		if (FAILED(Init_StaticMesh(pMeshFilePath)))
			return E_FAIL;
	}
	else
	{

	}
	if (FAILED(Compile_Shader(pShaderFile)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstancing_Mesh::NativeConstruct(void* pArg)
{
	if (FAILED(Create_InstancingBuffer(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstancing_Mesh::SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iSize)
{
	if (!m_pEffect)
		return E_FAIL;
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (!pVariable)
		return E_FAIL;
	return pVariable->SetRawValue(pData,0,iSize);
}

HRESULT CInstancing_Mesh::SetUp_TextureOnShader(const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (!m_vecMaterialDesc[iMeshContainerIndex]->pMeshTexture[eType])
		return S_OK;

	ID3DX11EffectShaderResourceVariable* pVariabe = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (!pVariabe)
		return E_FAIL;

	return pVariabe->SetResource(m_vecMaterialDesc[iMeshContainerIndex]->pMeshTexture[eType]->Get_ShaderResourceView());
}

HRESULT CInstancing_Mesh::Render(_uint iMeshContainerIndex, _int iPassindex)
{
	if (iPassindex >= (_uint)m_PassDesc.size())
		return E_FAIL;

	for (auto& pMeshContainer : m_vecMeshContainers[iMeshContainerIndex])
	{
 		_uint iMtrlIndex = pMeshContainer->getMaterialIndex();
		pMeshContainer->Render(m_pVBInstance, m_iInstStride);
	}
	m_pDeviceContext->IASetInputLayout(m_PassDesc[iPassindex]->pInputLayout);
	if (FAILED(m_PassDesc[iPassindex]->pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}


HRESULT CInstancing_Mesh::Init_StaticMesh(const wstring& pMeshFilePath)
{
	CSaveManager* pInstance = GET_INSTANCE(CSaveManager);

	CSaveManager::STATICDATA pData;
	ZeroMemory(&pData, sizeof(pData));

	if (FAILED(pInstance->Load_StaticModel(pData,pMeshFilePath)))
		return E_FAIL;

	RELEASE_INSTANCE(CSaveManager);

	m_iNumMeshes = pData.iMeshCount;
	m_vecMeshContainers.resize(pData.iMtrlCount);
	for (auto& pMesh : pData.pMeshData)
	{
		if (FAILED(Create_MeshContainer(pMesh.iMeshMtrlNum, pMesh.iVtxCount, pMesh.iIdxCount,pMesh.pVtxPoint, pMesh.pIndex)))
			return E_FAIL;
	}
	if (FAILED(Create_VertextIndexBuffer()))
		return E_FAIL;
	
	for (auto& pMtrl : pData.pMtrlData)
	{
		if (FAILED(Create_Material(pMtrl)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CInstancing_Mesh::Create_MeshContainer(_uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, void* pVtx, void* pIdx)
{
	CMeshContainer* pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, iMaterialIndex, iNumVtxCnt, iNumIdxCnt, (_uint)m_eType, pVtx, pIdx);
	if (!pMeshContainer)
		return E_FAIL;

	m_vecMeshContainers[iMaterialIndex].emplace_back(pMeshContainer);
	return S_OK;
}

HRESULT CInstancing_Mesh::Create_VertextIndexBuffer()
{
	for (auto& pMtrlMeshContaier : m_vecMeshContainers)
	{
		for (auto& pMtrlMeshContaier : pMtrlMeshContaier)
			pMtrlMeshContaier->Create_VertexIndexBuffer();
	}
	return S_OK;
}

HRESULT CInstancing_Mesh::Create_Material(const CSaveManager::MTRLDATA& pData)
{
	_uint iIndex = 0;
	_uint iTextureCnt = pData.iTextureCnt;

	MESHMATERIAL* pDesc = new MESHMATERIAL;
	ZeroMemory(pDesc, sizeof(MESHMATERIAL));
	for(_uint i=0; i< iTextureCnt; i++)
	{
		char szFilePath[MAX_PATH] = "";
		WideCharToMultiByte(CP_ACP, 0, pData.pTaxtureData[i].pTextureName, MAX_PATH, szFilePath, MAX_PATH, NULL, NULL);

		char	szFileName[MAX_PATH] = "";
		char	szExt[MAX_PATH] = "";

		_splitpath_s(szFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

		strcat_s(szFileName, ".tga");
		wstring     strTexture;
		_tchar		szFullName[MAX_PATH] = L"../bin/FBX/Texture/";
		_tchar		szTextureTag[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szFileName, (_int)strlen(szFileName), szTextureTag, MAX_PATH);

		lstrcat(szFullName, szTextureTag);
		CTextureManager* pTextureMgr = GET_INSTANCE(CTextureManager);

		if (FAILED(pTextureMgr->Add_Texture(m_pDevice, szTextureTag, szFullName)))
			return E_FAIL;

		RELEASE_INSTANCE(CTextureManager);

		CComponent_Manager* pInstance = GET_INSTANCE(CComponent_Manager);

		strTexture = szTextureTag;

		pDesc->pMeshTexture[pData.pTaxtureData[i].iType] = static_cast<CTexture*>(pInstance->Clone_Component(0, L"Texture", &strTexture));
		lstrcpy(pDesc->pMeshTextureName[pData.pTaxtureData[i].iType], szFullName);
		RELEASE_INSTANCE(CComponent_Manager);

		if (nullptr == pDesc->pMeshTexture[pData.pTaxtureData[i].iType])
			return E_FAIL;

	}
	m_vecMaterialDesc.emplace_back(pDesc);
	return S_OK;
}

HRESULT CInstancing_Mesh::Create_InstancingBuffer(void* pArg)
{
	INSTANCINGMESHDESC pDesc = (*(INSTANCINGMESHDESC*)pArg);

	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	m_iInstStride = sizeof(VTXMATRIX);
	m_iInstNumVertices = pDesc.iNumInstance;

	m_VBInstDesc.ByteWidth = m_iInstStride * m_iInstNumVertices;
	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstDesc.MiscFlags = 0;
	m_VBInstDesc.StructureByteStride = m_iInstStride;

	ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	VTXMATRIX* pVertices = new VTXMATRIX[m_iInstNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMATRIX) * m_iInstNumVertices);

	for (_uint i  = 0; i< m_iInstNumVertices; i++)
	{
		pVertices[i].vRight = (_float4)(pDesc.vecMatrix)[i].m[0];
		pVertices[i].vUp = (_float4)(pDesc.vecMatrix)[i].m[1];
		pVertices[i].vLook = (_float4)(pDesc.vecMatrix)[i].m[2];
		pVertices[i].vPosition = (_float4)(pDesc.vecMatrix)[i].m[3];
	}

	m_VBInstSubResourceData.pSysMem = pVertices;

	if(FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CInstancing_Mesh::Compile_Shader(const wstring& pShaderFilePath)
{
	_uint iNumElements = 0;
	D3D11_INPUT_ELEMENT_DESC Elements[D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT];
	ZeroMemory(Elements, sizeof(D3D11_INPUT_ELEMENT_DESC) * D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT);

	if (m_eType == INSTANCE_TYPE::STATIC)
	{
		iNumElements = 8;
		Elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		//Elements[3] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		Elements[4] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		Elements[5] = { "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		Elements[6] = { "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		Elements[7] = { "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
	}
	else
	{
		iNumElements = 10;
		Elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[4] = { "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Elements[5] = { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		//Elements[3] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		Elements[6] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		Elements[7] = { "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		Elements[8] = { "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		Elements[9] = { "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 };

	}
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif // _DEBUG	
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);

	D3DX11_TECHNIQUE_DESC			TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	pTechnique->GetDesc(&TechniqueDesc);

	m_PassDesc.reserve(TechniqueDesc.Passes);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		EFFECTDESC* pPassDesc = new EFFECTDESC;

		pPassDesc->pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC		PassDesc;
		ZeroMemory(&PassDesc, sizeof(D3DX11_PASS_DESC));

		pPassDesc->pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(Elements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pPassDesc->pInputLayout)))
			return E_FAIL;

		m_PassDesc.push_back(pPassDesc);
	}

	return S_OK;
}

CInstancing_Mesh* CInstancing_Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pMeshFilePath, const wstring& pShaderFile, INSTANCE_TYPE eType)
{
	CInstancing_Mesh* pInstance = new CInstancing_Mesh(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(pMeshFilePath,pShaderFile,eType)))
	{
		MSGBOX("CInstancing_Mesh Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CInstancing_Mesh::Clone(void* pArg)
{
	CInstancing_Mesh* pInstance = new CInstancing_Mesh(*this);
	if(FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CInstancing_Mesh Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInstancing_Mesh::Free()
{
	CComponent::Free();

	Safe_Release(m_pVBInstance);
	if (false == m_isCloned)
	{
		for (auto& pPassDesc : m_PassDesc)
			Safe_Delete(pPassDesc);
	}
	m_PassDesc.clear();
	Safe_Release(m_pEffect);

	for (auto& pMaterial : m_vecMaterialDesc)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(pMaterial->pMeshTexture[i]);
		if (false == m_isCloned)
			Safe_Delete(pMaterial);
	}
	m_vecMaterialDesc.clear();

	for (auto& pMtrlMeshContainer : m_vecMeshContainers)
	{
		for (auto& pMeshContainer : pMtrlMeshContainer)
			Safe_Release(pMeshContainer);
		pMtrlMeshContainer.clear();
	}
	m_vecMeshContainers.clear();

}
