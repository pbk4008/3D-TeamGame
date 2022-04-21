#include "pch.h"
#include "DropBoxData.h"

CDropBoxData::CDropBoxData(void)
{
	{
		CItemData item1;
		item1.iconTexName = L"T_Weapon_Hammer_2H_Player_Hinterclaw";
		item1.equipmentGrade = EEquipmentGrade::Epic;
		item1.ItemType = EItemType::Equipment;
		item1.equipmentType = EEquipmentType::Weapon;
		item1.szItemName = L"JinSung's Sword";
		item1.iMainStat = 52;
		item1.iLevel = 10;
		item1.weaponData.weaponName = L"Hinterclaw";
		item1.equipmentName = EEquipmentName::Hinterclaw;
		item1.szStatusName = L"T_2H_DPS_147";

		CItemData item2;
		item2.iconTexName = L"T_Weapon_Hammer_2H_Player_Harbinger";
		item2.equipmentGrade = EEquipmentGrade::Rare;
		item2.ItemType = EItemType::Equipment;
		item2.equipmentType = EEquipmentType::Weapon;
		item2.szItemName = L"SuBin's Sword";
		item2.iMainStat = 20;
		item2.iLevel = 16;
		item2.szWeaponName = L"2";
		item2.weaponData.weaponName = L"Harbinger";
		item2.equipmentName = EEquipmentName::Harbinger;
		item2.szStatusName = L"T_2H_DPS_132";

		CItemData item3;
		item3.iconTexName = L"T_Weapon_Sword_1H_Player_Crossblade";
		item3.equipmentGrade = EEquipmentGrade::Rare;
		item3.ItemType = EItemType::Equipment;
		item3.equipmentType = EEquipmentType::Weapon;
		item3.szItemName = L"YoungMo's Sword";
		item3.iMainStat = 20;
		item3.iLevel = 16;
		item3.szWeaponName = L"3";
		item3.weaponData.weaponName = L"Crossblade";
		item3.equipmentName = EEquipmentName::Crossblade;
		item3.szStatusName = L"T_2H_DPS_135";

		m_vecIteamData[0].push_back(item1);
		m_vecIteamData[0].push_back(item2);
		m_vecIteamData[0].push_back(item3);
	}
	{
		CItemData item1;
		item1.iconTexName = L"T_Weapon_Hammer_2H_Player_Hinterclaw";
		item1.equipmentGrade = EEquipmentGrade::Epic;
		item1.ItemType = EItemType::Equipment;
		item1.equipmentType = EEquipmentType::Weapon;
		item1.szItemName = L"JinSung's Sword";
		item1.iMainStat = 52;
		item1.iLevel = 10;
		item1.weaponData.weaponName = L"Hinterclaw";
		item1.equipmentName = EEquipmentName::Hinterclaw;
		item1.szStatusName = L"T_2H_DPS_147";

		CItemData item2;
		item2.iconTexName = L"T_Weapon_Hammer_2H_Player_Harbinger";
		item2.equipmentGrade = EEquipmentGrade::Rare;
		item2.ItemType = EItemType::Equipment;
		item2.equipmentType = EEquipmentType::Weapon;
		item2.szItemName = L"SuBin's Sword";
		item2.iMainStat = 20;
		item2.iLevel = 16;
		item2.szWeaponName = L"2";
		item2.weaponData.weaponName = L"Harbinger";
		item2.equipmentName = EEquipmentName::Harbinger;
		item2.szStatusName = L"T_2H_DPS_132";

		CItemData item3;
		item3.iconTexName = L"T_Weapon_Sword_1H_Player_Crossblade";
		item3.equipmentGrade = EEquipmentGrade::Rare;
		item3.ItemType = EItemType::Equipment;
		item3.equipmentType = EEquipmentType::Weapon;
		item3.szItemName = L"YoungMo's Sword";
		item3.iMainStat = 20;
		item3.iLevel = 16;
		item3.szWeaponName = L"3";
		item3.weaponData.weaponName = L"Crossblade";
		item3.equipmentName = EEquipmentName::Crossblade;
		item3.szStatusName = L"T_2H_DPS_135";

		m_vecIteamData[1].push_back(item1);
		m_vecIteamData[1].push_back(item2);
		m_vecIteamData[1].push_back(item3);
	}
	{
		CItemData item1;
		item1.iconTexName = L"T_Weapon_Hammer_2H_Player_Hinterclaw";
		item1.equipmentGrade = EEquipmentGrade::Epic;
		item1.ItemType = EItemType::Equipment;
		item1.equipmentType = EEquipmentType::Weapon;
		item1.szItemName = L"JinSung's Sword";
		item1.iMainStat = 52;
		item1.iLevel = 10;
		item1.weaponData.weaponName = L"Hinterclaw";
		item1.equipmentName = EEquipmentName::Hinterclaw;
		item1.szStatusName = L"T_2H_DPS_147";

		CItemData item2;
		item2.iconTexName = L"T_Weapon_Hammer_2H_Player_Harbinger";
		item2.equipmentGrade = EEquipmentGrade::Rare;
		item2.ItemType = EItemType::Equipment;
		item2.equipmentType = EEquipmentType::Weapon;
		item2.szItemName = L"SuBin's Sword";
		item2.iMainStat = 20;
		item2.iLevel = 16;
		item2.szWeaponName = L"2";
		item2.weaponData.weaponName = L"Harbinger";
		item2.equipmentName = EEquipmentName::Harbinger;
		item2.szStatusName = L"T_2H_DPS_132";

		CItemData item3;
		item3.iconTexName = L"T_Weapon_Sword_1H_Player_Crossblade";
		item3.equipmentGrade = EEquipmentGrade::Rare;
		item3.ItemType = EItemType::Equipment;
		item3.equipmentType = EEquipmentType::Weapon;
		item3.szItemName = L"YoungMo's Sword";
		item3.iMainStat = 20;
		item3.iLevel = 16;
		item3.szWeaponName = L"3";
		item3.weaponData.weaponName = L"Crossblade";
		item3.equipmentName = EEquipmentName::Crossblade;
		item3.szStatusName = L"T_2H_DPS_135";

		m_vecIteamData[2].push_back(item1);
		m_vecIteamData[2].push_back(item2);
		m_vecIteamData[2].push_back(item3);
	}
}
