#include "framework.h"
#include "pch.h"
#include "UI_Fill_Ckey2.h"
#include "GameInstance.h"

CUI_Fill_Ckey2::CUI_Fill_Ckey2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Fill_Ckey2::CUI_Fill_Ckey2(const CUI_Fill_Ckey2& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
	, m_pTexture2(rhs.m_pTexture2)
{
	Safe_AddRef(m_pTexture2);
}

HRESULT CUI_Fill_Ckey2::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	m_pTexture2 = g_pGameInstance->Clone_Component<CTexture>(0, L"Proto_Component_Texture");
	if (!m_pTexture2)
		return E_FAIL;
	if (FAILED(SetUp_Components(L"Com_Texture2", m_pTexture2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Fill_Ckey2::NativeConstruct(const _uint _iSceneID, void* pArg)
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

	if (FAILED(m_pTexture->Change_Texture(m_Desc.TextureTag2)))
		return E_FAIL;
	if (FAILED(m_pTexture2->Change_Texture(m_Desc.UIDesc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	_vector vScale = { m_Desc.UIDesc.fSize.x,m_Desc.UIDesc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	setActive(false);

	return S_OK;
}

_int CUI_Fill_Ckey2::Tick(_double TimeDelta)
{
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;

	if (g_pGameInstance->getkeyPress(DIK_C) && true == m_isPress)
	{
		m_fGapX += (_float)TimeDelta * 0.4f;
	}
	if (g_pGameInstance->getkeyUp(DIK_C))
	{
		m_fGapX = 0.f;
		m_isPress = false;
	}
	if (false == m_bActive)
	{
		m_fGapX = 1.f;
	}
	return 0;
}

_int CUI_Fill_Ckey2::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	//빌보드
	_matrix ViewMatrix;
	ViewMatrix = g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	m_pTransform->Set_State(CTransform::STATE::STATE_RIGHT, ViewMatrix.r[0]);
	m_pTransform->Set_State(CTransform::STATE::STATE_UP, ViewMatrix.r[1]);
	m_pTransform->Set_State(CTransform::STATE::STATE_LOOK, ViewMatrix.r[2]);

	_fvector MyPos = m_pTransform->Get_State(CTransform::STATE::STATE_POSITION);
	_fvector vDist = MyPos - g_pGameInstance->Get_CamPosition(L"Camera_Silvermane");
	_float fDistToUI = XMVectorGetX(XMVector3Length(vDist));

	_vector vScale = XMVectorZero();
	vScale = { fDistToUI * 0.06f,fDistToUI * 0.06f,1.f,1.f };
	m_pTransform->Scaling(vScale);

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Fill_Ckey2::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	// 1
	m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
	m_pBuffer->SetUp_ValueOnShader("g_fX", &m_fGapX, sizeof(_float));
	m_pBuffer->SetUp_ValueOnShader("g_fY", &m_fGapY, sizeof(_float));
	m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));

	m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pBuffer->Render(1);

	// 2 : 기본으로 깔려있는 비어있는 놈
	m_pBuffer2->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pBuffer2->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pBuffer2->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));

	m_pBuffer2->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture2);

	m_pBuffer2->Render(1);

	return S_OK;
}

void CUI_Fill_Ckey2::Set_Position(const _fvector& _svPos)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(_svPos, 1.f));
}

HRESULT CUI_Fill_Ckey2::Ready_Components()
{
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Bar", L"Com_Rect_UI_Bar", (CComponent**)&m_pBuffer)))
		return E_FAIL;
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_UI", L"Com_Rect_UI_Bar2", (CComponent**)&m_pBuffer2)))
		return E_FAIL;

	return S_OK;
}

CUI_Fill_Ckey2* CUI_Fill_Ckey2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Fill_Ckey2* pInstance = new CUI_Fill_Ckey2(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Fill_Ckey2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Fill_Ckey2::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Fill_Ckey2* pInstance = new CUI_Fill_Ckey2(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Fill_Ckey2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Fill_Ckey2::Free()
{
	__super::Free();
	Safe_Release(m_pBuffer2);
	Safe_Release(m_pTexture2);
}
