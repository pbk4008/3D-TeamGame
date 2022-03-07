﻿
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
class CToolView;
class CForm;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:
	_int Update_Form(_float fDeltaTime);
	HRESULT Render_Form();
private:
	HRESULT Init_Window();
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
public:
	inline CForm* getForm() { return m_pForm; }
private:
	CToolView* m_pToolView;
	CForm* m_pForm;
	CSplitterWnd m_tSpliiter;

protected:  // 컨트롤 모음이 포함된 멤버입니다.

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	virtual BOOL DestroyWindow();
};


