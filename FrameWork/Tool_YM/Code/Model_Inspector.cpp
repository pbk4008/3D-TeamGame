// ../Code/Model_Inspector.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Model_Inspector.h"
#include "afxdialogex.h"


// CModel_Inspector 대화 상자

IMPLEMENT_DYNAMIC(CModel_Inspector, CDialogEx)

CModel_Inspector::CModel_Inspector(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CModel_Inspector, pParent)
{

}

CModel_Inspector::~CModel_Inspector()
{
}

void CModel_Inspector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModel_Inspector, CDialogEx)
END_MESSAGE_MAP()


// CModel_Inspector 메시지 처리기
