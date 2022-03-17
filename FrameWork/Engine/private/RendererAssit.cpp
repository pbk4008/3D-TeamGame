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

#ifdef _DEBUG
	_float fwidth = ViewportDesc.Width;
	_float fheight = ViewportDesc.Height;
#endif // _DEBUG

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, TEXT("../../Reference/ShaderFile/Shader_RectViewPort.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Diffuse"), widht, height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f,0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Normal"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(1.f, 1.f, 1.f, 1.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Depth"), widht, height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Position"), widht, height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;

	// Shadow Map RenderTarget
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shadow"), SHADOW_MAP, SHADOW_MAP, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWMAP)))
		return E_FAIL;

	// Shadow Shade RenderTarget
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ShadeShadow"), widht, height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;

	// Light RenderTargets MRT
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shade"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 0.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Specular"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 0.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	// PBR RenderTargets MRT
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Metallic"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::PBR)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Roughness"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(1.f, 1.f, 1.f, 0.f), CRenderTarget::RTT::PBR)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_AO"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::PBR	)))
		return E_FAIL;

	// HDRBase RenderTarget
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_HDRDiffuse"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::HDRBase)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_HDRSpecular"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::HDRBase)))
		return E_FAIL;

	// Luminance RenderTargets 
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_LumGray"), 243, 243, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum1"), 81,81, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum2"), 27,27, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum3"), 9,9, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum4"), 3,3, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum5"), 1,1, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_LumGrayDb"), 243, 243, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum1Db"), 81, 81, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum2Db"), 27, 27, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum3Db"), 9, 9, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum4Db"), 3, 3, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Lum5Db"), 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	// PostProcess RenderTargets
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_BrightPass"),640, 360, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_BrightPassDS"), 320, 180, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_HorizontalBlur"), 320, 180, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_VerticalBlur"), 320, 180, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	// Tone mapping
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_BloomUpSample"), widht, height , DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ToneMapDiffuse"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ToneMapSpecular"), widht, height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::Luminance)))
		return E_FAIL;

	// Final blend
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Blend"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 0.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Priority"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 0.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;


	// Compute Target
	//if (FAILED(m_pTargetMgr->Add_CSTarget(m_pDevice, m_pDeviceContext, L"CSDownScale", L"../../Reference/ShaderFile/Shader_DownSample.hlsl", "CS_Main", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f), CCSTarget::CSType::DOWNSAMPLE)))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Add_CSTarget(m_pDevice, m_pDeviceContext, L"CSGSBlur", L"../../Reference/ShaderFile/Shader_CSGSBlur.hlsl", "Blur", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f), CCSTarget::CSType::GSBLUR)))
	//	return E_FAIL;

	// ¸ÖÆ¼ Å¸°Ù Deferred
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Position"))))
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

	// ¸ÖÆ¼ Å¸°Ù PBR
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_Metallic"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_Roughness"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_AO"))))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Blend"), TEXT("Target_Blend"))))
		return E_FAIL;

	// ¸ÖÆ¼ Å¸°Ù HDRBASE
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_HDRBASE"), TEXT("Target_HDRDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_HDRBASE"), TEXT("Target_HDRSpecular"))))
		return E_FAIL;

	// ´ÜÀÏ Å¸°Ù Luminance
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_LumGray"), TEXT("Target_LumGray"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum1"), TEXT("Target_Lum1"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum2"), TEXT("Target_Lum2"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum3"), TEXT("Target_Lum3"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum4"), TEXT("Target_Lum4"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum5"), TEXT("Target_Lum5"))))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_LumGrayDb"), TEXT("Target_LumGrayDb"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum1Db"), TEXT("Target_Lum1Db"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum2Db"), TEXT("Target_Lum2Db"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum3Db"), TEXT("Target_Lum3Db"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum4Db"), TEXT("Target_Lum4Db"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Lum5Db"), TEXT("Target_Lum5Db"))))
		return E_FAIL;

	// ´ÜÀÏ Å¸°Ù Post Process
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_BrightPass"), TEXT("Target_BrightPass"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_BrightPassDS"), TEXT("Target_BrightPassDS"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_HorizontalBlur"), TEXT("Target_HorizontalBlur"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_VerticalBlur"), TEXT("Target_VerticalBlur"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_BloomUpSample"), TEXT("Target_BloomUpSample"))))
		return E_FAIL;

	// ¸ÖÆ¼ Å¸°Ù ToneMapping
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_ToneMapping"), TEXT("Target_ToneMapDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_ToneMapping"), TEXT("Target_ToneMapSpecular"))))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Priority"), TEXT("Target_Priority"))))
		return E_FAIL;



#ifdef _DEBUG
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Diffuse"), (fwidth - fwidth), (fheight - fheight), 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Normal"), (fwidth - fwidth) + 100.f, (fheight - fheight), 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Depth"), (fwidth - fwidth) + 200.f, (fheight - fheight), 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Position"), (fwidth - fwidth) + 300.f, (fheight - fheight), 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Shadow"), (fwidth - fwidth), (fheight - fheight) + 100.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_ShadeShadow"), (fwidth - fwidth) + 100.f, (fheight - fheight) + 100.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Shade"), (fwidth - fwidth) + 400.f, (fheight - fheight), 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Specular"), (fwidth - fwidth) + 500.f, (fheight - fheight), 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Metallic"), fwidth - 100.f, (fheight - fheight), 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Roughness"), fwidth - 100.f, (fheight - fheight) + 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_AO"), fwidth - 100.f, (fheight - fheight) + 200, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Blend"), (fwidth - fwidth) + 600.f, (fheight - fheight), 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HDRDiffuse"), fwidth - 400.f, fheight - 300.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HDRSpecular"), fwidth - 300.f, fheight - 300.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum1"), fwidth - 500.f, fheight - 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum2"), fwidth - 400.f, fheight - 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum3"), fwidth - 300.f, fheight - 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum4"), fwidth - 200.f, fheight - 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum5"), fwidth - 100.f, fheight - 100.f, 100.f, 100.f)))
		return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum1Db"), fwidth - 500.f, fheight - 100.f, 100.f, 100.f)))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum2Db"), fwidth - 400.f, fheight - 100.f, 100.f, 100.f)))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum3Db"), fwidth - 300.f, fheight - 100.f, 100.f, 100.f)))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum4Db"), fwidth - 200.f, fheight - 100.f, 100.f, 100.f)))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Lum5Db"), fwidth - 100.f, fheight - 100.f, 100.f, 100.f)))
	//	return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_BrightPass"), fwidth - 500.f, fheight - 200.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_BrightPassDS"), fwidth -400.f, fheight - 200.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_HorizontalBlur"), fwidth - 300.f, fheight - 200.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_VerticalBlur"), fwidth - 200.f, fheight - 200.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_BloomUpSample"), fwidth - 100.f, fheight - 200.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_ToneMapDiffuse"), fwidth - 200.f, fheight - 300.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_ToneMapSpecular"), fwidth - 100.f, fheight - 300.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Priority"), 200.f, 620.f, 100.f, 100.f)))
		return E_FAIL;

	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer_CSTarget(L"CSDownScale", 0.f, 620.f, 100.f, 100.f)))
	//	return E_FAIL;
	//if (FAILED(m_pTargetMgr->Ready_Debug_Buffer_CSTarget(L"CSGSBlur", 100.f, 620.f, 100.f, 100.f)))
	//	return E_FAIL;

#endif // _DEBUG


	return S_OK;
}

HRESULT CRendererAssit::Render_LightAcc(const wstring& cameraTag, _bool bPBR)
{
	if (nullptr == m_pTargetMgr)
		return E_FAIL;

	CLight_Manager* pLight_Manager = GET_INSTANCE(CLight_Manager);

	m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_LightAcc"));

	pLight_Manager->SetPBRCheck(bPBR);
	pLight_Manager->Render_Lights(cameraTag);

	m_pTargetMgr->End_MRT(m_pDeviceContext);

	RELEASE_INSTANCE(CLight_Manager);

	return S_OK;
}

HRESULT CRendererAssit::Render_PostProcessing(_bool bblur)
{
	if (nullptr == m_pTargetMgr)
		return E_FAIL;

	// post processing
	// final pass rendering
	if (bblur)
	{
		if (FAILED(m_pTargetMgr->RunComputeShader(L"CSDownScale", m_pTargetMgr->Get_SRV(L"Target_Specular"), CCSTarget::CSType::DOWNSAMPLE)))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->RunComputeShader(L"CSGSBlur", m_pTargetMgr->Get_SRVCS(L"CSDownScale"), CCSTarget::CSType::GSBLUR)))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BlurTexture", m_pTargetMgr->Get_SRVCS(TEXT("CSDownScale")))))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bBlur", &bblur, sizeof(_bool))))
			return E_FAIL;
	}

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BlendTexture", m_pTargetMgr->Get_SRV(L"Target_Blend"))))
		return E_FAIL;

	m_pVIBuffer->Render(4);

	return S_OK;
}

HRESULT CRendererAssit::Render_BlurBloom()
{
	if (FAILED(m_pTargetMgr->RunComputeShader(L"CSDownScale", m_pTargetMgr->Get_SRV(L"Target_BrightPass"), CCSTarget::CSType::DOWNSAMPLE)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->RunComputeShader(L"CSGSBlur", m_pTargetMgr->Get_SRVCS(L"CSDownScale"), CCSTarget::CSType::GSBLUR)))
		return E_FAIL;

	//if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BlurTexture", m_pTargetMgr->Get_SRVCS(TEXT("CSGSBlur")))))
	//	return E_FAIL;
	

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
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTargetMgr);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
