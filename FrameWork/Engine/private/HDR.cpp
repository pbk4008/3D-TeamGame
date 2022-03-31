#include "HDR.h"
#include "GameInstance.h"
#include "VIBuffer_RectViewPort.h"

CHDR::CHDR(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CHDR::InitHDR()
{
	_uint		iViewportIndex = 1;
	D3D11_VIEWPORT		ViewportDesc;

	m_pDeviceContext->RSGetViewports(&iViewportIndex, &ViewportDesc);

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, TEXT("../../Reference/ShaderFile/Shader_HDRBase.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

HRESULT CHDR::Render_HDRBase(CTarget_Manager* pTargetMgr, _bool isShadow)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("MRT_HDRBASE"))))
		return E_FAIL;

	//if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_AlphaTexture", pTargetMgr->Get_SRV(TEXT("Target_Alpha"))))) return E_FAIL;

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", pTargetMgr->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_NormalTexture", pTargetMgr->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_ShadeTexture", pTargetMgr->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_SpecularTexture", pTargetMgr->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;
	//if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bShadow", &isShadow, sizeof(_bool))))
	//	return E_FAIL;
	m_pVIBuffer->Render(0);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

CHDR* CHDR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CHDR* pinstance = new CHDR(pDevice, pDeviceContext);

	if (FAILED(pinstance->InitHDR()))
	{
		MSGBOX("Failed To Creating Luminance");
		Safe_Release(pinstance);
	}

	return pinstance;
}

void CHDR::Free()
{
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
