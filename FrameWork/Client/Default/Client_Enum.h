#pragma once
#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

BEGIN(Client)
enum class SCENEID {SCENE_STATIC, SCENE_LOADING, SCENE_LOGO, SCENE_STAGE1, SCENE_STAGE2, SCENE_STAGE3,SCENE_TEST_JS, SCENE_END};
enum class CINEMA_INDEX{CINEMA1_1, CINEMA1_2, CINEMA2_1, CINEMA2_2, CINEMA2_3, CINEMA2_4, CINEMA_END};
enum class CINEMA_ACTOR {	ACTOR_SILVERMANE, ACTOR_GRAYEHAWK, ACTOR_PHOENIX, ACTOR_MIDBOSS, ACTOR_END};
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
	WEAPON_BOSS,
	WEAPON_EARTH,
	WEAPON_2HSword,
	WEAPON_1HSWORD,
	WEAPON_HEALER,
	WEAPON_BULLET,
	WEAPON_BRONZE,
	WEAPON_POLEARM,
	WEAPON_METEOR,
	// 트리거
	TRIGGER,
	JUMP_TRIGGER,
	JUMP_NODE,
	JUMP_BOX,
	DROP_BOX,
	EXPLOSION_ROCK,
	// END, MAX, SIZE
	GAMEOBJECT_END,
};

enum class EFFECT
{
	//AddEffect할때에 넣어줬던 순서 그대로 넣어줘야함!!!
	DASH,
	HIT_GROUND_SMOKE,
	HIT,
	HIT_FLOATING,
	HIT_FLOATING_2,
	DEATH,
	PLAYER_HIT,
	GUARD,
	ATTACK_LEFT,
	ATTACK_RIGHT,
	ATTACK_RIGHT_LAST,
	HIT_GROUND,
	OPENBOX,
	ATTACK_GROUND,
	ATTACK_GROUND_2,
	ITEM,
	BOX,
	EAT_ITEM,
	HIT_IMAGE,
	DEAD_SMOKE,
	EXPLOSION_ROCK_UP,
	EXPLOSION_ROCK_LEFT,
	EXPLOSION_ROCK_RIGHT,
	EXPLOSION_ROCK,
	HAMMER_DUST
};

enum class ELayer
{
	Enviroment,
	Player,
	Monster,
	Weapon,
	MonsterWeapon,
	Meteor,
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
	Equipment_Type_End
};

enum class EEquipmentName
{
	/* weapon */
	Needle,
	ArgentBlade,
	Crossblade,
	Dragon,
	Eclipse,
	Fury,
	GodHammer,
	Harbinger,
	Hinterclaw,
	Ironstone,
	Justice,
	Legend1,
	Legend2,
	Legend5,
	Mesa,
	Skymourne,
	Soothsayer,
	/* Banner */
	Banner_1,
	Banner_2,
	Banner_4,
	Banner_6,
	Banner_7,
	/* Charm */
	Charms_6,
	Charms_7,
	Charms_9,
	Charms_14,
	Charms_17,
	/* LifeStone */
	LifeStone_2,
	LifeStone_3,
	LifeStone_5,
	LifeStone_9,
	/* Ring */
	Ring_5,
	Ring_27,
	Ring_33,
	Ring_40,
	Ring_AilmentChance,
	/* Amulet */
	Amulet_11,
	Amulet_12,
	Amulet_13,
	Amulet_BlisteringPulse,
	Amulet_SunBloom,
	Equipment_Name_End
};

typedef enum EEquipmentMainStatType
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

enum class EWeaponMaterial
{
	D,
	N,
	MRA,
	CEO,
	Mask,
	MATERIAL_END
};

enum class EQuestTextType
{
	FindSunForge,   /* 태양로 입구를 찾으십시오 */
	AllKillMonster, /* 모든 몬스터 처치 */
	FindDropBox,    /* 은닉품 조사 */
	ChageNewWeapon, /* 새로운 무기로 교체 */
	QuestText_END
};

const wstring g_arrQuestTextTex[(int)EQuestTextType::QuestText_END]
{
	L"T_HUD_Find_Sunforge",
	L"T_HUD_KillAllMonster",
	L"T_HUD_Find_DropBox",
	L"T_HUD_EquipNewWeapon"
};

enum class EQuestHeaderType
{
	FirestStep,     /* 첫걸음 */
	Sunforge,		/* 태양로 신전 */
	Boss,			/* 수호자 */
	QuestHeader_END
};

const wstring g_arrQuestHeaderTex[(int)EQuestHeaderType::QuestHeader_END]
{
	L"T_HUD_FirstStep",
	L"T_HUD_GUARD",
	L"T_HUD_GUARD"
};

const wstring g_arrGradeName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"일반",
	L"고급",
	L"희귀",
	L"영웅",
	L"전설"
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
	"g_CEOTexture",
	"g_MASKTexture"
};

const wstring g_arrGradeSlotBgName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"T_Item_Bg_Grey",
	L"T_Item_Bg_Blue",
	L"T_Item_Bg_Green",
	L"T_Item_Bg_Orange",
	L"T_Item_Bg_Purple",
};

/* 인벤토리 아이템 설명 */
const wstring g_arrGradeStatusBgName[(int)EEquipmentName::Equipment_Name_End]
{
	/* Weapon */
	L"T_ItemStatus_Weapon_Needle",
	L"T_ItemStatus_Weapon_ArgentBlade",
	L"T_ItemStatus_Weapon_Crossblade",
	L"T_ItemStatus_Weapon_Dragon",
	L"T_ItemStatus_Weapon_Eclipse",
	L"T_ItemStatus_Weapon_Fury",
	L"T_ItemStatus_Weapon_GodHammer",
	L"T_ItemStatus_Weapon_Harbinger",
	L"T_ItemStatus_Weapon_Hinterclaw",
	L"T_ItemStatus_Weapon_Ironstone",
	L"T_ItemStatus_Weapon_Justice",
	L"T_ItemStatus_Weapon_Legend1",
	L"T_ItemStatus_Weapon_Legend2",
	L"T_ItemStatus_Weapon_Legend5",
	L"T_ItemStatus_Weapon_Mesa",
	L"T_ItemStatus_Weapon_Skymourne",
	L"T_ItemStatus_Weapon_Soothsayer",
	/* Banner */
	L"T_ItemStatus_Banner_1",
	L"T_ItemStatus_Banner_2",
	L"T_ItemStatus_Banner_4",
	L"T_ItemStatus_Banner_6",
	L"T_ItemStatus_Banner_7",
	/* Charm */
	L"T_ItemStatus_Charms_6",
	L"T_ItemStatus_Charms_7",
	L"T_ItemStatus_Charms_9",
	L"T_ItemStatus_Charms_14",
	L"T_ItemStatus_Charms_17",
	/* LifeStone */
	L"T_ItemStatus_LifeStone_2",
	L"T_ItemStatus_LifeStone_3",
	L"T_ItemStatus_LifeStone_5",
	L"T_ItemStatus_LifeStone_9",
	/* Ring */
	L"T_ItemStatus_Ring_5",
	L"T_ItemStatus_Ring_27",
	L"T_ItemStatus_Ring_33",
	L"T_ItemStatus_Ring_40",
	L"T_ItemStatus_Ring_AilmentChance",
	/* Amulet*/
	L"T_ItemStatus_Amulet_11",
	L"T_ItemStatus_Amulet_12",
	L"T_ItemStatus_Amulet_13",
	L"T_ItemStatus_Amulet_BlisteringPulse",
	L"T_ItemStatus_SunBloom"
};

/* 획득시 아이템 이름 */
const wstring g_arrHUDItemIcon[(int)EEquipmentName::Equipment_Name_End]
{
	L"T_HUD_WeaponName_Needle",
	L"T_HUD_WeaponName_ArgentBlade",
	L"T_HUD_WeaponName_Crossblade",
	L"T_HUD_WeaponName_Dragon",
	L"T_HUD_WeaponName_Eclipse",
	L"T_HUD_WeaponName_Fury",
	L"T_HUD_WeaponName_GodHammer",
	L"T_HUD_WeaponName_Harbinger",
	L"T_HUD_WeaponName_Hinterclaw",
	L"T_HUD_WeaponName_Ironstone",
	L"T_HUD_WeaponName_Justice",
	L"T_HUD_WeaponName_Legend1",
	L"T_HUD_WeaponName_Legend2",
	L"T_HUD_WeaponName_Legend5",
	L"T_HUD_WeaponName_Mesa",
	L"T_HUD_WeaponName_Skymourne",
	L"T_HUD_WeaponName_Soothsayer",
	L"T_HUD_BannerName_1",
	L"T_HUD_BannerName_2",
	L"T_HUD_BannerName_4",
	L"T_HUD_BannerName_6",
	L"T_HUD_BannerName_7",
	L"T_HUD_CharmsName_6",
	L"T_HUD_CharmsName_7",
	L"T_HUD_CharmsName_9",
	L"T_HUD_CharmsName_14",
	L"T_HUD_CharmsName_17",
	L"T_HUD_LifeStoneName_2",
	L"T_HUD_LifeStoneName_3",
	L"T_HUD_LifeStoneName_5",
	L"T_HUD_LifeStoneName_9",
	L"T_HUD_RingName_5",
	L"T_HUD_RingName_27",
	L"T_HUD_RingName_33",
	L"T_HUD_RingName_40",
	L"T_HUD_RingName_AilmentChance",
	L"T_HUD_AmuletName_11",
	L"T_HUD_AmuletName_12",
	L"T_HUD_AmuletName_13",
	L"T_HUD_AmuletName_BlisteringPulse",
	L"T_HUD_AmuletName_SunBloom"
};


const wstring g_arrHUDItemTypeIcon[(int)EEquipmentType::Equipment_Type_End]
{
	L"T_HUD_LootItemName_Weapon",
	L"T_HUD_LootItemName_Ring",
	L"T_HUD_LootItemName_Amulet", /* 목걸이 */
	L"T_HUD_LootItemName_Charm",  /* 장신구 */
	L"T_HUD_LootItemName_LifeStone",
	L"T_HUD_LootItemName_Banner"
};

const wstring g_arrEffectStatus[(int)EEquipmentGradeUI::GRADE_END]
{
	L"T_GearPopover_Glow_Common",
	L"T_GearPopover_Glow_UnCommon",
	L"T_GearPopover_Glow_Rare",
	L"T_GearPopover_Glow_Legendary",
	L"T_GearPopover_Glow_Epic",
};

const wstring g_arrGradeEffectName[(int)EEquipmentGradeUI::GRADE_END]
{
	L"T_Item_Glow_Grey",
	L"T_Item_Glow_Blue",
	L"T_Item_Glow_Green",
	L"T_Item_Glow_Orange",
	L"T_Item_Glow_Purple",
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
	L"T_1h_Sword_Dragon_CEO.dds",
};

const wstring g_arrMI_Eclipse[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1H_Sword_Eclipse_Gold_D.dds",
	L"T_1H_Sword_Eclipse_Gold_N.dds",
	L"T_1H_Sword_Eclipse_Gold_MRA.dds",
	L"T_1H_Sword_Eclipse_Gold_CEO.dds",
};

const wstring g_arrMI_Justice[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1H_sword_Justice_D.dds",
	L"T_1H_sword_Justice_N.dds",
	L"T_1H_sword_Justice_MRA.dds",
	L"T_1H_sword_Justice_CEO.dds",
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

const wstring g_arrMI_Needle[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_1h_Sword_Needle_D.dds",
	L"T_1h_Sword_Needle_N.dds",
	L"T_1h_Sword_Needle_MRA.dds",
	L"T_1h_Sword_Needle_CEO.dds",
};

const wstring g_arrMI_Fury[(int)EWeaponMaterial::MATERIAL_END]
{
	L"T_2h_Hammer_Fury_D.dds",
	L"T_2h_Hammer_Fury_N.dds",
	L"T_2h_Hammer_Fury_MRA.dds",
	L"T_2h_Hammer_Fury_CEO.dds",
};

END
#endif