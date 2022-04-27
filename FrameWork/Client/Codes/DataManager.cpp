#include "pch.h"
#include "DataManager.h"
#include "ScriptableData.h"

#include "PlayerData.h"
#include "InventoryData.h"
#include "EquipmentData.h"
#include "WeaponData.h"

CDataManager::CDataManager()
{
}

HRESULT CDataManager::NativeConstruct(void)
{
	/* for. CPlayerData */
	CPlayerData* pPlayerData = new CPlayerData;
	ADD_DATA(L"PlayerData", pPlayerData);
	/* for. CInventoryData */
	CInventoryData* pInventoryData = new CInventoryData();
	ADD_DATA(L"InventoryData", pInventoryData);
	/*for. CEquipmentData */
	CEquipmentData* pEquipmentData = new CEquipmentData();
	ADD_DATA(L"EquipmentData", pEquipmentData);

	/* Default Weapon */
	CWeaponData::Desc weaponDesc;
	weaponDesc.weaponName = L"Needle";
	weaponDesc.damage = 30.f;
	weaponDesc.weaponType = EWeaponType::LongSword;
	
	CWeaponData weaponData(weaponDesc);

	/* Default Weapon ItemData */
	CItemData weaponItemData;
	weaponItemData.equipmentGrade = EEquipmentGrade::Common;
	weaponItemData.equipmentType = EEquipmentType::Weapon;
	weaponItemData.ItemType = EItemType::Equipment;
	weaponItemData.iconTexName = L"T_Weapon_Sword_1H_Player_Needle";
	weaponItemData.weaponData = weaponData;
	weaponItemData.bEquiped = true;
	weaponItemData.equipmentName = EEquipmentName::Needle;
	weaponItemData.szStatusName = L"T_1H_DPS_100";
	weaponItemData.weaponType = EWeaponType::LongSword;

	pInventoryData->PushItem(weaponItemData);
	pEquipmentData->SetEquipment(EEquipSlot::Weapon1, weaponItemData);
	
	return S_OK;
}

_int CDataManager::Tick(void)
{
	return _int();
}

void CDataManager::AddScriptableData(std::wstring Key, CScriptableData* pData)
{
	m_scriptableDatas.insert(std::make_pair(Key, pData));
}

CScriptableData* CDataManager::GetScriptableData(std::wstring Key)
{
	return m_scriptableDatas[Key];
}

void CDataManager::Free()
{
	for (auto& data : m_scriptableDatas)
	{
		Safe_Delete(data.second);
	}
	m_scriptableDatas.clear();
}
