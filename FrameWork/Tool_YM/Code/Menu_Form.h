#pragma once

#include "MainFrm.h"
#include "Inspector_Form.h"
#include "Tool_Defines.h"
#include "Map_Tool.h"
#include "Cam_Tool.h"

// CMenu_Form 폼 보기

class CMenu_Form : public CFormView
{
	DECLARE_DYNCREATE(CMenu_Form)

protected:
	CMenu_Form();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMenu_Form();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMenu_Form };
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
	HRESULT Ready_Tab(void);
	_int	Update_Menu_Form(const _double& _dTimeDelta);
	HRESULT Create_Model_Prototype(const FILEINFO& _fileInfo);
	HRESULT Create_HierarchyTree(const MODELDESC& _ModelInfo);
	void	InitHierarchyTree(void);

public:
	void Get_Device(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext) {
		m_pDevice		 = _pDevice;
		m_pDeviceContext = _pDeviceContext;
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pDeviceContext);
	};

public:
	CTabCtrl	m_MenuTab;
	TAB_ID		m_eSelectTab = TAB_END;

public:
	class CMainFrame*		m_pMainFrm = nullptr;
	class CInspector_Form*  m_pInspec_Form = nullptr;
	class CMap_Tool*		m_pMap_Tool = nullptr;
	class CCam_Tool*		m_pCam_Tool = nullptr;

public:
	ID3D11Device*		 m_pDevice		  = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeCancel(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void PostNcDestroy();
};


