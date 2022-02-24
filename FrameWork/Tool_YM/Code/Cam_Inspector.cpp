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

void CCam_Inspector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCam_Inspector, CDialogEx)
END_MESSAGE_MAP()


// CCam_Inspector 메시지 처리기
