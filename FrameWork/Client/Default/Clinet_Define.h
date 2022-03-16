#pragma once
#ifndef __CLIENT_DEFINE_H__
#define __CLIENT_DEFINE_H__

const unsigned int g_iWinCx = 1280;
const unsigned int g_iWinCy = 720;
extern HWND g_hWnd;
extern HINSTANCE  g_hInst;

extern class CClient_Observer* g_pObserver;
#endif