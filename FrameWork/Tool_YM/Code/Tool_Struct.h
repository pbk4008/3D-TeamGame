#ifndef Tool_Struct_h__
#define Tool_Struct_h__

#include "Tool_Defines.h"

typedef struct tagFileInfo
{
	wstring cstrFileName;
	wstring cstrFolder;

}FILEINFO;


typedef struct tagModel
{
	wstring  strFileName;
	wstring  strName;
	wstring  strTag;
	LEVEL_ID eLevel;

}MODELDESC;

#endif // Tool_Struct_h__
