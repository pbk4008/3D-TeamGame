// UITool_Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "UITool.h"
#include "UITool_Dlg.h"
#include "afxdialogex.h"
#include "MFCObject_UI.h"


// CUITool_Dlg 대화 상자

IMPLEMENT_DYNAMIC(CUITool_Dlg, CDialog)

CUITool_Dlg::CUITool_Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUITool_Dlg, pParent)
	, m_PositionX(0)
	, m_PositionY(0)
	, m_SizeX(0)
	, m_SizeY(0)
{

}

CUITool_Dlg::~CUITool_Dlg()
{
}

void CUITool_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Text(pDX, IDC_EDIT1, m_PositionX);
	DDX_Text(pDX, IDC_EDIT2, m_PositionY);
	DDX_Text(pDX, IDC_EDIT3, m_SizeX);
	DDX_Text(pDX, IDC_EDIT4, m_SizeY);

	DDV_MinMaxFloat(pDX, m_PositionX, 0, 99999);
	DDV_MinMaxFloat(pDX, m_PositionY, 0, 99999);
	DDV_MinMaxFloat(pDX, m_SizeX, 1, 99999);
	DDV_MinMaxFloat(pDX, m_SizeY, 1, 99999);
}


BEGIN_MESSAGE_MAP(CUITool_Dlg, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CUITool_Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CUITool_Dlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON2, &CUITool_Dlg::OnBnClickedButtonStateSetting)
	ON_BN_CLICKED(IDC_BUTTON3, &CUITool_Dlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CUITool_Dlg::OnBnClickedButtonLoad)
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
	_tcscpy_s(m_strPickFileName, SelectName);
	//m_strPickFileName = SelectName;
	
	//int iTextureNameSize = SelectName.GetLength();
	//unsigned int i = 0;

	//for (; i < iTextureNameSize; ++i)
	//{
	//	if (isdigit(SelectName[i]))
	//	{
	//		break;
	//	}
	//}

}

void CUITool_Dlg::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	CMFCObject_UI::UIDESC Desc;
	//Desc.TextureTag = m_strPickFileName;
	_tcscpy_s(Desc.TextureTag, m_strPickFileName);
	Desc.fPos = { m_PositionX,m_PositionY};
	Desc.fScale = { m_SizeX,m_SizeY };
	
	wstring Name = m_strPickFileName;
	wstring FullName = L"Prototype_GameObject_" + Name;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_UI", FullName, &Desc)))
	{
		ERR_MSG(L"Failed to Creating in CUITool_Dlg::OnBnClickedButtonApply()");
		return;
	}

	UpdateData(FALSE);
}


void CUITool_Dlg::OnBnClickedButtonStateSetting()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr != m_pObject)
	{
		CMFCObject_UI::UIDESC Desc;
		_tcscpy_s(Desc.TextureTag, m_pObject->Get_UIDesc().TextureTag);
		Desc.fPos = { m_PositionX,m_PositionY };
		Desc.fScale = { m_SizeX,m_SizeY };
		m_pObject->Set_UIDesc(Desc);
	}
	UpdateData(FALSE);
}


void CUITool_Dlg::OnBnClickedButtonSave()
{
	/*CMFCObject_UI* Obj = (CMFCObject_UI*)g_pGameInstance->getObjectList(0, L"Layer_UI")->back();
	if (nullptr != Obj)
	{
		m_vecUI.push_back(Obj->Get_UIDesc());
	}*/
	list<CGameObject*> ListObj = *g_pGameInstance->getObjectList(0, L"Layer_UI");

	if (!ListObj.empty())
	{
		for (auto pObj : ListObj)
		{
			CMFCObject_UI* pUI = (CMFCObject_UI*)pObj;
			m_vecUI.push_back(pUI->Get_UIDesc());
			int a = 0;
		}
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(false, L"dat", L"*.dat"); //저장, 디폴트확장자, 디폴트파일이름
	TCHAR szFilePath[MAX_PATH] = L"";

	//GetCurrentDirectory(MAX_PATH, szFilePath); //현재 경로를 얻어오는 함수
	//PathRemoveFileSpec(szFilePath); //최하단의 경로를 잘라주는 함수
	//lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		wstring strFilePath = Dlg.GetPathName();
		g_pGameInstance->SaveFile<CMFCObject_UI::UIDESC>(&m_vecUI, strFilePath);
	}
}


void CUITool_Dlg::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(true, L"dat", L"*.dat");
	TCHAR szFilePath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;


	if (IDOK == Dlg.DoModal())
	{
		wstring strFilePath = Dlg.GetPathName();
		g_pGameInstance->LoadFile<CMFCObject_UI::UIDESC>(m_vecUI, strFilePath);
	}

	for (int i = 0; i < m_vecUI.size(); ++i)
	{
		wstring Tag = m_vecUI[i].TextureTag;
		wstring FullName = L"Prototype_GameObject_" + Tag;
		
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_UI", FullName, &m_vecUI[i])))
		{
			ERR_MSG(L"Failed to Creating in CMFCLevel_Logo::NativeConstruct()");
			return;
		}
	}
}
