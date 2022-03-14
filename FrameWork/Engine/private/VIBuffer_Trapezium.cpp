#include "..\public\VIBuffer_Trapezium.h"

CVIBuffer_Trapezium::CVIBuffer_Trapezium(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_Trapezium::CVIBuffer_Trapezium(const CVIBuffer_Trapezium & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Trapezium::NativeConstruct_Prototype(/*const _tchar* pShaderFilePath*/)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	//ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	///* D3D11_BUFFER_DESC */
	//m_iStride = sizeof(VTXTEX);
	//m_iNumVertices = 4;

	//m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	//m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//m_VBDesc.CPUAccessFlags = 0;
	//m_VBDesc.MiscFlags = 0;
	//m_VBDesc.StructureByteStride = m_iStride;

	//
	///* D3D11_SUBRESOURCE_DATA */
	//ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//m_pVertices = new VTXTEX[m_iNumVertices];
	//ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	//((VTXTEX*)m_pVertices)[0].vPosition = _float3(-0.3f, 0.5f, 0.f);
	//((VTXTEX*)m_pVertices)[0].vTexUV = _float2(0.0f, 0.f);

	//((VTXTEX*)m_pVertices)[1].vPosition = _float3(0.7f, 0.5f, 0.f);
	//((VTXTEX*)m_pVertices)[1].vTexUV = _float2(1.0f, 0.f);

	//((VTXTEX*)m_pVertices)[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	//((VTXTEX*)m_pVertices)[2].vTexUV = _float2(1.0f, 1.f);

	//((VTXTEX*)m_pVertices)[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	//((VTXTEX*)m_pVertices)[3].vTexUV = _float2(0.0f, 1.f);

	//
	//m_VBSubresourceData.pSysMem = m_pVertices;

	//if (FAILED(__super::Create_VertexBuffer()))
	//	return E_FAIL;


	///* D3D11_BUFFER_DESC */
	//m_IndicesByteLength = sizeof(FACEINDICES16);
	//m_iNumPrimitive = 2;
	//m_iNumIndicesFigure = 3;
	//m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//m_eFormat = DXGI_FORMAT_R16_UINT;

	//m_IBDesc.ByteWidth = m_IndicesByteLength * m_iNumPrimitive;
	//m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//m_IBDesc.CPUAccessFlags = 0;
	//m_IBDesc.MiscFlags = 0;
	//m_IBDesc.StructureByteStride = 0;


	///* D3D11_SUBRESOURCE_DATA */
	//ZeroMemory(&m_IBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	//FACEINDICES16*		pIndices = new FACEINDICES16[m_iNumPrimitive];
	//ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	//pIndices[0]._0 = 0;
	//pIndices[0]._1 = 1;
	//pIndices[0]._2 = 2;

	//pIndices[1]._0 = 0;
	//pIndices[1]._1 = 2;
	//pIndices[1]._2 = 3;

	//m_IBSubresourceData.pSysMem = pIndices;

	//if (FAILED(__super::Create_IndexBuffer()))
	//	return E_FAIL;


	//m_pIndices = new FACEINDICES16[m_iNumPrimitive];

	//memcpy(m_pIndices, pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	//Safe_Delete_Array(pIndices);

	//D3D11_INPUT_ELEMENT_DESC		ElementDescs[] = 
	//{		
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }		
	//};

	//if (FAILED(Compile_ShaderFiles(pShaderFilePath, ElementDescs, 2)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Trapezium::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(TRAPDESC));
	}

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;

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

	if (m_Desc.bMinus)
	{
		((VTXTEX*)m_pVertices)[0].vPosition = _float3(-m_Desc.fAngle, 0.5f, 0.f);
		((VTXTEX*)m_pVertices)[0].vTexUV = _float2(0.0f, 0.f);

		((VTXTEX*)m_pVertices)[1].vPosition = _float3(1.f - m_Desc.fAngle, 0.5f, 0.f);
		((VTXTEX*)m_pVertices)[1].vTexUV = _float2(1.0f, 0.f);

		((VTXTEX*)m_pVertices)[2].vPosition = _float3(0.5f, -0.5f, 0.f);
		((VTXTEX*)m_pVertices)[2].vTexUV = _float2(1.0f, 1.f);

		((VTXTEX*)m_pVertices)[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
		((VTXTEX*)m_pVertices)[3].vTexUV = _float2(0.0f, 1.f);
	}
	else
	{
		((VTXTEX*)m_pVertices)[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
		((VTXTEX*)m_pVertices)[0].vTexUV = _float2(0.0f, 0.f);

		((VTXTEX*)m_pVertices)[1].vPosition = _float3(0.5f, 0.5f, 0.f);
		((VTXTEX*)m_pVertices)[1].vTexUV = _float2(1.0f, 0.f);

		((VTXTEX*)m_pVertices)[2].vPosition = _float3(1.f - m_Desc.fAngle, -0.5f, 0.f);
		((VTXTEX*)m_pVertices)[2].vTexUV = _float2(1.0f, 1.f);

		((VTXTEX*)m_pVertices)[3].vPosition = _float3(-m_Desc.fAngle, -0.5f, 0.f);
		((VTXTEX*)m_pVertices)[3].vTexUV = _float2(0.0f, 1.f);
	}



	m_VBSubresourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;


	/* D3D11_BUFFER_DESC */
	m_IndicesByteLength = sizeof(FACEINDICES16);
	m_iNumPrimitive = 2;
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

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_IBSubresourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;


	m_pIndices = new FACEINDICES16[m_iNumPrimitive];

	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	Safe_Delete_Array(pIndices);

	D3D11_INPUT_ELEMENT_DESC		ElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Compile_ShaderFiles(m_Desc.ShaderFilePath, ElementDescs, 2)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Trapezium * CVIBuffer_Trapezium::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext/*, const _tchar* pShaderFilePath*/)
{
	CVIBuffer_Trapezium*		pInstance = new CVIBuffer_Trapezium(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(/*pShaderFilePath*/)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Trapezium");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Trapezium::Clone(void * pArg)
{
	CVIBuffer_Trapezium*		pInstance = new CVIBuffer_Trapezium(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Trapezium");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trapezium::Free()
{
	__super::Free();
}
