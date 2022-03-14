#include "..\public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_RectViewPort.h"
#include "GameInstance.h"
#include "Transform.h"

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
	case Engine::CRenderer::SHADOW: 
		m_bShadow = check;
		break;
	case Engine::CRenderer::PBRHDR:
		m_bPBRHDR = check;
		break;
	case Engine::CRenderer::BLUR:
		m_bBlur = check;
		break;
	case Engine::CRenderer::DEFERRED:
		m_bDeferred = check;
		break;
	}
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	if (FAILED(SetUp_RenderTarget()))
		return E_FAIL;

	lstrcpy(m_CameraTag, L"MainCamera");

	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void* pArg)
{
	return S_OK;
}

ID3D11ShaderResourceView* CRenderer::Get_SRV(const wstring& pTargetTag)
{

	ID3D11ShaderResourceView* SRV = m_pTargetMgr->Get_SRV(pTargetTag);
	

	return SRV;
}

HRESULT CRenderer::Add_RenderGroup(RENDER eRenderID, CGameObject* pGameObject)
{
	if (nullptr == pGameObject || eRenderID >= RENDER_END)
		return E_FAIL;

	m_RenderGroup[eRenderID].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha())) // 디퍼드 단계
		return E_FAIL;

	if (FAILED(Render_LightAcc())) // 빛연산
		return E_FAIL;

	if (FAILED(Render_Blend())) // 최종 결합
		return E_FAIL;

	if (FAILED(Render_PostProcessing()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG
	if (m_bDeferred)
	{
		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Shadow"))))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_ShaeShadow"))))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Deferred"))))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_PBR"))))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_LightAcc"))))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Blend"))))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->Render_Debug_Buffer_CSTarget(L"CSDownScale")))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->Render_Debug_Buffer_CSTarget(L"CSGSBlur")))
			return E_FAIL;
		
	}
#endif // _DEBUG

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

HRESULT CRenderer::Render_NonAlpha()
{
	if (m_bShadow)
	{
		if (FAILED(Render_Shadow())) // 그림자 맵
			return E_FAIL;

		if (FAILED(Render_ShadeShadow()))
			return E_FAIL;
	}

	if (m_bPBRHDR)
	{
		if (FAILED(Render_PBR()))
			return E_FAIL;
	}

	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Deferred"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_NONALPHA].clear();

	if (FAILED(m_pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
	for (auto& pGameObject : m_RenderGroup[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_ALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	/*if (!m_RenderGroup[RENDER_UI].empty())
	{
		sort(m_RenderGroup[RENDER_UI].begin(), m_RenderGroup[RENDER_UI].end(), [](CGameObject* pSour, CGameObject* pDest)
			{
				CTransform* pSourTrans = (CTransform*)pSour->Get_Component(L"Transform");
				CTransform* pDestTrans = (CTransform*)pDest->Get_Component(L"Transform");

				float sour = XMVectorGetZ(pSourTrans->Get_State(CTransform::STATE_POSITION));
				float dest = XMVectorGetZ(pDestTrans->Get_State(CTransform::STATE_POSITION));

				return  sour > dest;
			});
	}*/

	for (auto& pGameObject : m_RenderGroup[RENDER_UI])
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
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Shadow"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Shadow();
	}

	if (FAILED(m_pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CRenderer::Render_ShadeShadow()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_ShaeShadow"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadeShadow(m_pTargetMgr->Get_SRV(L"Target_Shadow"));
	}

	if (FAILED(m_pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CRenderer::Render_PBR()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_PBR"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_PBR();
	}

	if (FAILED(m_pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{

	CLight_Manager* pLight_Manager = GET_INSTANCE(CLight_Manager);

	/*  Target_Shader를 장치에 바인드하였다. */
	m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_LightAcc"));

	pLight_Manager->SetPBRCheck(m_bPBRHDR);
	pLight_Manager->Render_Lights(m_CameraTag);

	m_pTargetMgr->End_MRT(m_pDeviceContext);

	RELEASE_INSTANCE(CLight_Manager);
	

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Blend"))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTargetMgr->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_ShadeTexture", m_pTargetMgr->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_SpecularTexture", m_pTargetMgr->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;
	 
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_ShadowTexture", m_pTargetMgr->Get_SRV(TEXT("Target_ShadeShadow")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bShadow", &m_bShadow, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bPBRHDR", &m_bPBRHDR, sizeof(_bool))))
		return E_FAIL;

	m_pVIBuffer->Render(3);

	if (FAILED(m_pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CRenderer::Render_PostProcessing()
{
	// post processing
	if (m_bBlur)
	{
		if (FAILED(m_pTargetMgr->RunComputeShader(L"CSDownScale", m_pTargetMgr->Get_SRV(L"Target_Specular"), CCSTarget::CSType::DOWNSAMPLE)))
			return E_FAIL;

		if (FAILED(m_pTargetMgr->RunComputeShader(L"CSGSBlur", m_pTargetMgr->Get_SRVCS(L"CSDownScale"), CCSTarget::CSType::GSBLUR)))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BlurTexture", m_pTargetMgr->Get_SRVCS(L"CSDownScale"))))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bBlur", &m_bBlur, sizeof(_bool))))
			return E_FAIL;

	}

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_BlendTexture", m_pTargetMgr->Get_SRV(L"Target_Blend"))))
		return E_FAIL;

	m_pVIBuffer->Render(4);

	return S_OK;
}

HRESULT CRenderer::SetUp_RenderTarget()
{
	_uint		iViewportIndex = 1;
	D3D11_VIEWPORT		ViewportDesc;


	m_pDeviceContext->RSGetViewports(&iViewportIndex, &ViewportDesc);

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Diffuse"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Normal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(1.f, 1.f, 1.f, 1.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Depth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Position"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shadow"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWMAP)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ShadeShadow"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shade"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Specular"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 0.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	// Blend RenderTarget
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Blend"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 0.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	// PBR RenderTargets
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Metallic"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Roughness"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(1.f, 1.f, 1.f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_AO"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	// Compute Shader
	if (FAILED(m_pTargetMgr->Add_CSTarget(m_pDevice, m_pDeviceContext, L"CSDownScale", L"../../Reference/ShaderFile/Shader_DownSample.hlsl", "CS_Main", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f), CCSTarget::CSType::DOWNSAMPLE)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_CSTarget(m_pDevice, m_pDeviceContext, L"CSGSBlur", L"../../Reference/ShaderFile/Shader_CSGSBlur.hlsl", "Blur", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f), CCSTarget::CSType::GSBLUR)))
		return E_FAIL;

	// 멀티 랜더 타겟 Deferred
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Position"))))
		return E_FAIL;

	// 단일 랜더 타겟 ShadowMap
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_Shadow"))))
		return E_FAIL;
	// 단일 랜더 타겟 ShadowDepth
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_ShaeShadow"), TEXT("Target_ShadeShadow"))))
		return E_FAIL;

	// 멀티 랜더 타겟 Lighting
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	// 멀티 랜더 타겟 PBR
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_Metallic"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_Roughness"))))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_AO"))))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Add_MRT(TEXT("MRT_Blend"), TEXT("Target_Blend"))))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, TEXT("../../Reference/ShaderFile/Shader_RectViewPort.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Diffuse"), 0, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Normal"), 100.f, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Depth"), 200.f, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Position"), 300, 0, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Shadow"), 400.f, 0, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_ShadeShadow"), 500.f, 0, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Shade"), 600.f, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Specular"), 700.f, 0, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Metallic"), 1180.f, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Roughness"), 1180.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_AO"), 1180.f, 200, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Blend"), 0.f, 520, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer_CSTarget(L"CSDownScale", 0.f, 620, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(m_pTargetMgr->Ready_Debug_Buffer_CSTarget(L"CSGSBlur", 100.f, 620, 100.f, 100.f)))
		return E_FAIL;
#endif // _DEBUG

	

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

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderGroup[i])
			Safe_Release(pGameObject);

		m_RenderGroup[i].clear();
	}
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pTargetMgr);
}
