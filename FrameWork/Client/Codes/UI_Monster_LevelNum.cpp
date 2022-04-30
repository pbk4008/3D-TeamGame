#include "framework.h"
#include "pch.h"
#include "UI_Monster_LevelNum.h"
#include "GameInstance.h"

CUI_Monster_LevelNum::CUI_Monster_LevelNum(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice,pDeviceContext)
{
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

CUI_Monster_LevelNum::CUI_Monster_LevelNum(const CUI_Monster_LevelNum& rhs)
	: CUI(rhs)
	, m_Desc(rhs.m_Desc)
{
}

HRESULT CUI_Monster_LevelNum::NativeConstruct_Prototype()
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Monster_LevelNum::NativeConstruct(const _uint _iSceneID, void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_Desc, pArg, sizeof(UIACTIVEDESC));
	}

	if (FAILED(CGameObject::NativeConstruct(_iSceneID, pArg)))
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

	setActive(false);
	m_bShow = false;

	return S_OK;
}

_int CUI_Monster_LevelNum::Tick(_double TimeDelta)
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
		m_fAutoDisTimeAcc += TimeDelta;
	}

	if (5.f <= m_fAutoDisTimeAcc) //완전 처음세팅으로 다시 돌려줌
	{
		m_bAutoDis = false;
		m_fAutoDisTimeAcc = 0.f;
		m_fAlpha = 0.f;
		m_fDisappearTimeAcc = 0.f;
		m_bShow = false;
		//setActive(false);
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

_int CUI_Monster_LevelNum::LateTick(_double TimeDelta)
{
	if (FAILED(CUI::LateTick(TimeDelta)))
		return -1;

	if (nullptr != m_pRenderer)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_ALPHANB, this);
	}
	return _int();
}

HRESULT CUI_Monster_LevelNum::Render()
{
	if (m_bAutoDis)
	{
		wstring wstrCamTag = g_pGameInstance->Get_BaseCameraTag();
		_matrix XMWorldMatrix = XMMatrixTranspose(m_pTransform->Get_WorldMatrix());
		_matrix XMViewMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_VIEW));
		_matrix XMProjectMatrix = XMMatrixTranspose(g_pGameInstance->Get_Transform(wstrCamTag, TRANSFORMSTATEMATRIX::D3DTS_PROJECTION));

		m_pBuffer->SetUp_ValueOnShader("g_WorldMatrix", &XMWorldMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ViewMatrix", &XMViewMatrix, sizeof(_float) * 16);
		m_pBuffer->SetUp_ValueOnShader("g_ProjMatrix", &XMProjectMatrix, sizeof(XMMATRIX));
		m_pBuffer->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));

		m_pBuffer->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture, m_Desc.iTextureNum);

		m_pBuffer->Render(1);
	}
	
	return S_OK;
}

HRESULT CUI_Monster_LevelNum::SetUp_Components()
{
	if (FAILED(CGameObject::SetUp_Components((_uint)SCENEID::SCENE_STATIC, L"Proto_Component_Rect_Panel_UI", L"Com_Rect_Panel_UI", (CComponent**)&m_pBuffer)))
		return E_FAIL;


	//m_pBuffer = g_pGameInstance->Clone_Component<CVIBuffer_Rect>(0, L"Proto_Component_Rect_UI");

	//if (!m_pBuffer)
	//	return E_FAIL;

	//if (FAILED(CGameObject::SetUp_Components(L"Com_Rect_UI", m_pBuffer)))
	//	return E_FAIL;


	return S_OK;
}

CUI_Monster_LevelNum* CUI_Monster_LevelNum::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	/* 원형객체 생성할때 초기화 */
	CUI_Monster_LevelNum* pInstance = new CUI_Monster_LevelNum(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI_Monster_LevelNum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_LevelNum::Clone(const _uint _iSceneID, void* pArg)
{
	CUI_Monster_LevelNum* pInstance = new CUI_Monster_LevelNum(*this);
	if (FAILED(pInstance->NativeConstruct(_iSceneID, pArg)))
	{
		MSGBOX("Failed to Creating Clone CUI_Monster_LevelNum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_LevelNum::Free()
{
	__super::Free();
}
