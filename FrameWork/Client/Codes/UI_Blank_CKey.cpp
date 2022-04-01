#include "framework.h"
#include "pch.h"
#include "UI_Blank_CKey.h"
#include "GameInstance.h"
#include "Silvermane.h"

CUI_Blank_CKey::CUI_Blank_CKey(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Blank_CKey::CUI_Blank_CKey(const CUI_Blank_CKey& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
{
}

HRESULT CUI_Blank_CKey::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Blank_CKey::NativeConstruct(const _uint _iSceneID, void* pArg)
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

	_vector vPos = { m_Desc.UIDesc.fPos.x - (g_iWinCx >> 1),-m_Desc.UIDesc.fPos.y + (g_iWinCy >> 1), m_Desc.UIDesc.fPos.z,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_Desc.UIDesc.fSize.x,m_Desc.UIDesc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	setActive(false);

	return S_OK;
}

_int CUI_Blank_CKey::Tick(_double TimeDelta)
{
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	return 0;
}

_int CUI_Blank_CKey::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Blank_CKey::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->Render(1);

	return S_OK;
}

HRESULT CUI_Blank_CKey::SetUp_Components()
{
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_UI", L"Com_Rect_UI", (CComponent**)&m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_Blank_CKey* CUI_Blank_CKey::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Blank_CKey* pInstance = new CUI_Blank_CKey(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Blank_CKey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Blank_CKey::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Blank_CKey* pInstance = new CUI_Blank_CKey(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Blank_CKey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Blank_CKey::Free()
{
	__super::Free();
}
