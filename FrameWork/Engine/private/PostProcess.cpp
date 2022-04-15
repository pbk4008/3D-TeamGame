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
	if (nullptr == m_pVIBuffer)	MSGBOX("Failed To Creating PostProcess VIBuffer");

	m_pGRBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, m_viewport.Width, m_viewport.Height, TEXT("../../Reference/ShaderFile/Shader_GodRay.hlsl"));
	if (nullptr == m_pGRBuffer)	MSGBOX("Failed To Creating PostProcess GRbuffer");

	return S_OK;
}

HRESULT CPostProcess::AlphaBlur(CTarget_Manager* pTargetMgr, _bool alpha)
{
	if (alpha == true)
	{
		if (FAILED(BlurPass(pTargetMgr, L"Target_AlphaBlend", L"Target_ParticleV2", L"Target_ParticleH2", 640, 360))) return E_FAIL;
		if (FAILED(BlurPass(pTargetMgr, L"Target_ParticleH2", L"Target_ParticleV4", L"Target_ParticleH4", 320, 180))) return E_FAIL;

		if (FAILED(BloomPass(pTargetMgr,L"Target_Alpha", L"Target_AlphaBlend", L"Target_ParticleH2", L"Target_ParticleH4", 1.5f, false))) return E_FAIL;
	}

	return S_OK;
}

HRESULT CPostProcess::Shadowblur(CTarget_Manager* pTargetMgr, _bool shadow, _float weight)
{
	if (shadow == true)
	{
		if (FAILED(BlurPass(pTargetMgr, L"Target_ShadeShadow", L"Target_ShadowV2", L"Target_ShadowH2", 640, 360))) return E_FAIL;
		if (FAILED(BlurPass(pTargetMgr, L"Target_ShadowH2", L"Target_ShadowV4", L"Target_ShadowH4", 320, 180))) return E_FAIL;

		if (FAILED(BloomPass(pTargetMgr, L"Target_BlurShadow", L"Target_ShadeShadow", L"Target_ShadowH2", L"Target_ShadowH4", weight, true))) return E_FAIL;
	}

	return S_OK;
}

HRESULT CPostProcess::PossProcessing(CTonemapping* tone,CTarget_Manager* pTargetMgr, _bool hdr)
{
	if (FAILED(ComputeBrightPass(pTargetMgr, L"Target_HDRDiffuse", 640.f, 360.f))) return E_FAIL;

	if (FAILED(BlurPass(pTargetMgr, L"Target_BrightPass", L"Target_VT2", L"Target_HZ2", 640, 360))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_HZ2", L"Target_VT4", L"Target_HZ4", 320, 180))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_HZ4", L"Target_VT8", L"Target_HZ8", 160, 90))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_HZ8", L"Target_VT16", L"Target_HZ16", 64, 64))) return E_FAIL;

	if (FAILED(BloomPass(pTargetMgr,L"Target_Bloom", L"Target_HZ2", L"Target_HZ4", L"Target_HZ8", L"Target_HZ16",0.5f))) return E_FAIL;

	if (FAILED(tone->ToneMapping(pTargetMgr))) return E_FAIL;

	if (FAILED(BlurPass(pTargetMgr, L"Target_Emission", L"Target_Vertical2", L"Target_Horizontal2", 640, 360))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_Horizontal2", L"Target_Vertical4", L"Target_Horizontal4", 320, 180))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_Horizontal4", L"Target_Vertical8", L"Target_Horizontal8", 160, 90))) return E_FAIL;
	if (FAILED(BlurPass(pTargetMgr, L"Target_Horizontal8", L"Target_Vertical16", L"Target_Horizontal16", 64, 64))) return E_FAIL;

	if (FAILED(tone->Blend_FinalPass(pTargetMgr, hdr))) return E_FAIL;

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
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(in.c_str()))))
		return E_FAIL;

	m_pVIBuffer->Buffer_Resize(640.f, 360.f);
	m_pVIBuffer->Render(2);

	//if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))	return E_FAIL;
	if (FAILED(pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::BloomPass(CTarget_Manager* pTargetMgr, const wstring& target, const wstring& base1, const wstring& base2, const wstring& base3, const wstring& base4,_float weight)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, target.c_str())))	return E_FAIL;

	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float)))) MSGBOX("Not Apply BloomPass ValueOnShader Weight");

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur2Texture", pTargetMgr->Get_SRV(base1.c_str()))))	return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur4Texture", pTargetMgr->Get_SRV(base2.c_str()))))	return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur8Texture", pTargetMgr->Get_SRV(base3.c_str()))))	return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur16Texture", pTargetMgr->Get_SRV(base4.c_str())))) return E_FAIL;

	m_pVIBuffer->Render(3);

	//if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext))) return E_FAIL;
	if (FAILED(pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CPostProcess::BloomPass(CTarget_Manager* pTargetMgr, const wstring& target, const wstring& base, const wstring& base1, const wstring& base2, _float weight,_bool check)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, target.c_str())))	return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_Basetexture", pTargetMgr->Get_SRV(base.c_str()))))	return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur2Texture", pTargetMgr->Get_SRV(base1.c_str()))))	return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BaseBlur4Texture", pTargetMgr->Get_SRV(base2.c_str()))))	return E_FAIL;

	if (check == true)
	{
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float)))) MSGBOX("Not Apply BloomPass ValueOnShader Weight");
		m_pVIBuffer->Render(4);
	}
	else
	{
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_Weight", &weight, sizeof(_float)))) MSGBOX("Not Apply BloomPass ValueOnShader Weight");
		if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_WeightTexture", pTargetMgr->Get_SRV(L"Target_AlphaWeight"))))	return E_FAIL;
		m_pVIBuffer->Render(5);
	}

	if (FAILED(pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

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

	//if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))	return E_FAIL;
	if (FAILED(pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

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

	//if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))	return E_FAIL;
	if (FAILED(pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	m_pVIBuffer->Buffer_Resize(m_viewport.Width, m_viewport.Height);

	return S_OK;
}

HRESULT CPostProcess::GodRayPass(CTarget_Manager* pTargetMgr, const wstring& cmatag)
{
	const LIGHTDESC* plightdesc = g_pGameInstance->Get_LightDesc(0);

	_vector vcampos = g_pGameInstance->Get_CamPosition(cmatag);
	auto vlightpos = XMVector4Transform(XMVectorSetW(XMLoadFloat3(&plightdesc->vPosition), 1.f),
		XMMatrixTranslation(XMVectorGetX(vcampos),0.0f,XMVectorGetZ(vcampos)));
	
	
	_matrix camveiwpojs = XMMatrixMultiply(g_pGameInstance->Get_Transform(cmatag, TRANSFORMSTATEMATRIX::D3DTS_VIEW), g_pGameInstance->Get_Transform(cmatag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	_vector vlightposH = XMVector4Transform(vlightpos, camveiwpojs);
	_float4	fsspos = _float4(0,0,0,0);
	XMStoreFloat4(&fsspos, vlightposH);

	auto _sspos = _float4(0.5f * fsspos.x / fsspos.w + 0.5f, -0.5f * fsspos.y / fsspos.w + 0.5f, fsspos.z / fsspos.w, 1.0f);

	_float3	_lightpos = plightdesc->vPosition;
	_float	_raydecay = 0.825f;
	_float	_rayweight = 0.25f;
	_float	_raydensity = 0.975f;
	_float	_rayexposure = 2.0f;


	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext,L"Target_GodRay"))) MSGBOX("Failed to Begin MRT TargetGodRay");

	if(FAILED((m_pGRBuffer->SetUp_TextureOnShader("g_SkyTexture", pTargetMgr->Get_SRV(L"Target_SkyBox"))))) MSGBOX("Failed to Apply GodRay SkyTexture");

	if(FAILED(m_pGRBuffer->SetUp_ValueOnShader("g_sspos",&_sspos, sizeof(_float4)))) MSGBOX("Failed to Apply GodRay sspos");
	if(FAILED(m_pGRBuffer->SetUp_ValueOnShader("g_lightpos",&_lightpos, sizeof(_float3)))) MSGBOX("Failed to Apply GodRay lightpos");
	if(FAILED(m_pGRBuffer->SetUp_ValueOnShader("g_raydensity",&_raydensity, sizeof(_float))))MSGBOX("Failed to Apply GodRay raydensity");
	if(FAILED(m_pGRBuffer->SetUp_ValueOnShader("g_rayweight",&_rayweight, sizeof(_float))))MSGBOX("Failed to Apply GodRay rayweight");
	if(FAILED(m_pGRBuffer->SetUp_ValueOnShader("g_raydecay",&_raydecay, sizeof(_float))))MSGBOX("Failed to Apply GodRay raydecay");
	if(FAILED(m_pGRBuffer->SetUp_ValueOnShader("g_rayexposure",&_rayexposure, sizeof(_float))))MSGBOX("Failed to Apply GodRay rayexposure");

	m_pGRBuffer->Render(0);

	if (FAILED(pTargetMgr->End_MRTNotClear(m_pDeviceContext))) MSGBOX("Failed to End MRTNotClear TargetGodRay");

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
	Safe_Release(m_pGRBuffer);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}