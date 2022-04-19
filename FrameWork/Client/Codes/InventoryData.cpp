#include "pch.h"
#include "InventoryData.h"
#include "Hud.h"

CInventoryData::CInventoryData(void)
{
	InventoryItems.reserve(ModalMaxCount);
}

_bool CInventoryData::PushItem(CItemData itemData)
{
	if (IsFull())
		return false;
	else
	{
		InventoryItems.push_back(itemData);
		
		if (nullptr != g_pInvenUIManager)
		{
			CHud* pHud = g_pInvenUIManager->GetHud();
			assert("pHud is nullptr!!" && pHud);
			pHud->OnLootEquipment(&itemData);
		}

		return true;
	}
}

void CInventoryData::AddResource(_int iResource)
{
	m_iResources += iResource;
}

void CInventoryData::RemoveItem(CItemData itemData)
{
	for (_int i = 0; i < InventoryItems.size(); ++i)
	{
		if (InventoryItems[i].uid == itemData.uid)
		{
			InventoryItems.erase(InventoryItems.begin() + i);
			i--;
			break;
		}
	}
}

void CInventoryData::RemoveResource(_int iResource)
{
	m_iResources -= iResource;
}

_bool CInventoryData::ExistItem(_int iModalIndex)
{
	if (InventoryItems.size() <= iModalIndex)
		return false;
	return true;
}

_bool CInventoryData::TryGetItem(_int iIndex, CItemData* pItemData)
{
	if (ExistItem(iIndex))
	{
		*pItemData = InventoryItems[iIndex];
		return true;
	}
	else
		return false;
}

CItemData CInventoryData::GetItem(_int iIndex)
{
	return InventoryItems[iIndex];
}

CItemData CInventoryData::GetItem(void)
{
	return InventoryItems.back();
}

CItemData* CInventoryData::GetItemByID(_ulong _uid)
{
	for (auto& item : InventoryItems)
	{
		if (item.uid == _uid)
			return &item;
	}
	return nullptr;
}

_int CInventoryData::GetCount(void)
{
	return (_int)InventoryItems.size();
}

_bool CInventoryData::IsFull(void)
{
	return InventoryItems.size() >= ModalMaxCount;
}

void CInventoryData::SetEquiped(_int iIndex, _bool bEquiped)
{
	InventoryItems[iIndex].bEquiped = bEquiped;
}

void CInventoryData::SetResource(_int iResource)
{
	m_iResources = iResource;
}



