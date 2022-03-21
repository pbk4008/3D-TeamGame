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
	case Engine::CRenderer::SHADOW: 
		m_bShadow = check;
		break;
	case Engine::CRenderer::PBR:
		m_bPBR = check;
		break;
	case Engine::CRenderer::HDR:
		m_bHDR = check;
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

	if (m_bShadow)
	{
		if (FAILED(Render_Shadow()))
			return E_FAIL;

		if (FAILED(Render_ShadeShadow()))
			return E_FAIL;
	}

	if (m_bPBR)
	{
		if (FAILED(Render_PBR()))
			return E_FAIL;
	}

	if (FAILED(Render_NonAlpha())) // 디퍼드 단계
		return E_FAIL;

	if (FAILED(m_pRenderAssit->Render_LightAcc(m_CameraTag, m_bPBR))) // 빛연산
		return E_FAIL;

	if (m_bHDR)
	{
		if (FAILED(m_pHDR->Render_HDRBase(m_pTargetMgr, m_bShadow)))
			return E_FAIL;

		if (FAILED(m_pLuminance->DownSampling(m_pTargetMgr)))
			return E_FAIL;

		if (FAILED(m_pPostProcess->PostProcessing(m_pTargetMgr)))
			return E_FAIL;

		if (FAILED(m_pTonemapping->Render_HDR(m_pTargetMgr)))
			return E_FAIL;
	}

	m_pTonemapping->Set_HDR(m_bHDR);
	if (FAILED(m_pTonemapping->Blend_HDR(m_pTargetMgr)))
		return E_FAIL;
		

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_PhysX()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_UI_Active()))
		return E_FAIL;

#ifdef _DEBUG
	if (m_bDeferred)
	{
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Shadow"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_ShaeShadow"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Deferred"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_PBR"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_LightAcc"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Blend"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_HDRBASE"))))
		//	return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum1"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum2"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum3"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum4"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Lum5"))))
		//	return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_BrightPass"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_BrightPassDS"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_HorizontalBlur"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_VerticalBlur"))))
		//	return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_BloomUpSample"))))
		//	return E_FAIL;
		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_ToneMapping"))))
		//	return E_FAIL;

		//if (FAILED(m_pTargetMgr->Render_Debug_Buffer(TEXT("MRT_Priority"))))
		//	return E_FAIL;

	}
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::Remove_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; i++)
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
	if (FAILED(m_pTargetMgr->Begin_RT(m_pDeviceContext, L"MRT_Shadow", m_pShadowMap)))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_SHADOW])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Shadow();
	}

	if (FAILED(m_pTargetMgr->End_RT(m_pDeviceContext, m_pShadowMap)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_ShadeShadow()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_ShaeShadow"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_SHADOW])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadeShadow(m_pTargetMgr->Get_SRV(L"Target_Shadow"));

		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_SHADOW].clear();

	if (FAILED(m_pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_PBR()
{
	if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_PBR"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderGroup[RENDER_PBR])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_PBR();
		Safe_Release(pGameObject);
	}
	m_RenderGroup[RENDER_PBR].clear();

	if (FAILED(m_pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (nullptr == m_pTargetMgr)
		return E_FAIL;

	//if (FAILED(m_pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_Blend"))))
	//	return E_FAIL;

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
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bPBRHDR", &m_bPBR, sizeof(_bool))))
		return E_FAIL;

	m_pVIBuffer->Render(3);

	//if (FAILED(m_pTargetMgr->End_MRT(m_pDeviceContext)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_PhysX()
{
	if (g_pGameInstance->getkeyDown(DIK_RCONTROL))
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

	for (_uint i = 0; i < RENDER_END; ++i)
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

	Safe_Release(m_pTargetMgr);
	Safe_Release(m_pVIBuffer);
}
