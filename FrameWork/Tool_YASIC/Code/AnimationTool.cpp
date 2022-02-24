// AnimationTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YASIC.h"
#include "D:\MyData\GitHub\3D_Team\FrameWork\Tool_YASIC\Code\AnimationTool.h"
#include "afxdialogex.h"


// CAnimationTool 대화 상자

IMPLEMENT_DYNAMIC(CAnimationTool, CDialogEx)

CAnimationTool::CAnimationTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAnimationTool, pParent)
{

}

CAnimationTool::~CAnimationTool()
{
}

void CAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnimationTool, CDialogEx)
END_MESSAGE_MAP()


// CAnimationTool 메시지 처리기
