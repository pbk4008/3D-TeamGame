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


	//CItemData item3;
	//item3.iconTexName = L"T_Weapon_Sword_1H_Player_Eclipse_Gold";
	//item3.equipmentGrade = EEquipmentGrade::Legendary;
	//item3.ItemType = EItemType::Equipment;
	//item3.equipmentType = EEquipmentType::Weapon;
	//item3.szItemName = L"HeeDong's Sword_3";
	//item3.iMainStat = 20;
	//item3.iLevel = 16;
	//item3.szWeaponName = L"7";
	//item3.weaponData.weaponName = L"Eclipse";
	//item3.equipmentName = EEquipmentName::Eclipse;
	//item3.szStatusName = L"T_1H_DPS_145";
	//item3.weaponType = EWeaponType::LongSword;

	//CItemData item1;
	//item1.iconTexName = L"T_Weapon_Hammer_2H_Player_Legend1";
	//item1.equipmentGrade = EEquipmentGrade::Legendary;
	//item1.ItemType = EItemType::Equipment;
	//item1.equipmentType = EEquipmentType::Weapon;
	//item1.szItemName = L"HeeDong's Sword_6";
	//item1.iMainStat = 20;
	//item1.iLevel = 16;
	//item1.weaponData.weaponName = L"Legend1";
	//item1.equipmentName = EEquipmentName::Legend1;
	//item1.szStatusName = L"T_2H_DPS_156";
	//item1.weaponType = EWeaponType::Hammer;

	pInventoryData->PushItem(weaponItemData);
	//pInventoryData->PushItem(item3);
	//pInventoryData->PushItem(item1);

	pEquipmentData->SetEquipment(EEquipSlot::Weapon1, weaponItemData);
	//pEquipmentData->SetEquipment(EEquipSlot::Weapon2, item1);

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
