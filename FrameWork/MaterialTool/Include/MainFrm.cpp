
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Tool.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Form.h"


// CMainFrame

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

CMainFrame::CMainFrame() noexcept : m_pToolView(nullptr), m_pForm(nullptr)
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

	return 0;
}

_int CMainFrame::Update_Form(_float fDeltaTime)
{
	_int iExit = 0;
	CInputDev* pInputDev = GET_INSTANCE(CInputDev);
	pInputDev->Update_InputDev();
	iExit = m_pForm->Update_Form(fDeltaTime);
	RELEASE_INSTANCE(CInputDev);
	return iExit;
}

HRESULT CMainFrame::Render_Form()
{
	return m_pForm->Render_Form();
}

HRESULT CMainFrame::Init_Window()
{
	RECT rcMain = {};
	this->GetWindowRect(&rcMain); //전체 윈도우크기 구하기

	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);
	RECT rcView{};
	GetClientRect(&rcView); // 현재 클래스의 윈도우 크기 구하기

	const int iGapX = rcMain.right - rcView.right; //갭 차이 구하기
	const int iGapY = rcMain.bottom - rcView.bottom;
	this->SetWindowPos(nullptr, 0, 0, g_iToolCnX + iGapX+550, g_iToolCnY + iGapY, SWP_NOMOVE);
	// 갭차이만큼 더해서 진짜 자신의 크기를 구해준다

	//글로벌 핸들 만들어주기
	g_ToolHwnd = m_pToolView->m_hWnd;
	g_ToolHInst = AfxGetInstanceHandle();
	return S_OK;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

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


	m_tSpliiter.CreateStatic(this, 1, 2);

	m_tSpliiter.CreateView(0, 0, RUNTIME_CLASS(CForm), CSize(550, g_iToolCnY), pContext);
	m_tSpliiter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(g_iToolCnX, g_iToolCnY), pContext);

	m_pForm = (CForm*)m_tSpliiter.GetPane(0, 0);
	m_pToolView = (CToolView*)m_tSpliiter.GetPane(0, 1);

	if (FAILED(Init_Window()))
		return false;

	return true;
}


BOOL CMainFrame::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CFrameWnd::DestroyWindow();
}
