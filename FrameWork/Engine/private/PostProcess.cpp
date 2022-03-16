#include "PostProcess.h"
#include "GameInstance.h"
#include "VIBuffer_RectViewPort.h"

CPostProcess::CPostProcess(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

float CPostProcess::ComputeGaussianValue(_float x, _float mean, _float std_deviation)
{
	// The Gaussian equation is defined as such:
	/*
	-(x - mean)^2
	-------------
	1.0               2*std_dev^2
	f(x,mean,std_dev) = -------------------- * e^
	sqrt(2*pi*std_dev^2)

	*/

	return (1.0f / sqrt(2.0f * XM_PI * std_deviation * std_deviation))
		* expf((-((x - mean) * (x - mean))) / (2.0f * std_deviation * std_deviation));
}

HRESULT CPostProcess::InitPostProcess()
{
	_uint		iViewportIndex = 1;
	ZeroMemory(&m_viewport, sizeof(m_viewport));

	m_pDeviceContext->RSGetViewports(&iViewportIndex, &m_viewport);

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, m_viewport.Width, m_viewport.Height, TEXT("../../Reference/ShaderFile/Shader_PsotProcess.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::PostProcessing(CTarget_Manager* pTargetMgr)
{
	if (FAILED(DownSampling(pTargetMgr)))
		return E_FAIL;

	if (FAILED(HorizontalBlur(pTargetMgr)))
		return E_FAIL;

	if (FAILED(VerticalBlur(pTargetMgr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::DownSampling(CTarget_Manager* pTargetMgr)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_BrightPass"))))
		return E_FAIL;

	_float2 vTexUV = { 1.f / m_viewport.Width, 1.f / m_viewport.Height };

	_vector	offset[4];
	ZeroMemory(offset, sizeof(_vector) * 4);
	offset[0] = { -0.5f * vTexUV.x , 0.5f * vTexUV.y , 0, 0 };
	offset[1] = { 0.5f * vTexUV.x , 0.5f * vTexUV.y , 0, 0 };
	offset[2] = { -0.5f * vTexUV.x , -0.5f * vTexUV.y , 0, 0 };
	offset[3] = { 0.5f * vTexUV.x , -0.5f * vTexUV.y , 0, 0 };

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_BrightPassOffset", offset, sizeof(_vector) * 4)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_BrightPassThreshold", &m_BrightThreshold, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_HDRDiffuse"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(640.f, 360.f);
	m_pVIBuffer->Render(0);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(m_viewport.Width, m_viewport.Height);
	//---------------------------------------------------------------------//

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_BrightPassDS"))))
		return E_FAIL;

	_vector DSOffset[16];
	ZeroMemory(DSOffset, sizeof(_vector) * 16);
	_int idx1 = 0;
	for (_int i = -2; i < 2; i++)
	{
		for (_int j = -2; j < 2; j++)
		{
			//DSOffset[idx1++] = { ((_float)i + 0.5f) * (1.f / 640.f)
			//						,((_float)j + 0.5f) * (1.f / 360.f)
			//						, 0.f , 0.f };
			DSOffset[idx1++] = { (((_float)i + 0.5f) / 640.f),(((_float)j + 0.5f) / 320.f), 0.f , 0.f };
		}
	}
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_DSOffset", DSOffset, sizeof(_vector) * 16)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_BrightPass"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(320.f,180.f);
	m_pVIBuffer->Render(1);
	
	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(m_viewport.Width, m_viewport.Height);

	return S_OK;
}

HRESULT CPostProcess::HorizontalBlur(CTarget_Manager* pTargetMgr)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_HorizontalBlur"))))
		return E_FAIL;

	_float HBloomOffset[9];
	ZeroMemory(HBloomOffset, sizeof(_float) * 9);
	_float HBloomWeight[9];
	ZeroMemory(HBloomWeight, sizeof(_float) * 9);

	for (int i = 0; i < 9; i++)
	{
		//HBloomOffset[i] = ((_float)i - 4.f) * (1.f / (320.f));
		HBloomOffset[i] = ((_float)i - 4.f) / (320.f);
		_float x = ((_float)i - 4.f) / 4.f;

		HBloomWeight[i] = m_GaussMultipliter * ComputeGaussianValue(x, m_GaussMean, m_GaussStdDev);
	}

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_HBloomOffset", HBloomOffset, sizeof(_float) * 9)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_HBloomWeight", HBloomWeight, sizeof(_float) * 9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_BrightPassDS"))))
		return E_FAIL;
	
	m_pVIBuffer->Buffer_Resize(320.f, 180.f);
	m_pVIBuffer->Render(2);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(m_viewport.Width, m_viewport.Height);
	return S_OK;
}

HRESULT CPostProcess::VerticalBlur(CTarget_Manager* pTargetMgr)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_VerticalBlur"))))
		return E_FAIL;

	_float VBloomOffset[9];
	ZeroMemory(VBloomOffset, sizeof(_float) * 9);
	_float VBloomWeight[9];
	ZeroMemory(VBloomOffset, sizeof(_float) * 9);

	for (int i = 0; i < 9; i++)
	{
		//VBloomOffset[i] = ((_float)i - 4.f) * (1.f / (180.f));
		VBloomOffset[i] = ((_float)i - 4.f) / (180.f);
		_float x = ((_float)i - 4.f) / 4.f;

		VBloomWeight[i] = m_GaussMultipliter * ComputeGaussianValue(x, m_GaussMean, m_GaussStdDev);
	}

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_VBloomOffset", VBloomOffset, sizeof(_float) * 9)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_VBloomWeight", VBloomWeight, sizeof(_float) * 9)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(L"Target_HorizontalBlur"))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(320.f, 180.f);
	m_pVIBuffer->Render(3);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(m_viewport.Width, m_viewport.Height);
	return S_OK;
}

CPostProcess* CPostProcess::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CPostProcess* pInstance = new CPostProcess(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitPostProcess()))
	{
		MSGBOX("Failed To Creating PostProcess");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPostProcess::Free()
{
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}