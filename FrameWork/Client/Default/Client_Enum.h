#pragma once
#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

BEGIN(Client)
enum class SCENEID {SCENE_STATIC, SCENE_LOADING, SCENE_LOGO, SCENE_STAGE1, SCENE_STAGE2, SCENE_STAGE3,SCENE_TEST_JS, SCENE_END};
enum class CINEMA_INDEX{CINEMA1_1, CINEMA_END};
enum class GAMEOBJECT
{
	ENVIRONMENT,
	PLAYER,
	WEAPON,
	MONSTER_1H,
	MONSTER_2H,
	MONSTER_SPEAR,
	MONSTER_HEALER,
	MONSTER_SHOOTER,
	MONSTER_ANIMUS,
	MONSTER_CRYSTAL,
	MONSTER_ABERRANT,
	MIDDLE_BOSS, 
	BOSS,
	UI_STATIC,
	UI_DYNAMIC,
	EFFECT,
	//이 이후로 추가되는거 달아주세요, 위에 추가하면 꼬임
	// 무기
	WEAPON_MIDBOSS,
	WEAPON_EARTH,
	WEAPON_2HSword,
	WEAPON_1HSWORD,
	WEAPON_HEALER,
	WEAPON_BULLET,
	WEAPON_BRONZE,
	WEAPON_POLEARM,
	WEAPON_METEOR,
	// 트리거
	JUMP_TRIGGER,
	JUMP_NODE,
	JUMP_BOX,
	DROP_BOX,
	// END, MAX, SIZE
	GAMEOBJECT_END,
};

enum class EFFECT
{
	//AddEffect할때에 넣어줬던 순서 그대로 넣어줘야함!!!
	HIT,
	FLOATING,
	DEATH,
	PLAYERHIT,
	GUARD
};

enum class ELayer
{
	Enviroment,
	Player,
	Monster,
	Weapon,
	JumpTrigger,
	Max
};


/* for. Item Data */
enum class EScatterType
{
	Quad,
	Cone
};

enum class EItemType
{
	NONE,			//미장착
	Equipment,		
	Resorce,
	ITEMTYPE_END
};

enum class EEquipmentGrade
{
	Uncommon,
	Rare,
	Epic,
	Legendary,
	EQUIPGRADE_END
};

typedef enum EWeaponType
{
	LongSword,
	Hammer,
	Weapon_End
}WEAPON_TYPE;

enum class EEquipmentType
{
	Weapon,
	Ring,
	Amulet,
	Charm,
	LifeStone,
	Banner,
	Augment,
	Equipment_Type_End
};

typedef enum eEquipmentMainStatType
{
	//weapon
	PHYSICAL_DPS,
	//acc, banner
	Might,
	Spirit,
	Vitality,
	//Banner
	AllStat,
	//LifeStone
	HealthRecovery,

	MainStatType_End
}EQUIPMENT_MAINSTAT_TYPE;

typedef enum eModalIndex
{
	TAB_EQUIPMENT,
	TAB_ARMORY,
	TAB_RESOURCE,
	TAB_END
}INVEN_INDEX;

enum class EEquipSlot
{
	Weapon1,
	Weapon2,
	LifeStone,
	Banner,
	Ring1,
	Ring2,
	Amulet,
	Charm,
	Max,

	/* 무기1 무기2 */
	/* 목걸이 방패 */
	/* 반지1 반지2 */
	/* 생명석 배너 */
};

enum class EEquipmentGradeUI
{
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	GRADE_END
};

const wstring g_arrGradeName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"일반",
	L"고급",
	L"희귀",
	L"영웅",
	L"전설"
};

const wstring g_arrGradeSlotBgName[(int)EEquipmentGradeUI::GRADE_END]
{ 
	L"T_Item_Bg_Grey",
	L"T_Item_Bg_Blue",
	L"T_Item_Bg_Green",
	L"T_Item_Bg_Purple",
	L"T_Item_Bg_Orange"
};

const wstring g_arrGradeStatusBgName[(int)EEquipmentGradeUI::GRADE_END]
{ 
	L"T_ItemStatus_Bg_Common",
	L"T_ItemStatus_Bg_Rare",
	L"T_ItemStatus_Bg_Uncommon",
	L"T_ItemStatus_Bg_Epic",
	L"T_ItemStatus_Bg_Legendary"
};

const wstring g_arrGradeEffectName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"T_Item_Glow_Grey",
	L"T_Item_Glow_Blue",
	L"T_Item_Glow_Green",
	L"T_Item_Glow_Purple",
	L"T_Item_Glow_Orange"
};

END

#endif