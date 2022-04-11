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

typedef struct tagWallDesc
{
	_float4 pos;
	_float2 scale;
	_float radian;
	_float4 color;

}WALLDESC;

END
#endif