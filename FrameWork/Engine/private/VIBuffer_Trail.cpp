#include "VIBuffer_Trail.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: CVIBuffer(_pDevice, _pDeviceContext)
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& _rhs)
	: CVIBuffer(_rhs)
{
}

HRESULT CVIBuffer_Trail::NativeConstruct_Prototype(const _tchar* _pShaderFilePath, const _uint _iMaxVertices)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 512;

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	/* D3D11_SUBRESOURCE_DATA */
	ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	m_VBSubresourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;


	/* D3D11_BUFFER_DESC */
	m_IndicesByteLength = sizeof(FACEINDICES32);
	m_iNumPrimitive = 512 - 2;
	m_iNumIndicesFigure = 3;
	//m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
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
	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	m_IBSubresourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;


	D3D11_INPUT_ELEMENT_DESC		ElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Compile_ShaderFiles(_pShaderFilePath, ElementDescs, 2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Trail::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Trail::Render_Curve(const _uint _iPassIndex)
{
	if (FAILED(__super::Render(_iPassIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Trail::Set_VertexTrail(const _float3 _vStartPoints[], const _float3 _vEndPoints[], const _uint _iCount)
{
	D3D11_MAPPED_SUBRESOURCE pVertices, pIndices;

	// Vertex Buffer
	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pVertices);

	for (_uint i = 0; i < _iCount; ++i)
	{
		((VTXTEX*)pVertices.pData)[i * 2 + 0].vPosition = _vStartPoints[i];
		//((VTXTEX*)pVertices.pData)[i * 2 + 0].vTexUV = _float2(1.f, (i * 2 + 0) / _float(_iCount - 2.f));
		//((VTXTEX*)pVertices.pData)[i * 2 + 0].vTexUV = _float2(1.f, (i * 2 + 0) / _float((_iCount - 1) * 2));
		((VTXTEX*)pVertices.pData)[i * 2 + 0].vTexUV = _float2((i * 2 + 0) / _float((_iCount - 1) * 2), 0.f);

		((VTXTEX*)pVertices.pData)[i * 2 + 1].vPosition = _vEndPoints[i];
		//((VTXTEX*)pVertices.pData)[i * 2 + 1].vTexUV = _float2(0.f, (i * 2 + 1) / _float(_iCount - 1.f));
		//((VTXTEX*)pVertices.pData)[i * 2 + 1].vTexUV = _float2(1.f, (i * 2 + 1) / _float(_iCount * 2 - 1));
		((VTXTEX*)pVertices.pData)[i * 2 + 1].vTexUV = _float2((i * 2 + 0) / _float((_iCount - 1) * 2), 1.f);
	}
	m_iNumPrimitive = (_iCount - 1) * 2;

	m_pDeviceContext->Unmap(m_pVB, 0);

	// Index Buffer
	m_pDeviceContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pIndices);

	_uint iNumPrimitive = 0;
	for (_uint i = 0; i < m_iNumPrimitive / 2; ++i)
	{
		((FACEINDICES32*)pIndices.pData)[iNumPrimitive]._0 = i * 2;
		((FACEINDICES32*)pIndices.pData)[iNumPrimitive]._1 = i * 2 + 1;
		((FACEINDICES32*)pIndices.pData)[iNumPrimitive]._2 = i * 2 + 2;
		++iNumPrimitive;
		((FACEINDICES32*)pIndices.pData)[iNumPrimitive]._0 = i * 2 + 1;
		((FACEINDICES32*)pIndices.pData)[iNumPrimitive]._1 = i * 2 + 3;
		((FACEINDICES32*)pIndices.pData)[iNumPrimitive]._2 = i * 2 + 2;
		++iNumPrimitive;
	}

	m_pDeviceContext->Unmap(m_pIB, 0);

	return S_OK;
}

CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const _tchar* _pShaderFilePath, const _uint _iMaxVertices)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(_pDevice, _pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(_pShaderFilePath, _iMaxVertices)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* _pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this);
	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
}
