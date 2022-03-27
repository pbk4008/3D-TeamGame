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

	return S_OK;
}

HRESULT CMFCObject_UI::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIDESC));
	}

	m_iObectTag = m_Desc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.TextureTag)))
		return E_FAIL;

	wstring tag = m_Desc.TextureTag;
	if (FAILED(__super::NativeConstruct(iSceneID, &tag)))
	{
		return E_FAIL;
	}

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}


	return S_OK;
}

_int CMFCObject_UI::Tick(_double TimeDelta)
{
	_vector vPos = { m_Desc.fPos.x - (WINCX >> 1),-m_Desc.fPos.y + (WINCY >> 1), m_Desc.fPos.z ,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vSize = { m_Desc.fSize.x,m_Desc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vSize);

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

HRESULT CMFCObject_UI::SetUp_Components()
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

		m_pTrapziumBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Trapezium>(0, L"Proto_Component_Trapezium_UI",&Desc);

		if (!m_pTrapziumBuffer)
			return E_FAIL;

		if (FAILED(__super::SetUp_Components(L"Com_TrapeziumBuffer", m_pTrapziumBuffer)))
			return E_FAIL;
	}

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

CGameObject* CMFCObject_UI::Clone(const _uint iSceneID, void* pArg)
{
	CMFCObject_UI* pInstance = new CMFCObject_UI(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
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
