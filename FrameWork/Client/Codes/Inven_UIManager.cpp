#include "pch.h"
#include "Inven_UIManager.h"
#include "UI_ModalWindow.h"

CInven_UIManager::CInven_UIManager(void)
{
}

HRESULT CInven_UIManager::NativeConstruct(void)
{
	if(FAILED(g_pGameInstance->Add_GameObjectToLayer(
		(_uint)SCENEID::SCENE_STATIC, 
		L"Layer_UI_Inventory", 
		L"Proto_GameObject_UI_ModalWindow", 
		nullptr, 
		(CGameObject**)&m_pModal))
		)
		return S_OK;

	m_pModal->Hide();
}

_int CInven_UIManager::Tick(void)
{
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
	
}

void CInven_UIManager::HideItemStatus(void)
{
}

void CInven_UIManager::Free()
{
}
