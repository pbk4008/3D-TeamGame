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

HRESULT CLight::Render(const wstring& pCameraTag, _bool PBRHDRcheck, _bool Shadow)
{
	CTarget_Manager* pTarget_Manager = GET_INSTANCE(CTarget_Manager);
	if (m_pVIBuffer != nullptr)
	{
		_uint		iPassIndex = 0;

		if (m_LightDesc.eType == tagLightDesc::TYPE_DIRECTIONAL)
		{
			iPassIndex = 1;

			m_pVIBuffer->SetUp_ValueOnShader("g_vLightDir", &_float4(m_LightDesc.vDirection.x, m_LightDesc.vDirection.y, m_LightDesc.vDirection.z, 0.f), sizeof(_float4));
		}
		else if (m_LightDesc.eType == tagLightDesc::TYPE_POINT)
		{
			iPassIndex = 2;
			m_pVIBuffer->SetUp_ValueOnShader("g_fRange", &m_LightDesc.fRange, sizeof(_float));
		}
		m_pVIBuffer->SetUp_TextureOnShader("g_ShadowTexture", pTarget_Manager->Get_SRV(TEXT("Target_Shadow")));
		m_pVIBuffer->SetUp_ValueOnShader("g_vLightPos", &_float4(m_LightDesc.vPosition.x, m_LightDesc.vPosition.y, m_LightDesc.vPosition.z, 1.f), sizeof(_float4));

		m_pVIBuffer->SetUp_TextureOnShader("g_NormalTexture", pTarget_Manager->Get_SRV(TEXT("Target_Normal")));
		m_pVIBuffer->SetUp_TextureOnShader("g_DepthTexture", pTarget_Manager->Get_SRV(TEXT("Target_Depth")));
		m_pVIBuffer->SetUp_TextureOnShader("g_ShadowTexture", pTarget_Manager->Get_SRV(TEXT("Target_ShadeShadow")));

		m_pVIBuffer->SetUp_TextureOnShader("g_SkyBoxTexutre", pTarget_Manager->Get_SRV(TEXT("Target_SkyBox")));
		m_pVIBuffer->SetUp_TextureOnShader("g_DiffuseTexture", pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")));

		m_pVIBuffer->SetUp_TextureOnShader("g_Metallic", pTarget_Manager->Get_SRV(TEXT("Target_Metallic")));
		m_pVIBuffer->SetUp_TextureOnShader("g_Roughness", pTarget_Manager->Get_SRV(TEXT("Target_Roughness")));
		m_pVIBuffer->SetUp_TextureOnShader("g_AO", pTarget_Manager->Get_SRV(TEXT("Target_AO")));
		m_pVIBuffer->SetUp_TextureOnShader("g_BiNormal", pTarget_Manager->Get_SRV(TEXT("Target_PBRNormal")));

		m_pVIBuffer->SetUp_ValueOnShader("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4));
		m_pVIBuffer->SetUp_ValueOnShader("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4));
		m_pVIBuffer->SetUp_ValueOnShader("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4));

		_vector		vCamPosition = g_pGameInstance->Get_CamPosition(pCameraTag);

		_matrix		ViewMatrix = g_pGameInstance->Get_Transform(pCameraTag,TRANSFORMSTATEMATRIX::D3DTS_VIEW);
		ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
		_matrix		ProjMatrix = g_pGameInstance->Get_Transform(pCameraTag,TRANSFORMSTATEMATRIX::D3DTS_PROJECTION);
		ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

		m_pVIBuffer->SetUp_ValueOnShader("g_vCamPosition", &vCamPosition, sizeof(_float4));
		m_pVIBuffer->SetUp_ValueOnShader("g_ViewMatrixInv", &XMMatrixTranspose(ViewMatrix), sizeof(_float4x4));
		m_pVIBuffer->SetUp_ValueOnShader("g_ProjMatrixInv", &XMMatrixTranspose(ProjMatrix), sizeof(_float4x4));
		m_pVIBuffer->SetUp_ValueOnShader("g_bPBRHDR", &PBRHDRcheck, sizeof(_bool));
		m_pVIBuffer->SetUp_ValueOnShader("g_bShadow", &Shadow, sizeof(_bool));

		m_pVIBuffer->Render(iPassIndex);
	}
	RELEASE_INSTANCE(CTarget_Manager);

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

	//_vector eye = playerpos;
	//_vector lookat = eye + XMVectorSet(0,0,1,0);
	//_vector up = { 0, 1.f, 0,0 };

	//m_LightDesc.mLightView = XMMatrixLookAtLH(eye, lookat, up);
	// 
	//m_LightDesc.mOrthinfo[0] = 10.f;
	//m_LightDesc.mLightProj = XMMatrixOrthographicLH(75.f, 75.f, 1.f, 300.f);
	//XMStoreFloat3(&m_LightDesc.vPosition, ((XMVector3Normalize(XMLoadFloat3(&m_LightDesc.vDirection)) * m_LightDesc.mOrthinfo[0] * -1.f) + lookat));
	//m_LightDesc.mLightView = XMMatrixLookAtLH(XMLoadFloat3(&m_LightDesc.vPosition), lookat, up);

	//_vector origin = { 0,0,0,0 };
	//_float3	forigin;

	//origin = XMVector3TransformCoord(origin, m_LightDesc.mLightView);
	//XMStoreFloat3(&forigin, origin);

	//m_LightDesc.mOrthinfo[1] = forigin.x - m_LightDesc.mOrthinfo[0];
	//m_LightDesc.mOrthinfo[2] = forigin.x + m_LightDesc.mOrthinfo[0];
	//m_LightDesc.mOrthinfo[3] = forigin.y - m_LightDesc.mOrthinfo[0];
	//m_LightDesc.mOrthinfo[4] = forigin.y + m_LightDesc.mOrthinfo[0];

	//m_LightDesc.mLightProj = XMMatrixOrthographicLH(m_LightDesc.mOrthinfo[2] - m_LightDesc.mOrthinfo[1], m_LightDesc.mOrthinfo[4] - m_LightDesc.mOrthinfo[3], 0.1f, 300.f);

	//_float3 up = _float3(0, 1.f, 0);
	//_float3 lookat = _float3(0, 0, 0);

	//XMStoreFloat3(&lookat,playerpos);

	//_vector		vPosition = XMLoadFloat3(&m_LightDesc.vPosition);
	//vPosition = XMVectorSetW(vPosition, 1.f);

	//_vector		vLook = XMLoadFloat3(&lookat) - XMLoadFloat3(&m_LightDesc.vPosition);
	//vLook = XMVector3Normalize(vLook);

	////XMStoreFloat3(&LightDesc.vDirection, vLook);

	//_vector		vRight = XMVector3Cross(XMLoadFloat3(&up), vLook);
	//vRight = XMVector3Normalize(vRight);

	//_vector		vUp = XMVector3Cross(vLook, vRight);
	//vUp = XMVector3Normalize(vUp);

	//_matrix lightcam;
	//lightcam.r[0] = vRight;
	//lightcam.r[1] = vUp;
	//lightcam.r[2] = vLook;
	//lightcam.r[3] = vPosition;

	////_vector origin = { 0,0,0,0 };
	////_float3	forigin;

	//m_LightDesc.mLightView = XMMatrixInverse(nullptr, lightcam);

	//origin = XMVector3TransformCoord(origin, m_LightDesc.mLightView);
	//XMStoreFloat3(&forigin, origin);

	//m_LightDesc.mOrthinfo[0] = 20.f;

	//m_LightDesc.mOrthinfo[1] = forigin.x - m_LightDesc.mOrthinfo[0];
	//m_LightDesc.mOrthinfo[2] = forigin.x + m_LightDesc.mOrthinfo[0];
	//m_LightDesc.mOrthinfo[3] = forigin.y - m_LightDesc.mOrthinfo[0];
	//m_LightDesc.mOrthinfo[4] = forigin.y + m_LightDesc.mOrthinfo[0];

	//m_LightDesc.mLightProj = XMMatrixOrthographicLH(m_LightDesc.mOrthinfo[2] - m_LightDesc.mOrthinfo[1], m_LightDesc.mOrthinfo[4] - m_LightDesc.mOrthinfo[3], 0.1f, 1000.f);
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
