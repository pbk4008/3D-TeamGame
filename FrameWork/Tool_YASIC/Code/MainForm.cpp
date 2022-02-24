// MainForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YASIC.h"
#include "MainForm.h"

#include "AnimationTool.h"

// CMainForm

IMPLEMENT_DYNCREATE(CMainForm, CFormView)

CMainForm::CMainForm()
	: CFormView(IDD_CMainForm)
{

}

CMainForm::~CMainForm()
{
	Safe_Delete(m_pAnimationTool);
}

void CMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CMainForm, CFormView)
END_MESSAGE_MAP()


// CMainForm 진단

#ifdef _DEBUG
void CMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainForm 메시지 처리기


void CMainForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect rcMainForm;
	this->GetWindowRect(rcMainForm);

	m_tab.MoveWindow(0, 0, rcMainForm.Width(), rcMainForm.Height());

	m_tab.InsertItem((_int)ETab::AnimationTool, L"AnimationTool");

	CRect rcTab;
	m_tab.GetWindowRect(rcTab);

	m_pAnimationTool = new CAnimationTool();
	m_pAnimationTool->Create(IDD_CAnimationTool, &m_tab);
	m_pAnimationTool->MoveWindow(0, 25, rcTab.Width(), rcTab.Height());
	m_pAnimationTool->ShowWindow(SW_SHOW);
}

void CMainForm::Tick()
{
}
