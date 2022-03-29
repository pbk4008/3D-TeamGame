#include "PostProcess.h"
#include "GameInstance.h"
#include "VIBuffer_RectViewPort.h"
#include "Tonemapping.h"

CPostProcess::CPostProcess(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
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

HRESULT CPostProcess::PossProcessing(CTonemapping* tone,CTarget_Manager* pTargetMgr, _bool hdr, _bool shadow, _bool particle)
{
	if (FAILED(ComputeBrightPass(pTargetMgr, L"Target_Diffuse", 640.f, 360.f))) return E_FAIL;

	if (FAILED(BlurPass(pTargetMgr, L"Target_BrightPass", L"Target_VT2", L"Target_HZ2", 640, 360))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_HZ2", L"Target_VT4", L"Target_HZ4", 320, 180))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_HZ4", L"Target_VT8", L"Target_HZ8", 160, 90))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_HZ8", L"Target_VT16", L"Target_HZ16", 64, 64))) return E_FAIL;

	if (FAILED(BloomPass(pTargetMgr))) return E_FAIL;

	if (FAILED(tone->ToneMapping(pTargetMgr))) return E_FAIL;

	if (FAILED(BlurPass(pTargetMgr, L"Target_Emission", L"Target_Vertical2", L"Target_Horizontal2", 640, 360))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_Horizontal2", L"Target_Vertical4", L"Target_Horizontal4", 320, 180))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_Horizontal4", L"Target_Vertical8", L"Target_Horizontal8", 160, 90))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_Horizontal8", L"Target_Vertical16", L"Target_Horizontal16", 64, 64))) return E_FAIL;

	//-----------------------------------------//
	if (particle == true)
	{
		if (FAILED(BlurPass(pTargetMgr, L"Target_Particle", L"Target_ParticleV2", L"Target_ParticleH2", 640, 360))) return E_FAIL;
		if (FAILED(BlurPass(pTargetMgr, L"Target_ParticleH2", L"Target_ParticleV4", L"Target_ParticleH4", 320, 180))) return E_FAIL;
		if (FAILED(BlurPass(pTargetMgr, L"Target_ParticleH4", L"Target_ParticleV8", L"Target_ParticleH8", 160, 90))) return E_FAIL;
		if (FAILED(BlurPass(pTargetMgr, L"Target_ParticleH8", L"Target_ParticleV16", L"Target_ParticleH16", 64, 64))) return E_FAIL;
	}

	if (FAILED(tone->Blend_FinalPass(pTargetMgr, hdr, shadow, particle))) return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::ComputeBrightPass(CTarget_Manager* pTargetMgr, const wstring& in, _float sizex, _float sizey)
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
	m_pVIBuffer->Render(2);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::BloomPass(CTarget_Manager* pTargetMgr)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("Target_Bloom"))))	return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur2Texture", pTargetMgr->Get_SRV(L"Target_HZ2"))))	return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur4Texture", pTargetMgr->Get_SRV(L"Target_HZ4"))))	return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur8Texture", pTargetMgr->Get_SRV(L"Target_HZ8"))))	return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur16Texture", pTargetMgr->Get_SRV(L"Target_HZ16")))) return E_FAIL;

	m_pVIBuffer->Render(3);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::BlurPass(CTarget_Manager* pTargetMgr, const wstring& in, const wstring& middle, const wstring& out, _float sizex, _float sizey)
{
	if (FAILED(GaussianBlurVertical(pTargetMgr, in, middle, sizex, sizey))) return E_FAIL;

	if (FAILED(GaussianblurHorizontal(pTargetMgr, middle, out, sizex, sizey))) return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::GaussianBlurVertical(CTarget_Manager* pTargetMgr, const wstring& in, const wstring& bind, _float sizex, _float sizey)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, bind.c_str())))	return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(in.c_str())))) return E_FAIL;

	_float2 pixel = { 0.5f / sizex , 0.5f / sizey };
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_RtperPixel", &pixel, sizeof(_float2)))) return E_FAIL;

	m_pVIBuffer->Buffer_Resize(sizex, sizey);
	m_pVIBuffer->Render(0);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))	return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::GaussianblurHorizontal(CTarget_Manager* pTargetMgr, const wstring& in, const wstring& bind, _float sizex, _float sizey)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, bind.c_str())))	return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(in.c_str())))) return E_FAIL;

	_float2 pixel = { 0.5f / sizex , 0.5f / sizey };
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_RtperPixel", &pixel, sizeof(_float2)))) return E_FAIL;

	m_pVIBuffer->Buffer_Resize(sizex, sizey);
	m_pVIBuffer->Render(1);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))	return E_FAIL;

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