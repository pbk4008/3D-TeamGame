#include "framework.h"
#include "pch.h"
#include "UI_Blank_Space.h"
#include "GameInstance.h"
#include "Silvermane.h"

CUI_Blank_Space::CUI_Blank_Space(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Blank_Space::CUI_Blank_Space(const CUI_Blank_Space& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
{
}

HRESULT CUI_Blank_Space::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Blank_Space::NativeConstruct(const _uint _iSceneID, void* pArg)
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

	m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 140.f, -145.f, 0.3f, 1.f });
	_vector vScale = { m_Desc.UIDesc.fSize.x + 10.f , m_Desc.UIDesc.fSize.y + 10.f , 1.f ,1.f };
	m_pTransform->Scaling(vScale);

	setActive(false);

	return S_OK;
}

_int CUI_Blank_Space::Tick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;

	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	return 0;
}

_int CUI_Blank_Space::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;

	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);
	}

	return _int();
}

HRESULT CUI_Blank_Space::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->Render(0);

	return S_OK;
}

HRESULT CUI_Blank_Space::SetUp_Components()
{
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_UI", L"Com_Rect_UI", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_Blank_Space* CUI_Blank_Space::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Blank_Space* pInstance = new CUI_Blank_Space(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Blank_Space");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Blank_Space::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Blank_Space* pInstance = new CUI_Blank_Space(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Blank_Space");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Blank_Space::Free()
{
	__super::Free();
}
