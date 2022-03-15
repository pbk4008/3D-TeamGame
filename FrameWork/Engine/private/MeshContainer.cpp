#include "..\public\MeshContainer.h"
#include "HierarchyNode.h"
#include "Model.h"

CMeshContainer::CMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
	, m_iNumMesh(0)
{
}

CMeshContainer::CMeshContainer(const CMeshContainer& rhs)
	: CVIBuffer(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_pAIMesh(rhs.m_pAIMesh)
	, m_iNumMesh(rhs.m_iNumMesh)
{
}

HRESULT CMeshContainer::NativeConstruct_Prototype(CModel* pModel, aiMesh* pMesh, _fmatrix PivotMatirx)
{
	m_pAIMesh = pMesh;

	if(FAILED(Set_UpVerticesDesc(pModel, pMesh, PivotMatirx)))
		return E_FAIL;
	if (FAILED(Set_IndicesDesc(pMesh)))
		return E_FAIL;

	if (!pModel->getUsingMaterial())
		m_iMaterialIndex = pMesh->mMaterialIndex;

	return S_OK;
}

HRESULT CMeshContainer::NativeConstruct_Prototype(_uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, class CModel* pModel, void* pVtx, void* pIdx)
{
	m_iMaterialIndex = iMaterialIndex;

	if (FAILED(Set_UpVerticesDesc(iNumVtxCnt, pModel, pVtx)))
		return E_FAIL;
	if (FAILED(Set_IndicesDesc(iNumIdxCnt,pIdx)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshContainer::NativeConstruct_Prototype(_uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, _uint iType, void* pVtx, void* pIdx)
{
	if (FAILED(Set_UpVerticesDesc(iNumVtxCnt, iType, pVtx)))
		return E_FAIL;
	if (FAILED(Set_IndicesDesc(iNumIdxCnt, pIdx)))
		return E_FAIL;

	m_iMaterialIndex = iMaterialIndex;

	return S_OK;
}

HRESULT CMeshContainer::NativeConstruct(void* pArg)
{
	return S_OK;
}

HRESULT CMeshContainer::Render()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	_uint iOffset = 0;

	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVB, &m_iStride, &iOffset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	m_pDeviceContext->DrawIndexed(m_iNumPrimitive * m_iNumIndicesFigure, 0, 0);

	return S_OK;
}

HRESULT CMeshContainer::Render(ID3D11Buffer* pInstVetex, _uint iInstStride)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer * pVertexBuffer[] =
	{
		m_pVB,
		pInstVetex
	};
	_uint iStrides[] = {
		m_iStride,
		iInstStride
	};
	_uint iOffset[] = {
		0,
		0
	};

	m_pDeviceContext->IASetVertexBuffers(0, 2, pVertexBuffer, iStrides, iOffset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);
	m_pDeviceContext->DrawIndexedInstanced(m_iNumPrimitive *3, iInstStride, 0, 0,0);
	return S_OK;
}


HRESULT CMeshContainer::Create_VertexIndexBuffer()
{
	if (FAILED(CVIBuffer::Create_VertexBuffer()))
		return E_FAIL;
	if (FAILED(CVIBuffer::Create_IndexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshContainer::Add_Bone(CModel* pModel)
{
	if (m_iNumMesh == 0)
	{
		CHierarchyNode* pNode = pModel->Find_HierarchyNode(m_pAIMesh->mName.data);
		if (!pNode)
				return E_FAIL;
		pNode->Set_OffsetMatrix(XMMatrixIdentity());
		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}
	else
	{
		for (_uint j = 0; j < m_iNumMesh; ++j)
		{
			aiBone* pBone = m_pAIMesh->mBones[j];

			CHierarchyNode* pNode = pModel->Find_HierarchyNode(pBone->mName.data);
			if (!pNode)
				return E_FAIL;

			_matrix		OffSetMatrix;
			OffSetMatrix = XMMatrixTranspose(_matrix(pBone->mOffsetMatrix[0]));

			pNode->Set_OffsetMatrix(OffSetMatrix);

			m_Bones.emplace_back(pNode);
			Safe_AddRef(pNode);
		}
	}
	return S_OK;
}

HRESULT CMeshContainer::Add_Bone(vector<string>& vecBoneName, CModel* pModel)
{
	for (auto& pName : vecBoneName)
	{
		CHierarchyNode* pNode = pModel->Find_HierarchyNode(pName.c_str());
		if (!pNode)
			return E_FAIL;
		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}
	return S_OK;
}

void CMeshContainer::SetUp_BoneMatrices(_matrix * pBoneMatrices, _fmatrix PivotMatrix)
{
	_uint		iIndex = 0;

	for (auto& pBone : m_Bones)
	{
		_matrix OffsetMatrix = pBone->Get_OffsetMatrix();
		_matrix CombinedTransformationMatrix = pBone->Get_CombinedMatrix();

		pBoneMatrices[iIndex++] = XMMatrixTranspose(OffsetMatrix * CombinedTransformationMatrix* PivotMatrix);

		//pBoneMatrices[iIndex++] = PivotMatrix * XMMatrixTranspose(XMLoadFloat4x4(&pBone->OffsetMatrix) * pBone->pNode->Get_CombinedMatrix());
	}
}

const CSaveManager::STATICMESHDATA CMeshContainer::SetStaticSaveData()
{
	CSaveManager::STATICMESHDATA pData;

	pData.iIdxCount = m_iNumPrimitive;
	pData.iMeshMtrlNum = m_iMaterialIndex;
	pData.iVtxCount = m_iNumVertices;
	pData.pVtxPoint = (VTXMESH*)m_pVertices;
	pData.pIndex = (FACEINDICES32*)m_pIndices;

	return pData;
}

const CSaveManager::ANIMMESHDATA CMeshContainer::SetAnimSaveData()
{
	CSaveManager::ANIMMESHDATA pData;

	pData.iIdxCount = m_iNumPrimitive;
	pData.iMeshMtrlNum = m_iMaterialIndex;
	pData.iVtxCount = m_iNumVertices;
	pData.pVtxPoint = (VTXMESH_ANIM*)m_pVertices;
	pData.pIndex = (FACEINDICES32*)m_pIndices;

	pData.iBoneCnt = (_uint)m_Bones.size();
	for (auto& pBone : m_Bones)
	{
		string strName = pBone->Get_Name();
		pData.vecBoneName.emplace_back(strName);
	}
	return pData;
}

HRESULT CMeshContainer::Set_UpVerticesDesc(CModel* pModel, aiMesh* pMesh, _fmatrix PivotMatrix)
{
	m_iNumVertices = pMesh->mNumVertices;

	CModel::TYPE eType = pModel->getType();

	if (eType == CModel::TYPE_STATIC)
	{
		m_pVertices = new VTXMESH[m_iNumVertices];
		ZeroMemory(m_pVertices, sizeof(VTXMESH) * m_iNumVertices);
		m_iStride = sizeof(VTXMESH);
	}
	else
	{
		m_pVertices = new VTXMESH_ANIM[m_iNumVertices];
		ZeroMemory(m_pVertices, sizeof(VTXMESH_ANIM) * m_iNumVertices);
		m_iStride = sizeof(VTXMESH_ANIM);

		if (FAILED(SetUp_SkinnedDesc(pModel,pMesh)))
			return E_FAIL;
	}

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		VTXMESH* pVertices = (VTXMESH*)((_byte*)m_pVertices + (i * m_iStride));

		memcpy(&pVertices->vPosition, &pMesh->mVertices[i], sizeof(_float3));
 		_fvector Temp = XMVector3TransformCoord(XMLoadFloat3(&pVertices->vPosition), PivotMatrix);
		XMStoreFloat3(&pVertices->vPosition, Temp);
		
		_vector			vNormal;
		memcpy(&vNormal, &pMesh->mNormals[i], sizeof(_float3));
		vNormal = XMVectorSetW(vNormal, 0.f);
		vNormal = XMVector3Transform(vNormal, PivotMatrix);
		XMStoreFloat3(&pVertices->vNormal, vNormal);

		//memcpy(&pVertices->vNormal, &pMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices->vTexUV, &pMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices->vTangent, &pMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices->vBiNormal, &pMesh->mBitangents[i], sizeof(_float3));
	}
	m_VBSubresourceData.pSysMem = m_pVertices;

	return S_OK;
}

HRESULT CMeshContainer::Set_UpVerticesDesc(_uint iNumVtxCnt, class CModel* pModel, void* pVtx)
{
	m_iNumVertices = iNumVtxCnt;

	CModel::TYPE eType = pModel->getType();

	m_pVertices = pVtx;
	if (eType == CModel::TYPE_STATIC)
		m_iStride = sizeof(VTXMESH);
	else
		m_iStride = sizeof(VTXMESH_ANIM);

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_VBSubresourceData.pSysMem = m_pVertices;

	return S_OK;
}

HRESULT CMeshContainer::Set_UpVerticesDesc(_uint iNumVtxCnt, _uint pModelType, void* pVtx)
{
	m_iNumVertices = iNumVtxCnt;

	m_pVertices = pVtx;
	if (pModelType == 0)
		m_iStride = sizeof(VTXMESH);
	else
		m_iStride = sizeof(VTXMESH_ANIM);

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_VBSubresourceData.pSysMem = m_pVertices;

	return S_OK;
}

HRESULT CMeshContainer::Set_IndicesDesc(aiMesh* pMesh)
{
	m_iNumPrimitive = pMesh->mNumFaces;
	m_IndicesByteLength = sizeof(FACEINDICES32);
	m_iNumIndicesFigure = 3;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));

	m_IBDesc.ByteWidth = m_IndicesByteLength * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		((FACEINDICES32*)m_pIndices)[i]._0 = pMesh->mFaces[i].mIndices[0];
		((FACEINDICES32*)m_pIndices)[i]._1 = pMesh->mFaces[i].mIndices[1];
		((FACEINDICES32*)m_pIndices)[i]._2 = pMesh->mFaces[i].mIndices[2];
	}

	m_IBSubresourceData.pSysMem = m_pIndices;

	return S_OK;
}

HRESULT CMeshContainer::Set_IndicesDesc(_uint iNumIdxCnt, void* pIdx)
{
	m_iNumPrimitive = iNumIdxCnt;
	m_IndicesByteLength = sizeof(FACEINDICES32);
	m_iNumIndicesFigure = 3;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));

	m_IBDesc.ByteWidth = m_IndicesByteLength * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	m_pIndices = pIdx;

	m_IBSubresourceData.pSysMem = m_pIndices;

	return S_OK;
}

HRESULT CMeshContainer::SetUp_SkinnedDesc(CModel* pModel, aiMesh* pMesh)
{
	m_iNumMesh = pMesh->mNumBones;
	for (_uint i = 0; i < m_iNumMesh; i++)
	{
		aiBone* pBone = pMesh->mBones[i];

		for (_uint j = 0; j < pBone->mNumWeights; j++)
		{
			VTXMESH_ANIM* pVertices = (VTXMESH_ANIM*)m_pVertices;

			if (pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.x == 0.0f)
			{
				pVertices[pBone->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.x = pBone->mWeights[j].mWeight;
			}
			else if (pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.y == 0.0f)
			{
				pVertices[pBone->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.y = pBone->mWeights[j].mWeight;
			}
			else if (pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.z == 0.0f)
			{
				pVertices[pBone->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.z = pBone->mWeights[j].mWeight;
			}
			else
			{
				pVertices[pBone->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pBone->mWeights[j].mVertexId].vBlendWeight.w = pBone->mWeights[j].mWeight;
			}
		}
	}
	return S_OK;
}

CMeshContainer * CMeshContainer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, class CModel* pModel, aiMesh* pMesh, _fmatrix PivotMatrix)
{
	CMeshContainer*		pInstance = new CMeshContainer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pModel, pMesh, PivotMatrix)))
	{
		MSGBOX("Failed to Creating CMeshContainer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMeshContainer* CMeshContainer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, class CModel* pModel, void* pVtx, void* pIdx)
{
	CMeshContainer* pInstance = new CMeshContainer(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(iMaterialIndex, iNumVtxCnt, iNumIdxCnt,pModel,pVtx,pIdx)))
	{
		MSGBOX("CMeshContainer Load Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMeshContainer* CMeshContainer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iMaterialIndex, _uint iNumVtxCnt, _uint iNumIdxCnt, _uint iType, void* pVtx, void* pIdx)
{
	CMeshContainer* pInstance = new CMeshContainer(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(iMaterialIndex, iNumVtxCnt, iNumIdxCnt, iType, pVtx, pIdx)))
	{
		MSGBOX("CMeshContainer Load Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMeshContainer::Clone(void* pArg)
{
	CMeshContainer* pInstance = new CMeshContainer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CMeshContainer");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMeshContainer::Free()
{
	CVIBuffer::Free();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();
}
