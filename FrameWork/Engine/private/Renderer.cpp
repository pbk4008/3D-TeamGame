#include "Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_RectViewPort.h"
#include "GameInstance.h"
#include "Transform.h"
#include "RendererAssit.h"
#include "Luminance.h"
#include "HDR.h"
#include "PostProcess.h"
#include "Tonemapping.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pTargetMgr(CTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTargetMgr);
}

void CRenderer::SetRenderButton(RENDERBUTTON ebutton, _bool check)
{
	switch (ebutton)
	{
	case CRenderer::SHADOW:
		m_bRenderbtn[SHADOW] = check;
		break;
	case CRenderer::PBR:
		m_bRenderbtn[PBR] = check;
		break;
	case CRenderer::PIXEL:
		m_bRenderbtn[PIXEL] = check;
		break;
	case CRenderer::HDR:
		m_bRenderbtn[HDR] = check;
		break;
	case CRenderer::DBG:
		m_bRenderbtn[DBG] = check;
		break;
	case CRenderer::PARTICLE:
		m_bRenderbtn[PARTICLE] = check;
		break;
	case CRenderer::OUTLINE:
		m_bRenderbtn[OUTLINE] = check;
		break;
	case CRenderer::RADIAL:
		m_bRenderbtn[RADIAL] = check;
		break;
	case CRenderer::DISTORTION:
		m_bRenderbtn[DISTORTION] = check;
		break;
	case CRenderer::FOG:
		m_bRenderbtn[FOG] = check;
		break;
	case CRenderer::VELOCITYBLUR:
		m_bRenderbtn[VELOCITYBLUR] = check;
		break;
	case CRenderer::MOTIONTRAIL:
		m_bRenderbtn[MOTIONTRAIL] = check;
		break;
	}
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	_uint		iViewportIndex = 1;
	D3D11_VIEWPORT		ViewportDesc;

	m_pDeviceContext->RSGetViewports(&iViewportIndex, &ViewportDesc);

	if (FAILED(CreateShadowDepthStencilview(SHADOW_MAP, SHADOW_MAP, &m_pShadowMap)))
		return E_FAIL;

	m_pRenderAssit = CRendererAssit::Create(m_pDevice, m_pDeviceContext);
	m_pLuminance = CLuminance::Create(m_pDevice, m_pDeviceContext);
	m_pHDR = CHDR::Create(m_pDevice, m_pDeviceContext);
	m_pPostProcess = CPostProcess::Create(m_pDevice, m_pDeviceContext);
	m_pTonemapping = CTonemapping::Create(m_pDevice, m_pDeviceContext);

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, TEXT("../../Reference/ShaderFile/Shader_RectViewPort.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	for (_int i = 0; i < RENDERBUTTON_END; ++i)
		m_bRenderbtn[i] = false;

	lstrcpy(m_CameraTag, L"MainCamera");

#pragma region PhysX그리기용
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t			ShaderByteCodeLength = 0;
	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		E_FAIL;

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);
#pragma endregion

	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void* pArg)
{
	return S_OK;
}

HRESULT CRenderer::CreateShadowDepthStencilview(_uint iWidth, _uint iHeight,ID3D11DepthStencilView** ppDepthStencilView)
{
	ID3D11Texture2D* pDepthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, ppDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER eRenderID, CGameObject* pGameObject)
{
	if (nullptr == pGameObject || eRenderID >= RENDER_MAX)
		return E_FAIL;

	m_RenderGroup[eRenderID].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	m_pTargetMgr->All_Clear(m_pDeviceContext);

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_SkyBox())) MSGBOX("Failed To Rendering SkyPass");

	if (m_bRenderbtn[SHADOW] == true)
	{
		if (FAILED(Render_Shadow())) MSGBOX("Failed To Rendering ShadowMapPass");
	}

	if (FAILED(Render_NonAlpha())) MSGBOX("Failed To Rendering NonAlphaPaas");

	if (m_bRenderbtn[PIXEL]) // Pixel HDR
	{
		if (FAILED(MotionTrailPass())) MSGBOX("Failed To Rendering Motiontrail");

		if (m_bRenderbtn[SHADOW] == true)
		{
			if (FAILED(ShadowPass())) MSGBOX("Failed To Rendering ShadowPass");
		}

		if (FAILED(m_pRenderAssit->Render_LightAcc(m_pTargetMgr, m_CameraTag, m_bRenderbtn[PBR], m_bRenderbtn[SHADOW]))) MSGBOX("Failed To Rendering LightPass");

		if (FAILED(DistortionPass())) MSGBOX("Failed To Rendering Distortion");

		if (FAILED(VeloCityPass())) MSGBOX("Failed To Rendering Velocity");

		if (FAILED(Render_Alpha()))	MSGBOX("Failed To Rendering AlphaPass");

		if (FAILED(Render_AlphaNoBloom()))	MSGBOX("Failed To Rendering AlphaNoBloomPass");

		if (FAILED(m_pHDR->Render_HDRBase(m_pTargetMgr, m_bRenderbtn[SHADOW]))) MSGBOX("Failed To Rendering HDRBasePass");

		if (FAILED(m_pLuminance->DownSampling(m_pTargetMgr)))MSGBOX("Failed To Rendering DownSamplingPass");

		if (FAILED(m_pPostProcess->PossProcessing(m_pTonemapping, m_pTargetMgr, m_bRenderbtn[HDR]))) MSGBOX("Failed To Rendering PostProcessPass");

		if (FAILED(Render_Final())) MSGBOX("Failed To Rendering FinalPass");
	}

	if (FAILED(Render_UI_Active()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_PhysX()))
		return E_FAIL;
	
#ifdef _DEBUG
	if (m_bRenderbtn[DBG] == false)
	{
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_SkyBox"))))		return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Shadow"))))		return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_ShaeShadow"))))	return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Deferred"))))	return E_FAIL;
		
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_LightAcc"))))	return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_HDRBASE"))))		return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_ToneMapping"))))	return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum1"))))	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum2"))))	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum3"))))	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum4"))))	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum5"))))	return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_VT2")))) return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_HZ2")))) return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_VT4")))) return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_HZ4")))) return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_VT8")))) return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_HZ8")))) return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_VT16")))) return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_HZ16")))) return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Bloom")))) return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Vertical2")))) return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Horizontal2")))) return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Vertical4")))) return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Horizontal4")))) return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Vertical8")))) return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Horizontal8")))) return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Vertical16")))) return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Horizontal16")))) return E_FAIL;

		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Blend")))) return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Final")))) return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Alpha")))) return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_AlphaBlend")))) return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_BlurShadow")))) return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Distortion")))) return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_Velocity")))) return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_RimLight")))) return E_FAIL;
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("Target_AlphaNoBloom")))) return E_FAIL;
	}
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::Remove_RenderGroup()
{
	for (_uint i = 0; i < RENDER_MAX; i++)
	{
		for (auto pObj : m_RenderGroup[i])
			Safe_Release(pObj);

		m_RenderGroup[i].clear();
	}

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderGroup[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_SkyBox()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_SkyBox"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_SKYBOX])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_SKYBOX].clear();

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Deferred"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_NONALPHA].clear();

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;
	
	return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("Target_AlphaBlend")))) return E_FAIL;

	AlphaSorting(RENDER_ALPHA);

	for (auto& pGameObject : m_RenderGroup[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
		{
			m_bRenderbtn[PARTICLE] = true;
			pGameObject->Render();
		}

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_ALPHA].clear();

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_AlphaNoBloom()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("Target_AlphaNoBloom")))) return E_FAIL;

	AlphaSorting(RENDER_ALPHANB);

	for (auto& pGameObject : m_RenderGroup[RENDER_ALPHANB])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_ALPHANB].clear();

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::VeloCityPass()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("Target_Velocity"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_VELOCITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Velocity();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_VELOCITY].clear();

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::DistortionPass()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("Target_Distortion"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_DYDISTORTION])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_DYDISTORTION].clear();

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::MotionTrailPass()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("Target_RimLight"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_MOTIONTRAIL])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_MotionTrail();
		
		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_MOTIONTRAIL].clear();

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	_matrix view = g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	view = XMMatrixInverse(nullptr, view);

	AlphaSorting(RENDER_UI);

	for (auto& pGameObject : m_RenderGroup[RENDER_UI] )
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_UI].clear();


	return S_OK;
}

HRESULT CRenderer::Render_UI_Active()
{
	_matrix view = g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	view = XMMatrixInverse(nullptr, view);

	AlphaSorting(RENDER_UI_ACTIVE);

	for (auto& pGameObject : m_RenderGroup[RENDER_UI_ACTIVE])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_UI_ACTIVE].clear();
	return S_OK;
}

HRESULT CRenderer::Render_Shadow()
{
	if (FAILED(m_pTargetMgr->Begin_RT(m_pDeviceContext, L"MRT_Shadow", m_pShadowMap)))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_SHADOW])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Shadow();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_SHADOW].clear();

	if (FAILED(m_pTargetMgr->End_RT(m_pDeviceContext, m_pShadowMap)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::ShadowPass()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_ShaeShadow"))))	return E_FAIL;

	
	const LIGHTDESC* lightdesc = g_pGameInstance->Get_LightDesc(0);

	_matrix		lightviewproj = lightdesc->mLightView * lightdesc->mLightProj;
	_matrix		ViewMatrix = g_pGameInstance->Get_Transform(m_CameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	_matrix		ProjMatrix = g_pGameInstance->Get_Transform(m_CameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DepthTexture", m_pTargetMgr->Get_SRV(TEXT("Target_Depth"))))) MSGBOX("Failed To Apply ShadowPass DepthTexture");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_ShadowMapTex", m_pTargetMgr->Get_SRV(TEXT("Target_Shadow"))))) MSGBOX("Failed To Apply ShadowPass ShadowMapTexture");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_LightViewProj", &XMMatrixTranspose(lightviewproj), sizeof(_matrix)))) MSGBOX("Failed To Apply ShadowPass LightViewProj");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrixInv", &XMMatrixTranspose(ViewMatrix), sizeof(_float4x4)))) MSGBOX("Failed To Apply ShadowPass CamViewInv");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrixInv", &XMMatrixTranspose(ProjMatrix), sizeof(_float4x4)))) MSGBOX("Failed To Apply ShadowPass CamProjInv");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_vLightPos", &_float4(lightdesc->vPosition.x, lightdesc->vPosition.y, lightdesc->vPosition.z, 1.f), sizeof(_float4)))) MSGBOX("Failed To Apply ShadowPass LightPos");

	if (FAILED(m_pVIBuffer->Render(5))) MSGBOX("Failed To Rendering ShadowPass");

	if (FAILED(m_pTargetMgr->End_MRTNotClear(m_pDeviceContext))) return E_FAIL;

	if (FAILED(m_pPostProcess->Shadowblur(m_pTargetMgr, m_bRenderbtn[SHADOW], 1.f))) MSGBOX("Failed To Rendering ShadowBlurPass");

	return S_OK;
}

HRESULT CRenderer::Render_Final()
{
	if (!m_pTargetMgr)	return E_FAIL;

	_float thick = 0.2f;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTargetMgr->Get_SRV(TEXT("Target_Blend"))))) MSGBOX("Render Final DiffuseTeuxtre Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DepthTexture", m_pTargetMgr->Get_SRV(TEXT("Target_Depth"))))) MSGBOX("Render Final DepthTexture Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_RimLightTexture", m_pTargetMgr->Get_SRV(TEXT("Target_RimLight"))))) MSGBOX("Render Final DepthTexture Not Apply");

	if (m_bRenderbtn[VELOCITYBLUR] == true)
	{
		if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_VelocityTex", m_pTargetMgr->Get_SRV(TEXT("Target_Velocity"))))) MSGBOX("Render Final DiffuseTeuxtre Not Apply");
	}

	if (m_bRenderbtn[DISTORTION] == true)
	{
		if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DistortionTex", m_pTargetMgr->Get_SRV(TEXT("Target_Distortion"))))) MSGBOX("Render Final g_DistortionTex Not Apply");
	}

	if (g_pGameInstance->getCurrentLevel() == 4)
	{
		m_bRenderbtn[FOG] = true;
		_float4 fogcolor = _float4(0.8f, 0.8f, 0.8f, 1.f);
		_float fogstart = 20.f;
		_float fogdensity = 0.01f;
		_float4 campos;
		XMStoreFloat4(&campos,g_pGameInstance->Get_CamPosition(m_CameraTag));
		_matrix		ViewMatrix = g_pGameInstance->Get_Transform(m_CameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
		_matrix		ProjMatrix = g_pGameInstance->Get_Transform(m_CameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);
		ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
		ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_fogcolor", &fogcolor, sizeof(_float4)))) MSGBOX("Render Final Value fogcolor Not Apply");
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_fogstart", &fogstart, sizeof(_float)))) MSGBOX("Render Final Value fogdist Not Apply");
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_fogDenstiy", &fogdensity, sizeof(_float)))) MSGBOX("Render Final fogdensity thick Not Apply");
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_vCamPosition", &campos, sizeof(_float4)))) MSGBOX("Render Final fogdensity thick Not Apply");
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_fogType", &m_bfogtype, sizeof(_bool)))) MSGBOX("Render Final fogdensity thick Not Apply");
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrixInv", &XMMatrixTranspose(ViewMatrix), sizeof(_float4x4)))) MSGBOX("Failed To Apply LightRender ViewInvers");
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrixInv", &XMMatrixTranspose(ProjMatrix), sizeof(_float4x4)))) MSGBOX("Failed To Apply LightRender ProjInvers");
		if (m_bfogtype == true)
		{
			_float fogfalloff = 0.1f;
			if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_fogfalloff", &fogfalloff, sizeof(_float)))) MSGBOX("Render Final fogdensity thick Not Apply");
		}
	}

	_int cnt = 32;
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_outline", &m_bRenderbtn[OUTLINE], sizeof(_bool)))) MSGBOX("Render Final Value outline Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_radial", &m_bRenderbtn[RADIAL], sizeof(_bool)))) MSGBOX("Render Final Value raidal Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_distort", &m_bRenderbtn[DISTORTION], sizeof(_bool)))) MSGBOX("Render Final Value distort Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_fog", &m_bRenderbtn[FOG], sizeof(_bool)))) MSGBOX("Render Final Value g_fog Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_motion", &m_bRenderbtn[VELOCITYBLUR], sizeof(_bool)))) MSGBOX("Render Final Value distort Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_RadialCnt", &m_RadialCnt, sizeof(_int)))) MSGBOX("Render Final Value RaidalCnt Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_MotionblurCnt", &cnt, sizeof(_int)))) MSGBOX("Render Final Value RaidalCnt Not Apply");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_thick", &thick, sizeof(_float)))) MSGBOX("Render Final Value thick Not Apply");
	
	if (FAILED(m_pVIBuffer->Render(3))) MSGBOX("Final Rendering Failed");

	if (m_bRenderbtn[PARTICLE] == true)
	{
		if (FAILED(m_pPostProcess->AlphaBlur(m_pTargetMgr, m_bRenderbtn[PARTICLE]))) MSGBOX("Alpha Blur Failed");
		if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_AlphaTexture", m_pTargetMgr->Get_SRV(L"Target_Alpha")))) MSGBOX("Alpha Render Failed");
		if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_AlphaNBTexture", m_pTargetMgr->Get_SRV(L"Target_AlphaNoBloom")))) MSGBOX("Alpha Render Failed");

		if (FAILED(m_pVIBuffer->Render(4))) MSGBOX("Alpha Rendering Failed");
	}
	return S_OK;
}

HRESULT CRenderer::Render_PhysX()
{
	if (g_pGameInstance->getkeyDown(DIK_LCONTROL))
		m_isPhysXRender = !m_isPhysXRender;

	if (!m_isPhysXRender)
		return S_OK;

	CPhysicsXSystem* pPhysXSystem = GET_INSTANCE(CPhysicsXSystem);
	const PxRenderBuffer& rb = pPhysXSystem->Get_RenderBuffer();
	RELEASE_INSTANCE(CPhysicsXSystem);


	_float4 vColor = { 1.f, 0.f, 0.f, 1.f };

	m_pEffect->SetView(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	m_pEffect->SetProjection(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	PxU32 iNBLines = rb.getNbLines();
	for (PxU32 i = 0; i < iNBLines; ++i)
	{
		const PxDebugLine line = rb.getLines()[i];
		_float3 pos0 = FromPxVec3(line.pos0);
		_float3 pos1 = FromPxVec3(line.pos1);
		_vector vPos0 = XMLoadFloat4(&_float4(pos0.x, pos0.y, pos0.z, 1.f));
		_vector vPos1 = XMLoadFloat4(&_float4(pos1.x, pos1.y, pos1.z, 1.f));

		DX::DrawTriangle(m_pBatch, vPos0, vPos1, vPos0, XMLoadFloat4(&vColor));
	}

	//PxU32 iNBTriangles = rb.getNbTriangles();
	//for (PxU32 i = 0; i < iNBLines; ++i)
	//{
	//	const PxDebugTriangle triangle = rb.getTriangles()[i];
	//	_float3 pos0 = FromPxVec3(triangle.pos0);
	//	_float3 pos1 = FromPxVec3(triangle.pos1);
	//	_float3 pos2 = FromPxVec3(triangle.pos2);
	//	_vector vPos0 = XMLoadFloat4(&_float4(pos0.x, pos0.y, pos0.z, 1.f));
	//	_vector vPos1 = XMLoadFloat4(&_float4(pos1.x, pos1.y, pos1.z, 1.f));
	//	_vector vPos2 = XMLoadFloat4(&_float4(pos2.x, pos2.y, pos2.z, 1.f));

	//	DX::DrawTriangle(m_pBatch, vPos0, vPos1, vPos2, XMLoadFloat4(&vColor));
	//}

	m_pBatch->End();

	return S_OK;
}

HRESULT CRenderer::AlphaSorting(RENDER etype)
{
	_matrix view = g_pGameInstance->Get_Transform(m_CameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	view = XMMatrixInverse(nullptr, view);

	for (auto& iter : m_RenderGroup[etype])
		iter->ComputeViewZ(&view);

	m_RenderGroup[etype].sort(
		[](auto& psrc, auto& pdst)->bool
		{
			return psrc->Get_ViewZ() > pdst->Get_ViewZ();
		});

	return S_OK;
}


CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRenderer::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

#pragma region PhysX 그리기용
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pBatch);
	Safe_Release(m_pInputLayout);
#pragma endregion

	for (_uint i = 0; i < RENDER_MAX; ++i)
	{
		for (auto& pGameObject : m_RenderGroup[i])
			Safe_Release(pGameObject);

		m_RenderGroup[i].clear();
	}

	Safe_Release(m_pShadowMap);
	Safe_Release(m_pHDR);
	Safe_Release(m_pLuminance);
	Safe_Release(m_pPostProcess);
	Safe_Release(m_pTonemapping);
	Safe_Release(m_pRenderAssit);
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pTargetMgr);
}
