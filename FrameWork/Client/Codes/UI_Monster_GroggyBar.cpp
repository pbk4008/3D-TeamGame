#include "framework.h"
#include "pch.h"
#include "UI_Monster_GroggyBar.h"
#include "GameInstance.h"

CUI_Monster_GroggyBar::CUI_Monster_GroggyBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)

{
	ZeroMemory(&m_UIBarDesc, sizeof(m_UIBarDesc));
}

CUI_Monster_GroggyBar::CUI_Monster_GroggyBar(const CUI_Monster_GroggyBar& rhs)
	: CUI(rhs)
	, m_UIBarDesc(rhs.m_UIBarDesc)
{
}

HRESULT CUI_Monster_GroggyBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Monster_GroggyBar::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_UIBarDesc, pArg, sizeof(UIBARDESC));
	}

	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	m_iObectTag = m_UIBarDesc.UIDesc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_UIBarDesc.UIDesc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	m_fGapX = 0.f;
	m_fGapY = 0.5f;

	return S_OK;
}

_int CUI_Monster_GroggyBar::Tick(_double TimeDelta)
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
		m_fDisappearTimeAcc += (_float)TimeDelta;
	}

	if (5.f <= m_fDisappearTimeAcc)
	{
		m_fAlpha -= (_float)TimeDelta;
	}

	if (0 >= m_fAlpha)
	{
		m_fAlpha = 0.f;
		m_fDisappearTimeAcc = 0.f;
		setActive(false);
	}


	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;
	
	m_fGapX = m_fGroggyRatio;

	/*if (g_pGameInstance->getkeyDown(DIK_L))
	{
		m_fGapX -= 0.1f;
	}*/

	return 0;
}

_int CUI_Monster_GroggyBar::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHANB, this);
	}
	return _int();
}

HRESULT CUI_Monster_GroggyBar::Render()
{
	wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pTrapziumBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_fX", &m_fGapX, sizeof(_float));
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_fY", &m_fGapY, sizeof(_float));
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));

	_float CurAttackGauge = 0.f;
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_fCurAttack", &CurAttackGauge, sizeof(_float));


	m_pTrapziumBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pTrapziumBuffer->Render(m_UIBarDesc.iRenderPass);

	return S_OK;
}

HRESULT CUI_Monster_GroggyBar::SetUp_Components()
{
	CVIBuffer_Trapezium::TRAPDESC Desc;
	Desc.fAngle =  m_UIBarDesc.UIDesc.fAngle;
	_tcscpy_s(Desc.ShaderFilePath, L"../../Reference/ShaderFile/Shader_UI_Enemy_Bar.hlsl");

	Desc.bMinus = m_UIBarDesc.UIDesc.bMinus;

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Trapezium_UI", L"Com_Trapezium_UI", (CComponent**)&m_pTrapziumBuffer,&Desc)))
		return E_FAIL;

	return S_OK;
}

CUI_Monster_GroggyBar* CUI_Monster_GroggyBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Monster_GroggyBar* pInstance = new CUI_Monster_GroggyBar(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Monster_GroggyBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_GroggyBar::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Monster_GroggyBar* pInstance = new CUI_Monster_GroggyBar(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Monster_GroggyBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_GroggyBar::Free()
{
	__super::Free();
}
