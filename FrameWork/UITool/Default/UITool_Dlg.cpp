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
	ON_LBN_SELCHANGE(IDC_LIST1, &CUITool_Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CUITool_Dlg::OnBnClickedButtonApply)
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


void CUITool_Dlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	unsigned int iIndex = m_ListBox.GetCurSel();

	CString SelectName = L"";
	m_ListBox.GetText(iIndex, SelectName);

	unsigned int i = 0;
	for (; i < SelectName.GetLength(); ++i)
	{
		if (::isdigit(SelectName[i]))
		{
			break;
		}
	}
}


void CUITool_Dlg::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	
	CString FullName = L"Prototype_GameObject_" + m_strFileName;

	
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(0,L"Layer_UI", L"Prototype_GameObject_UI", L"Texture")))
	{
		ERR_MSG(L"Failed to Creating in CObjectToolDlg");
		return;
	}

}
