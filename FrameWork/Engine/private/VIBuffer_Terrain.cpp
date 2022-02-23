#include "..\public\VIBuffer_Terrain.h"
#include "Frustum.h"

#include "QuadTree.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_pQuadTree(rhs.m_pQuadTree)
{
	Safe_AddRef(m_pQuadTree);
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar* pShaderFilePath, const _tchar* pHeightMapPath)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	HANDLE		hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;
	_ulong				dwByte = 0;
	
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	

	_ulong*				pPixel = new _ulong[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);


	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	CloseHandle(hFile);

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	
	/* D3D11_SUBRESOURCE_DATA */
	ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	
	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			((VTXNORTEX*)m_pVertices)[iIndex].vPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / 20.0f, i);
			((VTXNORTEX*)m_pVertices)[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			((VTXNORTEX*)m_pVertices)[iIndex].vTexUV = _float2(j / _float((m_iNumVerticesX - 1)), i / _float((m_iNumVerticesZ - 1)));
		}
	}
	
	m_VBSubresourceData.pSysMem = m_pVertices;



	/* D3D11_BUFFER_DESC */
	m_IndicesByteLength = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesFigure = 3;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eFormat = DXGI_FORMAT_R32_UINT;

	m_IBDesc.ByteWidth = m_IndicesByteLength * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;


	/* D3D11_SUBRESOURCE_DATA */
	ZeroMemory(&m_IBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	_uint	iNumPrimitive = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			_uint	iIndices[] = {
				{ iIndex + m_iNumVerticesX },
				{ iIndex + m_iNumVerticesX + 1},
				{ iIndex + 1 },
				{ iIndex }
			};

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[1];
			pIndices[iNumPrimitive]._2 = iIndices[2];

			_vector		vSour, vDest, vNormal;

			vSour = XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._1].vPosition) - 
				XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._0].vPosition);

			vDest = XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._2].vPosition) -
				XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._1].vPosition);

			vNormal = XMVector3Cross(vSour, vDest);
			vNormal = XMVector3Normalize(vNormal);

			XMStoreFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._0].vNormal, 
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._0].vNormal) + vNormal));
			XMStoreFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._1].vNormal) + vNormal));
			XMStoreFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._2].vNormal, 
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._2].vNormal) + vNormal));

			++iNumPrimitive;

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[2];
			pIndices[iNumPrimitive]._2 = iIndices[3];

			vSour = XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._1].vPosition) -
				XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._0].vPosition);

			vDest = XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._2].vPosition) -
				XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._1].vPosition);

			vNormal = XMVector3Cross(vSour, vDest);
			vNormal = XMVector3Normalize(vNormal);

			XMStoreFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._0].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._0].vNormal) + vNormal));
			XMStoreFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._1].vNormal) + vNormal));
			XMStoreFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[pIndices[iNumPrimitive]._2].vNormal) + vNormal));

			++iNumPrimitive;
		}
	}

	m_IBSubresourceData.pSysMem = pIndices;


	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	Safe_Delete_Array(pIndices);

	D3D11_INPUT_ELEMENT_DESC		ElementDescs[] = 
	{		
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }		
	};

	if (FAILED(Compile_ShaderFiles(pShaderFilePath, ElementDescs, 3)))
		return E_FAIL;

	Safe_Delete_Array(pPixel);



	m_pQuadTree = CQuadTree::Create(m_iNumVerticesX, m_iNumVerticesZ);
	if (nullptr == m_pQuadTree)
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Culling(_fmatrix WorldMatrixInverse)
{
	CFrustum*	pFrustum = GET_INSTANCE(CFrustum);

	pFrustum->Transform_ToLocalSpace(WorldMatrixInverse);

	_uint		iNumFaces = 0;

	//for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	//{
	//	for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
	//	{
	//		_uint	iIndex = i * m_iNumVerticesX + j;

	//		_uint	iIndices[] = {
	//			{ iIndex + m_iNumVerticesX },
	//			{ iIndex + m_iNumVerticesX + 1 },
	//			{ iIndex + 1 },
	//			{ iIndex }
	//		};			

	//		_uint	isIn[4] = {
	//			pFrustum->isInLocal(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[iIndices[0]].vPosition)), 
	//			pFrustum->isInLocal(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[iIndices[1]].vPosition)),
	//			pFrustum->isInLocal(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[iIndices[2]].vPosition)),
	//			pFrustum->isInLocal(XMLoadFloat3(&((VTXNORTEX*)m_pVertices)[iIndices[3]].vPosition))
	//		};

	//		/* ������ �� �ﰢ���� �׷����ٸ�? */
	//		if (true == isIn[0] ||
	//			true == isIn[1] ||
	//			true == isIn[2])
	//		{
	//			((FACEINDICES32*)m_pIndices)[iNumFaces]._0 = iIndices[0];
	//			((FACEINDICES32*)m_pIndices)[iNumFaces]._1 = iIndices[1];
	//			((FACEINDICES32*)m_pIndices)[iNumFaces]._2 = iIndices[2];
	//			++iNumFaces;
	//		}

	//		/* �����ϴ� �ﰢ���� �׷����ٸ�? */
	//		if (true == isIn[0] ||
	//			true == isIn[2] ||
	//			true == isIn[3])
	//		{
	//			((FACEINDICES32*)m_pIndices)[iNumFaces]._0 = iIndices[0];
	//			((FACEINDICES32*)m_pIndices)[iNumFaces]._1 = iIndices[2];
	//			((FACEINDICES32*)m_pIndices)[iNumFaces]._2 = iIndices[3];
	//			++iNumFaces;
	//		}
	//	}
	//}

	m_pQuadTree->Culling(pFrustum, (VTXNORTEX*)m_pVertices, (FACEINDICES32*)m_pIndices, &iNumFaces, WorldMatrixInverse);

	m_iNumPrimitive = iNumFaces;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pDeviceContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);
	
	memcpy(SubResource.pData, m_pIndices, sizeof(FACEINDICES32) * iNumFaces);	

	m_pDeviceContext->Unmap(m_pIB, 0);

	RELEASE_INSTANCE(CFrustum);

	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pShaderFilePath, const _tchar* pHeightMapPath)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, pHeightMapPath)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pQuadTree);


}
