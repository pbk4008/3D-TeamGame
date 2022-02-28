// EffectTool_Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "UITool.h"
#include "EffectTool_Dlg.h"
#include "afxdialogex.h"
#include "MFCEffect.h"


// CEffectTool_Dlg 대화 상자

IMPLEMENT_DYNAMIC(CEffectTool_Dlg, CDialog)

CEffectTool_Dlg::CEffectTool_Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CEffectTool_Dlg, pParent)
	, m_PositionX(0)
	, m_PositionY(0)
	, m_PositionZ(0)
	, m_VelocityX(1)
	, m_VelocityY(1)
	, m_VelocityZ(1)
	, m_LiftTime(1)
	, m_BaseCount(1)
	, m_Age(0)
{

}

CEffectTool_Dlg::~CEffectTool_Dlg()
{
}

void CEffectTool_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_PositionX);
	DDX_Text(pDX, IDC_EDIT2, m_PositionY);
	DDX_Text(pDX, IDC_EDIT3, m_PositionZ);
	DDX_Text(pDX, IDC_EDIT4, m_VelocityX);
	DDX_Text(pDX, IDC_EDIT5, m_VelocityY);
	DDX_Text(pDX, IDC_EDIT6, m_VelocityZ);
	DDX_Text(pDX, IDC_EDIT7, m_LiftTime);
	DDX_Text(pDX, IDC_EDIT8, m_BaseCount);
	DDX_Text(pDX, IDC_EDIT9, m_Age);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);

	DDV_MinMaxFloat(pDX, m_PositionX, 0, 99999);
	DDV_MinMaxFloat(pDX, m_PositionY, 0, 99999);
	DDV_MinMaxFloat(pDX, m_PositionZ, 0, 99999);
	DDV_MinMaxFloat(pDX, m_VelocityX, 1, 99999);
	DDV_MinMaxFloat(pDX, m_VelocityY, 1, 99999);
	DDV_MinMaxFloat(pDX, m_VelocityZ, 1, 99999);
	DDV_MinMaxFloat(pDX, m_LiftTime, 1, 99999);
	DDV_MinMaxFloat(pDX, m_Age, 0, 99999);
	DDV_MinMaxInt(pDX, m_BaseCount, 1, 99999);
}


BEGIN_MESSAGE_MAP(CEffectTool_Dlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEffectTool_Dlg::OnLbnSelchangeList1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CEffectTool_Dlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CEffectTool_Dlg 메시지 처리기

void CEffectTool_Dlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	unsigned int iIndex = m_ListBox.GetCurSel();

	CString SelectName = L"";

	m_ListBox.GetText(iIndex, SelectName);
	_tcscpy_s(m_strPickFileName, SelectName);
}


void CEffectTool_Dlg::OnDropFiles(HDROP hDropInfo)
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

		//m_strFileName = FileName;
	}

	CDialog::OnDropFiles(hDropInfo);
}


void CEffectTool_Dlg::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	CMFCEffect::EFFECTDESC Desc;
	_tcscpy_s(Desc.TextureTag, m_strPickFileName);
	Desc.fPos = { m_PositionX, m_PositionY, m_PositionZ,1.f };
	Desc.fVelocity = { m_VelocityX, m_VelocityY, m_VelocityZ };

	wstring Name = m_strPickFileName;
	wstring FullName = L"Prototype_GameObject_" + Name;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_Effect", FullName, &Desc)))
	{
		ERR_MSG(L"Failed to Creating in CEffectTool_Dlg::OnBnClickedButtonApply()");
		return;
	}

	UpdateData(FALSE);
}
