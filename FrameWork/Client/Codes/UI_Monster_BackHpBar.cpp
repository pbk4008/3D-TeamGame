#include "framework.h"
#include "pch.h"
#include "UI_Monster_BackHpBar.h"
#include "GameInstance.h"

CUI_Monster_BackHpBar::CUI_Monster_BackHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_UIBarDesc, sizeof(m_UIBarDesc));
}

CUI_Monster_BackHpBar::CUI_Monster_BackHpBar(const CUI_Monster_BackHpBar& rhs)
	: CUI(rhs)
	, m_UIBarDesc(rhs.m_UIBarDesc)
{
}

HRESULT CUI_Monster_BackHpBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Monster_BackHpBar::NativeConstruct(const _uint _iSceneID, void* pArg)
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

	m_fGapX = 1.f;
	m_fGapY = 0.5f;

	return S_OK;
}

_int CUI_Monster_BackHpBar::Tick(_double TimeDelta)
{
	if (FAILED(CUI::Tick(TimeDelta)))
		return -1;


	if (false == m_bFirstShow)
	{
		setActive(true);
		m_bFirstShow = true;
	}

#pragma region raycast때 화면중앙에 닿으면 ui보였다가 5초뒤에 사라질수있게 해주는
	if (true == m_bShow && false == m_bChange)
	{
		m_fAlpha = 1.f;
		m_fDisappearTimeAcc = 0.f;
	}

	else if (false == m_bShow)
	{
		m_fDisappearTimeAcc += (_float)TimeDelta;
	}

	//5초 지나면 다시 안보이게, raycast
	if (5.f <= m_fDisappearTimeAcc)
	{
		m_fAlpha -= (_float)TimeDelta;

		if (0 >= m_fAlpha)
		{
			m_fAlpha = 0.f;
			m_fDisappearTimeAcc = 0.f;
		}
	}

#pragma endregion

	if (m_fGapX != m_fHpRatio && false == m_bChange)
	{
		//공격이 들어왔다는뜻
		m_bChange = true;
	}

	if (0 >= m_fHpRatio)
	{
		m_bShow = false;
	}

	if (g_pObserver->IsAttack() || g_pObserver->IsThrownObject())
	{
		
		
	}
	
	if (true == m_bChange)
	{
		m_fAlpha -= TimeDelta * 2.f;

		if (0 >= m_fAlpha)
		{
			m_fAlpha = 0.f;
			m_fGapX = m_fHpRatio;
			m_bChange = false;
		}

		if (m_fExHpRatio != m_fHpRatio)
		{
			m_fExHpRatio = m_fHpRatio;
			m_fAlpha = 1.f;
		}
	} 

	return 0;
}

_int CUI_Monster_BackHpBar::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Monster_BackHpBar::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"Camera_Silvermane", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

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

HRESULT CUI_Monster_BackHpBar::SetUp_Components()
{
	CVIBuffer_Trapezium::TRAPDESC Desc;
	Desc.fAngle =  m_UIBarDesc.UIDesc.fAngle;
	_tcscpy_s(Desc.ShaderFilePath, L"../../Reference/ShaderFile/Shader_UI_Enemy_Bar.hlsl");

	Desc.bMinus = m_UIBarDesc.UIDesc.bMinus;

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Trapezium_UI", L"Com_Trapezium_UI", (CComponent**)&m_pTrapziumBuffer,&Desc)))
		return E_FAIL;

	//m_pTrapziumBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Trapezium>(0, L"Proto_Component_Trapezium_UI", &Desc);

	//if (!m_pTrapziumBuffer)
	//	return E_FAIL;

	//if (FAILED(CGameObject::SetUp_Components(L"Com_Trapezium_UI", m_pTrapziumBuffer)))
	//	return E_FAIL;

	return S_OK;
}

CUI_Monster_BackHpBar* CUI_Monster_BackHpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Monster_BackHpBar* pInstance = new CUI_Monster_BackHpBar(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Monster_BackHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_BackHpBar::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Monster_BackHpBar* pInstance = new CUI_Monster_BackHpBar(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Monster_BackHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_BackHpBar::Free()
{
	__super::Free();
}
