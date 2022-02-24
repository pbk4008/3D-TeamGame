// UITool_Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "UITool.h"
#include "UITool_Dlg.h"
#include "afxdialogex.h"


// CUITool_Dlg 대화 상자

IMPLEMENT_DYNAMIC(CUITool_Dlg, CDialog)

CUITool_Dlg::CUITool_Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUITool_Dlg, pParent)
{

}

CUITool_Dlg::~CUITool_Dlg()
{
}

void CUITool_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CUITool_Dlg, CDialog)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CUITool_Dlg 메시지 처리기


void CUITool_Dlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	unsigned int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFilePath[128] = L"";
	CString FileName = L"";

	for (unsigned int i = 0; i < iCount; ++i)
	{
		// -1이 아니기때문에 드래그앤 드롭된 대상 파일중 i번째의 대상파일의 경로를 리턴한다.
		::DragQueryFile(hDropInfo, i, szFilePath, 128);

		::PathRemoveExtension(szFilePath); //이게 있으면 확장자도 지워짐

		// 경로에서 파일이름과 확장자를 추출하는 함수
		FileName = ::PathFindFileName(szFilePath);

		m_ListBox.AddString(FileName);

		m_strFileName = FileName;
	}

	CDialog::OnDropFiles(hDropInfo);
}
