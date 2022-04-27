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


	/* Default Weapon ItemData */
	CItemData weaponItemData2;
	weaponItemData2.equipmentGrade = EEquipmentGrade::Common;
	weaponItemData2.equipmentType = EEquipmentType::Weapon;
	weaponItemData2.ItemType = EItemType::Equipment;
	weaponItemData2.iconTexName = L"T_Weapon_Hammer_2H_Player_INE";
	weaponItemData2.weaponData = weaponData;
	weaponItemData2.bEquiped = true;
	weaponItemData2.equipmentName = EEquipmentName::Legend5;
	weaponItemData2.szStatusName = L"T_1H_DPS_100";
	weaponItemData2.weaponType = EWeaponType::Hammer;

	pInventoryData->PushItem(weaponItemData);
	pInventoryData->PushItem(weaponItemData2);
	pEquipmentData->SetEquipment(EEquipSlot::Weapon1, weaponItemData);
	pEquipmentData->SetEquipment(EEquipSlot::Weapon2, weaponItemData2);

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
