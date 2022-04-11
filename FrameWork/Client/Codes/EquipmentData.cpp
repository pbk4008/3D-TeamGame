#include "pch.h"
#include "EquipmentData.h"

CEquipmentData::CEquipmentData(void)
{
	m_vecEquipmentItems.resize((_int)EEquipSlot::Max);
}

_bool CEquipmentData::IsExistEquip(EEquipSlot slot)
{
	return _bool();
}

_bool CEquipmentData::ExistItem(CItemData item)
{
	return _bool();
}

_bool CEquipmentData::CompareEquipment(EEquipSlot slot, CItemData item)
{
	return _bool();
}

void CEquipmentData::ChangeEquipment(EEquipSlot slot, CItemData item)
{
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
