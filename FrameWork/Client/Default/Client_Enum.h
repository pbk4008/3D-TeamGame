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
	GUARD,
	ATTACK_LEFT,
	ATTACK_RIGHT,
	HITGROUND
};

enum class ELayer
{
	Enviroment,
	Player,
	Monster,
	Weapon,
	MonsterWeapon,
	JumpTrigger,
	ItemBox,
	Trigger,
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
	Common,
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


enum class EWeaponMaterial
{
	D,
	N,
	MRA,
	CEO,
	Mask,
	MATERIAL_END
};

static TEXTURETYPE g_arrMI_Texture[(int)EWeaponMaterial::MATERIAL_END]
{
	TEXTURETYPE::TEX_DIFFUSE,
	TEXTURETYPE::TEX_NORMAL,
	TEXTURETYPE::TEX_MRA,
	TEXTURETYPE::TEX_CEO,
	TEXTURETYPE::TEX_MASK
};

static const string g_arrMI_Tex[(int)EWeaponMaterial::MATERIAL_END]
{
	"g_DiffuseTexture",
	"g_BiNormalTexture",
	"g_MRATexture",
	"g_CEOTexture"
	"g_MASKTexture"
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

const wstring g_arrMI_ArgentBlade[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1H_Sword_ArgentBlade_D.dds",
	L"T_1H_Sword_ArgentBlade_N.dds",
	L"T_1H_sword_ArgentBlade_MRA.dds",
};

const wstring g_arrMI_Crossblade[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1h_Sword_Crossblade_D.dds",
	L"T_1h_Sword_Crossblade_N.dds",
	L"T_1h_Sword_Crossblade_MRA.dds",
};

const wstring g_arrMI_Dragon[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1h_Sword_Dragon_D.dds",
	L"T_1h_Sword_Dragon_N.dds",
	L"T_1h_Sword_Dragon_MRA.dds",
	L"T_1h_Sword_Dragon_CEO.dds"
};

const wstring g_arrMI_Eclipse[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1H_Sword_Eclipse_Gold_D.dds",
	L"T_1H_Sword_Eclipse_Gold_N.dds",
	L"T_1H_Sword_Eclipse_Gold_MRA.dds",
	L"T_1H_Sword_Eclipse_Gold_CEO.dds"
};

const wstring g_arrMI_Justice[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1H_sword_Justice_D.dds",
	L"T_1H_sword_Justice_N.dds",
	L"T_1H_sword_Justice_MRA.dds",
	L"T_1H_sword_Justice_CEO.dds"
};

const wstring g_arrMI_GodHammer[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Godhammer_D.dds",
	L"T_2H_hammer_Godhammer_N.dds",
	L"T_2H_hammer_Godhammer_MRA.dds",
};

const wstring g_arrMI_Harbinger[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_Hammer_Harbinger_D.dds",
	L"T_2h_Hammer_Harbinger_N.dds",
	L"T_2h_Hammer_Harbinger_MRA.dds",
	L"T_2h_Hammer_Harbinger_CEO.dds"
};

const wstring g_arrMI_Hinterclaw[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Hinterclaw_D.dds",
	L"T_2H_hammer_Hinterclaw_N.dds",
	L"T_2H_hammer_Hinterclaw_MRA.dds",
	L"T_2H_hammer_Hinterclaw_CEO.dds"
};

const wstring g_arrMI_Ironstone[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_hammer_Ironstone_D.dds",
	L"T_2h_hammer_Ironstone_N.dds",
	L"T_2h_hammer_Ironstone_MRA.dds",
	L"T_2h_hammer_Ironstone_CEO.dds"
};

const wstring g_arrMI_Legend1[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Legend1_D.dds",
	L"T_2H_hammer_Legend1_N.dds",
	L"T_2H_hammer_Legend1_MRA.dds",
	L"T_2H_hammer_Legend1_CEO.dds",
	L"T_2H_hammer_Legend1_MASK.dds"
};

const wstring g_arrMI_Legend2[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Legend2_D.dds",
	L"T_2H_hammer_Legend2_N.dds",
	L"T_2H_hammer_Legend2_MRA.dds",
	L"T_2H_hammer_Legend2_CEO.dds",
};

const wstring g_arrMI_Legend5[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_hammer_Legend5_D.dds",
	L"T_2H_hammer_Legend5_N.dds",
	L"T_2H_hammer_Legend5_MRA.dds",
	L"T_2H_hammer_Legend5_CEO.dds",
};

const wstring g_arrMI_Mesa[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_hammer_Mesa_D.dds",
	L"T_2h_hammer_Mesa_N.dds",
	L"T_2h_hammer_Mesa_MRA.dds",
	L"T_2h_hammer_Mesa_CEO.dds",
};

const wstring g_arrMI_Skymourne[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_hammer_Skymourne_D.dds",
	L"T_2h_hammer_Skymourne_N.dds",
	L"T_2h_hammer_Skymourne_MRA.dds",
	L"T_2h_hammer_Skymourne_CEO.dds",
};

const wstring g_arrMI_Soothsayer[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2H_Hammer_Soothsayer_D.dds",
	L"T_2H_Hammer_Soothsayer_N.dds",
	L"T_2H_Hammer_Soothsayer_MRA.dds",
	L"T_2H_Hammer_Soothsayer_CEO.dds",
};

END

#endif