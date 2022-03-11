#pragma once

#include "Engine_Defines.h"
#include <io.h>
#define WINCX 1280
#define WINCY 720

extern HINSTANCE g_hInst;
extern HWND g_hWnd;


enum TOOL_LEVEL {
	TOOL_LEVEL_LOGO, TOOL_LEVEL_GAMEPLAY, TOOL_LEVEL_END
};

#ifdef _AFX
#define ERR_MSG(message) AfxMessageBox(message)
#else 
#define ERR_MSG(message) MessageBox(nullptr,message, L"System_Error", MB_OK) 
#endif // _AFX