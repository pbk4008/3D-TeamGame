#include "RendererAssit.h"
#include "GameInstance.h"
#include "VIBuffer_RectViewPort.h"

CRendererAssit::CRendererAssit(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
	, m_pTargetMgr(CTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTargetMgr);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CRendererAssit::InitRenderAssit()
{
	if (FAILED(Setup_RenderTarget()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRendererAssit::Setup_RenderTarget()
{
	_uint		iViewportIndex = 1;
	D3D11_VIEWPORT		ViewportDesc;

	m_pDeviceContext->RSGetViewports(&iViewportIndex, &ViewportDesc);

	_uint widht = (_uint)ViewportDesc.Width;
	_uint height = (_uint)ViewportDesc.Height;

	_float fwidth = ViewportDesc.Width;
	_float fheight = ViewportDesc.Height;


	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_SkyBox"), widht, height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::RTT_END)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Diffuse"), widht, height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Normal"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(1.f, 1.f, 1.f, 1.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Depth"), widht, height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_MRA"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Emission"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;

	// Shadow Map RenderTarget
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shadow"), SHADOW_MAP, SHADOW_MAP, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWMAP)))
		return E_FAIL;

	// Shadow Shade RenderTarget
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ShadeShadow"), widht, height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_BlurShadow"), widht, height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ShadowV2"), (_uint)(fwidth * 0.5f), (_uint)(fheight * 0.5f), DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ShadowH2"), (_uint)(fwidth * 0.5f), (_uint)(fheight * 0.5f), DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ShadowV4"), (_uint)(fwidth * 0.25f), (_uint)(fheight * 0.25f), DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ShadowH4"), (_uint)(fwidth * 0.25f), (_uint)(fheight * 0.25f), DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;

	// Light RenderTargets MRT
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shade"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Specular"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_VT2"), (_uint)(fwidth * 0.5f), (_uint)(fheight * 0.5f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_HZ2"), (_uint)(fwidth * 0.5f), (_uint)(fheight * 0.5f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;																																			
																																								
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_VT4"), (_uint)(fwidth * 0.25f), (_uint)(fheight * 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;																																			
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_HZ4"), (_uint)(fwidth * 0.25f), (_uint)(fheight * 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Bloom"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	// HDRBase RenderTarget
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_HDRDiffuse"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::HDRBase)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_HDRSpecular"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::HDRBase)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Vertical2"), (_uint)(fwidth * 0.5f), (_uint)(fheight * 0.5f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Horizontal2"), (_uint)(fwidth * 0.5f), (_uint)(fheight * 0.5f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Vertical4"), (_uint)(fwidth * 0.25f), (_uint)(fheight * 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Horizontal4"), (_uint)(fwidth * 0.25f), (_uint)(fheight * 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Vertical8"), (_uint)(fwidth * 0.125f), (_uint)(fheight * 0.125f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Horizontal8"), (_uint)(fwidth * 0.125f), (_uint)(fheight * 0.125f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Vertical16"), 64, 64, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Horizontal16"), 64, 64, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	//--------------------------//
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Blend"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 0.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ToneMapDiffuse"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ToneMapSpecular"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	//----------------------------------//

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_AlphaBlend"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_AlphaWeight"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_AlphaNoBloom"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ParticleV2"), (_uint)(fwidth * 0.5f), (_uint)(fheight * 0.5f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ParticleH2"), (_uint)(fwidth * 0.5f), (_uint)(fheight * 0.5f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ParticleV4"), (_uint)(fwidth * 0.25f), (_uint)(fheight * 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ParticleH4"), (_uint)(fwidth * 0.25f), (_uint)(fheight * 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Alpha"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;


	// distortion grab target
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Distortion"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	// velocity map
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Velocity"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	// rim light
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_RimLight"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;

	//----------------------------------//


	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_SkyBox"), TEXT("Target_SkyBox"))))
		return E_FAIL;

	// ¸ÖÆ¼ Å¸°Ù Deferred
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_MRA"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Emission"))))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_RimLight"), TEXT("Target_RimLight"))))
		return E_FAIL;

	// ´ÜÀÏ Å¸°Ù ShadowMap
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_Shadow"))))
		return E_FAIL;
	// ´ÜÀÏ Å¸°Ù ShadowDepth
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_ShaeShadow"), TEXT("Target_ShadeShadow"))))
		return E_FAIL;

	// ¸ÖÆ¼ Å¸°Ù Lighting
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	// ¸ÖÆ¼ Å¸°Ù HDRBASE
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_HDRBASE"), TEXT("Target_HDRDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_HDRBASE"), TEXT("Target_HDRSpecular"))))
		return E_FAIL;

	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_LumGray"), TEXT("Target_LumGray"))))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum1"), TEXT("Target_Lum1"))))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum2"), TEXT("Target_Lum2"))))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum3"), TEXT("Target_Lum3"))))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum4"), TEXT("Target_Lum4"))))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum5"), TEXT("Target_Lum5"))))
	//	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_BrightPass"), TEXT("Target_BrightPass"))))
	//	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Bloom"), TEXT("Target_Bloom"))))
	//	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_ToneMapping"), TEXT("Target_ToneMapDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_ToneMapping"), TEXT("Target_ToneMapSpecular"))))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_VT2"), TEXT("Target_VT2"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_HZ2"), TEXT("Target_HZ2"))))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_VT4"), TEXT("Target_VT4"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_HZ4"), TEXT("Target_HZ4"))))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Bloom"), TEXT("Target_Bloom"))))
		return E_FAIL;

	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_VT8"), TEXT("Target_VT8"))))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_HZ8"), TEXT("Target_HZ8"))))	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_VT16"), TEXT("Target_VT16"))))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_HZ16"), TEXT("Target_HZ16"))))	return E_FAIL;


	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Vertical2"), TEXT("Target_Vertical2"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Horizontal2"), TEXT("Target_Horizontal2"))))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Vertical4"), TEXT("Target_Vertical4"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Horizontal4"), TEXT("Target_Horizontal4"))))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Vertical8"), TEXT("Target_Vertical8"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Horizontal8"), TEXT("Target_Horizontal8"))))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Vertical16"), TEXT("Target_Vertical16"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Horizontal16"), TEXT("Target_Horizontal16"))))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Blend"), TEXT("Target_Blend"))))	return E_FAIL;

	//-----------------------------------//
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_AlphaBlend"), TEXT("Target_AlphaBlend"))))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_AlphaBlend"), TEXT("Target_AlphaWeight"))))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_AlphaNoBloom"), TEXT("Target_AlphaNoBloom"))))	return E_FAIL;
	
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Alpha"), TEXT("Target_Alpha"))))			return E_FAIL;


	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_ParticleV2"), TEXT("Target_ParticleV2"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_ParticleH2"), TEXT("Target_ParticleH2"))))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_ParticleV4"), TEXT("Target_ParticleV4"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_ParticleH4"), TEXT("Target_ParticleH4"))))	return E_FAIL;

	//------------------------------------//
	
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_BlurShadow"), TEXT("Target_BlurShadow"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_ShadowV2"), TEXT("Target_ShadowV2"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_ShadowH2"), TEXT("Target_ShadowH2"))))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_ShadowV4"), TEXT("Target_ShadowV4"))))	return E_FAIL;	
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_ShadowH4"), TEXT("Target_ShadowH4"))))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Distortion"), TEXT("Target_Distortion"))))			return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("Target_Velocity"), TEXT("Target_Velocity"))))		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_SkyBox"), (fwidth - fwidth), (fheight - fheight) + 300.f, 100.f, 100.f)))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Diffuse"), (fwidth - fwidth), (fheight - fheight) + 300.f, 100.f, 100.f)))			return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Normal"), (fwidth - fwidth) + 100.f, (fheight - fheight) + 300.f, 100.f, 100.f)))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Depth"), (fwidth - fwidth) + 200.f, (fheight - fheight) + 300.f, 100.f, 100.f)))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_MRA"), (fwidth - fwidth) +300.f, (fheight - fheight) + 300.f, 100.f, 100.f)))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Emission"), (fwidth - fwidth) + 400.f, (fheight - fheight) + 300.f, 100.f, 100.f)))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_RimLight"), (fwidth - fwidth) + 300.f, (fheight - fheight) + 400.f, 100.f, 100.f)))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Shadow"), (fwidth - fwidth), (fheight - fheight) + 400.f, 100.f, 100.f)))				return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_ShadeShadow"), (fwidth - fwidth) + 100.f, (fheight - fheight) + 400.f, 100.f, 100.f)))	return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_BlurShadow"), (fwidth - fwidth) + 200.f, (fheight - fheight) + 400.f, 100.f, 100.f)))		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Shade"), (fwidth - fwidth) + 400, (fheight - fheight) + 400.f, 100.f, 100.f)))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Specular"), (fwidth - fwidth) + 500.f, (fheight - fheight) + 400.f, 100.f, 100.f)))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HDRDiffuse"), fwidth - 400.f, fheight - 500.f, 100.f, 100.f)))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HDRSpecular"), fwidth - 300.f, fheight - 500.f, 100.f, 100.f)))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_ToneMapDiffuse"), fwidth - 200.f, fheight - 500.f, 100.f, 100.f))) 	return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_ToneMapSpecular"), fwidth - 100.f, fheight - 500.f, 100.f, 100.f)))	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum1"), fwidth - (fwidth), fheight - 100.f, 100.f, 100.f)))			return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum2"), fwidth - (fwidth - 100.f), fheight - 100.f, 100.f, 100.f)))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum3"), fwidth - (fwidth - 200.f), fheight - 100.f, 100.f, 100.f)))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum4"), fwidth - (fwidth - 300.f), fheight - 100.f, 100.f, 100.f)))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum5"), fwidth - (fwidth - 400.f), fheight - 100.f, 100.f, 100.f)))	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_BrightPass"), fwidth - (fwidth - 500), fheight - 100.f, 100.f, 100.f)))	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_VT2"), fwidth - (fwidth), fheight - 300.f, 100.f, 100.f)))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HZ2"), fwidth - (fwidth), fheight - 200.f, 100.f, 100.f)))	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_VT4"), fwidth - (fwidth - 100.f), fheight - 300.f, 100.f, 100.f)))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HZ4"), fwidth - (fwidth - 100.f), fheight - 200.f, 100.f, 100.f)))	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_VT8"), fwidth - (fwidth - 200.f), fheight - 300.f, 100.f, 100.f)))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HZ8"), fwidth - (fwidth - 200.f), fheight - 200.f, 100.f, 100.f)))	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_VT16"), fwidth - (fwidth - 300.f), fheight - 300.f, 100.f, 100.f)))	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HZ16"), fwidth - (fwidth - 300.f), fheight - 200.f, 100.f, 100.f)))	return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Bloom"), fwidth - (fwidth - 400.f), fheight - 200.f, 100.f, 100.f)))	return E_FAIL;

	////-----------------------------------------------------------------------------------------------------------------------------------//
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Vertical2"), fwidth - 400.f, fheight - 400.f, 100.f, 100.f)))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Horizontal2"), fwidth - 400.f, fheight - 300.f, 100.f, 100.f)))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Vertical4"), fwidth - 300.f, fheight - 400.f, 100.f, 100.f)))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Horizontal4"), fwidth - 300.f, fheight - 300.f, 100.f, 100.f)))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Vertical8"), fwidth - 200.f, fheight - 400.f, 100.f, 100.f)))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Horizontal8"), fwidth - 200.f, fheight - 300.f, 100.f, 100.f)))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Vertical16"), fwidth - 100.f, fheight - 400.f, 100.f, 100.f)))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Horizontal16"), fwidth - 100.f, fheight - 300.f, 100.f, 100.f)))	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Blend"), fwidth - 100.f, fheight - 200.f, 100.f, 100.f)))					return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Bloom"), fwidth - 200.f, fheight - 200.f, 100.f, 100.f)))					return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Alpha"), 0.f, fheight - 400.f, 100.f, 100.f)))					return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_AlphaBlend"), 100.f, fheight - 400.f, 100.f, 100.f)))				return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_AlphaWeight"), 200.f, fheight - 400.f, 100.f, 100.f)))				return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_AlphaNoBloom"), 300.f, fheight - 400.f, 100.f, 100.f)))			return E_FAIL;
	
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Distortion"), 400.f, (fheight - fheight) + 400.f, 100.f, 100.f)))		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Velocity"), 500.f, (fheight - fheight) + 300.f, 100.f, 100.f)))			return E_FAIL;

#endif // _DEBUG

	return S_OK;
}

HRESULT CRendererAssit::Render_LightAcc(CTarget_Manager* pTarget_Manager, const wstring& cameraTag, _bool bPBR,_bool shadow,_bool halfview)
{
	if (nullptr == m_pTargetMgr)
		return E_FAIL;

	CLight_Manager* pLight_Manager = GET_INSTANCE(CLight_Manager);

	m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_LightAcc"));

	pLight_Manager->Render_Lights(pTarget_Manager,cameraTag, bPBR,shadow, halfview);

	//m_pTargetMgr->End_MRT(m_pDeviceContext);
	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	RELEASE_INSTANCE(CLight_Manager);

	return S_OK;
}

HRESULT CRendererAssit::Render_VolumetricLightAcc(CTarget_Manager* pTarget_Manager, const wstring& cameraTag)
{
	CLight_Manager* pLight_Manager = GET_INSTANCE(CLight_Manager);
	
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, L"Target_GodRay"))) MSGBOX("Failed to Begin MRT TargetGodRay");

	pLight_Manager->Render_VolumetricLights(pTarget_Manager, cameraTag);

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) MSGBOX("Failed to End MRTNotClear TargetGodRay");

	RELEASE_INSTANCE(CLight_Manager);

	return S_OK;
}

CRendererAssit* CRendererAssit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CRendererAssit* pinstance = new CRendererAssit(pDevice, pDeviceContext);

	if (FAILED(pinstance->InitRenderAssit()))
	{
		MSGBOX("Failed To Create RenderAssit");
		Safe_Release(pinstance);
	}

	return pinstance;
}

void CRendererAssit::Free()
{
	Safe_Release(m_pTargetMgr);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
