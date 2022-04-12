#include "pch.h"
#include "SingleImage.h"
#include "UI_Texture.h"
#include "VIBuffer_Rect.h"

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

	m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_RectBuffer");
	//Safe_AddRef(m_pBuffer);

	if (!m_pBuffer)
		return E_FAIL;

	return S_OK;
}

_int CSingleImage::Tick(_double TimeDelta)
{
	return _int();
}

_int CSingleImage::LateTick(_double TimeDelta)
{
	//if (nullptr != m_pRenderer)
	//	m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, m_pCreator);

	return _int();
}

HRESULT CSingleImage::Render(CTransform* _sender)
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	if (nullptr != m_pBuffer)
	{
		m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
		m_pBuffer->SetUp_ValueOnShader("g_Color", &m_fColor, sizeof(_float4));

		m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pImage);

		m_pBuffer->Render(1);
	}
	return S_OK;
}

void CSingleImage::SetTexture(std::wstring name)
{
	m_pImage = g_pGameInstance->GetResource<UI_Texture>(name)->Get_Texture(0);
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
