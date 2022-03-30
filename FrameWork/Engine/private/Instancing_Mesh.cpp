#include "Instancing_Mesh.h"
#include "MeshContainer.h"
#include "Material.h"
#include "TextureManager.h"
#include "Component_Manager.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CullingBox.h"

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
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_VBInstSubResourceData(rhs.m_VBInstSubResourceData)
	, m_iInstStride(rhs.m_iInstStride)
	, m_iInstNumVertices(rhs.m_iInstNumVertices)
	, m_eType(rhs.m_eType)
	, m_matPivot(rhs.m_matPivot)
	, m_vecMaterials(rhs.m_vecMaterials)
	, m_vecCullingBox(rhs.m_vecCullingBox)
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

	for (auto& pMaterial : m_vecMaterials)
		Safe_AddRef(pMaterial);

	for (auto& pCullingBox : m_vecCullingBox)
		Safe_AddRef(pCullingBox);
}

HRESULT CInstancing_Mesh::NativeConstruct_Prototype(const wstring& pMeshFilePath, INSTANCE_TYPE eType)
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


	return S_OK;
}

HRESULT CInstancing_Mesh::NativeConstruct(void* pArg)
{
	if (FAILED(Create_InstancingBuffer(pArg)))
		return E_FAIL;



	return S_OK;
}

void CInstancing_Mesh::Update_InstanceBuffer(const vector<_float4x4>& pMatrix)
{
	vector<_float4x4> matMatrix;
	for (auto& pData : pMatrix)
	{
		_matrix matData = XMLoadFloat4x4(&pData);
		if (!XMVector4Equal(matData.r[0], XMVectorZero())
			|| !XMVector4Equal(matData.r[1], XMVectorZero())
			|| !XMVector4Equal(matData.r[2], XMVectorZero())
			|| !XMVector4Equal(matData.r[3], XMVectorZero()))
			matMatrix.emplace_back(pData);
	}

	m_iInstNumVertices = (_uint)matMatrix.size();

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iInstNumVertices; ++i)
	{
		_float4 vRight;
		memcpy(&vRight, &matMatrix[i].m[0], sizeof(_float4));
		_float4 vUp;
		memcpy(&vUp, &matMatrix[i].m[1], sizeof(_float4));
		_float4 vLook;
		memcpy(&vLook, &matMatrix[i].m[2], sizeof(_float4));
		_float4 vPos;
		memcpy(&vPos, &matMatrix[i].m[3], sizeof(_float4));

		((VTXMATRIX*)SubResource.pData)[i].vRight = vRight;
		((VTXMATRIX*)SubResource.pData)[i].vUp = vUp;
		((VTXMATRIX*)SubResource.pData)[i].vLook = vLook;
		((VTXMATRIX*)SubResource.pData)[i].vPosition = vPos;
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

HRESULT CInstancing_Mesh::Render(const wstring& pCameraTag)
{
	if (m_vecCullingBox.empty())
		return E_FAIL;

	for (auto& pBox : m_vecCullingBox)
		pBox->Render(pCameraTag);

	return S_OK;
}

HRESULT CInstancing_Mesh::SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iSize)
{
	for (auto& pMtrl : m_vecMaterials)
	{
		if (pMtrl)
			pMtrl->SetUp_ValueOnShader(pConstantName, pData, iSize);
	}
	return S_OK;
}

HRESULT CInstancing_Mesh::SetUp_TextureOnShader(const char* pConstantName, ID3D11ShaderResourceView* pSRV, _ulong Numindex)
{
	if (FAILED(m_vecMaterials[Numindex]->SetUp_TextureOnShader(pConstantName, pSRV)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstancing_Mesh::Render(_uint iMeshContainerIndex, _int iPassindex)
{
	for (auto& pMeshContainer : m_vecMeshContainers[iMeshContainerIndex])
	{
		_uint iMtrlIndex = pMeshContainer->getMaterialIndex();
		if (m_vecMaterials[iMtrlIndex])
		{
			m_vecMaterials[iMtrlIndex]->Set_InputLayout(iPassindex);

			/*	if (m_eType == CInstancing_Mesh::INSTANCE_TYPE::ANIM)
				{
					_matrix		BoneMatrices[256];
					ZeroMemory(BoneMatrices, sizeof(_matrix) * 256);

					pMeshContainer->SetUp_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

					if (FAILED(m_vecMaterials[iMtrlIndex]->SetUp_ValueOnShader("g_BoneMatrices", BoneMatrices, sizeof(_matrix) * 256)))
						return E_FAIL;
				}*/
			m_vecMaterials[iMtrlIndex]->Render(iPassindex);
			pMeshContainer->Render(m_pVBInstance,m_iInstStride,m_iInstNumVertices);
		}
	}
	return S_OK;
}


HRESULT CInstancing_Mesh::Init_StaticMesh(const wstring& pMeshFilePath)
{
	CSaveManager* pInstance = GET_INSTANCE(CSaveManager);

	CSaveManager::STATICDATA pData;

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
	if (FAILED(Create_CullingBox()))
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

HRESULT CInstancing_Mesh::Create_CullingBox()
{
	vector<_vector> vMax;
	vector<_vector> vMin;

	for (auto& pMtrlMesh : m_vecMeshContainers)
	{
		for (auto& pMesh : pMtrlMesh)
		{
			vMax.emplace_back(pMesh->Get_MaxPoint());
			vMin.emplace_back(pMesh->Get_MinPoint());
		}
	}
	
	_vector vBoxMax, vBoxMin;

	_float fMaxX, fMaxY, fMaxZ;
	fMaxX = 0.f;
	fMaxY = 0.f;
	fMaxZ = 0.f;

	_float fMinX, fMinY, fMinZ;
	fMinX = 0.f;
	fMinY = 0.f;
	fMinZ = 0.f;

	_uint iSize = (_uint)vMax.size();
	for (_uint i = 0; i < iSize; i++)
	{
		if (fMaxX < XMVectorGetX(vMax[i]))
			fMaxX = XMVectorGetX(vMax[i]);
		if (fMaxY < XMVectorGetY(vMax[i]))
			fMaxY = XMVectorGetY(vMax[i]);
		if (fMaxZ < XMVectorGetZ(vMax[i]))
			fMaxZ = XMVectorGetZ(vMax[i]);

		if (fMinX > XMVectorGetX(vMin[i]))
			fMinX = XMVectorGetX(vMin[i]);
		if (fMinY > XMVectorGetY(vMin[i]))
			fMinY = XMVectorGetY(vMin[i]);
		if (fMinZ > XMVectorGetZ(vMin[i]))
			fMinZ = XMVectorGetZ(vMin[i]);
	}

	vBoxMax = XMVectorSet(fMaxX, fMaxY, fMaxZ, 1.f);
	vBoxMin = XMVectorSet(fMinX, fMinY, fMinZ, 1.f);

	CCullingBox* pBox = g_pGameInstance->Clone_Component<CCullingBox>(0, L"Proto_Component_CullingBox");
	if (!pBox)
		return E_FAIL;

	pBox->CreateWithPoints(vBoxMin, vBoxMax);
	m_vecCullingBox.emplace_back(pBox);

	return S_OK;
}

HRESULT CInstancing_Mesh::Create_Material(const CSaveManager::MTRLDATA& pData)
{
	CMaterial* pMaterial = g_pGameInstance->Get_Material(pData.pMtrlName);
	if (!pMaterial)
	{
		pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, pData.pMtrlName, pData.pShader_Path, (CMaterial::EType)pData.iMtrlType);
		g_pGameInstance->Add_Material(pData.pMtrlName, pMaterial);
		if (!pMaterial)
			return E_FAIL;

		for (auto& pTextureData : pData.vecTextureData)
		{
			wstring wstrMaterialPath = L"../bin/FBX/Texture/";
			wstrMaterialPath += pTextureData.pTextureName;
			pMaterial->Set_Texture((TEXTURETYPE)pTextureData.iType, pTextureData.pTextureName, wstrMaterialPath);
		}
	}
	pMaterial->SetUp_TextureOnShader();
	m_vecMaterials.emplace_back(pMaterial);
	Safe_AddRef(pMaterial);

	return S_OK;
}

HRESULT CInstancing_Mesh::Create_InstancingBuffer(void* pArg)
{
	INSTANCINGMESHDESC pDesc = (*(INSTANCINGMESHDESC*)pArg);

	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	m_iInstStride = sizeof(VTXMATRIX);
	m_iInstNumVertices = pDesc.iNumInstance;

	CCullingBox* pBox = m_vecCullingBox[0];
	for (_uint i = 0; i < m_iInstNumVertices - 1; i++)
		m_vecCullingBox.emplace_back(static_cast<CCullingBox*>(pBox->Clone(nullptr)));

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

		_matrix matTransform;
		matTransform.r[0] = XMLoadFloat4(&pVertices[i].vRight);
		matTransform.r[1] = XMLoadFloat4(&pVertices[i].vUp);
		matTransform.r[2] = XMLoadFloat4(&pVertices[i].vLook);
		matTransform.r[3] = XMLoadFloat4(&pVertices[i].vPosition);

		m_vecCullingBox[i]->Update_Matrix(matTransform);
	}

	m_VBInstSubResourceData.pSysMem = pVertices;

	if(FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CInstancing_Mesh* CInstancing_Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const wstring& pMeshFilePath, INSTANCE_TYPE eType)
{
	CInstancing_Mesh* pInstance = new CInstancing_Mesh(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(pMeshFilePath,eType)))
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

	for (auto& pMtrlMeshContainer : m_vecMeshContainers)
	{
		for (auto& pMeshContainer : pMtrlMeshContainer)
			Safe_Release(pMeshContainer);
		pMtrlMeshContainer.clear();
	}
	m_vecMeshContainers.clear();

	for (auto& pMaterial : m_vecMaterials)
		Safe_Release(pMaterial);

	m_vecMaterials.clear();

	for (auto& pBox : m_vecCullingBox)
		Safe_Release(pBox);

	m_vecCullingBox.clear();
}
