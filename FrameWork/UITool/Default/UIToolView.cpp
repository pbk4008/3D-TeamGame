
// UIToolView.cpp: CUIToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "UITool.h"
#endif

#include "UIToolDoc.h"
#include "UIToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView
HINSTANCE g_hInst;
HWND g_hWnd;

IMPLEMENT_DYNCREATE(CUIToolView, CView)

BEGIN_MESSAGE_MAP(CUIToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CUIToolView 생성/소멸

CUIToolView::CUIToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CUIToolView::~CUIToolView()
{
}

BOOL CUIToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CUIToolView 그리기

void CUIToolView::OnDraw(CDC* /*pDC*/)
{
	CUIToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CUIToolView 인쇄

BOOL CUIToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CUIToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CUIToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CUIToolView 진단

#ifdef _DEBUG
void CUIToolView::AssertValid() const
{
	CView::AssertValid();
}

void CUIToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUIToolDoc* CUIToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIToolDoc)));
	return (CUIToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CUIToolView 메시지 처리기


void CUIToolView::OnInitialUpdate()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;

	auto pMainWnd = ::AfxGetApp()->m_pMainWnd;
	if (nullptr == pMainWnd)
	{
		ERR_MSG(L"Failed to Get MainWnd In CUIToolView::OnInitialUpdate");
		return;
	}

	RECT rcMainWnd;
	pMainWnd->GetWindowRect(&rcMainWnd);
	unsigned int iMainWndWidth = rcMainWnd.right - rcMainWnd.left;
	unsigned int iMainWndHeight = rcMainWnd.bottom - rcMainWnd.top;

	RECT rcView;
	GetClientRect(&rcView);
	unsigned int iClientWndWidth = rcView.right - rcView.left;
	unsigned int iClientWndHeight = rcView.bottom - rcView.top;

	unsigned int iGapX = iMainWndWidth - iClientWndWidth;
	unsigned int iGapY = iMainWndHeight - iClientWndHeight;

	pMainWnd->SetWindowPos(nullptr, 100, 100, WINCX + iGapX, WINCY + iGapY, SWP_NOOWNERZORDER);
}
