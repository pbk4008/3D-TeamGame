
// Tool_YMView.cpp: CToolYMView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool_YM.h"
#endif

#include "Tool_YMDoc.h"
#include "Tool_YMView.h"
#include "Tool_Defines.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolYMView
HWND g_hWnd;
HINSTANCE g_hInst;
IMPLEMENT_DYNCREATE(CToolYMView, CView)

BEGIN_MESSAGE_MAP(CToolYMView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CToolYMView 생성/소멸

CToolYMView::CToolYMView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolYMView::~CToolYMView()
{
}

BOOL CToolYMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolYMView 그리기

void CToolYMView::OnDraw(CDC* /*pDC*/)
{
	CToolYMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolYMView 인쇄

BOOL CToolYMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolYMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolYMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolYMView 진단

#ifdef _DEBUG
void CToolYMView::AssertValid() const
{
	CView::AssertValid();
}

void CToolYMView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolYMDoc* CToolYMView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolYMDoc)));
	return (CToolYMDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolYMView 메시지 처리기


void CToolYMView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Tool View 초기화

	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	RECT rcMain = {};
	m_pMainFrm->GetWindowRect(&rcMain);

	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);
	RECT rcView{};
	GetClientRect(&rcView);

	_int iGapX = rcMain.right - rcView.right;
	_int iGapY = rcMain.bottom - rcView.bottom;

	m_pMainFrm->SetWindowPos(nullptr, 0, 0, g_iWINCX + iGapX, g_iWINCY + iGapY, SWP_NOMOVE);
	m_hInst = AfxGetInstanceHandle();
}
