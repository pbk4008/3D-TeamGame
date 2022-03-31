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
	//이 이후로 추가되는거 달아주세요, 위에 추가하면 꼬임
	// 무기
	WEAPON_MIDBOSS,
	WEAPON_EARTH,
	WEAPON_2HSword,
	WEAPON_HEALER,
	WEAPON_BULLET,
	WEAPON_BRONZE,
	WEAPON_POLEARM,
	// 트리거
	JUMP_TRIGGER,
	JUMP_NODE,
	JUMP_BOX,
	// END, MAX, SIZE
	GAMEOBJECT_END,
};
END
enum class EFFECT
{
	//AddEffect할때에 넣어줬던 순서 그대로 넣어줘야함!!!
	HIT,
	FLOATING,
	DEATH
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

#endif