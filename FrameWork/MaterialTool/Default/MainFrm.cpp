
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"

#include "framework.h"
#include "Tool_YASIC.h"
#include "MainFrm.h"
#include "Tool_YASICView.h"
#include "MainForm.h"


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pInstance->Ready_Timer(L"Timer_Default")))
		return E_FAIL;
	if (FAILED(pInstance->Ready_Timer(L"Timer_60")))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

void CMainFrame::Tick()
{
	g_pGameInstance->Update_TimeDelta(L"Timer_Default");
	m_dTimerAcc += g_pGameInstance->Get_TimeDelta(L"Timer_Default");
	if (m_dTimerAcc >=1.0/60.f)
	{
		m_dTimerAcc = 0.0;

		g_pGameInstance->Update_TimeDelta(L"Timer_60");

		if (0 > m_pMainForm->Tick(g_pGameInstance->Get_TimeDelta(L"Timer_60")))
			return;

		if (FAILED(m_pMainForm->Render()))
			return;
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_tMainSplitter.CreateStatic(this, 1, 2);

	m_tMainSplitter.CreateView(0, 0, RUNTIME_CLASS(CToolYASICView), CSize(1280, 720), pContext);
	m_tMainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMainForm), CSize(620, 720), pContext);

	m_pToolView = static_cast<CToolYASICView*>(m_tMainSplitter.GetPane(0, 0));
	CRect rcToolView = {};
	m_pToolView->GetWindowRect(rcToolView);

	m_pMainForm = static_cast<CMainForm*>(m_tMainSplitter.GetPane(0, 1));
	CRect rcMainForm = {};
	m_pMainForm->GetWindowRect(rcMainForm);

	CRect rcMain;
	this->GetWindowRect(rcMain);
	this->SetWindowPos(nullptr, 10, 100, rcToolView.Width() + rcMainForm.Width(), rcMain.Height(), NULL);

	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}
