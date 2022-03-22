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
		
	m_pFinalBlend = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, m_viewport.Width, m_viewport.Height, TEXT("../../Reference/ShaderFile/Shader_Blend.hlsl"));
	if (nullptr == m_pFinalBlend)
		return E_FAIL; 

	return S_OK;
}

HRESULT CTonemapping::Blend_ToneMapping(CTarget_Manager* pTargetMgr, _bool check)
{
	if (FAILED(pTargetMgr->Begin_MRT(m_pDeviceContext, TEXT("Target_Blend"))))	return E_FAIL;

	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_DiffuseTexture", pTargetMgr->Get_SRV(L"Target_HDRDiffuse"))))return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_SpecularTexture", pTargetMgr->Get_SRV(L"Target_Specular"))))	return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_EmissionTexture", pTargetMgr->Get_SRV(L"Target_Emission"))))	return E_FAIL;

	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_Blur2Texture", pTargetMgr->Get_SRV(L"Target_Horizontal2"))))	return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_Blur4Texture", pTargetMgr->Get_SRV(L"Target_Horizontal4"))))	return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_Blur8Texture", pTargetMgr->Get_SRV(L"Target_Horizontal8"))))	return E_FAIL;
	if (FAILED(m_pFinalBlend->SetUp_TextureOnShader("g_Blur16Texture", pTargetMgr->Get_SRV(L"Target_Horizontal16")))) return E_FAIL;

	if (FAILED(m_pFinalBlend->SetUp_ValueOnShader("g_bhdr", &check, sizeof(_bool)))) return E_FAIL;

	m_pFinalBlend->Render(0);

	if (FAILED(pTargetMgr->End_MRT(m_pDeviceContext))) return E_FAIL;

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
	Safe_Release(m_pFinalBlend);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
