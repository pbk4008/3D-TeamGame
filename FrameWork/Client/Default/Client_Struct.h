#pragma once
#ifndef __CLIENT_STRUCT_H__
#define __CLIENT_STRUCT_H__
BEGIN(Client)

typedef struct tagEnvironMentLoadData
{
	_tchar FolderName[MAX_PATH];
	_tchar FileName[MAX_PATH];
	_tchar Tag[MAX_PATH];
	_tchar Name[MAX_PATH];
	_float4x4 WorldMat;
	_float3 fInitPos;
	LEVEL_ID eLevel;
	_uint	  iType;
}ENVIRONMENTLOADDATA;

typedef struct tagAttackDesc
{
	// 1 : 약공격, 2 : 강공격, 3 : 넉백될 정도의 강려크한 공격
	_uint iLevel = 0;
	_float fDamage = 0.f;
}ATTACKDESC;

END
#endif