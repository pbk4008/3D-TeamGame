#pragma once


// CUITool_Dlg 대화 상자

class CUITool_Dlg : public CDialog
{
	DECLARE_DYNAMIC(CUITool_Dlg)

public:
	CUITool_Dlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUITool_Dlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUITool_Dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

public:
	wstring m_strFileName;
	wstring m_strPickFileName;
	CListBox m_ListBox;

	class CMFCObject_UI* m_pObject = nullptr;

	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonApply();
	float m_PositionX;
	float m_PositionY;
	float m_SizeX;
	float m_SizeY;
	afx_msg void OnBnClickedButtonStateSetting();
};
