#include "pch.h"
#include "DropBoxData.h"

CDropBoxData::CDropBoxData(void)
{
	{
		CItemData item1;
		item1.iconTexName = L"T_Amulet_BlisteringPulse";
		item1.equipmentGrade = EEquipmentGrade::Legendary;
		item1.ItemType = EItemType::Equipment;
		item1.equipmentType = EEquipmentType::Amulet;
		item1.szItemName = L"HeeDong's Sword_6";
		item1.iMainStat = 20;
		item1.iLevel = 16;
		item1.equipmentName = EEquipmentName::Amulet_BlisteringPulse;

		CItemData item2;
		item2.iconTexName = L"T_Ring_5";
		item2.szStatusName = L"T_INT_60";
		item2.equipmentGrade = EEquipmentGrade::Legendary;
		item2.ItemType = EItemType::Equipment;
		item2.equipmentType = EEquipmentType::Ring;
		item2.szItemName = L"HeeDong's Sword_6";
		item2.iMainStat = 20;
		item2.iLevel = 16;
		item2.equipmentName = EEquipmentName::Ring_5;

		CItemData item3;
		item3.iconTexName = L"T_Weapon_Sword_1H_Player_Eclipse_Gold";
		item3.equipmentGrade = EEquipmentGrade::Legendary;
		item3.ItemType = EItemType::Equipment;
		item3.equipmentType = EEquipmentType::Weapon;
		item3.szItemName = L"HeeDong's Sword_3";
		item3.iMainStat = 20;
		item3.iLevel = 16;
		item3.szWeaponName = L"7";
		item3.weaponData.weaponName = L"Eclipse";
		item3.equipmentName = EEquipmentName::Eclipse;
		item3.szStatusName = L"T_1H_DPS_145";
		item3.weaponType = EWeaponType::LongSword;

		CItemData item4;
		item4.ItemType = EItemType::Potion;

		CItemData item5;
		item5.ItemType = EItemType::Potion;

		CItemData item6;
		item6.ItemType = EItemType::Potion;

		m_vecIteamData[0].push_back(item1);
		m_vecIteamData[0].push_back(item2);
		m_vecIteamData[0].push_back(item3);
		m_vecIteamData[0].push_back(item4);
		m_vecIteamData[0].push_back(item5);
		m_vecIteamData[0].push_back(item6);
	}
	{
		CItemData item1;
		item1.iconTexName = L"T_Weapon_Hammer_2H_Player_INE";
		item1.equipmentGrade = EEquipmentGrade::Legendary;
		item1.ItemType = EItemType::Equipment;
		item1.equipmentType = EEquipmentType::Weapon;
		item1.szItemName = L"HeeDong's Sword_2";
		item1.iMainStat = 20;
		item1.iLevel = 16;
		item1.szWeaponName = L"6";
		item1.weaponData.weaponName = L"Legend5";
		item1.equipmentName = EEquipmentName::Legend5;
		item1.szStatusName = L"T_2H_DPS_160";
		item1.weaponType = EWeaponType::Hammer;

		CItemData item2;
		item2.iconTexName = L"T_Weapon_Hammer_2H_Player_Mesa";
		item2.equipmentGrade = EEquipmentGrade::Epic;
		item2.ItemType = EItemType::Equipment;
		item2.equipmentType = EEquipmentType::Weapon;
		item2.szItemName = L"HeeDong's Sword_6";
		item2.iMainStat = 20;
		item2.iLevel = 16;
		item2.weaponData.weaponName = L"Mesa";
		item2.equipmentName = EEquipmentName::Mesa;
		item2.szStatusName = L"T_2H_DPS_144";
		item2.weaponType = EWeaponType::Hammer;

		CItemData item3;
		item3.iconTexName = L"T_Amulet_11";
		item3.equipmentGrade = EEquipmentGrade::Legendary;
		item3.ItemType = EItemType::Equipment;
		item3.equipmentType = EEquipmentType::Amulet;
		item3.szItemName = L"HeeDong's Sword_6";
		item3.iMainStat = 20;
		item3.iLevel = 16;
		item3.equipmentName = EEquipmentName::Amulet_11;

		m_vecIteamData[1].push_back(item1);
		m_vecIteamData[1].push_back(item2);
		m_vecIteamData[1].push_back(item3);
	}
	{
		CItemData item1;
		item1.iconTexName = L"T_Banner_1";
		item1.szStatusName = L"T_STR_65";
		item1.equipmentGrade = EEquipmentGrade::Epic;
		item1.ItemType = EItemType::Equipment;
		item1.equipmentType = EEquipmentType::Banner;
		item1.szItemName = L"HeeDong's Sword_6";
		item1.iMainStat = 20;
		item1.iLevel = 16;
		item1.equipmentName = EEquipmentName::Banner_1;

		CItemData item2;
		item2.iconTexName = L"T_Banner_2";
		item2.szStatusName = L"T_STR_48";
		item2.equipmentGrade = EEquipmentGrade::Uncommon;
		item2.ItemType = EItemType::Equipment;
		item2.equipmentType = EEquipmentType::Banner;
		item2.szItemName = L"HeeDong's Sword_6";
		item2.iMainStat = 20;
		item2.iLevel = 16;
		item2.equipmentName = EEquipmentName::Banner_2;

		CItemData item3;
		item3.iconTexName = L"T_Charm_6";
		item3.szStatusName = L"T_DEX_48";
		item3.equipmentGrade = EEquipmentGrade::Uncommon;
		item3.ItemType = EItemType::Equipment;
		item3.equipmentType = EEquipmentType::Charm;
		item3.szItemName = L"HeeDong's Sword_6";
		item3.iMainStat = 20;
		item3.iLevel = 16;
		item3.equipmentName = EEquipmentName::Charms_6;


		CItemData item4;
		item4.iconTexName = L"T_Ring_5";
		item4.szStatusName = L"T_INT_60";
		item4.equipmentGrade = EEquipmentGrade::Legendary;
		item4.ItemType = EItemType::Equipment;
		item4.equipmentType = EEquipmentType::Ring;
		item4.szItemName = L"HeeDong's Sword_6";
		item4.iMainStat = 20;
		item4.iLevel = 16;
		item4.equipmentName = EEquipmentName::Ring_5;

		CItemData item5;
		item5.iconTexName = L"T_Ring_27";
		item5.szStatusName = L"T_INT_58";
		item5.equipmentGrade = EEquipmentGrade::Epic;
		item5.ItemType = EItemType::Equipment;
		item5.equipmentType = EEquipmentType::Ring;
		item5.szItemName = L"HeeDong's Sword_6";
		item5.iMainStat = 20;
		item5.iLevel = 16;
		item5.equipmentName = EEquipmentName::Ring_27;

		CItemData item6;
		item6.iconTexName = L"T_Consumable_Healing_2";
		item6.equipmentType = EEquipmentType::LifeStone;
		item6.equipmentName = EEquipmentName::LifeStone_2;
		item6.equipmentGrade = EEquipmentGrade::Legendary;
		item6.ItemType = EItemType::Equipment;
		item6.szItemName = L"HeeDong's Sword_6";
		item6.iMainStat = 20;
		item6.iLevel = 16;
		item6.szStatusName = L"T_Max_HP_220";

		m_vecIteamData[2].push_back(item1);
		m_vecIteamData[2].push_back(item2);
		m_vecIteamData[2].push_back(item3);
		m_vecIteamData[2].push_back(item4);
		m_vecIteamData[2].push_back(item5);
		m_vecIteamData[2].push_back(item6);
	}

	//Stage 2
	{
		CItemData item1;
		item1.iconTexName = L"T_Weapon_Hammer_2H_Player_Legend1";
		item1.equipmentGrade = EEquipmentGrade::Legendary;
		item1.ItemType = EItemType::Equipment;
		item1.equipmentType = EEquipmentType::Weapon;
		item1.szItemName = L"HeeDong's Sword_6";
		item1.iMainStat = 20;
		item1.iLevel = 16;
		item1.weaponData.weaponName = L"Legend1";
		item1.equipmentName = EEquipmentName::Legend1;
		item1.szStatusName = L"T_2H_DPS_156";
		item1.weaponType = EWeaponType::Hammer;

		CItemData item2;
		item2.iconTexName = L"T_Ring_27";
		item2.szStatusName = L"T_INT_58";
		item2.equipmentGrade = EEquipmentGrade::Epic;
		item2.ItemType = EItemType::Equipment;
		item2.equipmentType = EEquipmentType::Ring;
		item2.szItemName = L"HeeDong's Sword_6";
		item2.iMainStat = 20;
		item2.iLevel = 16;
		item2.equipmentName = EEquipmentName::Ring_27;

		CItemData item3;
		item3.iconTexName = L"T_Charm_17";
		item3.szStatusName = L"T_DEX_60";
		item3.equipmentGrade = EEquipmentGrade::Legendary;
		item3.ItemType = EItemType::Equipment;
		item3.equipmentType = EEquipmentType::Charm;
		item3.szItemName = L"HeeDong's Sword_6";
		item3.iMainStat = 20;
		item3.iLevel = 16;
		item3.equipmentName = EEquipmentName::Charms_17;


		CItemData item4;
		item4.ItemType = EItemType::Potion;
		CItemData item5;
		item5.ItemType = EItemType::Potion;
		CItemData item6;
		item6.ItemType = EItemType::Potion;

		m_vecStage2IteamData[0].push_back(item1);
		m_vecStage2IteamData[0].push_back(item2);
		m_vecStage2IteamData[0].push_back(item3);
		m_vecStage2IteamData[0].push_back(item4);
		m_vecStage2IteamData[0].push_back(item5);
		m_vecStage2IteamData[0].push_back(item6);
	}

	{
		CItemData item1;
		item1.iconTexName = L"T_Consumable_Healing_3";
		item1.equipmentGrade = EEquipmentGrade::Epic;
		item1.ItemType = EItemType::Equipment;
		item1.equipmentType = EEquipmentType::LifeStone;
		item1.szItemName = L"HeeDong's Sword_6";
		item1.iMainStat = 20;
		item1.iLevel = 16;
		item1.equipmentName = EEquipmentName::LifeStone_3;
		item1.szStatusName = L"T_Max_HP_220";

		CItemData item2;
		item2.iconTexName = L"T_Weapon_Sword_1H_Player_Eclipse_Gold";
		item2.equipmentGrade = EEquipmentGrade::Legendary;
		item2.ItemType = EItemType::Equipment;
		item2.equipmentType = EEquipmentType::Weapon;
		item2.szItemName = L"HeeDong's Sword_3";
		item2.iMainStat = 20;
		item2.iLevel = 16;
		item2.szWeaponName = L"7";
		item2.weaponData.weaponName = L"Eclipse";
		item2.equipmentName = EEquipmentName::Eclipse;
		item2.szStatusName = L"T_1H_DPS_145";
		item2.weaponType = EWeaponType::LongSword;

		CItemData item3;
		item3.iconTexName = L"T_Weapon_Hammer_2H_Player_Harbinger";
		item3.equipmentGrade = EEquipmentGrade::Rare;
		item3.ItemType = EItemType::Equipment;
		item3.equipmentType = EEquipmentType::Weapon;
		item3.szItemName = L"SuBin's Sword";
		item3.iMainStat = 20;
		item3.iLevel = 16;
		item3.szWeaponName = L"2";
		item3.weaponData.weaponName = L"Harbinger";
		item3.equipmentName = EEquipmentName::Harbinger;
		item3.szStatusName = L"T_2H_DPS_132";
		item3.weaponType = EWeaponType::Hammer;

		m_vecStage2IteamData[1].push_back(item1);
		m_vecStage2IteamData[1].push_back(item2);
		m_vecStage2IteamData[1].push_back(item3);
	}

	//두손검 드랍 아이템
	{

		CItemData item5;
		item5.iconTexName = L"T_Ring_27";
		item5.szStatusName = L"T_INT_58";
		item5.equipmentGrade = EEquipmentGrade::Epic;
		item5.ItemType = EItemType::Equipment;
		item5.equipmentType = EEquipmentType::Ring;
		item5.szItemName = L"HeeDong's Sword_6";
		item5.iMainStat = 20;
		item5.iLevel = 16;
		item5.equipmentName = EEquipmentName::Ring_27;

		CItemData item6;
		item6.iconTexName = L"T_Ring_AilmentChance";
		item6.equipmentType = EEquipmentType::Ring;
		item6.equipmentName = EEquipmentName::Ring_AilmentChance;
		item6.equipmentGrade = EEquipmentGrade::Legendary;
		item6.ItemType = EItemType::Equipment;
		item6.szItemName = L"HeeDong's Sword_6";
		item6.iMainStat = 20;
		item6.iLevel = 16;
		item6.szStatusName = L"T_ItemStatus_Ring_AilmentChance";
		

		m_vec2HItem[0].push_back(item5);
		m_vec2HItem[0].push_back(item6);
	}
}
