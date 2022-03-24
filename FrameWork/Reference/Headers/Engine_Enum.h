#pragma once
#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

enum class 	TRANSFORMSTATEMATRIX { D3DTS_VIEW, D3DTS_PROJECTION, D3DTS_END };
enum class  LEVEL_ID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_STAGE_1, LEVEL_STAGE_2, LEVEL_STAGE_BOSS, LEVEL_END };
enum class  ERootOption { X, Y, Z, XY, XZ, YZ, XYZ, Max };
enum class TEXTURETYPE
{
	TEX_DIFFUSE,
	TEX_NORMAL,
	TEX_METALIC,
	TEX_OCCLUSION,
	TEX_ROUGHNESS,
	TEX_ORM,
	TEX_OMER,
	TEX_CEO,
	TEX_MRA,
	TEX_HEIGHT,
	TEX_TINT,
	TEX_MT,
	TEX_ORS,
	TEX_NM,
	TEX_RM,
	TEX_NRM,
	TEX_MASK,
	TEX_DR,
	TEX_NOH,
	TEX_ORH,
	TEX_COEFF,
	TEX_ND,
	TEX_END
};
/* MONSTER_1, MONSTER_2, MONSTER_3, MONSTER_4, MONSTER_5, MONSTER_6, MONSTER_7, MONSTER_8 */
/* �Ѽհ�, �μհ�, ������, ����, �߰�����, ����, ����, �̸� �� */
enum class TRIGGERTYPE { TRIGGER_LOD, TRIGGER_SCENE, TRIGGER_LIGHT, TRIGGER_MONSTER, TRIGGER_QUEST, MONSTER_1, MONSTER_2, MONSTER_3, MONSTER_4, MONSTER_5, MONSTER_6, MONSTER_7, MONSTER_8, TRIGGER_END };

#endif