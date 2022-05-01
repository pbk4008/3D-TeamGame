#include "pch.h"
#include "Inven_UIManager.h"
#include "UI_ModalWindow.h"
#include "UI_ItemStatusWindow.h"
#include "UI_PlayerStatusWindow.h"
#include "Hud.h"

CInven_UIManager::CInven_UIManager(void)
{
}

HRESULT CInven_UIManager::NativeConstruct(void)
{
	/* for. Proto_GameObject_UI_ModalWindow */
	if(FAILED(g_pGameInstance->Add_GameObjectToLayer(
		(_uint)SCENEID::SCENE_STATIC, 
		L"Layer_UI_Inventory", 
		L"Proto_GameObject_UI_ModalWindow", 
		nullptr, 
		(CGameObject**)&m_pModal))
		)
		return E_FAIL;

	m_pModal->Hide();

	/* for. Proto_GameObject_UI_ItemStatusWindow */
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(
		(_uint)SCENEID::SCENE_STATIC,
		L"Layer_UI_Inventory",
		L"Proto_GameObject_UI_ItemStatusWindow",
		nullptr,
		(CGameObject**)&m_pItemStatus))
		)
		return E_FAIL;

	m_pItemStatus->Hide();

	/* for. Proto_GameObject_UI_PlayerStatus_Window */

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(
		(_uint)SCENEID::SCENE_STATIC,
		L"Layer_UI_Inventory",
		L"Proto_GameObject_UI_PlayerStatus_Window",
		nullptr,
		(CGameObject**)&m_pPlayerStatus))
		)
		return E_FAIL;

	m_pPlayerStatus->Hide();

	/* for. Proto_GameObject_UI_Hud */
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(
		(_uint)SCENEID::SCENE_STATIC,
		L"Layer_UI_Hud",
		L"Proto_GameObject_UI_Hud",
		nullptr,
		(CGameObject**)&m_pHud))
		)
		return E_FAIL;

	return S_OK;
}

_int CInven_UIManager::Tick(_double _dTimeDelta)
{

	//if (m_pHud)
	//	m_pHud->Tick(g_dImmutableTime);
	

	m_pHud->EquipmentRenderNo(m_bRender);


	return _int();
}

void CInven_UIManager::OnModalOpen(void*)
{
}

void CInven_UIManager::OnModalClose(void*)
{
}

void CInven_UIManager::OpenModal(void)
{
	m_pModal->Show();
	m_bOpenModal = true;
}

void CInven_UIManager::CloseModal(void)
{
	m_pModal->Hide();
	m_bOpenModal = false;

	HideItemStatus();
}

_bool CInven_UIManager::IsOpenModal(void)
{
	return m_bOpenModal;
}

void CInven_UIManager::ShowLevelUp(void)
{
	m_pHud->ShowLevelUp_HUD();
}

void CInven_UIManager::ShowLevelUpHUD(_int iLevel)
{
	m_pHud->ShowLevelUp_HUD(iLevel);
}

void CInven_UIManager::ShowMapInfo(_bool OnOff, _int TexIdx/* = 0*/)
{
	m_pHud->ShowMapInfo_HUD(OnOff, TexIdx);
}

CHud* CInven_UIManager::GetHud(void)
{
	if(nullptr == m_pHud)
		return nullptr;

	return m_pHud;
}

void CInven_UIManager::ShowItemStatus(CItemData* _pItemData)
{
	if (m_pItemStatus)
	{
		m_pItemStatus->Show(_pItemData);
		if (m_pModal->OpenArmory())
			m_pPlayerStatus->Hide();
		else
			m_pPlayerStatus->Show();
	}
}

void CInven_UIManager::HideItemStatus(void)
{
	if (m_pItemStatus)
	{
		m_pItemStatus->Hide();
		if (m_pModal->OpenEquipment())
			m_pPlayerStatus->Show();
		else
			m_pPlayerStatus->Hide();
	}
}

void CInven_UIManager::SetLevelBG(_int PlayerLevel)
{
	m_pHud->SetLevelBG(PlayerLevel);
}

void CInven_UIManager::Free()
{
}
