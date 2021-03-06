#ifndef Tool_Defines_h__
#define Tool_Defines_h__

#include "Engine_Defines.h"
#include <process.h>

static const unsigned int g_iWINCX = 1280;
static const unsigned int g_iWINCY = 800;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

enum TAB_ID { TAB_STATIC, TAB_MAP, TAB_CAM, TAB_END };

#define TKEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define TKEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define ID_CloneDeleteMenu 32771

#endif // Tool_Defines_h__
