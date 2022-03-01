// MyFormView.cpp: 구현 파일
//

#include "pch.h"
#include "UITool.h"
#include "MyFormView.h"


// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(IDD_CMyFormView)
{

}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyFormView::OnBnClickedButton_Tool)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyFormView::OnBnClickedButton_EffectTool)
END_MESSAGE_MAP()


// CMyFormView 진단

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyFormView 메시지 처리기


void CMyFormView::OnBnClickedButton_Tool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_UIDlg.GetSafeHwnd())
	{
		m_UIDlg.Create(IDD_CUITool_Dlg);
	}
	m_UIDlg.ShowWindow(SW_SHOW);
}

void CMyFormView::OnBnClickedButton_EffectTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_EffectDlg.GetSafeHwnd())
	{
		m_EffectDlg.Create(IDD_CEffectTool_Dlg);
	}
	m_EffectDlg.ShowWindow(SW_SHOW);
}
