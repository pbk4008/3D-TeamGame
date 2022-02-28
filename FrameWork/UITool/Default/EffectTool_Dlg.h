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

	DECLARE_MESSAGE_MAP()
public:
	_tchar m_strPickFileName[MAX_PATH];

	float m_PositionX;
	float m_PositionY;
	float m_PositionZ;
	float m_VelocityX;
	float m_VelocityY;
	float m_VelocityZ;
	float m_LiftTime;
	int m_BaseCount;
	float m_Age;
	CListBox m_ListBox;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonApply();
};
