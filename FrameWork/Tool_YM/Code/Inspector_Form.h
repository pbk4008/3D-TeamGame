#pragma once

// CInspector_Form 폼 보기

class CMenu_Form;
class CModel_Inspector;
class CCam_Inspector;
class CInspector_Form : public CFormView
{
	DECLARE_DYNCREATE(CInspector_Form)

protected:
	CInspector_Form();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CInspector_Form();

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

//개발자 재정의
public:
	enum INSPEC_ID { INSPEC_MODEL, INSPEC_CAM, INSPEC_END };

public:
	void Ready_Inspector(void);
	_int Update_Inspector_Form(const _double& _dTimeDelta);

public:
	HRESULT		Get_ModelInfo(const FILEINFO& _FileInfo);
	HRESULT		Delete_Clone_ModelList(wstring _ModelName);
	void		Clear_Clone_ModelList(void);
	HRESULT		Add_GameObjectToLayer(const MESHDESC& ModelDesc);

public:
	void Get_Device(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext) { 
		m_pDevice = _pDevice;
		m_pDeviceContext = _pDeviceContext;
		Safe_AddRef(m_pDevice);
		Safe_AddRef(m_pDeviceContext);
	};

public:
	class CMainFrame*			m_pMainFrm		= nullptr;
	class CMenu_Form*		    m_pMenu_Form	= nullptr;
	class CModel_Inspector*		m_pModel_Inspec = nullptr;
	class CCam_Inspector*		m_pCam_Inspec	= nullptr;
public:
	CTabCtrl	m_TabInspec;
	INSPEC_ID	m_eSelectTab = INSPEC_END;

public:
	ID3D11Device*		 m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void PostNcDestroy();
};


