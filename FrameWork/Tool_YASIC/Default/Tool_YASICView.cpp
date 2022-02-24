
// Tool_YASICView.cpp: CToolYASICView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool_YASIC.h"
#endif

#include "Tool_YASICDoc.h"
#include "Tool_YASICView.h"

#include "MainFrm.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


HINSTANCE g_hInst;
HWND g_hWnd;


// CToolYASICView

IMPLEMENT_DYNCREATE(CToolYASICView, CView)

BEGIN_MESSAGE_MAP(CToolYASICView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CToolYASICView 생성/소멸

CToolYASICView::CToolYASICView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolYASICView::~CToolYASICView()
{
}

BOOL CToolYASICView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolYASICView 그리기

void CToolYASICView::Tick()
{
}

void CToolYASICView::OnDraw(CDC* /*pDC*/)
{
	CToolYASICDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolYASICView 인쇄

BOOL CToolYASICView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolYASICView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolYASICView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolYASICView 진단

#ifdef _DEBUG
void CToolYASICView::AssertValid() const
{
	CView::AssertValid();
}

void CToolYASICView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolYASICDoc* CToolYASICView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolYASICDoc)));
	return (CToolYASICDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolYASICView 메시지 처리기


void CToolYASICView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());

	CRect rcMain = {};
	pMainFrame->GetWindowRect(rcMain);

	CRect rcView = {};
	GetClientRect(rcView);
	_int iGapX = rcMain.Width() - rcView.Width();
	_int iGapY = rcMain.Height() - rcView.Height();

	pMainFrame->SetWindowPos(nullptr, 0, 0, 1280 + iGapX + 1, 720 + iGapY + 1, SWP_NOMOVE);

	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();


}
