#include "..\public\VIBuffer_PointInstance_Explosion.h"
#include <time.h>

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
	, m_pRandomPos(rhs.m_pRandomPos)
	, m_pDir(rhs.m_pDir)
	, m_pNormal(rhs.m_pNormal)
{
	Safe_AddRef(m_pVBInstance);	
}

HRESULT CVIBuffer_PointInstance_Explosion::NativeConstruct_Prototype(/*const _tchar* pShaderFilePath, _uint iNumInstance*/)
{
	srand(unsigned(time(NULL)));

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

	///* �ν��Ͻ̿� �������۸� ��������.(�ν��Ͻ��� ������ŭ ����(�簢�� �ϳ��� �����̱� ���� ���)�� �����Ͽ� �����ϴ� ����) */
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
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	memcpy(&m_Desc, pArg, sizeof(PIDESC));

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
		((VTXPOINT*)m_pVertices)[i].vPosition = _float3(0.f, 0.f, 0.f);
		((VTXPOINT*)m_pVertices)[i].vPSize = _float2(m_Desc.fParticleSize.x, m_Desc.fParticleSize.y);
	}

	m_VBSubresourceData.pSysMem = m_pVertices;


	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	/* �ν��Ͻ̿� �������۸� ��������.(�ν��Ͻ��� ������ŭ ����(�簢�� �ϳ��� �����̱� ���� ���)�� �����Ͽ� �����ϴ� ����) */
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
		XMStoreFloat4(&pVertices[i].vRight, m_Desc.matParticle.r[0]);
		XMStoreFloat4(&pVertices[i].vUp, m_Desc.matParticle.r[1]);
		XMStoreFloat4(&pVertices[i].vLook, m_Desc.matParticle.r[2]);
		pVertices[i].vPosition =
			_float4(XMVectorGetX(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleRandomPos.x),
				XMVectorGetY(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleRandomPos.y),
				XMVectorGetZ(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleRandomPos.z), 1.f);
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

	if (FAILED(Compile_ShaderFiles(m_Desc.ShaderFilePath, ElementDescs, 6)))
		return E_FAIL;

	m_pRandomPos = new _float3[m_Desc.iNumInstance];
	m_pDir = new _float3[m_Desc.iNumInstance];
	m_pNormal = new _float3[m_Desc.iNumInstance];
	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		m_pRandomPos[i] = { -(rand() % (_int)(m_Desc.fParticleRandomDir.x * 2)) + m_Desc.fParticleRandomDir.x,
			-(rand() % (_int)(m_Desc.fParticleRandomDir.y * 2)) + m_Desc.fParticleRandomDir.y,
			-(rand() % (_int)(m_Desc.fParticleRandomDir.z * 2)) + m_Desc.fParticleRandomDir.z};


		while (0 == m_pRandomPos[i].x && 0 == m_pRandomPos[i].y && 0 == m_pRandomPos[i].z)
		{
			m_pRandomPos[i].x = -(rand() % (_int)(m_Desc.fParticleRandomDir.x * 2)) + m_Desc.fParticleRandomDir.x;
			m_pRandomPos[i].y = -(rand() % (_int)(m_Desc.fParticleRandomDir.y * 2)) + m_Desc.fParticleRandomDir.y;
			m_pRandomPos[i].z = -(rand() % (_int)(m_Desc.fParticleRandomDir.z * 2)) + m_Desc.fParticleRandomDir.z;
		}
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

	/* �׷����� ���ؚù��۵��� ��ġ�� ���ε��Ѵ�. */
	m_pDeviceContext->IASetVertexBuffers(0, 2, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);
	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex]->pInputLayout);


	if (FAILED(m_EffectDescs[iPassIndex]->pPass->Apply(0, m_pDeviceContext)))
		return E_FAIL;

	m_pDeviceContext->DrawInstanced(1, m_Desc.iNumInstance, 0, 0);

	return S_OK;
}

void CVIBuffer_PointInstance_Explosion::Update(_double TimeDelta, _uint eAxis)
{
	if ((_uint)AXIS::AXIS_END <= eAxis)
	{
		MSGBOX("Failed to Update In CVIBuffer_PointInstance_Explosion::Update");
		return;
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pDeviceContext->Map(m_pVBInstance, 0, /*D3D11_MAP_WRITE_DISCARD*/D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	if (!m_bSettingDir) //����ó�����ѹ������������� �״㿣 ����
	{
		for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
		{
			m_pDir[i] = { m_pRandomPos[i].x - ((VTXMATRIX*)SubResource.pData)[i].vPosition.x,
				m_pRandomPos[i].y - ((VTXMATRIX*)SubResource.pData)[i].vPosition.y,
				m_pRandomPos[i].z - ((VTXMATRIX*)SubResource.pData)[i].vPosition.z };

			while (0 == m_pRandomPos[i].x && 0 == m_pRandomPos[i].y && 0 == m_pRandomPos[i].z)
			{
				m_pRandomPos[i].x = -(rand() % (_int)(m_Desc.fParticleRandomDir.x * 2)) + m_Desc.fParticleRandomDir.x;
				m_pRandomPos[i].y = -(rand() % (_int)(m_Desc.fParticleRandomDir.y * 2)) + m_Desc.fParticleRandomDir.y;
				m_pRandomPos[i].z = -(rand() % (_int)(m_Desc.fParticleRandomDir.z * 2)) + m_Desc.fParticleRandomDir.z;
			}

			_vector vNormal = XMVector3Normalize(XMLoadFloat3(&m_pDir[i]));
			XMStoreFloat3(&m_pNormal[i], vNormal);
		}
		m_bSettingDir = true;
	}

	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		if (!m_bReset)
		{
			if ((_uint)AXIS::AXIS_X == eAxis)
			{
				((VTXMATRIX*)SubResource.pData)[i].vPosition.x = 0;
			}
			else if ((_uint)AXIS::AXIS_Y == eAxis)
			{
				((VTXMATRIX*)SubResource.pData)[i].vPosition.y = 0;
			}
			else if ((_uint)AXIS::AXIS_Z == eAxis)
			{
				((VTXMATRIX*)SubResource.pData)[i].vPosition.z = 0;
			}

			if ((_uint)AXIS::AXIS_X != eAxis)
			{
				((VTXMATRIX*)SubResource.pData)[i].vPosition.x += m_pNormal[i].x * (_float)TimeDelta * m_Desc.fParticleSpeed;
			}
			if ((_uint)AXIS::AXIS_Y != eAxis)
			{
				((VTXMATRIX*)SubResource.pData)[i].vPosition.y += m_pNormal[i].y * (_float)TimeDelta * m_Desc.fParticleSpeed;
			}
			if ((_uint)AXIS::AXIS_Z != eAxis)
			{
				((VTXMATRIX*)SubResource.pData)[i].vPosition.z += m_pNormal[i].z * (_float)TimeDelta * m_Desc.fParticleSpeed;
			}

			if ((_uint)AXIS::AXIS_ALL == eAxis)
			{
				((VTXMATRIX*)SubResource.pData)[i].vPosition.x += m_pNormal[i].x * (_float)TimeDelta * m_Desc.fParticleSpeed;
				((VTXMATRIX*)SubResource.pData)[i].vPosition.y += m_pNormal[i].y * (_float)TimeDelta * m_Desc.fParticleSpeed;
				((VTXMATRIX*)SubResource.pData)[i].vPosition.z += m_pNormal[i].z * (_float)TimeDelta * m_Desc.fParticleSpeed;
				((VTXMATRIX*)SubResource.pData)[i].vPosition.w = 1.f;
			}
		}
	}

	/*if (0 > m_fCamLookDir.z)
	{
		sort(((VTXMATRIX*)SubResource.pData), ((VTXMATRIX*)SubResource.pData) + m_Desc.iNumInstance, [](VTXMATRIX pSour, VTXMATRIX pDest)
			{
				return pSour.vPosition.z > pDest.vPosition.z;
			});
	}
	else if (0 <= m_fCamLookDir.z)
	{
		sort(((VTXMATRIX*)SubResource.pData), ((VTXMATRIX*)SubResource.pData) + m_Desc.iNumInstance, [](VTXMATRIX pSour, VTXMATRIX pDest)
			{
				return pSour.vPosition.z < pDest.vPosition.z;
			});
	}*/

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_PointInstance_Explosion::Particle_Reset()
{
	m_bSettingDir = false;

	//���µɶ����� ���Ӱ� ���ͼ��� 
	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		m_pRandomPos[i] = { -(rand() % (_int)(m_Desc.fParticleRandomDir.x * 2)) + m_Desc.fParticleRandomDir.x,
			-(rand() % (_int)(m_Desc.fParticleRandomDir.y * 2)) + m_Desc.fParticleRandomDir.y,
			-(rand() % (_int)(m_Desc.fParticleRandomDir.z * 2)) + m_Desc.fParticleRandomDir.z };

		while (0 == m_pRandomPos[i].x && 0 == m_pRandomPos[i].y && 0 == m_pRandomPos[i].z)
		{
			m_pRandomPos[i].x = -(rand() % (_int)(m_Desc.fParticleRandomDir.x * 2)) + m_Desc.fParticleRandomDir.x;
			m_pRandomPos[i].y = -(rand() % (_int)(m_Desc.fParticleRandomDir.y * 2)) + m_Desc.fParticleRandomDir.y;
			m_pRandomPos[i].z = -(rand() % (_int)(m_Desc.fParticleRandomDir.z * 2)) + m_Desc.fParticleRandomDir.z;
		}
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);


	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		((VTXPOINT*)SubResource.pData)[i].vPSize.x = m_Desc.fParticleSize.x;
		((VTXPOINT*)SubResource.pData)[i].vPSize.y = m_Desc.fParticleSize.y;

		((VTXMATRIX*)SubResource.pData)[i].vPosition.x = XMVectorGetX(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleRandomPos.x);
		((VTXMATRIX*)SubResource.pData)[i].vPosition.y = XMVectorGetY(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleRandomPos.y);
		((VTXMATRIX*)SubResource.pData)[i].vPosition.z = XMVectorGetZ(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleRandomPos.z);
	}

	////�����ؾߴ� ���⼭ , �ָ��ִ°ͺ��� �׸����ְ�,, ���� �����̾����ϴ�.. 
	if (0 > m_fCamLookDir.z)
	{
		sort(((VTXMATRIX*)SubResource.pData), ((VTXMATRIX*)SubResource.pData) + m_Desc.iNumInstance, [](VTXMATRIX pSour, VTXMATRIX pDest)
			{
				return pSour.vPosition.z > pDest.vPosition.z;
			});
	}
	if (0 <= m_fCamLookDir.z)
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

	Safe_Delete_Array(m_pDir);
	Safe_Delete_Array(m_pNormal);
	Safe_Delete_Array(m_pRandomPos);

	Safe_Release(m_pVBInstance);
}
