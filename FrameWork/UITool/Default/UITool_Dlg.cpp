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
	, m_IDTag(0)
	, m_PositionZ(0)
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
	DDX_Text(pDX, IDC_EDIT5, m_IDTag);

	DDV_MinMaxFloat(pDX, m_PositionX, 0, 99999);
	DDV_MinMaxFloat(pDX, m_PositionY, 0, 99999);
	DDV_MinMaxFloat(pDX, m_PositionZ, 0, 99999);
	DDV_MinMaxFloat(pDX, m_SizeX, 1, 99999);
	DDV_MinMaxFloat(pDX, m_SizeY, 1, 99999);
	DDV_MinMaxFloat(pDX, m_IDTag, 0, 99999);
	DDX_Control(pDX, IDC_TREE1, m_TextureTree);
	DDX_Text(pDX, IDC_EDIT7, m_PositionZ);
}


BEGIN_MESSAGE_MAP(CUITool_Dlg, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CUITool_Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CUITool_Dlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON2, &CUITool_Dlg::OnBnClickedButtonStateSetting)
	ON_BN_CLICKED(IDC_BUTTON3, &CUITool_Dlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CUITool_Dlg::OnBnClickedButtonLoad)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CUITool_Dlg::OnTvnSelchangedTree1)
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

void CUITool_Dlg::InitialTextureTree()
{
	HTREEITEM hItem = m_TextureTree.InsertItem(_T("..\\bin\\Resource\\Textures\\UI\\", hItem));

	CFileFind fFinder;

	BOOL bWorking = fFinder.FindFile(_T("\\*.dds"));

	while (bWorking)
	{
		bWorking = fFinder.FindNextFile();

		if (fFinder.IsDots()) continue;
		if (fFinder.IsDirectory())
			m_TextureTree.InsertItem(fFinder.GetFileName(), hItem);
	}
	m_TextureTree.EnsureVisible(hItem);
}

void CUITool_Dlg::Setting_Desc(CUI::UIDESC* Desc)
{
	UpdateData(TRUE);

	m_PositionX = Desc->fPos.x;
	m_PositionY = Desc->fPos.y;
	m_PositionZ = Desc->fPos.z;
	m_SizeX = Desc->fSize.x;
	m_SizeY = Desc->fSize.y;
	m_IDTag = Desc->IDTag;
	_tcscpy_s(m_strPickFileName, Desc->TextureTag);

	UpdateData(FALSE);
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
	CUI::UIDESC Desc;
	//Desc.TextureTag = m_strPickFileName;
	::PathRemoveExtension(m_strPickFileName); //이게 있으면 확장자도 지워짐
	_tcscpy_s(Desc.TextureTag, m_strPickFileName);

	Desc.fPos = { m_PositionX,m_PositionY, m_PositionZ};
	Desc.fSize = { m_SizeX,m_SizeY };
	Desc.IDTag = m_IDTag;
	
	wstring Name = m_strPickFileName;
	wstring FullName = L"Prototype_GameObject_UI_" + Name;

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
		CUI::UIDESC Desc;
		_tcscpy_s(Desc.TextureTag, m_pObject->Get_UIDesc().TextureTag);
		Desc.fPos = { m_PositionX,m_PositionY, m_PositionZ };
		Desc.fSize = { m_SizeX,m_SizeY };
		Desc.IDTag = m_IDTag;
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
		wstring FullName = L"Prototype_GameObject_UI_" + Tag;
		
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_UI", FullName, &m_vecUI[i])))
		{
			ERR_MSG(L"Failed to Creating in CMFCLevel_Logo::NativeConstruct()");
			return;
		}
	}
}


void CUITool_Dlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem;
	HTREEITEM hParentItem = hSelected;

	CString	  strExt;
	CString   strPath;

	/* 노드 선택시 펼쳐짐 */
	if (0 == m_TextureTree.GetItemData(hSelected))
	{
		CString pathSelectd;

		while (hParentItem != NULL)
		{
			pathSelectd = _T("\\") + pathSelectd;
			pathSelectd = m_TextureTree.GetItemText(hParentItem) + pathSelectd;
			hParentItem = m_TextureTree.GetParentItem(hParentItem);
		}

		pathSelectd = pathSelectd + _T("*.*");

		CFileFind finder;
		BOOL bWorking = finder.FindFile(pathSelectd);

		while (bWorking) {
			bWorking = finder.FindNextFile();
			strPath = finder.GetFileName();
			AfxExtractSubString(strExt, strPath, 1, '.');

			if (finder.IsDots()) continue;
				if (0 != strExt.CompareNoCase(_T("tga")))

			m_TextureTree.InsertItem(finder.GetFileName(), hSelected);
		}
	}
	m_TextureTree.SetItemData(hSelected, 1);

	/* 선택한 fbx 파일에 대한 정보를 가져온다 */
	CString strSelectItem = m_TextureTree.GetItemText(hSelected);
	CString strFilter = L".dds";

	if (-1 != strSelectItem.Find(strFilter))
	{
		TextureFileName = strSelectItem;
		_tcscpy_s(m_strPickFileName, TextureFileName.c_str());

		HTREEITEM temp = m_TextureTree.GetParentItem(hSelected);
		if (NULL != temp)
		{
			//TextureFolderPath = m_TextureTree.GetItemText(temp);
			//int a = 0;
		}
	}

	*pResult = 0;
}


BOOL CUITool_Dlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitialTextureTree();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
