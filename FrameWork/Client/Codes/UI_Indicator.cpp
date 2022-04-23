#include "framework.h"
#include "pch.h"
#include "UI_Indicator.h"
#include "GameInstance.h"

CUI_Indicator::CUI_Indicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Indicator::CUI_Indicator(const CUI_Indicator& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
{
}

HRESULT CUI_Indicator::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Indicator::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIACTIVEDESC));
	}

	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	m_iObectTag = m_Desc.UIDesc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.UIDesc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	setActive(false);

	return S_OK;
}

_int CUI_Indicator::Tick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	/*if (false == m_bFirstShow)
	{
		setActive(true);
		m_bFirstShow = true;
	}

	if (true == m_bShow)
	{
		m_fAlpha = 1.f;
		m_fDisappearTimeAcc = 0.f;
	}

	else if (false == m_bShow)
	{
		m_fDisappearTimeAcc += (_float)TimeDelta;
	}

	if (5.f <= m_fDisappearTimeAcc)
	{
		m_fAlpha -= (_float)TimeDelta;
	}

	if (0 >= m_fAlpha)
	{
		m_fAlpha = 0.f;
		m_fDisappearTimeAcc = 0.f;
	}*/

	_vector vTargetLook = XMLoadFloat4(&m_TargetPos) - g_pObserver->Get_PlayerPos();
	_vector vDir = XMVector3Normalize(vTargetLook);

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vTargetLook);
	vTargetLook = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	
	vTargetLook = XMVector3Normalize(vTargetLook) * m_pTransform->Get_Scale(CTransform::STATE_LOOK);
	vRight = XMVector3Normalize(vRight) * m_pTransform->Get_Scale(CTransform::STATE_RIGHT);

	m_pTransform->Set_State(CTransform::STATE_UP, vTargetLook);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);

	_vector PlayerPos = g_pObserver->Get_PlayerPos();
	PlayerPos = XMVectorSetY(PlayerPos ,XMVectorGetY(PlayerPos) + 0.2f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, PlayerPos + (vDir * 1.f));

	m_pTransform->Scaling(XMVectorSet(1.2f, 1.2f, 1.2f, 0.f));
	
	return 0;
}

_int CUI_Indicator::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHA, this);
	}
	return _int();
}

HRESULT CUI_Indicator::Render()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
	m_fAlpha = 1.f;
	m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->Render(1);

	return S_OK;
}

HRESULT CUI_Indicator::SetUp_Components()
{
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Panel_UI", L"Com_Rect_Panel_UI", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_Indicator* CUI_Indicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Indicator* pInstance = new CUI_Indicator(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Indicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Indicator::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Indicator* pInstance = new CUI_Indicator(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Indicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Indicator::Free()
{
	__super::Free();
}
