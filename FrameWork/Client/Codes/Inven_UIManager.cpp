#include "pch.h"
#include "Inven_UIManager.h"
#include "UI_ModalWindow.h"
#include "UI_ItemStatusWindow.h"
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
		return S_OK;

	m_pModal->Hide();

	/* for. Proto_GameObject_UI_ItemStatusWindow */
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(
		(_uint)SCENEID::SCENE_STATIC,
		L"Layer_UI_Inventory",
		L"Proto_GameObject_UI_ItemStatusWindow",
		nullptr,
		(CGameObject**)&m_pItemStatus))
		)
		return S_OK;

	m_pItemStatus->Hide();

	/* for. Proto_GameObject_UI_Hud */
	/*if (FAILED(g_pGameInstance->Add_GameObjectToLayer(
		(_uint)SCENEID::SCENE_STATIC,
		L"Layer_UI_Hud",
		L"Proto_GameObject_UI_Hud",
		nullptr,
		(CGameObject**)&m_pHud))
		)
		return S_OK;*/

	return S_OK;
}

_int CInven_UIManager::Tick(_double _dTimeDelta)
{
	if (m_pHud)
		m_pHud->Tick(_dTimeDelta);

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

void CInven_UIManager::ShowItemStatus(CItemData* _pItemData)
{
	if (m_pItemStatus)
		m_pItemStatus->Show(_pItemData);
}

void CInven_UIManager::HideItemStatus(void)
{
	if (m_pItemStatus)
		m_pItemStatus->Hide();
}

void CInven_UIManager::Free()
{
}
