#include "framework.h"
#include "pch.h"
#include "MFCObject_UI.h"
#include "GameInstance.h"

CMFCObject_UI::CMFCObject_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
}

CMFCObject_UI::CMFCObject_UI(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CMFCObject_UI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	//m_ProjectionMatrix = XMMatrixOrthographicLH(WINCX, WINCY, 0.f, 1.f);
	//XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CMFCObject_UI::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIDESC));
	}

	if (FAILED(__super::NativeConstruct(&m_Desc.TextureTag)))
	{
		return E_FAIL;
	}

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	//m_Desc.fPos.x = 500.f;
	//m_Desc.fPos.y = 500.f;
	//m_Desc.fScale.x = 100.f;
	//m_Desc.fScale.y = 100.f;


	return S_OK;
}

_int CMFCObject_UI::Tick(_double TimeDelta)
{
	_vector vPos = { m_Desc.fPos.x - (WINCX >> 1),-m_Desc.fPos.y + (WINCY >> 1),1.f,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_Desc.fScale.x,m_Desc.fScale.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);



	//XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	//m_WorldMatrix._11 = m_fSizeX;
	//m_WorldMatrix._22 = m_fSizeY;
	//
	//m_WorldMatrix._41 = m_fX - (WINCX >> 1);
	//m_WorldMatrix._42 = -m_fY + (WINCY >> 1);
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
	//	cout << "MFCObject_UI 안에 들어와있음" << endl;
	//}
	//else
	//{
	//	m_bIn = false;
	//}

	return 0;
}

_int CMFCObject_UI::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CMFCObject_UI::Render()
{
	//_matrix XMWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix));
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MFCCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MFCCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture); // 0 이랑 1로 나눠줘야함 

	m_pBuffer->Render(0);
	
	return S_OK;
}

HRESULT CMFCObject_UI::SetUp_Components()
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
	//if (FAILED(__super::SetUp_Components((_uint)LEVEL::LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_MFCObject_UI"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

CMFCObject_UI* CMFCObject_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CMFCObject_UI* pInstance = new CMFCObject_UI(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CMFCObject_UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMFCObject_UI::Clone(void* pArg)
{
	CMFCObject_UI* pInstance = new CMFCObject_UI(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating Clone CMFCObject_UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMFCObject_UI::Free()
{
	__super::Free();
}
