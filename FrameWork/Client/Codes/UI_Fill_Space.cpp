#include "framework.h"
#include "pch.h"
#include "UI_Fill_Space.h"
#include "GameInstance.h"

CUI_Fill_Space::CUI_Fill_Space(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Fill_Space::CUI_Fill_Space(const CUI_Fill_Space& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
{
}

HRESULT CUI_Fill_Space::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Fill_Space::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIACTIVEDESC));
	}

	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}
	m_iSceneID = _iSceneID;

	m_iObectTag = m_Desc.UIDesc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.UIDesc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 110.f, -97.f, 0.1f, 1.f });
	_vector vScale = { m_Desc.UIDesc.fSize.x,m_Desc.UIDesc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	setActive(false);

	return S_OK;
}

_int CUI_Fill_Space::Tick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;

	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	if (g_pGameInstance->getkeyPress(DIK_SPACE))
	{
		m_fGapX += (_float)TimeDelta;
		m_fPressTiime += (_float)TimeDelta;

		if (1.0f <= m_fPressTiime && false == m_bRespawn)
		{
			m_bRespawn = true;
		}
	}

	if (g_pGameInstance->getkeyUp(DIK_SPACE))
	{
		m_fGapX = 0.f;
		m_fPressTiime = 0.f;
	}

	if (false == m_bActive)
	{
		m_fGapX = 1.f;
	}

	if (true == m_bRespawn)
	{
		g_pGuideManager->Respawn();
		m_bRespawn = false;
	}
	return 0;
}

_int CUI_Fill_Space::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;

	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}

	return _int();
}

HRESULT CUI_Fill_Space::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
	m_pBuffer->SetUp_ValueOnShader("g_fX", &m_fGapX, sizeof(_float));
	m_pBuffer->SetUp_ValueOnShader("g_fY", &m_fGapY, sizeof(_float));
	m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->Render(1);

	return S_OK;
}

void CUI_Fill_Space::ResetVal(void)
{
	m_fPressTiime = 0.f;
	m_bRespawn = false;
}

HRESULT CUI_Fill_Space::SetUp_Components()
{
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Bar", L"Com_Rect_UI_Bar", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_Fill_Space* CUI_Fill_Space::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Fill_Space* pInstance = new CUI_Fill_Space(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Fill_Space");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Fill_Space::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Fill_Space* pInstance = new CUI_Fill_Space(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Fill_Space");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Fill_Space::Free()
{
	__super::Free();
}
