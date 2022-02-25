
// Tool_YM.h: Tool_YM 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include "Engine_Defines.h"

// CToolYMApp:
// 이 클래스의 구현에 대해서는 Tool_YM.cpp을(를) 참조하세요.
//
class CTool_Main;
class CMainFrame;
class CToolYMView;
class CToolYMApp : public CWinApp
{
public:
	CToolYMApp() noexcept;
	
// 개발자 재정의
private:
	CTool_Main*  m_pTool_Main = nullptr;
	CMainFrame*  m_pMainFrm = nullptr;
	CToolYMView* m_pToolView = nullptr;
	
private:
	//C* m_pToolMain = nullptr;

private:
	_double m_dTimerAcc = 0.0f;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CToolYMApp theApp;
