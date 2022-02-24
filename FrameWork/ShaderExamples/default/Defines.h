#pragma once

#include <process.h>

static const unsigned long	g_iWinCX = 1280;
static const unsigned long	g_iWinCY = 720;
static const float			g_fCamFar = 900.f;

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

#include "Engine_Defines.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;

namespace Example {}

using namespace Example;