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

	m_fX = 290.f;
	m_fY = 263.f;
	m_fSizeX = 49.2f;
	m_fSizeY = 45.f;

	//m_ProjectionMatrix = XMMatrixOrthographicLH(WINCX, WINCY, 0.f, 1.f);
	//XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	return S_OK;
}

HRESULT CMFCObject_UI::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}
	
	//m_pTextureCom->Change_Texture(L"Texture");

	_vector vPos = { 500.f,500.f ,1.f,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { 1.f,1.f ,1.f ,1.f };
	m_pTransform->Scaling(vScale);
	/*m_WorldMatrix.r[0] = XMVectorSet(g_iWinCX, 0.f, 0.f, 0.f);
	m_WorldMatrix.r[1] = XMVectorSet(0.0f, g_iWinCY, 0.f, 0.f);
	m_WorldMatrix.r[2] = XMVectorSet(0.0f, 0.f, 1.f, 0.f);*/

	return S_OK;
}

_int CMFCObject_UI::Tick(_double TimeDelta)
{
	//XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	//m_WorldMatrix._11 = m_fSizeX;
	//m_WorldMatrix._22 = m_fSizeY;

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

	//cout << "X : " << ptMouse.x << " , " << " Y : " << ptMouse.y << endl;
	
	return _int();
}

_int CMFCObject_UI::LateTick(_double TimeDelta)
{
	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CMFCObject_UI::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pVIBufferCom->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pVIBufferCom->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pVIBufferCom->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pVIBufferCom->SetUp_TextureOnShader("g_DiffuseTexture", m_pTextureCom, m_iCurrentImage); // 0 이랑 1로 나눠줘야함 

	m_pVIBufferCom->Render(0);
	
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

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
