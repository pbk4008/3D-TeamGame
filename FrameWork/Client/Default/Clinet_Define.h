#pragma once
#ifndef __CLIENT_DEFINE_H__
#define __CLIENT_DEFINE_H__

const unsigned int g_iWinCx = 1280;
const unsigned int g_iWinCy = 720;
const float MAXGROOGUGAGUE  = 10.0f;

#define HALF_WINCX 640
#define HALF_WINCY 360
#define Max_InventorySlot 32
#define Max_EquipmentSlot 8

extern HWND g_hWnd;
extern HINSTANCE  g_hInst;

extern CMainApp*		 g_pMainApp;
extern CClient_Observer* g_pObserver;
extern CDebugSystem*	 g_pDebugSystem;
extern CShakeManager*	 g_pShakeManager;
extern CDataManager*	 g_pDataManager;
extern CInven_UIManager* g_pInvenUIManager;
extern CInteractManager* g_pInteractManager;
extern CDropManager*	 g_pDropManager;
extern CWeaponGenerator* g_pWeaponGenerator;
extern CQuestManager*	 g_pQuestManager;
extern CGuideUIManager*  g_pGuideManager;
#endif