﻿
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

class CToolYASICView;
class CMainForm;

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:
	CSplitterWndEx m_tMainSplitter;
	CToolYASICView* m_pToolView = nullptr;
	CMainForm* m_pMainForm = nullptr;

// 작업입니다.
public:
	void Tick();

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar        m_wndStatusBar;
	_double  m_dTimerAcc = 0.0;
// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


