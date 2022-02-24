// ../Code/Cam_Tool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Cam_Tool.h"
#include "afxdialogex.h"


// CCam_Tool 대화 상자

IMPLEMENT_DYNAMIC(CCam_Tool, CDialogEx)

CCam_Tool::CCam_Tool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCam_Tool, pParent)
{

}

CCam_Tool::~CCam_Tool()
{
}

void CCam_Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCam_Tool, CDialogEx)
END_MESSAGE_MAP()


// CCam_Tool 메시지 처리기



BOOL CCam_Tool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCam_Tool::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	delete this;

	CDialogEx::PostNcDestroy();
}
