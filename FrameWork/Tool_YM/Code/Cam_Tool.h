#pragma once

#include "Menu_Form.h"

// CCam_Tool 대화 상자

class CCam_Tool : public CDialogEx
{
	DECLARE_DYNAMIC(CCam_Tool)

public:
	CCam_Tool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCam_Tool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCam_Tool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	//개발자 정의

public:
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
};
