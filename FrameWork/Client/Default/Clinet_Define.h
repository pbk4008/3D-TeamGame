#pragma once
#ifndef __CLIENT_DEFINE_H__
#define __CLIENT_DEFINE_H__

const unsigned int g_iWinCx = 1280;
const unsigned int g_iWinCy = 720;
const float MAXGROOGUGAGUE  = 10.0f;

extern HWND g_hWnd;
extern HINSTANCE  g_hInst;

extern _float g_fDeltaTime;
extern _double g_dDeltaTime;

extern CMainApp* g_pMainApp;
extern CClient_Observer* g_pObserver;
extern CDebugSystem* g_pDebugSystem;
extern CShakeManager* g_pShakeManager;
extern CDataManager*  g_pDataManager;

#endif