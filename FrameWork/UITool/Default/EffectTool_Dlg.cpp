// EffectTool_Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "UITool.h"
#include "EffectTool_Dlg.h"
#include "afxdialogex.h"
#include "Effect.h"


// CEffectTool_Dlg 대화 상자

IMPLEMENT_DYNAMIC(CEffectTool_Dlg, CDialog)

CEffectTool_Dlg::CEffectTool_Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CEffectTool_Dlg, pParent)
	, m_PositionX(0.f)
	, m_PositionY(0.f)
	, m_PositionZ(0.f)
	, m_VelocityX(1.f)
	, m_VelocityY(10.f)
	, m_VelocityZ(1.f)
	, m_LiftTime(1.f)
	, m_BaseCount(100)
	, m_Age(0)
	, m_ParticleSizeX(1.f)
	, m_ParticleSizeY(1.f)
	, m_iRandomX(0)
	, m_iRandomY(10)
	, m_iRandomZ(0)
	, m_bCheck(true)
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
	DDX_Text(pDX, IDC_EDIT12, m_ParticleSizeX);
	DDX_Text(pDX, IDC_EDIT11, m_ParticleSizeY);
	DDX_Text(pDX, IDC_EDIT15, m_iRandomX);
	DDX_Text(pDX, IDC_EDIT13, m_iRandomY);
	DDX_Text(pDX, IDC_EDIT14, m_iRandomZ);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);

	DDV_MinMaxFloat(pDX, m_PositionX, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_PositionY, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_PositionZ, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_VelocityX, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_VelocityY, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_VelocityZ, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_ParticleSizeX, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_ParticleSizeY, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_iRandomX, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_iRandomY, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_iRandomZ, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_LiftTime, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_Age, 0, 99999);
	DDV_MinMaxInt(pDX, m_BaseCount, 1, 99999);
	DDX_Control(pDX, IDC_TREE1, m_ShaderPathTree);
	DDX_Control(pDX, IDC_TREE3, m_TextureTree);


	DDX_Control(pDX, IDC_CHECK1, m_CheckGravity);
}

void CEffectTool_Dlg::InitialShaderTree()
{
	HTREEITEM hItem = m_ShaderPathTree.InsertItem(_T("..\\..\\Reference\\ShaderFile\\", hItem));

	CFileFind fFinder;

	BOOL bWorking = fFinder.FindFile(_T("\\*.hlsl"));

	while (bWorking)
	{
		bWorking = fFinder.FindNextFile();

		if (fFinder.IsDots()) continue;
		if (fFinder.IsDirectory())
			m_ShaderPathTree.InsertItem(fFinder.GetFileName(), hItem);
	}
	m_ShaderPathTree.EnsureVisible(hItem);
}

void CEffectTool_Dlg::InitialTextureTree()
{
	HTREEITEM hItem = m_TextureTree.InsertItem(_T("..\\bin\\Resource\\Textures\\Effect\\", hItem));

	CFileFind fFinder;

	BOOL bWorking = fFinder.FindFile(_T("\\*.tga"));

	while (bWorking)
	{
		bWorking = fFinder.FindNextFile();

		if (fFinder.IsDots()) continue;
		if (fFinder.IsDirectory())
			m_TextureTree.InsertItem(fFinder.GetFileName(), hItem);
	}
	m_TextureTree.EnsureVisible(hItem);
}

BEGIN_MESSAGE_MAP(CEffectTool_Dlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEffectTool_Dlg::OnLbnSelchangeList1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CEffectTool_Dlg::OnBnClickedButtonApply)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CEffectTool_Dlg::OnTvnSelchangedTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE3, &CEffectTool_Dlg::OnTvnSelchangedTree3)
	ON_BN_CLICKED(IDC_CHECK1, &CEffectTool_Dlg::OnBnClickedCheck1)
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
	CEffect::EFFECTDESC Desc;


	::PathRemoveExtension(m_strPickFileName); //이게 있으면 확장자도 지워짐
	_tcscpy_s(Desc.TextureTag, m_strPickFileName);

	Desc.fPos = { m_PositionX, m_PositionY, m_PositionZ };
	Desc.fVelocity = { m_VelocityX, m_VelocityY, m_VelocityZ };
	Desc.fParticleSize = { m_ParticleSizeX,m_ParticleSizeY };
	Desc.fRandom = { (_float)m_iRandomX, (_float)m_iRandomY,(_float)m_iRandomZ };
	Desc.iNumInstance = m_BaseCount;
	Desc.fMaxLifTime = m_LiftTime;
	Desc.bUsingGravity = m_bCheck;

	//LifeTime, Age아직안함

	wstring ShaderFullPath = ShaderFolderPath + ShaderFileName;
	_tcscpy_s(Desc.ShaderFilePath, ShaderFullPath.c_str());

	wstring Name = m_strPickFileName;
	wstring FullName = L"Prototype_GameObject_" + Name;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_GAMEPLAY, L"Layer_Effect", FullName, &Desc)))
	{
		ERR_MSG(L"Failed to Creating in CEffectTool_Dlg::OnBnClickedButtonApply()");
		return;
	}

	UpdateData(FALSE);
}



BOOL CEffectTool_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitialShaderTree();
	InitialTextureTree();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//쉐이더선택하는트리
void CEffectTool_Dlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem;
	HTREEITEM hParentItem = hSelected;

	CString	  strExt;
	CString   strPath;

	/* 노드 선택시 펼쳐짐 */
	if (0 == m_ShaderPathTree.GetItemData(hSelected))
	{
		CString pathSelectd;

		while (hParentItem != NULL)
		{
			pathSelectd = _T("\\") + pathSelectd;
			pathSelectd = m_ShaderPathTree.GetItemText(hParentItem) + pathSelectd;
			hParentItem = m_ShaderPathTree.GetParentItem(hParentItem);
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
				m_ShaderPathTree.InsertItem(finder.GetFileName(), hSelected);
		}
	}
	m_ShaderPathTree.SetItemData(hSelected, 1);

	/* 선택한 fbx 파일에 대한 정보를 가져온다 */
	CString strSelectItem = m_ShaderPathTree.GetItemText(hSelected);
	wstring strFilter = L".hlsl";

	if (-1 != strSelectItem.Find(strFilter.c_str()))
	{
		ShaderFileName = strSelectItem;

		HTREEITEM temp = m_ShaderPathTree.GetParentItem(hSelected);
		if (NULL != temp)
		{
			ShaderFolderPath = m_ShaderPathTree.GetItemText(temp);
		}
	}

	*pResult = 0;
}

//텍스쳐선택하는 트리
void CEffectTool_Dlg::OnTvnSelchangedTree3(NMHDR* pNMHDR, LRESULT* pResult)
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
			//if (0 != strExt.CompareNoCase(_T("tga")))

			m_TextureTree.InsertItem(finder.GetFileName(), hSelected);
		}
	}
	m_TextureTree.SetItemData(hSelected, 1);

	/* 선택한 fbx 파일에 대한 정보를 가져온다 */
	CString strSelectItem = m_TextureTree.GetItemText(hSelected);
	CString strFilter = L".png";

	if (-1 != strSelectItem.Find(strFilter))
	{
		TextureFileName = strSelectItem;
		_tcscpy_s(m_strPickFileName , TextureFileName.c_str());

		HTREEITEM temp = m_TextureTree.GetParentItem(hSelected);
		if (NULL != temp)
		{
			TextureFolderPath = m_TextureTree.GetItemText(temp);
		}
	}

	*pResult = 0;
}


void CEffectTool_Dlg::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bCheck = m_CheckGravity.GetCheck();
}
