#include "..\public\VIBuffer_PointInstance_Respawn.h"
#include <time.h>

CVIBuffer_PointInstance_Respawn::CVIBuffer_PointInstance_Respawn(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_PointInstance_Respawn::CVIBuffer_PointInstance_Respawn(const CVIBuffer_PointInstance_Respawn & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_VBInstSubresourceData(rhs.m_VBInstSubresourceData)
	, m_iInstStride(rhs.m_iInstStride)
	, m_iInstNumVertices(rhs.m_iInstNumVertices)
	, m_pRandomPos(rhs.m_pRandomPos)
	, m_pDir(rhs.m_pDir)
	, m_pNormal(rhs.m_pNormal)
	, m_pDropSpeed(rhs.m_pDropSpeed)
{
	Safe_AddRef(m_pVBInstance);	
}

HRESULT CVIBuffer_PointInstance_Respawn::NativeConstruct_Prototype()
{
	srand(unsigned(time(NULL)));

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Respawn::NativeConstruct(void * pArg)
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

	/* 인스턴싱용 정점버퍼를 생성하자.(인스턴스의 갯수만큼 정점(사각형 하나를 움직이기 위한 행렬)을 생성하여 보관하는 버퍼) */
	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	/* D3D11_BUFFER_DESC */
	m_iInstStride = sizeof(VTXPARTICLE);
	m_iInstNumVertices = m_Desc.iNumInstance;

	m_VBInstDesc.ByteWidth = m_iInstStride * m_iInstNumVertices;
	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstDesc.MiscFlags = 0;
	m_VBInstDesc.StructureByteStride = m_iInstStride;

	ZeroMemory(&m_VBInstSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	VTXPARTICLE* pVertices = new VTXPARTICLE[m_iInstNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPARTICLE) * m_iInstNumVertices);

	m_pFirstPos = new _float3[m_Desc.iNumInstance];

	for (_uint i = 0; i < m_iInstNumVertices; ++i)
	{
		XMStoreFloat4(&pVertices[i].vRight, m_Desc.matParticle.r[0]);
		XMStoreFloat4(&pVertices[i].vUp, m_Desc.matParticle.r[1]);
		XMStoreFloat4(&pVertices[i].vLook, m_Desc.matParticle.r[2]);
		pVertices[i].vPosition =
			_float4(XMVectorGetX(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleStartRandomPos.x),
				XMVectorGetY(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleStartRandomPos.y),
				XMVectorGetZ(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleStartRandomPos.z), 1.f);

		//맨처음 들어오는 y값을 미리 저장해둠
		m_pFirstPos[i].x = pVertices[i].vPosition.x;
		m_pFirstPos[i].y = pVertices[i].vPosition.y;
		m_pFirstPos[i].z = pVertices[i].vPosition.z;

		pVertices[i].vTime.x = (rand() % 9) * 0.1f + m_Desc.fCurTime;
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

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};


	if (FAILED(Compile_ShaderFiles(m_Desc.ShaderFilePath, ElementDescs, 7)))
		return E_FAIL;

	m_pRandomPos = new _float3[m_Desc.iNumInstance];
	m_pDir = new _float3[m_Desc.iNumInstance];
	m_pNormal = new _float3[m_Desc.iNumInstance];

	m_pDropSpeed = new _double[m_Desc.iNumInstance];
	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
		m_pDropSpeed[i] = rand() % 10 * 0.1f + 0.1f;

	Particle_Setting_RandomPos();

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Respawn::Render(_uint iPassIndex)
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

void CVIBuffer_PointInstance_Respawn::Update(_double TimeDelta, _uint eAxis)
{
	if ((_uint)AXIS::AXIS_END <= eAxis)
	{
		MSGBOX("Failed to Update In CVIBuffer_PointInstance_Respawn::Update");
		return;
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pDeviceContext->Map(m_pVBInstance, 0, /*D3D11_MAP_WRITE_DISCARD*/D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	if (!m_bSettingDir) 
	{
		//방향 처음에 지정해줬으면 리셋하기 전까지 그 방향으로 고정

		for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
		{
			while (0 == m_pRandomPos[i].x && 0 == m_pRandomPos[i].y && 0 == m_pRandomPos[i].z)
			{
				//만약에 세 축이 다 0이면 아닐때까지 계속돌리기
				m_pRandomPos[i].x = -(rand() % (_int)(m_Desc.fParticleMinusRandomDir.x)) + m_Desc.fParticleRandomDir.x;
				m_pRandomPos[i].y = -(rand() % (_int)(m_Desc.fParticleMinusRandomDir.y)) + m_Desc.fParticleRandomDir.y;
				m_pRandomPos[i].z = -(rand() % (_int)(m_Desc.fParticleMinusRandomDir.z)) + m_Desc.fParticleRandomDir.z;
			}

			m_pDir[i] = { m_pRandomPos[i].x - ((VTXPARTICLE*)SubResource.pData)[i].vPosition.x,
				m_pRandomPos[i].y - ((VTXPARTICLE*)SubResource.pData)[i].vPosition.y,
				m_pRandomPos[i].z - ((VTXPARTICLE*)SubResource.pData)[i].vPosition.z };

			_vector vNormal = XMVector3Normalize(XMLoadFloat3(&m_pDir[i]));
			XMStoreFloat3(&m_pNormal[i], vNormal);
		}

		m_bSettingDir = true;
	}


	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		if (!m_bReset)
		{
			
			if (m_Desc.fLifeTime > ((VTXPARTICLE*)SubResource.pData)[i].vTime.x)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vTime.x += (_float)TimeDelta;
			}

			if (m_Desc.fLifeTime < ((VTXPARTICLE*)SubResource.pData)[i].vTime.x)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vTime.x = m_Desc.fLifeTime;
			}

			if (m_Desc.bGravity) //중력값 줬을때만 ,,
			{
				_float fY = 0.f;
				m_fGravityTime = m_Desc.fLifeTime - ((VTXPARTICLE*)SubResource.pData)[i].vTime.x;

				if (m_Desc.fLifeTime > m_fGravityTime && 0.f <= m_fGravityTime)
				{
					fY = ((VTXPARTICLE*)SubResource.pData)[i].vPosition.y + (-2.f * 9.8f * m_pDropSpeed[i] * (_float)TimeDelta * ((m_Desc.fLifeTime - m_fGravityTime) * (m_Desc.fParticleSpeed * 0.1f)));
					((VTXPARTICLE*)SubResource.pData)[i].vPosition.y = fY;
				}
			}

			//리셋일때는 돌지않게
			if ((_uint)AXIS::AXIS_X == eAxis)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.x = 0;
			}
			else if ((_uint)AXIS::AXIS_Y == eAxis)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.y = 0;
			}
			else if ((_uint)AXIS::AXIS_Z == eAxis)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.z = 0;
			}

			if ((_uint)AXIS::AXIS_X != eAxis)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.x += m_pNormal[i].x * (_float)TimeDelta * m_Desc.fParticleSpeed;
			}
			if ((_uint)AXIS::AXIS_Y != eAxis)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.y += m_pNormal[i].y * (_float)TimeDelta * m_Desc.fParticleSpeed;
			}
			if ((_uint)AXIS::AXIS_Z != eAxis)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.z += m_pNormal[i].z * (_float)TimeDelta * m_Desc.fParticleSpeed;
			}

			if ((_uint)AXIS::AXIS_ALL == eAxis)
			{
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.x += m_pNormal[i].x * (_float)TimeDelta * m_Desc.fParticleSpeed;
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.y += m_pNormal[i].y * (_float)TimeDelta * m_Desc.fParticleSpeed;
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.z += m_pNormal[i].z * (_float)TimeDelta * m_Desc.fParticleSpeed;
				((VTXPARTICLE*)SubResource.pData)[i].vPosition.w = 1.f;
			}
		}

		//처음 지정해준 높이 보다 낮아지면 다시위로올림 
		if (m_pFirstPos[i].y - 20.f >= ((VTXPARTICLE*)SubResource.pData)[i].vPosition.y)
		{
			((VTXPARTICLE*)SubResource.pData)[i].vPosition.x = m_pFirstPos[i].x + rand() % (_int)(m_Desc.fParticleStartRandomPos.x);
			((VTXPARTICLE*)SubResource.pData)[i].vPosition.y = m_pFirstPos[i].y;
			((VTXPARTICLE*)SubResource.pData)[i].vPosition.z = m_pFirstPos[i].z + rand() % (_int)(m_Desc.fParticleStartRandomPos.z);
		}
	}

	/*if (0 > m_fCamLookDir.z)
	{
		sort(((VTXPARTICLE*)SubResource.pData), ((VTXPARTICLE*)SubResource.pData) + m_Desc.iNumInstance, [](VTXPARTICLE pSour, VTXPARTICLE pDest)
			{
				return pSour.vPosition.z > pDest.vPosition.z;
			});
	}
	else if (0 <= m_fCamLookDir.z)
	{
		sort(((VTXPARTICLE*)SubResource.pData), ((VTXPARTICLE*)SubResource.pData) + m_Desc.iNumInstance, [](VTXPARTICLE pSour, VTXPARTICLE pDest)
			{
				return pSour.vPosition.z < pDest.vPosition.z;
			});
	}*/

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_PointInstance_Respawn::Particle_Setting_RandomPos()
{
	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		m_pRandomPos[i] = { -(rand() % (_int)(m_Desc.fParticleMinusRandomDir.x)) + m_Desc.fParticleRandomDir.x,
			-(rand() % (_int)(m_Desc.fParticleMinusRandomDir.y)) + m_Desc.fParticleRandomDir.y,
			-(rand() % (_int)(m_Desc.fParticleMinusRandomDir.z)) + m_Desc.fParticleRandomDir.z };

		while (0 == m_pRandomPos[i].x && 0 == m_pRandomPos[i].y && 0 == m_pRandomPos[i].z)
		{
			m_pRandomPos[i].x = -(rand() % (_int)(m_Desc.fParticleMinusRandomDir.x)) + m_Desc.fParticleRandomDir.x;
			m_pRandomPos[i].y = -(rand() % (_int)(m_Desc.fParticleMinusRandomDir.y)) + m_Desc.fParticleRandomDir.y;
			m_pRandomPos[i].z = -(rand() % (_int)(m_Desc.fParticleMinusRandomDir.z)) + m_Desc.fParticleRandomDir.z;
		}
	}
}

void CVIBuffer_PointInstance_Respawn::Particle_Reset()
{
	m_bSettingDir = false;
	m_fGravityTime = 0.f;
	//리셋될때마다 새롭게 벡터세팅 
	Particle_Setting_RandomPos();

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_Desc.iNumInstance; ++i)
	{
		((VTXPARTICLE*)SubResource.pData)[i].vTime.x = (rand() % 9) * 0.1f + m_Desc.fCurTime;

		m_Desc.matParticle.r[0] = XMVectorSetX(m_Desc.matParticle.r[0], m_Desc.fParticleSize.x);
		m_Desc.matParticle.r[1] = XMVectorSetY(m_Desc.matParticle.r[1], m_Desc.fParticleSize.y);

		XMStoreFloat4(&((VTXPARTICLE*)SubResource.pData)[i].vRight, m_Desc.matParticle.r[0]);
		XMStoreFloat4(&((VTXPARTICLE*)SubResource.pData)[i].vUp, m_Desc.matParticle.r[1]);
		XMStoreFloat4(&((VTXPARTICLE*)SubResource.pData)[i].vLook, m_Desc.matParticle.r[2]);
		((VTXPARTICLE*)SubResource.pData)[i].vPosition.x = XMVectorGetX(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleStartRandomPos.x);
		((VTXPARTICLE*)SubResource.pData)[i].vPosition.y = XMVectorGetY(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleStartRandomPos.y);
		((VTXPARTICLE*)SubResource.pData)[i].vPosition.z = XMVectorGetZ(m_Desc.matParticle.r[3]) + rand() % (_int)(m_Desc.fParticleStartRandomPos.z);

		m_pFirstPos[i].x = ((VTXPARTICLE*)SubResource.pData)[i].vPosition.x;
		m_pFirstPos[i].y = ((VTXPARTICLE*)SubResource.pData)[i].vPosition.y;
		m_pFirstPos[i].z = ((VTXPARTICLE*)SubResource.pData)[i].vPosition.z;
	}

	////소팅해야댐 여기서 , 멀리있는것부터 그릴수있게,, 저는 생각이없습니다.. 
	if (0 > m_fCamLookDir.z)
	{
		sort(((VTXPARTICLE*)SubResource.pData), ((VTXPARTICLE*)SubResource.pData) + m_Desc.iNumInstance, [](VTXPARTICLE pSour, VTXPARTICLE pDest)
			{
				return pSour.vPosition.z > pDest.vPosition.z;
			});
	}
	if (0 <= m_fCamLookDir.z)
	{
		sort(((VTXPARTICLE*)SubResource.pData), ((VTXPARTICLE*)SubResource.pData) + m_Desc.iNumInstance, [](VTXPARTICLE pSour, VTXPARTICLE pDest)
			{
				return pSour.vPosition.z < pDest.vPosition.z;
			});
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_PointInstance_Respawn * CVIBuffer_PointInstance_Respawn::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext/*, const _tchar* pShaderFilePath, _uint iNumInstance*/)
{
	CVIBuffer_PointInstance_Respawn*		pInstance = new CVIBuffer_PointInstance_Respawn(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(/*pShaderFilePath, iNumInstance*/)))
	{
		MSGBOX("Failed to Creating CVIBuffer_PointInstance_Respawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_PointInstance_Respawn::Clone(void * pArg)
{
	CVIBuffer_PointInstance_Respawn*		pInstance = new CVIBuffer_PointInstance_Respawn(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_PointInstance_Respawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_PointInstance_Respawn::Free()
{
	Safe_Delete_Array(m_pDir);
	Safe_Delete_Array(m_pNormal);
	Safe_Delete_Array(m_pRandomPos);
	Safe_Delete_Array(m_pVertices);
	Safe_Delete_Array(m_pIndices);
	Safe_Delete_Array(m_pDropSpeed);
	Safe_Delete_Array(m_pFirstPos);

	Safe_Release(m_pVBInstance);

	for (auto& pEffectDesc : m_EffectDescs)
	{
		Safe_Release(pEffectDesc->pInputLayout);
		Safe_Delete(pEffectDesc);
	}
	m_EffectDescs.clear();

	Safe_Release(m_pEffect);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}
