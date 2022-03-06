#include "framework.h"
#include "pch.h"
#include "UI_Ingame.h"
#include "GameInstance.h"

CUI_Ingame::CUI_Ingame(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
}

CUI_Ingame::CUI_Ingame(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Ingame::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	//m_ProjectionMatrix = XMMatrixOrthographicLH(WINCX, WINCY, 0.f, 1.f);
	//XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CUI_Ingame::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIDESC));
	}

	wstring tag = m_Desc.TextureTag;
	if (FAILED(__super::NativeConstruct(&tag)))
	{
		return E_FAIL;
	}

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	_vector vPos = { m_Desc.fPos.x - (g_iWinCx >> 1),-m_Desc.fPos.y + (g_iWinCy >> 1),0.5f,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_Desc.fScale.x,m_Desc.fScale.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	return S_OK;
}

_int CUI_Ingame::Tick(_double TimeDelta)
{
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	//XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	//m_WorldMatrix._11 = m_Desc.fScale.x;
	//m_WorldMatrix._22 = m_Desc.fScale.y;

	//m_WorldMatrix._41 = m_Desc.fPos.x - (g_iWinCx >> 1);
	//m_WorldMatrix._42 = -m_Desc.fPos.y + (g_iWinCy >> 1);
	//m_WorldMatrix._43 = 0.69f;

	//RECT		rc;
	//SetRect(&rc, (_uint)(m_fX - m_fSizeX * 0.5f), (_uint)(m_fY - m_fSizeY * 0.5f),
	//	(_uint)(m_fX + m_fSizeX * 0.5f), (_uint)(m_fY + m_fSizeY * 0.5f));

	//POINT		ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);

	//if (TRUE == PtInRect(&rc, ptMouse))
	//{
	//	m_bIn = true;
	//	cout << "UI_Ingame 안에 들어와있음" << endl;
	//}
	//else
	//{
	//	m_bIn = false;
	//}

	return 0;
}

_int CUI_Ingame::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Ingame::Render()
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

HRESULT CUI_Ingame::SetUp_Components()
{
	///* Com_Renderer */
	//if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	//{
	//	return E_FAIL;
	//}

	///* Com_VIBuffer */
	//if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_UI"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	//{
	//	return E_FAIL;
	//}

	///* Com_Texture */
	//if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Ingame"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

CUI_Ingame* CUI_Ingame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Ingame* pInstance = new CUI_Ingame(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Ingame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Ingame::Clone(void* pArg)
{
	CUI_Ingame* pInstance = new CUI_Ingame(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Ingame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Ingame::Free()
{
	__super::Free();
}
