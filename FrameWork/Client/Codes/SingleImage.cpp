#include "pch.h"
#include "SingleImage.h"
#include "UI_Texture.h"
#include "VIBuffer_Rect.h"
#include "..\Headers\SingleImage.h"

CSingleImage::CSingleImage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CSingleImage::CSingleImage(const CSingleImage& rhs)
	: CComponent(rhs)
{
}

HRESULT CSingleImage::NativeConstruct_Prototype()
{
	if (FAILED(CComponent::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSingleImage::NativeConstruct(void* pArg)
{
	if (FAILED(CComponent::NativeConstruct(pArg)))
		return E_FAIL;

	Desc texDesc = (*(Desc*)pArg);

	if (!texDesc.pTexture)
	{
		m_pImage = g_pGameInstance->GetResource<UI_Texture>(texDesc.textureName)->Get_Texture(0);
	}
	else
	{
		m_pImage = texDesc.pTexture;
	}

	m_fColor = texDesc.fColor;

	m_pCreator = texDesc.pCreator;
	m_pRenderer = texDesc.pRenderer;
	m_pTransform = texDesc.pTransform;
	m_fOffsetPosition = texDesc.fOffsetPos;
	m_fOffsetScale = texDesc.fOffsetScale;
	m_ERenderType = texDesc.renderType;
	m_bFadeOpt = texDesc.bFadeOption;

	/* for VerticalGauge */
	m_fGapX = texDesc.fGapX;

	switch (m_ERenderType)
	{
	case CSingleImage::Alpha:
		m_iRenderPass = 1;
		m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_RectBuffer");
		assert("Failed to Get Buffer" && m_pBuffer);
		break;
	case CSingleImage::Nonalpha:
		m_iRenderPass = 0;
		m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_RectBuffer");
		assert("Failed to Get Buffer" && m_pBuffer);
		break;
	case CSingleImage::VerticalGaugeRight:
	{
		m_pBuffer = texDesc.pBuffer;
		m_iRenderPass = 1;
	}
		break;
	case CSingleImage::VerticalGaugeLeft:
	{
		m_pBuffer = texDesc.pBuffer;
		m_iRenderPass = 1;
	}
	break;
	default:
		m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_RectBuffer");
		assert("Failed to Get Buffer" && m_pBuffer);
		m_iRenderPass = 1;
		break;
	}
	return S_OK;
}

_int CSingleImage::Tick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	return _int();
}

_int CSingleImage::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	return _int();
}

HRESULT CSingleImage::Render(CTransform* _sender)
{
	if (false == m_bFadeOut)
	{
		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;
		else
			m_fAlpha += g_fDeltaTime * 0.5f;
	}
	else
	{
		if (m_fAlpha <= 0.f)
			m_fAlpha = 0.f;
		else
			m_fAlpha -= g_fDeltaTime * 0.5f;
	}

	if (RenderType::Alpha == m_ERenderType || RenderType::Nonalpha == m_ERenderType || RenderType::Type_End == m_ERenderType)
	{
		_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
		_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
		_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

		if (nullptr != m_pBuffer)
		{
			m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
			m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
			m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
			m_pBuffer->SetUp_ValueOnShader("g_Fade", &m_bFadeOpt, sizeof(_bool));
			m_pBuffer->SetUp_ValueOnShader("g_Alpha", &m_fAlpha, sizeof(_float));
			//m_pBuffer->SetUp_ValueOnShader("g_Color", &m_fColor, sizeof(_float4));
			m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pImage);

			m_pBuffer->Render(m_iRenderPass);
		}
	}
	else if(RenderType::VerticalGaugeRight == m_ERenderType)
	{
		wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
		_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
		_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
		_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

		m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
		m_pBuffer->SetUp_ValueOnShader("g_fX", &m_fGapX, sizeof(_float));
		m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fColor.z, sizeof(_float));

		m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pImage);

		m_pBuffer->Render(1);
	}
	else if (RenderType::VerticalGaugeLeft == m_ERenderType)
	{
		wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
		_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
		_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
		_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

		m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
		m_pBuffer->SetUp_ValueOnShader("g_fX", &m_fGapX, sizeof(_float));
		m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fColor.z, sizeof(_float));

		m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pImage);

		m_pBuffer->Render(2);
	}

	return S_OK;
}

void CSingleImage::SetTexture(std::wstring name)
{
	m_pImage = g_pGameInstance->GetResource<UI_Texture>(name)->Get_Texture(0);
}

void CSingleImage::SetRenderVal(void* val)
{
	m_fExpRatio = (*(RenderVal*)val).fExpRatio;
	m_fGapX = (*(RenderVal*)val).fGapX;
}

void CSingleImage::SetFadeOut(void)
{
	m_bFadeOut = true;
}

CSingleImage* CSingleImage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CSingleImage* pInstance = new CSingleImage(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CSingleImage Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CSingleImage::Clone(void* pArg)
{
	CSingleImage* pInstance = new CSingleImage(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CSingleImage Clone Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSingleImage::Free()
{
	if (m_isCloned == true)
	{
		Safe_Release(m_pBuffer);
	}

	__super::Free();
}
