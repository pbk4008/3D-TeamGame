#include "Trail_VIBuffer.h"

CTrail_VIBuffer::CTrail_VIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CTrail_VIBuffer::CTrail_VIBuffer(const CTrail_VIBuffer& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CTrail_VIBuffer::NativeConstruct_Prototype(const _tchar* pShaderFilePath, _uint MaxVtex)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = MaxVtex;

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
	m_IndicesByteLength = sizeof(FACEINDICES16);
	m_iNumPrimitive = MaxVtex - 2;
	m_iNumIndicesFigure = 3;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eFormat = DXGI_FORMAT_R16_UINT;

	m_IBDesc.ByteWidth = m_IndicesByteLength * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;


	/* D3D11_SUBRESOURCE_DATA */
	ZeroMemory(&m_IBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	m_pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);


	m_IBSubresourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//m_pIndices = new FACEINDICES16[m_iNumPrimitive];

	//memcpy(m_pIndices, pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	//Safe_Delete_Array(pIndices);

	D3D11_INPUT_ELEMENT_DESC		ElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Compile_ShaderFiles(pShaderFilePath, ElementDescs, 2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrail_VIBuffer::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrail_VIBuffer::RenderBuffer(list<pair<_fvector, _fvector>>* pTrailList, _uint passIndx, _uint InterpoleCnt)
{
	if (pTrailList->size() < 4)
		return;

	Add_Vertex_CatmullRom(pTrailList, InterpoleCnt);

	CVIBuffer::Render(passIndx);
}

HRESULT CTrail_VIBuffer::Add_Vertex(_fvector Startpos, _fvector Endpos)
{
	if (m_dwUsingVtxcnt >= m_iNumVertices || m_dwUsingTriCnt >= m_iNumPrimitive)
		return S_OK;


	D3D11_MAPPED_SUBRESOURCE Vertices, Indices;

	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &Vertices);

	_float3 startpos, endpos;
	XMStoreFloat3(&startpos, Startpos);
	XMStoreFloat3(&endpos, Endpos);

	((VTXTEX*)Vertices.pData)[m_dwUsingVtxcnt].vPosition = startpos;
	++m_dwUsingVtxcnt;

	((VTXTEX*)Vertices.pData)[m_dwUsingVtxcnt].vPosition = endpos;
	++m_dwUsingVtxcnt;

	if (m_dwUsingVtxcnt <= 2)
	{
		m_pDeviceContext->Unmap(m_pVB, 0);
		return S_OK;
	}

	for (_uint i = 0; i < m_dwUsingVtxcnt; i += 2)
	{
		((VTXTEX*)Vertices.pData)[i].vTexUV = _float2((i / 2) / (_float)(m_dwUsingVtxcnt / 2), 0.f);
		((VTXTEX*)Vertices.pData)[i + 1].vTexUV = _float2(((i + 1) / 2) / (_float)(m_dwUsingVtxcnt / 2), 1.f);
	}
	m_pDeviceContext->Unmap(m_pVB, 0);

	// Index Buffer
	m_pDeviceContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &Indices);

	((FACEINDICES16*)Indices.pData)[m_dwUsingTriCnt]._0 = m_dwUsingVtxcnt - 4;
	((FACEINDICES16*)Indices.pData)[m_dwUsingTriCnt]._1 = m_dwUsingVtxcnt - 2;
	((FACEINDICES16*)Indices.pData)[m_dwUsingTriCnt]._2 = m_dwUsingVtxcnt - 1;
	++m_dwUsingTriCnt;

	((FACEINDICES16*)Indices.pData)[m_dwUsingTriCnt]._0 = m_dwUsingVtxcnt - 4;
	((FACEINDICES16*)Indices.pData)[m_dwUsingTriCnt]._1 = m_dwUsingVtxcnt - 1;
	((FACEINDICES16*)Indices.pData)[m_dwUsingTriCnt]._2 = m_dwUsingVtxcnt - 3;
	++m_dwUsingTriCnt;

	m_pDeviceContext->Unmap(m_pIB, 0);

	return S_OK;
}

HRESULT CTrail_VIBuffer::Clear_Vertex()
{
	m_dwUsingVtxcnt = 0;
	m_dwUsingTriCnt = 0;

	return S_OK;
}

HRESULT CTrail_VIBuffer::Add_Vertex_CatmullRom(list<pair<_fvector, _fvector>>* pTrailList, _uint InterpoleCnt)
{
	list<pair<_fvector, _fvector>> ItplList(pTrailList->begin(), pTrailList->end());
	for (_uint i = 0; i < InterpoleCnt; ++i)
	{
		auto iter0 = ItplList.begin();
		auto iter1 = ++iter0;
		auto iter2 = ++iter1;
		auto iter3 = ++iter2;
		auto iterEnd = ItplList.end();

		for (; iter3 != iterEnd;)
		{
			_vector vItplTop, vItplBottom;
			vItplTop = XMVectorCatmullRom(iter0->first, iter1->first, iter2->first, iter3->first, 0.5f);
			vItplBottom = XMVectorCatmullRom(iter0->second, iter1->second, iter2->second, iter3->second, 0.5f);

			ItplList.insert(iter2, make_pair(vItplTop, vItplBottom));

			iter0 = iter1;
			iter1 = iter2;
			iter2 = iter3;
			++iter3;
		}
	}
	D3D11_MAPPED_SUBRESOURCE Vertices, Indices;

	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Vertices);

	m_dwUsingVtxcnt = (_ushort)ItplList.size() * 2;
	m_dwUsingTriCnt = m_dwUsingVtxcnt - 2;

	if ((_uint)ItplList.size() > m_iNumVertices)
	{
		wstring wstrmsg = L"The number of vertex is over, There are" + to_wstring(m_iNumVertices) + L"reserved vertices";
		MessageBox(NULL, wstrmsg.c_str(), L"Trail Buffer", MB_OK);
	}

	_uint i = 0; 
	for (auto& Pair : ItplList)
	{
		_float3 startpos, endpos;
		XMStoreFloat3(&startpos, Pair.first);
		((VTXTEX*)Vertices.pData)[i].vPosition = startpos;
		((VTXTEX*)Vertices.pData)[i].vTexUV = _float2(((_float)i / 2) / (m_dwUsingVtxcnt / 2), 0.f);
		++i;
		
		XMStoreFloat3(&endpos, Pair.second);
		((VTXTEX*)Vertices.pData)[i].vPosition = endpos;
		((VTXTEX*)Vertices.pData)[i].vTexUV = _float2(((_float)i / 2) / (m_dwUsingVtxcnt / 2), 1.f);
		++i;
	}

	m_pDeviceContext->Unmap(m_pVB, 0);

	// Index Buffer
	m_pDeviceContext->Map(m_pIB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Indices);

	for (_uint i = 0; i < m_dwUsingTriCnt; ++i)
	{
		((FACEINDICES16*)Indices.pData)[i]._0 = i;
		((FACEINDICES16*)Indices.pData)[i]._1 = i + 2;
		((FACEINDICES16*)Indices.pData)[i]._2 = i + 3;

		++i;

		((FACEINDICES16*)Indices.pData)[i]._0 = i - 1;
		((FACEINDICES16*)Indices.pData)[i]._1 = i + 2;
		((FACEINDICES16*)Indices.pData)[i]._2 = i;
	}

	m_pDeviceContext->Unmap(m_pIB, 0);


	return S_OK;
}


CTrail_VIBuffer* CTrail_VIBuffer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath,_uint MaxVtex)
{
	CTrail_VIBuffer* pInstance = new CTrail_VIBuffer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, MaxVtex)))
	{
		MSGBOX("Failed to Creating CTrail_VIBuffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTrail_VIBuffer::Clone(void* pArg)
{
	CTrail_VIBuffer* pInstance = new CTrail_VIBuffer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTrail_VIBuffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail_VIBuffer::Free()
{
	__super::Free();
}
