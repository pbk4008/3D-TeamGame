#pragma once

#include "UITool_Dlg.h"
#include "EffectTool_Dlg.h"

// CMyFormView 폼 보기

class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

protected:
	CMyFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMyFormView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton_Tool();

	CUITool_Dlg m_UIDlg;
	CEffectTool_Dlg m_EffectDlg;
	afx_msg void OnBnClickedButton_EffectTool();
};


