#pragma once


// CModel_Inspector 대화 상자

class CModel_Inspector : public CDialogEx
{
	DECLARE_DYNAMIC(CModel_Inspector)

public:
	CModel_Inspector(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CModel_Inspector();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CModel_Inspector1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
