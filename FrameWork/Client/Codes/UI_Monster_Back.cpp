#include "framework.h"
#include "pch.h"
#include "UI_Monster_Back.h"
#include "GameInstance.h"

CUI_Monster_Back::CUI_Monster_Back(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Monster_Back::CUI_Monster_Back(const CUI_Monster_Back& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
{
}

HRESULT CUI_Monster_Back::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Monster_Back::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIACTIVEDESC));
	}

	if (FAILED(__super::NativeConstruct(_iSceneID, pArg)))
	{
		return E_FAIL;
	}

	m_iObectTag = m_Desc.UIDesc.IDTag;

	if (FAILED(m_pTexture->Change_Texture(m_Desc.UIDesc.TextureTag)))
		return E_FAIL;

	/* 복제받은 데이터로 내가 원하는 값 세팅 */
	if (FAILED(SetUp_Components()))
	{
		return E_FAIL;
	}

	m_fGapX = 1.f;
	m_fGapY = 0.63f;

	setActive(false);
	m_bShow = false;

	return S_OK;
}

_int CUI_Monster_Back::Tick(_double TimeDelta)
{
	//if (false == m_bFirstShow)
	//{
	//	setActive(true);
	//	m_bFirstShow = true;
	//}

	if (true == m_bShow)
	{
		m_fAlpha = 1.f;
		m_fDisappearTimeAcc = 0.f;

		m_bAutoDis = true; //true로 들어왔을대 제대로 안꺼지게되면 자동으로꺼질수있게
	}

	if (m_bAutoDis && m_bShow) //m_bshow가 false가안되어버리고 버그가있을때 오토로 끌수있게끔
	{
		m_fAutoDisTimeAcc += (_float)TimeDelta;
	}

	if (5.f <= m_fAutoDisTimeAcc) //완전 처음세팅으로 다시 돌려줌
	{
		m_fAutoDisTimeAcc = 0.f;
		m_bAutoDis = false;
		m_fAlpha = 0.f;
		m_fDisappearTimeAcc = 0.f;
		m_bShow = false;
		setActive(false);
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
	
	return 0;
}

_int CUI_Monster_Back::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI, this);
	}
	return _int();
}

HRESULT CUI_Monster_Back::Render()
{
	if (m_bAutoDis)
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

		m_pTrapziumBuffer->Render(1); //테스트로하면 알파값적용이안돼서 바로꺼지는거같음, 근데 블랜드하면,, 소팅해줘야함,,
	}
	return S_OK;
}

HRESULT CUI_Monster_Back::SetUp_Components()
{
	CVIBuffer_Trapezium::TRAPDESC Desc;
	Desc.fAngle = m_Desc.UIDesc.fAngle;
	_tcscpy_s(Desc.ShaderFilePath, L"../../Reference/ShaderFile/Shader_UI_Enemy_Bar.hlsl");

	Desc.bMinus = m_Desc.UIDesc.bMinus;

	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Trapezium_UI", L"Com_Trapezium_UI", (CComponent**)&m_pTrapziumBuffer, &Desc)))
		return E_FAIL;

	//m_pTrapziumBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Trapezium>(0, L"Proto_Component_Trapezium_UI", &Desc);

	//if (!m_pTrapziumBuffer)
	//	return E_FAIL;

	//if (FAILED(CGameObject::SetUp_Components(L"Com_Trapezium_UI", m_pTrapziumBuffer)))
	//	return E_FAIL;

	return S_OK;
}

CUI_Monster_Back* CUI_Monster_Back::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Monster_Back* pInstance = new CUI_Monster_Back(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Monster_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_Back::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Monster_Back* pInstance = new CUI_Monster_Back(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Monster_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_Back::Free()
{
	__super::Free();
}
