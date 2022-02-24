#pragma once

#include "MainFrm.h"
#include "Map_Tool.h"
#include "Cam_Tool.h"

// CMenuForm 폼 보기
class CMenuForm : public CFormView
{
	DECLARE_DYNCREATE(CMenuForm)

protected:
	CMenuForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMenuForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMenuForm };
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

	//개발자 재정의
public:
	enum TAB_ID  { TAB_MAP, TAB_CAM, TAB_END };

public:
	HRESULT Ready_Tab(void);

public:
	class CMainFrame* m_pMainFrm = nullptr;
	class CMap_Tool*  m_pMap_Tool = nullptr;
	class CCam_Tool*  m_pCam_Tool = nullptr;

public:
	CTabCtrl	m_tabMenu;
	TAB_ID		m_eSelectTab = TAB_END;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeCancel(NMHDR* pNMHDR, LRESULT* pResult);
};



