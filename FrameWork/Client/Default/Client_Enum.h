#pragma once
#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

BEGIN(Client)
enum class SCENEID {SCENE_STATIC, SCENE_LOADING, SCENE_LOGO, SCENE_STAGE1, SCENE_STAGE2, SCENE_STAGE3,SCENE_TEST_JS, SCENE_TEST_YM, SCENE_TEST_SB, SCENE_END};
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
	//�� ���ķ� �߰��Ǵ°� �޾��ּ���, ���� �߰��ϸ� ����
	// ����
	WEAPON_MIDBOSS,
	WEAPON_EARTH,
	WEAPON_2HSword,
	WEAPON_1HSWORD,
	WEAPON_HEALER,
	WEAPON_BULLET,
	WEAPON_BRONZE,
	WEAPON_POLEARM,
	WEAPON_METEOR,
	// Ʈ����
	JUMP_TRIGGER,
	JUMP_NODE,
	JUMP_BOX,
	DROP_BOX,
	// END, MAX, SIZE
	GAMEOBJECT_END,
};

enum class EFFECT
{
	//AddEffect�Ҷ��� �־���� ���� �״�� �־������!!!
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
	NONE,			//������
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


END

#endif