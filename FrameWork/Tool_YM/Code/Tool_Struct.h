#ifndef Tool_Struct_h__
#define Tool_Struct_h__

#include "Tool_Defines.h"

typedef struct tagFileInfo
{
	_uint   cstrFBX_Type;
	wstring cstrFileName;
	wstring cstrFolder;
}FILEINFO;

typedef struct tagModel
{
	wstring   strFileName;
	wstring   strFolder;
	wstring   strName;
	wstring   strTag;
	_float3	  fInitPos;
	_float4x4 WorldMat ;
	LEVEL_ID eLevel;
}MODELDESC;

typedef struct tagMeshDesc
{
	_tchar FolderName[MAX_PATH];
	_tchar FileName[MAX_PATH];
	_tchar Tag[MAX_PATH];
	_tchar Name[MAX_PATH];
	_float4x4 WorldMat;
	_float3 fInitPos;
	LEVEL_ID eLevel;
}MESHDESC;

typedef struct tagNavMeshDesc
{
	_float3 Point[3];
}NAVMESHDESC;
#endif // Tool_Struct_h__
