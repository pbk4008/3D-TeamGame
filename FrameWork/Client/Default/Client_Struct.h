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

typedef struct tagDropBox
{
	_float4x4 WorldMat;
	std::vector<class CItemData> itemData;
}DROPBOXDESC;

END
#endif