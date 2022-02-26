#include "VIBuffer_Plane.h"

CVIBuffer_Plane::CVIBuffer_Plane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CVIBuffer(_pDevice, _pDeviceContext)
{
}

CVIBuffer_Plane::CVIBuffer_Plane(const CVIBuffer_Plane& _rhs)
	: CVIBuffer(_rhs)
	, m_pIndices(_rhs.m_pIndices)
	, m_iNumVerticesX(_rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(_rhs.m_iNumVerticesZ)

{
}

HRESULT CVIBuffer_Plane::NativeConstruct_Prototype(const _tchar* _pShaderFilePath, _uint _numVertsX, _uint _numVertsZ)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumVerticesX = _numVertsX;
	m_iNumVerticesZ = _numVertsZ;

	/* size of one Vertex */
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	/* for. VB */
	/* D3D11_BUFFER_DESC : 생성해야할 버텍스 버퍼 오브젝트에 관한 정보 */
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	/* D3D11_SUBRESOURCE_DATA : 생성도중에 버텍스 버퍼로 복사될 실제 데이터에 관한 정보*/
	ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			((VTXTEX*)m_pVertices)[iIndex].vPosition = _float3(_float(j), 0.f, _float(i));
			((VTXTEX*)m_pVertices)[iIndex].vTexUV = _float2(j / _float((m_iNumVerticesX - 1)), i / _float((m_iNumVerticesZ - 1)));
		}
	}
	m_VBSubresourceData.pSysMem = m_pVertices;


	/* for.IB */
	/* D3D11_BUFFER_DESC : 생성해야할 인덱스 버퍼 오브젝트에 관한 정보 */
	m_IndicesByteLength = sizeof(FACEINDICES32);
	/* Num of figure */
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesFigure = 3;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eFormat = DXGI_FORMAT_R32_UINT;

	m_IBDesc.ByteWidth = m_IndicesByteLength * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	/* D3D11_SUBRESOURCE_DATA : 생성도중에 인데스 버퍼로 복사될 실제 데이터에 관한 정보*/
	ZeroMemory(&m_IBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

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

			m_pIndices[iNumPrimitive]._0 = iIndices[0];
			m_pIndices[iNumPrimitive]._1 = iIndices[1];
			m_pIndices[iNumPrimitive]._2 = iIndices[2];

			++iNumPrimitive;

			m_pIndices[iNumPrimitive]._0 = iIndices[0];
			m_pIndices[iNumPrimitive]._1 = iIndices[2];
			m_pIndices[iNumPrimitive]._2 = iIndices[3];

			++iNumPrimitive;
		}
	}

	m_IBSubresourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

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

HRESULT CVIBuffer_Plane::NativeConstruct(void* _pArg)
{
	if (FAILED(__super::NativeConstruct(_pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Plane* CVIBuffer_Plane::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const _tchar* _pShaderFilePath, _uint _numVertsX, _uint _numVertsZ)
{
	CVIBuffer_Plane* pInstance = new CVIBuffer_Plane(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(_pShaderFilePath, _numVertsX, _numVertsZ)))
	{
		MSGBOX("Failed to Create CVIBuffer!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Plane::Clone(void* _pArg)
{
	CVIBuffer_Plane* pInstance = new CVIBuffer_Plane(*this);

	if (FAILED(pInstance->NativeConstruct(_pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Plane::Free(void)
{
	__super::Free();


	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pIndices);
	}
}
