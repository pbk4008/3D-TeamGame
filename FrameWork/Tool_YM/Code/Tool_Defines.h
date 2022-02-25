#ifndef Tool_Defines_h__
#define Tool_Defines_h__

#include "Engine_Defines.h"
#include <process.h>

static const unsigned int g_iWINCX = 1280;
static const unsigned int g_iWINCY = 800;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

enum TAB_ID { TAB_STATIC, TAB_MAP, TAB_CAM, TAB_END };

#endif // Tool_Defines_h__
