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

	return S_OK;
}

HRESULT CUI_Ingame::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIDESC));
	}

	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}
	
	m_iObectTag = m_Desc.IDTag;
	wstring TextureTag = m_Desc.TextureTag;
	wstring tag = L"Texture_" + TextureTag;
	if (FAILED(m_pTexture->Change_Texture(tag)))
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


	if (nullptr != m_pBuffer)
	{
		m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

		m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture); // 0 이랑 1로 나눠줘야함 

		m_pBuffer->Render(1);
	}

	else if (nullptr != m_pTrapziumBuffer)
	{
		m_pTrapziumBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
		m_pTrapziumBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
		m_pTrapziumBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

		m_pTrapziumBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture); // 0 이랑 1로 나눠줘야함 

		m_pTrapziumBuffer->Render(1);
	}
	
	return S_OK;
}

HRESULT CUI_Ingame::SetUp_Components()
{
	if (13 == m_Desc.IDTag) //static
	{
		m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_Rect_UI");

		if (!m_pBuffer)
			return E_FAIL;

		if (FAILED(__super::SetUp_Components(L"Com_Rect_UI", m_pBuffer)))
			return E_FAIL;
	}

	else if (14 == m_Desc.IDTag) //dynamic
	{
		CVIBuffer_Trapezium::TRAPDESC Desc;
		Desc.fAngle = m_Desc.fAngle;
		_tcscpy_s(Desc.ShaderFilePath, L"../../Reference/ShaderFile/Shader_UI.hlsl");

		Desc.bMinus = m_Desc.bMinus;

		m_pTrapziumBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Trapezium>(0, L"Proto_Component_Trapezium_UI", &Desc);

		if (!m_pTrapziumBuffer)
			return E_FAIL;

		if (FAILED(__super::SetUp_Components(L"Com_Trapezium_UI", m_pTrapziumBuffer)))
			return E_FAIL;
	}

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

CGameObject* CUI_Ingame::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Ingame* pInstance = new CUI_Ingame(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
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
