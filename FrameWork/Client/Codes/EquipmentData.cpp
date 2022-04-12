#include "pch.h"
#include "EquipmentData.h"

CEquipmentData::CEquipmentData(void)
{
	m_vecEquipmentItems.resize((_int)EEquipSlot::Max);
}

_bool CEquipmentData::IsExistEquip(EEquipSlot slot)
{ /* 장착하려는 슬롯이 비어있는가 */
	EItemType itemType = m_vecEquipmentItems[(_int)slot].ItemType;

	if (EItemType::NONE == itemType)
		return false;
	else
		return true;
}

_bool CEquipmentData::ExistItem(CItemData item)
{
	for (auto EquipmentItem : m_vecEquipmentItems)
	{
		if (EquipmentItem.uid == item.uid)
			return true;
	}
	return false;
}

_bool CEquipmentData::CompareEquipment(EEquipSlot slot, CItemData item)
{
	return _bool();
}

void CEquipmentData::ChangeEquipment(EEquipSlot slot, CItemData item)
{
	if (IsExistEquip(slot))
		return;
	else
	{
		m_vecEquipmentItems[(_int)slot] = item;
		m_vecEquipmentItems[(_int)slot].bEquiped = TRUE;
	}
}

void CEquipmentData::RemoveEquipment(EEquipSlot slot)
{
}

void CEquipmentData::RemoveEquipment(CItemData item)
{
}

CItemData CEquipmentData::GetEquipment(EEquipSlot slot)
{
	if (m_vecEquipmentItems.size() <= (_int)slot)
	{
		return CItemData();
	}
	return m_vecEquipmentItems[(_int)slot];
}

void CEquipmentData::SetEquipment(EEquipSlot slot, CItemData _data)
{
	m_vecEquipmentItems[(_int)slot] = _data;
}
