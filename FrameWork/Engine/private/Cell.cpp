#include "..\public\Cell.h"
#include "VIBuffer_Triangle.h"
#include "PipeLine.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CCell::NativeConstruct(_float3 * pPoints, _uint iIndex)
{
	m_iIndex = iIndex;

	ZeroMemory(m_pNeighbor, sizeof(CCell*) * LINE_END);

	memcpy(m_vPoint, pPoints, sizeof(_float3) * POINT_END);

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoint[POINT_B]) - XMLoadFloat3(&m_vPoint[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoint[POINT_C]) - XMLoadFloat3(&m_vPoint[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoint[POINT_A]) - XMLoadFloat3(&m_vPoint[POINT_C]));

	m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, m_vLine[LINE_AB].y, m_vLine[LINE_AB].x);
	m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, m_vLine[LINE_BC].y, m_vLine[LINE_BC].x);
	m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, m_vLine[LINE_CA].y, m_vLine[LINE_CA].x);

	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vNormal = XMVector3Normalize(XMLoadFloat3(&m_vNormal[i]));
		XMStoreFloat3(&m_vNormal[i], vNormal);
	}

#ifdef _DEBUG
	if (FAILED(Ready_DebugBuffer()))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

_bool CCell::Compare(_fvector SourPoint, _fvector DestPoint)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_A]), SourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_B]), DestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_C]), DestPoint))
			return true;		
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_B]), SourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_C]), DestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_A]), DestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_C]), SourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_A]), DestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_B]), DestPoint))
			return true;
	}
		
	return false;
}

_bool CCell::isIn(_fvector vPosition, CCell** ppOutNeighbor)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vDir = vPosition - XMLoadFloat3(&m_vPoint[i]);

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMLoadFloat3(&m_vNormal[i]))))
		{
			*ppOutNeighbor = m_pNeighbor[i];

			return false;

		}
	}

	return true;
}

#ifdef _DEBUG
HRESULT CCell::Ready_DebugBuffer()
{
	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pDevice, m_pDeviceContext, TEXT("../../Reference/ShaderFile/Shader_Triangle.hlsl"), m_vPoint);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;


	return S_OK;
}
HRESULT CCell::Render(_fmatrix WorldMatrix, _uint iCurrentIndex, const wstring& pCameraTag)
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pVIBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_float4x4));
	m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(pPipeLine->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW)), sizeof(_float4x4));
	m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(pPipeLine->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION)), sizeof(_float4x4));

	if (iCurrentIndex == m_iIndex)	
		m_pVIBuffer->SetUp_ValueOnShader("g_vCellColor", &XMVectorSet(1.f, 0.f, 0.f, 1.f), sizeof(_float4));	
	else
		m_pVIBuffer->SetUp_ValueOnShader("g_vCellColor", &XMVectorSet(0.f, 1.f, 0.f, 1.f), sizeof(_float4));

	m_pVIBuffer->Render(0);

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}
#endif // _DEBUG

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _float3 * pPoints, _uint iIndex)
{
	CCell*		pInstance = new CCell(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pPoints, iIndex)))
	{
		MSGBOX("Failed to Creating CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif // _DEBUG

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

}
