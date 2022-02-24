// ../Code/Inspector.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Inspector_Form.h"


// CInspector_Form

IMPLEMENT_DYNCREATE(CInspector_Form, CFormView)

CInspector_Form::CInspector_Form()
	: CFormView(IDD_CInspector)
{

}

CInspector_Form::~CInspector_Form()
{
}

void CInspector_Form::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInspector_Form, CFormView)
END_MESSAGE_MAP()


// CInspector_Form 진단

#ifdef _DEBUG
void CInspector_Form::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInspector_Form::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInspector_Form 메시지 처리기
