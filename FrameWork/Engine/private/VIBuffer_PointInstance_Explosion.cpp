#include "..\public\VIBuffer_PointInstance_Explosion.h"

CVIBuffer_PointInstance_Explosion::CVIBuffer_PointInstance_Explosion(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_PointInstance_Explosion::CVIBuffer_PointInstance_Explosion(const CVIBuffer_PointInstance_Explosion & rhs)
	: CVIBuffer(rhs)
	//, m_iNumInstance(rhs.m_iNumInstance)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_VBInstSubresourceData(rhs.m_VBInstSubresourceData)
	, m_iInstStride(rhs.m_iInstStride)
	, m_iInstNumVertices(rhs.m_iInstNumVertices)
	, m_pRandomSpeed(rhs.m_pRandomSpeed)
{
	Safe_AddRef(m_pVBInstance);	
}

HRESULT CVIBuffer_PointInstance_Explosion::NativeConstruct_Prototype(/*const _tchar* pShaderFilePath, _uint iNumInstance*/)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	//ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	///* D3D11_BUFFER_DESC */
	//m_iStride = sizeof(VTXPOINT);
	//m_iNumVertices = iNumInstance;
	//m_iNumInstance = iNumInstance;
	//m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	//m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	//m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//m_VBDesc.CPUAccessFlags = 0;
	//m_VBDesc.MiscFlags = 0;
	//m_VBDesc.StructureByteStride = m_iStride;

	//
	///* D3D11_SUBRESOURCE_DATA */
	//ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//m_pVertices = new VTXPOINT[m_iNumVertices];
	//ZeroMemory(m_pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	//for (_uint i = 0; i < m_iNumVertices; ++i)
	//{
	//	((VTXPOINT*)m_pVertices)[i].vPosition = _float3(0.0f, 0.0f, 0.f);
	//	((VTXPOINT*)m_pVertices)[i].vPSize = _float2(1.0f, 1.f);
	//}
	//
	//m_VBSubresourceData.pSysMem = m_pVertices;


	//if (FAILED(__super::Create_VertexBuffer()))
	//	return E_FAIL;

	///* 인스턴싱용 정점버퍼를 생성하자.(인스턴스의 갯수만큼 정점(사각형 하나를 움직이기 위한 행렬)을 생성하여 보관하는 버퍼) */
	//ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	///* D3D11_BUFFER_DESC */
	//m_iInstStride = sizeof(VTXMATRIX);
	//m_iInstNumVertices = m_iNumInstance;

	//m_VBInstDesc.ByteWidth = m_iInstStride * m_iInstNumVertices;
	//m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//m_VBInstDesc.MiscFlags = 0;
	//m_VBInstDesc.StructureByteStride = m_iInstStride;

	//ZeroMemory(&m_VBInstSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//VTXMATRIX*	pVertices = new VTXMATRIX[m_iInstNumVertices];
	//ZeroMemory(pVertices, sizeof(VTXMATRIX) * m_iInstNumVertices);

	//for (_uint i = 0; i < m_iInstNumVertices; ++i)
	//{
	//	pVertices[i].vRight = _float4(1.0f, 0.f, 0.f, 0.f);
	//	pVertices[i].vUp = _float4(0.f, 1.0f, 0.f, 0.f);
	//	pVertices[i].vLook = _float4(0.f, 0.f, 1.0f, 0.f);
	//	pVertices[i].vPosition = _float4(rand() % 20, 15.0f, rand() % 20, 1.f);
	//}
	//m_VBInstSubresourceData.pSysMem = pVertices;

	//if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubresourceData, &m_pVBInstance)))
	//	return E_FAIL;

	//Safe_Delete_Array(pVertices);

	//D3D11_INPUT_ELEMENT_DESC		ElementDescs[] = 
	//{		
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "PSIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 

	//	{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//	{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//	{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//	{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//};

	//if (FAILED(Compile_ShaderFiles(pShaderFilePath, ElementDescs, 6)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Explosion::NativeConstruct(void * pArg)
{
	memcpy(&m_Desc, pArg, sizeof(PIDESC));

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = m_Desc.iNumInstance;
	//m_iNumInstance = iNumInstance;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;


	/* D3D11_SUBRESOURCE_DATA */
	ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_pVertices = new VTXPOINT[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		((VTXPOINT*)m_pVertices)[i].vPosition = _float3(0.0f, 0.0f, 0.f);
		((VTXPOINT*)m_pVertices)[i].vPSize = _float2(m_Desc.fParticleSize.x, m_Desc.fParticleSize.y);
		int a = 0;
	}

	m_VBSubresourceData.pSysMem = m_pVertices;


	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	/* 인스턴싱용 정점버퍼를 생성하자.(인스턴스의 갯수만큼 정점(사각형 하나를 움직이기 위한 행렬)을 생성하여 보관하는 버퍼) */
	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iInstStride = sizeof(VTXMATRIX);
	m_iInstNumVertices = m_Desc.iNumInstance;

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
		pVertices[i].vRight = _float4(1.0f, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, 1.0f, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, 1.0f, 0.f);
		pVertices[i].vPosition = _float4(rand() % 20, 15.0f + rand() % (_int)(m_Desc.fRandom.y), rand() % 20, 1.f);
	}
	m_VBInstSubresourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubresourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	D3D11_INPUT_ELEMENT_DESC		ElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PSIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	/*if (FAILED(Compile_ShaderFiles(m_Desc. m_wstrShaderFilePath, ElementDescs, 6)))
		return E_FAIL;*/
	if (FAILED(Compile_ShaderFiles(m_Desc.ShaderFilePath, ElementDescs, 6)))
		return E_FAIL;


	m_pRandomSpeed = new _double[m_Desc.iNumInstance];
	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		m_pRandomSpeed[i] = -rand() % (_int)(m_Desc.fRandom.y) + m_Desc.fSpeed;
		int a = 0;
	}
	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Explosion::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer*		pVertexBuffers[] = {
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

	/* 그려야할 버텍싀버퍼들을 장치에 바인드한다. */
	m_pDeviceContext->IASetVertexBuffers(0, 2, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);
	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex]->pInputLayout);


	if (FAILED(m_EffectDescs[iPassIndex]->pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

	m_pDeviceContext->DrawInstanced(1, m_Desc.iNumInstance, 0, 0);

	return S_OK;
}

void CVIBuffer_PointInstance_Explosion::Update(_double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource; 

	m_pDeviceContext->Map(m_pVBInstance, 0, /*D3D11_MAP_WRITE_DISCARD*/D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		((VTXMATRIX*)SubResource.pData)[i].vPosition.y -= m_pRandomSpeed[i] * TimeDelta;

		if (0 > ((VTXMATRIX*)SubResource.pData)[i].vPosition.y)
			((VTXMATRIX*)SubResource.pData)[i].vPosition.y = 15.f + rand() % (_int)(m_Desc.fRandom.y);
	}


	if (0 > m_Desc.fDir.z)
	{
		sort(((VTXMATRIX*)SubResource.pData), ((VTXMATRIX*)SubResource.pData) + m_Desc.iNumInstance, [](VTXMATRIX pSour, VTXMATRIX pDest)
			{
				return pSour.vPosition.z > pDest.vPosition.z;
			});
	}
	if (0 <= m_Desc.fDir.z)
	{
		sort(((VTXMATRIX*)SubResource.pData), ((VTXMATRIX*)SubResource.pData) + m_Desc.iNumInstance, [](VTXMATRIX pSour, VTXMATRIX pDest)
			{
				return pSour.vPosition.z < pDest.vPosition.z;
			});
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_PointInstance_Explosion * CVIBuffer_PointInstance_Explosion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext/*, const _tchar* pShaderFilePath, _uint iNumInstance*/)
{
	CVIBuffer_PointInstance_Explosion*		pInstance = new CVIBuffer_PointInstance_Explosion(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(/*pShaderFilePath, iNumInstance*/)))
	{
		MSGBOX("Failed to Creating CVIBuffer_PointInstance_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_PointInstance_Explosion::Clone(void * pArg)
{
	CVIBuffer_PointInstance_Explosion*		pInstance = new CVIBuffer_PointInstance_Explosion(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_PointInstance_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_PointInstance_Explosion::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pRandomSpeed);

	Safe_Release(m_pVBInstance);
}
