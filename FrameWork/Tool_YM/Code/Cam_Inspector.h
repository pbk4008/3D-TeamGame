#pragma once


// CCam_Inspector 대화 상자

class CCam_Inspector : public CDialogEx
{
	DECLARE_DYNAMIC(CCam_Inspector)

public:
	CCam_Inspector(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCam_Inspector();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCam_Inspector };
#endif

//개발자 재정의

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	virtual BOOL OnInitDialog();
};
