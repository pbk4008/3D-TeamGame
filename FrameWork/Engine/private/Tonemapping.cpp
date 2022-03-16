#include "Tonemapping.h"
#include "GameInstance.h"
#include "VIBuffer_RectViewPort.h"

CTonemapping::CTonemapping(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CTonemapping::InitToneMapping()
{
	_uint		iViewportIndex = 1;
	ZeroMemory(&m_viewport, sizeof(m_viewport));

	m_pDeviceContext->RSGetViewports(&iViewportIndex, &m_viewport);

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, m_viewport.Width, m_viewport.Height, TEXT("../Bin/ShaderFiles/Shader_Tonemapping.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pFinalBlend = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, m_viewport.Width, m_viewport.Height, TEXT("../Bin/ShaderFiles/Shader_Blend.hlsl"));
	if (nullptr == m_pFinalBlend)
		return E_FAIL; 

	return S_OK;
}

HRESULT CTonemapping::Render_HDR(CTarget_Manager* pTargetMgr)
{
	if (FAILED(UpSampling(pTargetMgr)))
		return E_FAIL;

	if (FAILED(ToneMapping(pTargetMgr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTonemapping::Blend_HDR(CTarget_Manager* pTargetMgr)
{
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_DiffuseTexture", pTargetMgr->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_ShadeTexture", pTargetMgr->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_ShadowTexture", pTargetMgr->Get_SRV(TEXT("Target_ShadeShadow")))))
		return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_SpecularTexture", pTargetMgr->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;

	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_ToneTexture", pTargetMgr->Get_SRV(TEXT("Target_ToneMapDiffuse")))))
		return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_ToneSepcular", pTargetMgr->Get_SRV(TEXT("Target_ToneMapSpecular")))))
		return E_FAIL;

	if (FAILED(m_pFinalBlend->SetUp_ValueOnShader("g_bHDR", &m_bHDR, sizeof(_bool))))
		return E_FAIL;

	m_pFinalBlend->Render(0);

	return S_OK;
}

HRESULT CTonemapping::UpSampling(CTarget_Manager* pTargetMgr)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_BloomUpSample"))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BloomTexture", pTargetMgr->Get_SRV(L"Target_VerticalBlur"))))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTonemapping::ToneMapping(CTarget_Manager* pTargetMgr)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_ToneMapping"))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_HDRDiffuseTexture", pTargetMgr->Get_SRV(L"Target_HDRDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_HDRSpecularTexture", pTargetMgr->Get_SRV(L"Target_HDRSpecular"))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_LuminanceTexture", pTargetMgr->Get_SRV(L"Target_Lum5"))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BloomTexture", pTargetMgr->Get_SRV(L"Target_BloomUpSample"))))
		return E_FAIL;

	_float rcp_w = (1.f / m_viewport.Width);
	_float rcp_h = (1.f / m_viewport.Height);
	_float expsure = 1.0f;
	_float GaussianScalar = 1.f;

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("Exposure", &expsure, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("GaussianScalar", &GaussianScalar, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("rcp_bloom_tex_w", &rcp_w, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("rcp_bloom_tex_h", &rcp_h, sizeof(_float))))
		return E_FAIL;

	m_pVIBuffer->Render(1);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

CTonemapping* CTonemapping::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTonemapping* pInstance = new CTonemapping(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitToneMapping()))
	{
		MSGBOX("Failed To Creating PostProcess");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTonemapping::Free()
{
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pFinalBlend);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
