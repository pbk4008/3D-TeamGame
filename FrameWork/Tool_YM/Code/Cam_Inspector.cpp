// ../Code/Cam_Inspector.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Cam_Inspector.h"
#include "afxdialogex.h"


// CCam_Inspector 대화 상자

IMPLEMENT_DYNAMIC(CCam_Inspector, CDialogEx)

CCam_Inspector::CCam_Inspector(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCam_Inspector, pParent)
{

}

CCam_Inspector::~CCam_Inspector()
{
}


BOOL CCam_Inspector::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCam_Inspector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCam_Inspector, CDialogEx)
END_MESSAGE_MAP()


// CCam_Inspector 메시지 처리기


void CCam_Inspector::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	delete this;

	CDialogEx::PostNcDestroy();
}

