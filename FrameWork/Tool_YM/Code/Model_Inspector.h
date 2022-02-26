#pragma once


// CModel_Inspector 대화 상자
#include "Engine_Defines.h"

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
	void		Ready_Tag_Combo(void);
	void		Ready_Level_Combo(void);

public:
	HRESULT		Clear_Clone_ModelList(wstring _ModelName);

public:
	FILEINFO	m_FileInfo; /* Map_Tool에서 선택한 원본 모델에 대한 정보를 저장합니다. */
	MODELDESC   m_ModelDesc;

public:
	list<wstring>				m_CloneMode_NameList;

public:
	class CMainFrame*			m_pMainFrm = nullptr;
	class CInspector_Form*		m_pInspec_Form = nullptr;

public:
	CEdit m_StaticTxt_FileName;
	CEdit m_EditTxt_Model_Name;
	CComboBox m_Combo_Tag;
	CComboBox m_Combo_Level;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();


public:
	afx_msg void OnBnClickedAddButton();
};
