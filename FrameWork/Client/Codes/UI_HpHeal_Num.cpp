#include "framework.h"
#include "pch.h"
#include "UI_HpHeal_Num.h"
#include "GameInstance.h"

CUI_HpHeal_Num::CUI_HpHeal_Num(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI_Monster_Panel(pDevice,pDeviceContext)
{
}

CUI_HpHeal_Num::CUI_HpHeal_Num(const CUI_Monster_Panel& rhs)
	: CUI_Monster_Panel(rhs)
{
}

HRESULT CUI_HpHeal_Num::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_HpHeal_Num::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIACTIVEDESC));
	}

	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	m_iObectTag = m_Desc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	_vector vPos = { m_Desc.fPos.x - (g_iWinCx >> 1),-m_Desc.fPos.y + (g_iWinCy >> 1), m_Desc.fPos.z,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_Desc.fSize.x,m_Desc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	m_pInventoryData = g_pDataManager->GET_DATA(CInventoryData, L"InventoryData");

	return S_OK;
}

_int CUI_HpHeal_Num::Tick(_double TimeDelta)
{
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	return 0;
}

_int CUI_HpHeal_Num::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		if (g_pInvenUIManager->Get_OpenCheck() == false)
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);
	}
	return _int();
}

HRESULT CUI_HpHeal_Num::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	_uint iPotionCount = m_pInventoryData->GetPotionCount();
	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, iPotionCount);

	m_pBuffer->Render(1);
	
	return S_OK;
}

HRESULT CUI_HpHeal_Num::SetUp_Components()
{
	m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_Rect_UI");

	if (!m_pBuffer)
		return E_FAIL;

	if (FAILED(CGameObject::SetUp_Components(L"Com_Rect_UI", m_pBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_HpHeal_Num* CUI_HpHeal_Num::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_HpHeal_Num* pInstance = new CUI_HpHeal_Num(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_HpHeal_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_HpHeal_Num::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_HpHeal_Num* pInstance = new CUI_HpHeal_Num(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_HpHeal_Num");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HpHeal_Num::Free()
{
	__super::Free();
}
