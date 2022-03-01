#pragma once


// CEffectTool_Dlg 대화 상자

class CEffectTool_Dlg : public CDialog
{
	DECLARE_DYNAMIC(CEffectTool_Dlg)

public:
	CEffectTool_Dlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEffectTool_Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CEffectTool_Dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void InitialShaderTree();
	void InitialTextureTree();

	DECLARE_MESSAGE_MAP()
public:
	_tchar m_strPickFileName[MAX_PATH];

	wstring TextureFileName = L"";
	wstring TextureFolderPath = L"";

	wstring ShaderFileName = L"";
	wstring ShaderFolderPath = L"";

	float m_PositionX;
	float m_PositionY;
	float m_PositionZ;
	float m_VelocityX;
	float m_VelocityY;
	float m_VelocityZ;
	float m_ParticleSizeX;
	float m_ParticleSizeY;
	float m_Age;
	float m_LiftTime;
	int m_iRandomX;
	int m_iRandomY;
	int m_iRandomZ;
	int m_BaseCount;

	_bool m_bCheck;
	
	CListBox m_ListBox;
	CTreeCtrl m_TextureTree;
	CTreeCtrl m_ShaderPathTree;
	CButton m_CheckGravity;

	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelchangedTree3(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBnClickedCheck1();
};
