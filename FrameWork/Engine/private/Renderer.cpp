#include "..\public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_RectViewPort.h"
#include "GameInstance.h"
#include "Transform.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
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
	}
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	//if (nullptr == pTargetMgr)
	//	return E_FAIL;

	_uint		iViewportIndex = 1;
	D3D11_VIEWPORT		ViewportDesc;

	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);
	m_pDeviceContext->RSGetViewports(&iViewportIndex, &ViewportDesc);

	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Diffuse"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Normal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(1.f, 1.f, 1.f, 1.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Depth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Position"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f), CRenderTarget::RTT::DEFFURED)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shadow"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWMAP)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_ShadeShadow"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f), CRenderTarget::RTT::SHADOWDEPTH)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shade"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Specular"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.0f, 0.0f, 0.0f, 0.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	// PBR RenderTargets MRT
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Metallic"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Roughness"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(1.f, 1.f, 1.f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_AO"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f), CRenderTarget::RTT::LIGHTING)))
		return E_FAIL;

	// 멀티 랜더 타겟 Deferred
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Position"))))
		return E_FAIL;

	// 단일 랜더 타겟 ShadowMap
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_Shadow"))))
		return E_FAIL;
	// 단일 랜더 타겟 ShadowDepth
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_ShaeShadow"), TEXT("Target_ShadeShadow"))))
		return E_FAIL;

	// 멀티 랜더 타겟 Lighting
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	// 멀티 랜더 타겟 PBR
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_Metallic"))))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_Roughness"))))
		return E_FAIL;
	if (FAILED(pTargetMgr->Add_MRT(TEXT("MRT_PBR"), TEXT("Target_AO"))))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, TEXT("../../Reference/ShaderFile/Shader_RectViewPort.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Diffuse"), 0, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Normal"), 100.f, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Depth"), 200.f, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Position"), 300, 0, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Shadow"), 400.f, 0, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_ShadeShadow"), 500.f, 0, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Shade"), 600.f, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Specular"), 700.f, 0, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Metallic"), 1180.f, 0, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_Roughness"), 1180.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(pTargetMgr->Ready_Debug_Buffer(TEXT("Target_AO"), 1180.f, 200, 100.f, 100.f)))
		return E_FAIL;
#endif // _DEBUG

	RELEASE_INSTANCE(CTarget_Manager);
	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void* pArg)
{
	return S_OK;
}

ID3D11ShaderResourceView* CRenderer::Get_SRV(const wstring& pTargetTag)
{
	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);
	ID3D11ShaderResourceView* SRV = pTargetMgr->Get_SRV(pTargetTag);
	RELEASE_INSTANCE(CTarget_Manager);

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

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG
	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);

	if (FAILED(pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Shadow"))))
		return E_FAIL;

	if (FAILED(pTargetMgr->Render_Debug_Buffer(TEXT("MRT_ShaeShadow"))))
		return E_FAIL;

	if (FAILED(pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Deferred"))))
		return E_FAIL;

	if (FAILED(pTargetMgr->Render_Debug_Buffer(TEXT("MRT_PBR"))))
		return E_FAIL;

	if (FAILED(pTargetMgr->Render_Debug_Buffer(TEXT("MRT_LightAcc"))))
		return E_FAIL;
	RELEASE_INSTANCE(CTarget_Manager);
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
	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);

	if (m_bPBRHDR)
	{
		if (FAILED(Render_PBR())) // Ready PBR  
			return E_FAIL;
	}

	if (m_bShadow)
	{
		if (FAILED(Render_Shadow())) // 그림자 맵
			return E_FAIL;

		if (FAILED(Render_ShadeShadow())) // 그림자 뎁스 비교
			return E_FAIL;
	}

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Deferred"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();
		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_NONALPHA].clear();

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	RELEASE_INSTANCE(CTarget_Manager);
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

HRESULT CRenderer::Render_Shadow()
{
	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Shadow"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Shadow();
	}

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	RELEASE_INSTANCE(CTarget_Manager);
	return S_OK;
}

HRESULT CRenderer::Render_ShadeShadow()
{
	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_ShaeShadow"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadeShadow(pTargetMgr->Get_SRV(L"Target_Shadow"));
	}

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	RELEASE_INSTANCE(CTarget_Manager);
	return S_OK;
}

HRESULT CRenderer::Render_PBR()
{
	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);

	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_PBR"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_PBR();
	}

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	RELEASE_INSTANCE(CTarget_Manager);
	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);
	CLight_Manager* pLight_Manager = GET_INSTANCE(CLight_Manager);

	/*  Target_Shader를 장치에 바인드하였다. */
	pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_LightAcc"));

	pLight_Manager->SetPBRCheck(m_bPBRHDR);
	pLight_Manager->Render_Lights(m_CameraTag);

	pTargetMgr->End_MRT(m_pDeviceContext);

	RELEASE_INSTANCE(CLight_Manager);
	RELEASE_INSTANCE(CTarget_Manager);

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	CTarget_Manager* pTargetMgr = GET_INSTANCE(CTarget_Manager);

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", pTargetMgr->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_ShadeTexture", pTargetMgr->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_SpecularTexture", pTargetMgr->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;
	 
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_ShadowTexture", pTargetMgr->Get_SRV(TEXT("Target_ShadeShadow")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bShadow", &m_bShadow, sizeof(_bool))))
		return E_FAIL;


	m_pVIBuffer->Render(3);

	RELEASE_INSTANCE(CTarget_Manager);
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
}
