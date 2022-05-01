#include "framework.h"
#include "pch.h"
#include "UI_Skill_Meter_Gauge.h"
#include "GameInstance.h"

CUI_Skill_Meter_Gauge::CUI_Skill_Meter_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
}

CUI_Skill_Meter_Gauge::CUI_Skill_Meter_Gauge(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill_Meter_Gauge::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Skill_Meter_Gauge::NativeConstruct(const _uint _iSceneID, void* pArg)
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

	if (FAILED(m_pTexture->Change_Texture(m_Desc.TextureTag)))
		return E_FAIL;

	/* �������� �����ͷ� ���� ���ϴ� �� ���� */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}


	_vector vPos = { m_Desc.fPos.x - (g_iWinCx >> 1),-m_Desc.fPos.y + (g_iWinCy >> 1), m_Desc.fPos.z,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_Desc.fSize.x,m_Desc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);


	m_fGapX = 1.f;
	m_fGapY = 1.f;


	return S_OK;
}

_int CUI_Skill_Meter_Gauge::Tick(_double TimeDelta)
{
	if (FAILED(__super::Tick(TimeDelta)))
		return -1;

	m_fRatio = g_pObserver->Get_Player()->Get_SkillGuage() / 50.f;

	if (1.f == m_fRatio)
	{
		m_fGapX = 1.f;
	}

	if (1.f <= m_fGapX)
	{
		m_bFull = true;
	}

	if (m_fGapX < m_fRatio)
	{
		m_fGapX = m_fRatio;
	}

	if (m_fGapX > m_fRatio)
	{
		m_fGapX -= (_float)TimeDelta * 3.f;

		if (0.f >= m_fGapX)
		{
			m_fGapX = 0.f;
		}
	}

	return 0;
}

_int CUI_Skill_Meter_Gauge::LateTick(_double TimeDelta)
{
	if (FAILED(__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer && 
		!g_pGuideManager->IsOpenDeathUI())
	{
		if (g_pInvenUIManager->Get_OpenCheck() == false)
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);
	}
	return _int();
}

HRESULT CUI_Skill_Meter_Gauge::Render()
{
	_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
	_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_VIEW));
	_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(L"MainOrthoCamera", TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

	m_pTrapziumBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_fX", &m_fGapX, sizeof(_float));
	m_pTrapziumBuffer->SetUp_ValueOnShader("g_fY", &m_fGapY, sizeof(_float));

	m_pTrapziumBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	m_pTrapziumBuffer->Render(1);

	return S_OK;
}

HRESULT CUI_Skill_Meter_Gauge::SetUp_Components()
{
	CVIBuffer_Trapezium::TRAPDESC Desc;
	Desc.fAngle = m_Desc.fAngle;
	_tcscpy_s(Desc.ShaderFilePath, L"../../Reference/ShaderFile/Shader_UI_Bar.hlsl");

	Desc.bMinus = m_Desc.bMinus;

	m_pTrapziumBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Trapezium>(0, L"Proto_Component_Trapezium_UI", &Desc);

	if (!m_pTrapziumBuffer)
		return E_FAIL;

	if (FAILED(CGameObject::SetUp_Components(L"Com_Trapezium_UI", m_pTrapziumBuffer)))
		return E_FAIL;

	return S_OK;
}

CUI_Skill_Meter_Gauge* CUI_Skill_Meter_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* ������ü �����Ҷ� �ʱ�ȭ */
	CUI_Skill_Meter_Gauge* pInstance = new CUI_Skill_Meter_Gauge(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Skill_Meter_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Skill_Meter_Gauge::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Skill_Meter_Gauge* pInstance = new CUI_Skill_Meter_Gauge(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Skill_Meter_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_Meter_Gauge::Free()
{
	__super::Free();
}
