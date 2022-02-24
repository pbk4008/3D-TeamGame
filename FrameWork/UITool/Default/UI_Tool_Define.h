#pragma once

#define WINCX 1280
#define WINCY 720

extern HINSTANCE g_hInst;
extern HWND g_hWnd;


#ifdef _AFX
#define ERR_MSG(message) AfxMessageBox(message)
#else 
#define ERR_MSG(message) MessageBox(nullptr,message, L"System_Error", MB_OK) 
#endif // _AFX