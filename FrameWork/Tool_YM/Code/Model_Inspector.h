#pragma once


// CModel_Inspector 대화 상자
#include "Engine_Defines.h"

BEGIN(Engine)
class CCell;
class CMeshCollider;
END

class CObserver;
class CMainFrame;
class CInspector_Form;
class CModel_Inspector : public CDialogEx
{
	DECLARE_DYNAMIC(CModel_Inspector)
	
public:
	CModel_Inspector(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CModel_Inspector();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CModel_Inspector };
#endif

//개발자 재정의
public: 
	HRESULT		Get_ModelInfo(const FILEINFO& _FileInfo);
	_int		Update_Model_Inspector(_double _dTimeDelta);
	void		Ready_Level_Combo(void);

public:
	HRESULT		Delete_Clone_ModelList(wstring _ModelName);
	void		Clear_Clone_ModelList(void);
	HRESULT		Add_GameObjectToLayer(const MESHDESC& ModelDesc);
	void		CCW_Sort(_float3* pPoints[]);

public:
	FILEINFO	m_FileInfo; /* Map_Tool에서 선택한 원본 모델에 대한 정보를 저장합니다. */
	MODELDESC   m_ModelDesc;

public:
	list<wstring>				m_CloneMode_NameList;
	vector<Engine::CCell*>		m_vecCells; 
	NAVMESHDESC					m_NavMesh;
	vector<NAVMESHDESC>			m_NavMeshList_Pos;
	vector<TRIGGER>				m_vecTrigger;

public:
	class CMainFrame*			m_pMainFrm	    = nullptr;
	class CInspector_Form*		m_pInspec_Form  = nullptr;
	class CObserver*			m_pObserver	    = nullptr;
	Engine::CMeshCollider*	    m_pMeshCollider = nullptr;
	/* TRIGGERTYPE { TRIGGER_LOD, TRIGGER_SCENE, TRIGGER_LIGHT, TRIGGER_MONSTER, TRIGGER_QUEST, TRIGGER_END } */

public:
	_bool	m_bModelInfo_View = true;
	_int	m_iLodIndex = 0;
	_int	m_iSceneIndex = 0;
	_int	m_iLightIndex = 0;
	_int	m_iMonsterIndex = 0;
	_int	m_iQuestIndex = 0;

public:
	CString m_ModelPosX, m_ModelPosY, m_ModelPosZ;
	CString m_ModelScaleX, m_ModelScaleY, m_ModelScaleZ;
	
	CEdit m_StaticTxt_FileName;
	CEdit m_EditText_Model_Tag;
	CEdit m_EditTxt_Model_Name;
	CComboBox m_Combo_Level;
	CComboBox m_TriggerCombo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();


public:
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedModelSetMode();
	afx_msg void OnBnClickedNaveSetMode();
	afx_msg void OnBnClickedNavSaveButton();
	afx_msg void OnBnClickedNavLoadButton();
	afx_msg void OnBnClickedRotXButton();
	afx_msg void OnBnClickedRotYButton();
	afx_msg void OnBnClickedZRotButton();
	afx_msg void OnBnClickedRotNoneButton();
	afx_msg void OnBnClickedCreateNavMesh();
	afx_msg void OnBnClickedTriggerAdd();
	afx_msg void OnBnClickedSaveTrigger();
	afx_msg void OnBnClickedLodTrigger();
};
