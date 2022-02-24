#pragma once

#include "Model_Inspector.h"
#include "Cam_Inspector.h"

// CInspector 폼 보기

class CInspector : public CFormView
{
	DECLARE_DYNCREATE(CInspector)

protected:
	CInspector();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CInspector();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CInspector };
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
	enum ID_INSPEC { ID_MODEL, ID_CAM, ID_END };

public:
	HRESULT Ready_Inspector(void);


public:
	class CModel_Inspector*		m_pModel_Inspector = nullptr;
	class CCam_Inspector*		m_pCam_Inspector = nullptr;

public:
	CTabCtrl		m_tabInspector;
	ID_INSPEC		m_eSelectInspec = ID_END;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};


