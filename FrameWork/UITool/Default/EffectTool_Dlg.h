#pragma once


// CEffectTool_Dlg 대화 상자

#include "Effect.h"
#include "MFCEffect.h"
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
	float m_Velocity;
	float m_ParticleSizeX;
	float m_ParticleSizeY;
	float m_Age;
	float m_LiftTime;
	float m_fRandomPosX;
	float m_fRandomPosY;
	float m_fRandomPosZ;
	float m_fRandomDirX;
	float m_fRandomDirY;
	float m_fRandomDirZ;
	float m_fRandomMinusDirX;
	float m_fRandomMinusDirY;
	float m_fRandomMinusDirZ;
	int m_BaseCount;

	_bool m_bCheck;
	_bool m_bReset = false;
	CEffect::EFFECTDESC m_EffectDesc;
	vector<CMFCEffect::EFFECTDESC> m_vecEffect;
	
	CListBox m_ListBox;
	CTreeCtrl m_TextureTree;
	CTreeCtrl m_ShaderPathTree;
	CButton m_CheckGravity;
	CButton m_AxisXBtn;
	CButton m_AxisYBtn;
	CButton m_AxisZBtn;
	CButton m_AxisAllBtn;

	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelchangedTree3(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton2();

	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonNameTag();

	CString m_EffectName;
	int m_RenderPassNum;
	float m_frame;
	int m_ImagecountX;
	int m_ImagecountY;
	float m_EffectPlaySpeed;
};
