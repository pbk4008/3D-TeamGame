#include "pch.h"
#include "Quest.h"
#include "UI_QuestText.h"
#include "UI_QuestClear.h"

CQuest::CQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CQuest::CQuest(const CUI& rhs)
	:CUI(rhs)
{
}

HRESULT CQuest::NativeConstruct_Prototype(void)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuest::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(__super::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_UIObject(pArg)))
		return E_FAIL;

	m_szQuestText = g_arrQuestTextTex[(_int)(*(Desc*)pArg).EQuestText];

	setActive(false);

	return S_OK;
}


HRESULT CQuest::Ready_UIObject(void* pArg)
{
	/* Quest ÀÎµ¦½ºx20.f */
	{
		CUI_QuestText::Desc desc;
		desc.EQuestText = (*(Desc*)pArg).EQuestText;
		desc.pOwner = this;
		this->m_pTransform->Get_State(CTransform::STATE_POSITION);

		m_pQuestText = (CUI_QuestText*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_QuestText", &desc));
		assert(m_pQuestText);
	}
	{
		CUI_QuestClear::Desc desc;
		desc.pOwner = this;
		desc.pQuestTextTrans = this->Get_Transform();
		desc.fInitPosY = XMVectorGetY(m_pQuestText->Get_Transform()->Get_State(CTransform::STATE_POSITION));

		m_pQuestClear = (CUI_QuestClear*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_QuestClear", &desc));
		assert(m_pQuestClear);
	}
	return S_OK;
}

_int CQuest::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	if (FAILED(__super::Tick(dDeltaTime)))
		return E_FAIL;

	if (!m_bSetY)
	{
  		m_pQuestText->SetPosy(30.f * m_iIndex);
		m_pQuestClear->SetPosy(30.f * m_iIndex);
		m_bSetY = true;
	}


	if (m_bPosUp)
	{
		if (m_fUpYInitPos < m_fUpYEndPos)
		{
			m_fUpYInitPos += (_float)dDeltaTime * 20;

			if (m_fUpYInitPos >= m_fUpYEndPos)
			{
				m_bPosUp = false;
			}
			m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, m_fUpYInitPos, 0.0f, 1.f });
		}
	}

	m_pQuestText->Tick(dDeltaTime);

	if (m_pQuestClear->getActive())
		m_pQuestClear->Tick(dDeltaTime);

	return _int();
}

_int CQuest::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	if (FAILED(__super::LateTick(TimeDelta)))
		return E_FAIL;

	if (nullptr != m_pRenderer)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER::RENDER_UI_ACTIVE, this);

	if (m_bBye)
	{
		m_fDisapearTime += (_float)TimeDelta;

		if (3.5f <= m_fDisapearTime)
			Pulling(TimeDelta);

	}

	if (m_bPosUp)
	{
		if (m_fUpYInitPos < m_fUpYEndPos)
		{
			m_fUpYInitPos += (_float)TimeDelta * 25;

			if (m_fUpYInitPos >= m_fUpYEndPos)
			{
				m_fUpYInitPos = m_fUpYEndPos;
				m_bPosUp = false;
			}
			m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, m_fUpYInitPos, 0.0f, 1.f });
		}
	}

	m_pQuestText->LateTick(TimeDelta);

	if (m_pQuestClear->getActive())
		m_pQuestClear->LateTick(TimeDelta);

	return _int();
}

HRESULT CQuest::Render()
{
	if (!g_pInvenUIManager->IsOpenModal() &&
		!g_pGuideManager->IsOpenDeathUI())
	{
		m_pQuestText->Render();

		if (m_pQuestClear->getActive())
			m_pQuestClear->Render();
	}
	return S_OK;
}

HRESULT CQuest::Ready_Component(void)
{
	return S_OK;
}


void CQuest::ActiveQuestClear(_bool active)
{
	m_pQuestClear->setActive(active);
	m_bBye = true;
}

void CQuest::Pulling(_double dDeltaTime)
{
	if(m_fDisaperXInitPos > m_fDisaperXEndPos)
	{
		m_fDisaperXInitPos -= (_float)dDeltaTime * 200;

		if (m_fDisaperXInitPos <= m_fDisaperXEndPos)
		{
			m_fDisaperXInitPos = m_fDisaperXEndPos;
			m_bAlive = false;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _vector{ m_fDisaperXInitPos, m_fUpYInitPos, 0.0f, 1.f });
		SetFadeOutAll();
	}
}

void CQuest::SetFadeOutAll(void)
{
	m_pQuestText->SetFadeOut();
	m_pQuestClear->SetFadeOut();
}

void CQuest::SetPosUp(void)
{
	m_bPosUp = true;
	m_iIndex -= 1;
}

CQuest* CQuest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CQuest* pInstance = new CQuest(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CQuest Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CQuest::Clone(const _uint iSceneID, void* pArg)
{
	CQuest* pInstance = new CQuest(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CQuest Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CQuest::Free()
{
	__super::Free();

	Safe_Release(m_pQuestText);
	Safe_Release(m_pQuestClear);
}
