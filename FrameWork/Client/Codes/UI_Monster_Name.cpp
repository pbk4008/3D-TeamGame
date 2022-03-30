#include "framework.h"
#include "pch.h"
#include "UI_Monster_Name.h"
#include "GameInstance.h"

CUI_Monster_Name::CUI_Monster_Name(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Monster_Name::CUI_Monster_Name(const CUI_Monster_Name& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
{
	
}

HRESULT CUI_Monster_Name::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Monster_Name::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UINAMEDESC));
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

	return S_OK;
}

_int CUI_Monster_Name::Tick(_double TimeDelta)
{
	if (false == m_bFirstShow)
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
		m_fDisappearTimeAcc += TimeDelta;
	}

	if (5.f <= m_fDisappearTimeAcc)
	{
		m_fAlpha -= TimeDelta;
	}

	if (0 >= m_fAlpha)
	{
		m_fAlpha = 0.f;
		m_fDisappearTimeAcc = 0.f;
	}

	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	return 0;
}

_int CUI_Monster_Name::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Monster_Name::Render()
{
	if (m_bShow)
	{
	}

	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
	m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, m_Desc.iTextureNum);

	m_pBuffer->Render(1);
	

	return S_OK;
}

HRESULT CUI_Monster_Name::SetUp_Components()
{
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Panel_UI", L"Com_Rect_Panel_UI", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	//m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_Rect_UI");

	//if (!m_pBuffer)
	//	return E_FAIL;

	//if (FAILED(CGameObject::SetUp_Components(L"Com_Rect_UI", m_pBuffer)))
	//	return E_FAIL;

	return S_OK;
}

CUI_Monster_Name* CUI_Monster_Name::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Monster_Name* pInstance = new CUI_Monster_Name(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Monster_Name");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_Name::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Monster_Name* pInstance = new CUI_Monster_Name(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Monster_Name");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_Name::Free()
{
	__super::Free();
}
