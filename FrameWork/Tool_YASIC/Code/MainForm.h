﻿#pragma once



// CMainForm 폼 보기
class CAnimationTool;

class CMainForm : public CFormView
{
	DECLARE_DYNCREATE(CMainForm)
public:
	enum class ETab{ AnimationTool, Max };
protected:
	CMainForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMainForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMainForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	void Tick();

public:
	CTabCtrl m_tab;

public:
	CAnimationTool* m_pAnimationTool = nullptr;
};

