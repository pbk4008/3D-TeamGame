#include "pch.h"
#include "MFCEffect.h"
#include "GameInstance.h"

CMFCEffect::CMFCEffect()
{
}

CMFCEffect::CMFCEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    :CEffect(pDevice,pDeviceContext)
{
}

CMFCEffect::CMFCEffect(const CEffect& rhs)
    :CEffect(rhs)
{
}

HRESULT CMFCEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

    return S_OK;
}

HRESULT CMFCEffect::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(EFFECTDESC));
	}

	wstring tag = m_Desc.TextureTag;
	if (FAILED(__super::NativeConstruct(&tag)))
	{
		return E_FAIL;
	}

	if (FAILED(__super::SetUp_Components(0, L"Prototype_Component_VIBuffer_PointInstance", L"Com_VIBuffer", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	m_pTexture = (CTexture*)g_pGameInstance->Clone_Component(0, L"bubble");

	return S_OK;
}

_int CMFCEffect::Tick(_double TimeDelta)
{
	m_pBuffer->Update(TimeDelta);
    return 0;
}

_int CMFCEffect::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHA, this);
	}

	return 0;
}

HRESULT CMFCEffect::Render()
{
	//_matrix XMWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix));
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MFCCamera_Proj", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MFCCamera_Proj", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));
	_vector CamPos = g_pGameInstance->Get_CamPosition(L"MFCCamera_Proj");

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);
	m_pBuffer->SetUp_ValueOnShader("g_vCamPosition", (void*)&CamPos, sizeof(_vector));

	m_pBuffer->Render(0);

	return S_OK;
}

CMFCEffect* CMFCEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CMFCEffect* pInstance = new CMFCEffect(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMFCEffect::Clone(void* pArg)
{
	/* 복제본 생성할때는 아래함수 호출해서 추가 초기화를 진행 */
	CMFCEffect* pInstance = new CMFCEffect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMFCEffect::Free()
{
	__super::Free();
}
