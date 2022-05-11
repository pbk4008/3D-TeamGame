#pragma once
#ifndef __CLIENT_DEFINE_H__
#define __CLIENT_DEFINE_H__

const unsigned int g_iWinCx = 1920;
const unsigned int g_iWinCy = 1080;
const float MAXGROOGUGAGUE  = 10.0f;

#define HALF_WINCX 960
#define HALF_WINCY 540
#define Max_InventorySlot 32
#define Max_EquipmentSlot 8

typedef CSoundMgr::CHANNELID CHANNEL;
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
extern CVoiceManager*	 g_pVoiceManager;

#endif