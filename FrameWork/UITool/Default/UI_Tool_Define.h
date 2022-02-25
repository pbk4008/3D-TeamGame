#pragma once

#include "Engine_Defines.h"

#define WINCX 1280
#define WINCY 720

extern HINSTANCE g_hInst;
extern HWND g_hWnd;


enum TOOL_LEVEL {
	TOOL_LEVEL_LOGO, TOOL_LEVEL_GAMEPLAY, TOOL_LEVEL_END
};

typedef struct tagUIDesc
{
	wstring TextureTag;
	_float2 fPos;
	_float2 fScale;
}UIDESC;

#ifdef _AFX
#define ERR_MSG(message) AfxMessageBox(message)
#else 
#define ERR_MSG(message) MessageBox(nullptr,message, L"System_Error", MB_OK) 
#endif // _AFX