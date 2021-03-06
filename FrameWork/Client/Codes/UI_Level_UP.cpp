#include "pch.h"
#include "UI_Level_UP.h"
#include "UI_LevelUP_Background.h"
#include "UI_LevelUP_BG_Left.h"
#include "UI_LevelUP_BG_Right.h"
#include "UI_LevelUP_Fill_Lead_Left.h"
#include "UI_LevelUP_Fill_Lead_Right.h"
#include "UI_LevelUP_Fill_Left.h"
#include "UI_LevelUP_Fill_Right.h"
#include "UI_LevelHUD.h"

CLevel_UP::CLevel_UP(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CLevel_UP::CLevel_UP(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CLevel_UP::NativeConstruct_Prototype(void)
{
	if (FAILED(CUI::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_UP::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CUI::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	setActive(false);

	return S_OK;
}

_int CLevel_UP::Tick(_double dDeltaTime)
{
	dDeltaTime = g_dImmutableTime;
	m_pFIll_Left->Tick(dDeltaTime);
	m_pFIll_Right->Tick(dDeltaTime);

	if (this->getActive())
		ShowUp(dDeltaTime);

	if (true == m_pLevelUp_HUD->getActive())
	{
		m_fTimeLevelUp_HUD += (_float)dDeltaTime;

		if (3.f <= m_fTimeLevelUp_HUD)
		{
			m_fTimeLevelUp_HUD = 0.f;
			HideLevelUp_HUD();
		}

	}
	return _int();
}

_int CLevel_UP::LateTick(_double TimeDelta)
{
	TimeDelta = g_dImmutableTime;
	m_pBg->LateTick(TimeDelta);

	m_pBgLeft->LateTick(TimeDelta);
	m_pBgRight->LateTick(TimeDelta);

	m_pFill_Lead_Left->LateTick(TimeDelta);
	m_pFill_Lead_Right->LateTick(TimeDelta);

	m_pFIll_Left->LateTick(TimeDelta);
	m_pFIll_Right->LateTick(TimeDelta);

	if (m_pLevelUp_HUD->getActive())
		m_pLevelUp_HUD->LateTick(TimeDelta);

	return _int();
}

HRESULT CLevel_UP::Render()
{
	return S_OK;
}

HRESULT CLevel_UP::Ready_Component(void)
{
	return S_OK;
}

HRESULT CLevel_UP::Ready_UIObject(void)
{
	{
		CUI_LevelUP_Background::Desc desc;
		desc.pOwner = this;
		m_pBg = (CUI_LevelUP_Background*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LevelUp_Background", &desc));
		assert(m_pBg);
	}
	/* for. left set */
	{
		CUI_LevelUP_BG_Left::Desc desc;
		desc.pOwner = this;
		m_pBgLeft = (CUI_LevelUP_BG_Left*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LevelUp_BG_Left", &desc));
		assert(m_pBg);
	}
	{
		CUI_LevelUP_Fill_Lead_Left::Desc desc;
		desc.pOwner = this;
		m_pFill_Lead_Left = (CUI_LevelUP_Fill_Lead_Left*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LevelUp_Fill_Lead_Left", &desc));
		assert(m_pFill_Lead_Left);
	}
	{
		UI_LevelUP_Fill_Left::Desc desc;
		desc.pOwner = this;
		m_pFIll_Left = (UI_LevelUP_Fill_Left*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LevelUp_Fill_Left", &desc));
		assert(m_pFIll_Left);

	}

	/* for. right set */
	{
		CUI_LevelUP_BG_Right::Desc desc;
		desc.pOwner = this;
		m_pBgRight = (CUI_LevelUP_BG_Right*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LevelUp_BG_Right", &desc));
		assert(m_pBg);
	}
	{
		CUI_LevelUP_Fill_Lead_Right::Desc desc;

		desc.pOwner = this;
		m_pFill_Lead_Right = (CUI_LevelUP_Fill_Lead_Right*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LevelUp_Fill_Lead_Right", &desc));
		assert(m_pFill_Lead_Right);

	}
	{
		UI_LevelUP_Fill_Right::Desc desc;
		desc.pOwner = this;
		m_pFIll_Right = (UI_LevelUP_Fill_Right*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_LevelUp_Fill_Right", &desc));
		assert(m_pFIll_Right);

	}
	/* HUD */
	{
		m_pLevelUp_HUD = (CUI_LevelHUD*) static_cast<CUI*>(
			g_pGameInstance->Clone_GameObject((_uint)SCENEID::SCENE_STATIC, L"Proto_GameObject_UI_Level_HUD"));
		assert(m_pLevelUp_HUD);
	}
	return S_OK;











































































}

void CLevel_UP::Show(CPlayerData* pPlayerData)
{
	this->setActive(true);
    FadeInAll();

	m_pBg->setActive(true);

	m_pBgLeft->setActive(true);
	m_pBgRight->setActive(true);

	m_pFill_Lead_Left->setActive(true);
	m_pFill_Lead_Right->setActive(true);

	m_pFIll_Left->SetUI(pPlayerData);
	m_pFIll_Right->SetUI(pPlayerData);

	m_pFill_Lead_Left->Set_Fill(m_pFIll_Right);
	m_pFill_Lead_Right->Set_Fill(m_pFIll_Right);

	m_pFIll_Left->setActive(true);
	m_pFIll_Right->setActive(true);
}

void CLevel_UP::Hide(void)
{
	if (m_fHideYInitPos > m_fHideYEndPos)
	{
		m_fHideYInitPos -= g_fImmutableTime * 250.f;

		if (m_fHideYInitPos <= m_fHideYEndPos)
		{
			m_fHideYInitPos = m_fHideYEndPos;
			m_fInitYPos = -100.f;
			this->setActive(false);
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f, m_fHideYInitPos, 0.f, 1.f });
		FadeOutAll();
	}
}

void CLevel_UP::ShowUp(_double TimeDelta)
{
	if (m_fInitYPos < m_fEndYPos)
	{
		m_fInitYPos += (_float)TimeDelta * 250.f;
		if (m_fInitYPos >= m_fEndYPos)
		{
			m_fInitYPos = m_fEndYPos;
			m_fHideYInitPos = 0.f;
		}
		m_pTransform->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f, m_fInitYPos, 0.f, 1.f });
		FadeInAll();
	}
}

void CLevel_UP::FadeOutAll(void)
{
	m_pBg->SetFadeOut();
	m_pBgLeft->SetFadeOut();
	m_pBgRight->SetFadeOut();
	m_pFill_Lead_Left->SetFadeOut();
	m_pFill_Lead_Right->SetFadeOut();
	m_pFIll_Left->SetFadeOut();
	m_pFIll_Right->SetFadeOut();
}

void CLevel_UP::FadeInAll(void)
{
	m_pBg->FadeIn();
	m_pBgLeft->FadeIn();
	m_pBgRight->FadeIn();
	m_pFill_Lead_Left->FadeIn();
	m_pFill_Lead_Right->FadeIn();
	m_pFIll_Left->FadeIn();
	m_pFIll_Right->FadeIn();
}

void CLevel_UP::ShowLevelUp_HUD(_int iLevel)
{
	/* Level 0 ???? 1?????? */
	m_pLevelUp_HUD->SetImage(m_arrLevelTex[iLevel]);
	m_pLevelUp_HUD->setActive(true);
}

void CLevel_UP::HideLevelUp_HUD(void)
{
	m_pLevelUp_HUD->setActive(false);
}

void CLevel_UP::SetLevelBG(_int PlayerLevel)
{
	m_pBg->SetBg(PlayerLevel);
}

void CLevel_UP::FixPos(void)
{
	m_pBg->FixPos();
}

CLevel_UP* CLevel_UP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CLevel_UP* pInstance = new CLevel_UP(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CLevel_UP Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLevel_UP::Clone(const _uint iSceneID, void* pArg)
{
	CLevel_UP* pInstance = new CLevel_UP(*this);
	if (FAILED(pInstance->NativeConstruct(iSceneID, pArg)))
	{
		MSGBOX("CLevel_UP Clone Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_UP::Free()
{
	__super::Free();

	Safe_Release(m_pBg);
	Safe_Release(m_pBgLeft);
	Safe_Release(m_pBgRight);
	Safe_Release(m_pFIll_Left);
	Safe_Release(m_pFIll_Right);
	Safe_Release(m_pFill_Lead_Left);
	Safe_Release(m_pFill_Lead_Right);
	Safe_Release(m_pLevelUp_HUD);
}
