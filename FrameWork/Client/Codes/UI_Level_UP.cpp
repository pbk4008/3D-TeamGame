#include "pch.h"
#include "UI_Level_UP.h"
#include "UI_LevelUP_Background.h"
#include "UI_LevelUP_BG_Left.h"
#include "UI_LevelUP_BG_Right.h"
#include "UI_LevelUP_Fill_Lead_Left.h"
#include "UI_LevelUP_Fill_Lead_Right.h"
#include "UI_LevelUP_Fill_Left.h"
#include "UI_LevelUP_Fill_Right.h"

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
	m_pFIll_Left->Tick(dDeltaTime);
	m_pFIll_Right->Tick(dDeltaTime);

	return _int();
}

_int CLevel_UP::LateTick(_double TimeDelta)
{
	m_pBg->LateTick(TimeDelta);

	m_pBgLeft->LateTick(TimeDelta);
	m_pBgRight->LateTick(TimeDelta);

	m_pFill_Lead_Left->LateTick(TimeDelta);
	m_pFill_Lead_Right->LateTick(TimeDelta);

	m_pFIll_Left->LateTick(TimeDelta);
	m_pFIll_Right->LateTick(TimeDelta);

	return _int();
}

HRESULT CLevel_UP::Render()
{
	//m_pBg->Render();

	//m_pBgLeft->Render();
	//m_pBgRight->Render();

	//m_pFill_Lead_Left->Render();
	//m_pFill_Lead_Right->Render();

	//m_pFIll_Left->Render();
	//m_pFIll_Right->Render();

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
	return S_OK;
}

void CLevel_UP::Show(CPlayerData* pPlayerData)
{
	this->setActive(true);

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
	Safe_Release(m_pFill_Lead_Left);
	Safe_Release(m_pFill_Lead_Right);
	Safe_Release(m_pFIll_Left);
	Safe_Release(m_pFIll_Right);
}
