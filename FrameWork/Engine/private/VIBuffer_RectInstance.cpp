#include "..\public\VIBuffer_RectInstance.h"

CVIBuffer_RectInstance::CVIBuffer_RectInstance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_RectInstance::CVIBuffer_RectInstance(const CVIBuffer_RectInstance & rhs)
	: CVIBuffer(rhs)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_VBInstSubresourceData(rhs.m_VBInstSubresourceData)
	, m_iInstStride(rhs.m_iInstStride)
	, m_iInstNumVertices(rhs.m_iInstNumVertices)
{
	Safe_AddRef(m_pVBInstance);	
}

HRESULT CVIBuffer_RectInstance::NativeConstruct_Prototype(const _tchar* pShaderFilePath, _uint iNumInstance)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumInstance = iNumInstance;

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;


	/* D3D11_SUBRESOURCE_DATA */
	ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	((VTXTEX*)m_pVertices)[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	((VTXTEX*)m_pVertices)[0].vTexUV = _float2(0.0f, 0.f);

	((VTXTEX*)m_pVertices)[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	((VTXTEX*)m_pVertices)[1].vTexUV = _float2(1.0f, 0.f);

	((VTXTEX*)m_pVertices)[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	((VTXTEX*)m_pVertices)[2].vTexUV = _float2(1.0f, 1.f);

	((VTXTEX*)m_pVertices)[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	((VTXTEX*)m_pVertices)[3].vTexUV = _float2(0.0f, 1.f);


	m_VBSubresourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;


	/* D3D11_BUFFER_DESC */
	m_IndicesByteLength = sizeof(FACEINDICES16);
	m_iNumPrimitive = 2 * iNumInstance;
	m_iNumIndicesFigure = 3;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eFormat = DXGI_FORMAT_R16_UINT;

	m_IBDesc.ByteWidth = m_IndicesByteLength * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;


	/* D3D11_SUBRESOURCE_DATA */
	ZeroMemory(&m_IBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	_uint		iFaceIndex = 0;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pIndices[iFaceIndex]._0 = 0;
		pIndices[iFaceIndex]._1 = 1;
		pIndices[iFaceIndex]._2 = 2;
		++iFaceIndex;

		pIndices[iFaceIndex]._0 = 0;
		pIndices[iFaceIndex]._1 = 2;
		pIndices[iFaceIndex]._2 = 3;
		++iFaceIndex;
	}
	m_IBSubresourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;


	m_pIndices = new FACEINDICES16[m_iNumPrimitive];

	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	Safe_Delete_Array(pIndices);

	/* ?????????? ?????????? ????????.(?????????? ???????? ????(?????? ?????? ???????? ???? ????)?? ???????? ???????? ????) */
	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iInstStride = sizeof(VTXMATRIX);
	m_iInstNumVertices = m_iNumInstance;

	m_VBInstDesc.ByteWidth = m_iInstStride * m_iInstNumVertices;
	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstDesc.MiscFlags = 0;
	m_VBInstDesc.StructureByteStride = m_iInstStride;

	ZeroMemory(&m_VBInstSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	VTXMATRIX* pVertices = new VTXMATRIX[m_iInstNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMATRIX) * m_iInstNumVertices);

	for (_uint i = 0; i < m_iInstNumVertices; ++i)
	{
		pVertices[i].vRight = _float4(0.5f, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, 0.5f, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, 0.5f, 0.f);
		pVertices[i].vPosition = _float4(0, 0, 0, 1.f);
	}
	m_VBInstSubresourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubresourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	D3D11_INPUT_ELEMENT_DESC		ElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	if (FAILED(Compile_ShaderFiles(pShaderFilePath, ElementDescs, 6)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_RectInstance::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_RectInstance::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint				iStrides[] = {
		m_iStride,
		m_iInstStride
	};

	_uint				iOffsets[] = {
		0,
		0
	};

	/* ???????? ?????????????? ?????? ??????????. */
	m_pDeviceContext->IASetVertexBuffers(0, 2, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);
	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex]->pInputLayout);


	if (FAILED(m_EffectDescs[iPassIndex]->pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

	m_pDeviceContext->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_RectInstance::Update(_double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource; 

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	//for (_uint i = 0; i < m_iNumInstance; ++i)
	//{
	//	
	//	((VTXMATRIX*)SubResource.pData)[i].vPosition.y -= _float(m_pDropSpeed[i] * TimeDelta);

	//	if (0 > ((VTXMATRIX*)SubResource.pData)[i].vPosition.y)
	//		((VTXMATRIX*)SubResource.pData)[i].vPosition.y = 15.f;
	//}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_RectInstance * CVIBuffer_RectInstance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pShaderFilePath, _uint iNumInstance)
{
	CVIBuffer_RectInstance*		pInstance = new CVIBuffer_RectInstance(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, iNumInstance)))
	{
		MSGBOX("Failed to Creating CVIBuffer_RectInstance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectInstance::Clone(void * pArg)
{
	CVIBuffer_RectInstance*		pInstance = new CVIBuffer_RectInstance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_RectInstance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectInstance::Free()
{
	__super::Free();



	Safe_Release(m_pVBInstance);
}
