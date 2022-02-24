#pragma once
#include "MenuForm.h"

// CMap_Tool 대화 상자

class CMap_Tool : public CDialogEx
{
	DECLARE_DYNAMIC(CMap_Tool)

public:
	CMap_Tool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMap_Tool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMap_Tool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	virtual BOOL OnInitDialog();
};
