#include "framework.h"
#include "pch.h"
#include "UI_Boss_ShieldBar_Blue.h"
#include "GameInstance.h"

#include "Boss_Solaris.h"

CUI_Boss_ShieldBar_Blue::CUI_Boss_ShieldBar_Blue(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
}

CUI_Boss_ShieldBar_Blue::CUI_Boss_ShieldBar_Blue(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Boss_ShieldBar_Blue::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Boss_ShieldBar_Blue::NativeConstruct(const _uint _iSceneID, void* pArg)
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

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}



	_vector vPos = { m_Desc.fPos.x - (g_iWinCx >> 1),-m_Desc.fPos.y + (g_iWinCy >> 1), m_Desc.fPos.z,1.f };
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	_vector vScale = { m_Desc.fSize.x,m_Desc.fSize.y,1.f ,1.f };
	m_pTransform->Scaling(vScale);

	m_fGapX = 1.f;

	CBoss_Solaris* pBoss = (CBoss_Solaris*)g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Boss")->front();


	m_fBossMaxShield = pBoss->Get_MaxGroggyGauge();
	m_fBossShield = pBoss->Get_GroggyGauge();

	return S_OK;
}

_int CUI_Boss_ShieldBar_Blue::Tick(_double TimeDelta)
{
	if (FAILED(__super::Tick(TimeDelta)))
		return -1;


	CBoss_Solaris* pBoss = (CBoss_Solaris*)g_pGameInstance->getObjectList(m_iSceneID, L"Layer_Boss")->front();
	if (nullptr != pBoss)
	{
		m_fBossMaxShield = pBoss->Get_MaxGroggyGauge();
		m_fBossShield = pBoss->Get_GroggyGauge();
	}
	
	m_fRatio = m_fBossShield / m_fBossMaxShield;

	if (1.f == m_fRatio)
	{
		m_fGapX = 1.f;
	}

	if (m_fGapX > m_fRatio)
	{
		m_fGapX -= (_float)TimeDelta * 1.f;
	}
	if (m_fGapX < m_fRatio)
	{
		m_fGapX = m_fRatio;
	}

	return 0;
}

_int CUI_Boss_ShieldBar_Blue::LateTick(_double TimeDelta)
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

HRESULT CUI_Boss_ShieldBar_Blue::Render()
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

HRESULT CUI_Boss_ShieldBar_Blue::SetUp_Components()
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

CUI_Boss_ShieldBar_Blue* CUI_Boss_ShieldBar_Blue::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Boss_ShieldBar_Blue* pInstance = new CUI_Boss_ShieldBar_Blue(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Boss_ShieldBar_Blue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Boss_ShieldBar_Blue::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Boss_ShieldBar_Blue* pInstance = new CUI_Boss_ShieldBar_Blue(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Boss_ShieldBar_Blue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_ShieldBar_Blue::Free()
{
	__super::Free();
}
