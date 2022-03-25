#pragma once
#ifndef __CLIENT_DEFINE_H__
#define __CLIENT_DEFINE_H__

const unsigned int g_iWinCx = 1280;
const unsigned int g_iWinCy = 720;
const float MAXGROOGUGAGUE  = 10.0f;

extern HWND g_hWnd;
extern HINSTANCE  g_hInst;

extern CClient_Observer* g_pObserver;
extern CDebugSystem* g_pDebugSystem;


#endif