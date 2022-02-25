
// UITool.h: UITool 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include "../../Reference/Headers/Engine_Defines.h"
#include "../../Reference/Headers/GameInstance.h"

BEGIN(Engine)
class Renderer;
END

// CUIToolApp:
// 이 클래스의 구현에 대해서는 UITool.cpp을(를) 참조하세요.
//

class CUIToolApp : public CWinApp
{
public:
	CUIToolApp() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();


	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);


	class CMFCMainApp* m_pMFCMainApp = nullptr;
	float m_TimerAcc = 0.0f;
};

extern CUIToolApp theApp;
