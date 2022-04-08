#include "..\public\Light.h"
#include "VIBuffer_RectViewPort.h"
#include "Target_Manager.h"
#include "PipeLine.h"
#include "GameInstance.h"

CLight::CLight(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CLight::NativeConstruct(const LIGHTDESC& LightDesc)
{
	m_LightDesc = LightDesc;

	D3D11_VIEWPORT		ViewportDesc;
	_uint		iNumViewports = 1;
	m_pDeviceContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_pVIBuffer = CVIBuffer_RectViewPort::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, TEXT("../../Reference/ShaderFile/Shader_RectViewPort.hlsl"));
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

void CLight::Tick()
{
}

HRESULT CLight::Render(CTarget_Manager* pTarget_Manager,const wstring& pCameraTag, _bool PBRHDRcheck, _bool Shadow)
{
	if (m_pVIBuffer != nullptr && true == m_bShow)
	{
		_uint		iPassIndex = 0;

		if (m_LightDesc.eType == tagLightDesc::TYPE_DIRECTIONAL)
		{
			iPassIndex = 1;
			if (FAILED(Render_Directional(pTarget_Manager, pCameraTag, PBRHDRcheck, Shadow, iPassIndex))) MSGBOX("Failed To Rendering Direction Light");
		}
		else if (m_LightDesc.eType == tagLightDesc::TYPE_POINT)
		{
			iPassIndex = 2;
			if(FAILED(Render_PointLight(pTarget_Manager, pCameraTag, PBRHDRcheck, iPassIndex))) MSGBOX("Failed To Rendering Point Light")
		}
	}

	return S_OK;
}

HRESULT CLight::RenderVolumetric(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag)
{
	if (m_pVIBuffer != nullptr)
	{
		_uint		iPassIndex = 0;

		if (m_LightDesc.eType == tagLightDesc::TYPE_DIRECTIONAL)
		{
			iPassIndex = 5;
		
			m_pVIBuffer->SetUp_ValueOnShader("g_vLightDir", &_float4(m_LightDesc.vDirection.x, m_LightDesc.vDirection.y, m_LightDesc.vDirection.z, 0.f), sizeof(_float4));
		}
		else if (m_LightDesc.eType == tagLightDesc::TYPE_POINT)
		{
			iPassIndex = 6;
			m_pVIBuffer->SetUp_ValueOnShader("g_fRange", &m_LightDesc.fRange, sizeof(_float));
		}

		m_pVIBuffer->SetUp_TextureOnShader("g_ShadowTexture", pTarget_Manager->Get_SRV(TEXT("Target_ShadeShadow")));
		if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DepthTexture", pTarget_Manager->Get_SRV(TEXT("Target_Depth"))))) MSGBOX("Failed To Apply DetphTex VolumetricAcc");

		_matrix		ViewMatrix = g_pGameInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
		_matrix		ProjMatrix = g_pGameInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);
		_matrix		shadowmatrix = XMMatrixInverse(nullptr, ViewMatrix * (m_LightDesc.mLightView * m_LightDesc.mLightProj));

		ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
		ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

		m_pVIBuffer->SetUp_ValueOnShader("g_shadowmatrix", &XMMatrixTranspose(shadowmatrix), sizeof(_float4x4));
		m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrixInv", &XMMatrixTranspose(ViewMatrix), sizeof(_float4x4));
		m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrixInv", &XMMatrixTranspose(ProjMatrix), sizeof(_float4x4));

		m_pVIBuffer->Render(iPassIndex);
	}

	return S_OK;
}

HRESULT CLight::Render_Directional(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag, _bool PBRHDRcheck, _bool Shadow, _uint PassIndx)
{

	if(FAILED(Ready_PBRLighting(pTarget_Manager, pCameraTag, m_LightDesc.eType))) MSGBOX("Failed To Apply Directional ReadyPBRLight");
	if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bPBRHDR", &PBRHDRcheck, sizeof(_bool)))) MSGBOX("Failed To Apply Directional PBRCheck");
	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_shadow", &Shadow, sizeof(_bool)))) MSGBOX("Failed To Apply Directional ShadowCheck");

	if(FAILED(m_pVIBuffer->Render(PassIndx))) MSGBOX("Failed To Rendering Directional Lighting");

	return S_OK;
}

HRESULT CLight::Render_PointLight(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag, _bool PBRHDRcheck, _uint PassIndx)
{
	if(FAILED(Ready_PBRLighting(pTarget_Manager, pCameraTag, m_LightDesc.eType))) MSGBOX("Failed To Apply PointLight ReadyPBRLight");
	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_fRange", &m_LightDesc.fRange, sizeof(_float)))) MSGBOX("Failed To Apply PointLight Range");
	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_bPBRHDR", &PBRHDRcheck, sizeof(_bool)))) MSGBOX("Failed To Apply PointLight PBRCheck");
	if(FAILED(m_pVIBuffer->Render(PassIndx))) MSGBOX("Failed To Rendering Point Light");

	return S_OK;
}

HRESULT CLight::Ready_PBRLighting(CTarget_Manager* pTarget_Manager, const wstring& pCameraTag, LIGHTDESC::TYPE Type)
{
	_vector		vCamPosition = g_pGameInstance->Get_CamPosition(pCameraTag);
	_matrix		ViewMatrix = g_pGameInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	_matrix		ProjMatrix = g_pGameInstance->Get_Transform(pCameraTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);

	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_NormalTexture", pTarget_Manager->Get_SRV(TEXT("Target_Normal"))))) MSGBOX("Failed To Apply LightRender NormalTexture");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DepthTexture", pTarget_Manager->Get_SRV(TEXT("Target_Depth"))))) MSGBOX("Failed To Apply LightRender DepthTexture");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", pTarget_Manager->Get_SRV(TEXT("Target_Diffuse"))))) MSGBOX("Failed To Apply LightRender DiffuseTexture");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_MRATexture", pTarget_Manager->Get_SRV(TEXT("Target_MRA"))))) MSGBOX("Failed To Apply LightRender MetalicTexture");

	if (FAILED((m_pVIBuffer->SetUp_TextureOnShader("g_SkyBoxTexture", pTarget_Manager->Get_SRV(TEXT("Target_SkyBox")))))) MSGBOX("Failed To Apply Direction Light SkyTexture");
	if (FAILED(m_pVIBuffer->SetUp_TextureOnShader("g_ShadowTexture", pTarget_Manager->Get_SRV(TEXT("Target_BlurShadow"))))) MSGBOX("Failed To Apply LightRender ShadowTexture");

	if (Type == LIGHTDESC::TYPE::TYPE_DIRECTIONAL)
	{
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_vLightDir", &_float4(m_LightDesc.vDirection.x, m_LightDesc.vDirection.y, m_LightDesc.vDirection.z, 0.f), sizeof(_float4)))) MSGBOX("Failed To Apply Directional LightDir");
	}
	else if (Type == LIGHTDESC::TYPE::TYPE_POINT)
	{
		_float scale = m_LightDesc.fRange * 2.f;
		_matrix world =
		{
			scale,0,0,0,
			0,scale,0,0,
			0,0,scale,0,
			m_LightDesc.vPosition.x,m_LightDesc.vPosition.y,m_LightDesc.vPosition.z,1
		};
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_PointWorld", &XMMatrixTranspose(world), sizeof(_float4x4)))) MSGBOX("Failed To Apply LightRender ViewMatrix");
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(ViewMatrix), sizeof(_float4x4)))) MSGBOX("Failed To Apply LightRender ViewMatrix");
		if (FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(ProjMatrix), sizeof(_float4x4)))) MSGBOX("Failed To Apply LightRender ProjMatrix");
	}

	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_vLightPos", &_float4(m_LightDesc.vPosition.x, m_LightDesc.vPosition.y, m_LightDesc.vPosition.z, 1.f), sizeof(_float4)))) MSGBOX("Failed To Apply LightRender LightPostion");
	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4)))) MSGBOX("Failed To Apply LightRender LightDiffuse");
	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4)))) MSGBOX("Failed To Apply LightRender LightAmbient");
	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4)))) MSGBOX("Failed To Apply LightRender LightSepcualr");

	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_vCamPosition", &vCamPosition, sizeof(_float4)))) MSGBOX("Failed To Apply LightRender CamPosition");
	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrixInv", &XMMatrixTranspose(ViewMatrix), sizeof(_float4x4)))) MSGBOX("Failed To Apply LightRender ViewInvers");
	if(FAILED(m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrixInv", &XMMatrixTranspose(ProjMatrix), sizeof(_float4x4)))) MSGBOX("Failed To Apply LightRender ProjInvers");

	return S_OK;
}


void CLight::UpdateLightCam(_fvector playerpos)
{
	_vector up = { 0, 1.f, 0,0 };
	_vector lookat = playerpos;

	m_LightDesc.mOrthinfo[0] = 50.f;

	_float3 dir = _float3(-1.f, -1.f, 1.f);
	_vector vdir = XMVector3Normalize(XMLoadFloat3(&m_LightDesc.vDirection));
	XMStoreFloat3(&m_LightDesc.vPosition, (vdir * m_LightDesc.mOrthinfo[0] * -1.f) + lookat);
	m_LightDesc.mLightView = XMMatrixLookAtLH(XMLoadFloat3(&m_LightDesc.vPosition), lookat, up);
}

CLight * CLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC& LightDesc)
{
	CLight*	pInstance = new CLight(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(LightDesc)))
	{
		MSGBOX("Failed to Creating CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
