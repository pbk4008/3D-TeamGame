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
	, m_Velocity(10.f)
	, m_LiftTime(1.f)
	, m_BaseCount(100)
	, m_Age(0)
	, m_ParticleSizeX(1.f)
	, m_ParticleSizeY(1.f)
	, m_fRandomPosX(1)
	, m_fRandomPosY(1)
	, m_fRandomPosZ(1)
	, m_fRandomDirX(360)
	, m_fRandomDirY(360)
	, m_fRandomDirZ(360)
	, m_bCheck(false)
	, m_fRandomMinusDirX(1)
	, m_fRandomMinusDirY(1)
	, m_fRandomMinusDirZ(1)
	, m_EffectName(_T(""))
	, m_RenderPassNum(1)
	, m_frame(1)
	, m_ImagecountX(1)
	, m_ImagecountY(1)
	, m_EffectPlaySpeed(1)
	, m_IDTag(0)
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
	DDX_Text(pDX, IDC_EDIT6, m_Velocity);
	DDX_Text(pDX, IDC_EDIT7, m_LiftTime);
	DDX_Text(pDX, IDC_EDIT8, m_BaseCount);
	DDX_Text(pDX, IDC_EDIT9, m_Age);
	DDX_Text(pDX, IDC_EDIT12, m_ParticleSizeX);
	DDX_Text(pDX, IDC_EDIT11, m_ParticleSizeY);
	DDX_Text(pDX, IDC_EDIT15, m_fRandomPosX);
	DDX_Text(pDX, IDC_EDIT13, m_fRandomPosY);
	DDX_Text(pDX, IDC_EDIT14, m_fRandomPosZ);
	DDX_Text(pDX, IDC_EDIT16, m_fRandomDirX);
	DDX_Text(pDX, IDC_EDIT17, m_fRandomDirY);
	DDX_Text(pDX, IDC_EDIT18, m_fRandomDirZ);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);

	DDV_MinMaxFloat(pDX, m_PositionX, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_PositionY, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_PositionZ, -99999, 99999);
	DDV_MinMaxFloat(pDX, m_Velocity, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_ParticleSizeX, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_ParticleSizeY, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_fRandomPosX, 1, 99999);
	DDV_MinMaxFloat(pDX, m_fRandomPosY, 1, 99999);
	DDV_MinMaxFloat(pDX, m_fRandomPosZ, 1, 99999);
	DDV_MinMaxFloat(pDX, m_fRandomDirX, 1, 99999);
	DDV_MinMaxFloat(pDX, m_fRandomDirY, 1, 99999);
	DDV_MinMaxFloat(pDX, m_fRandomDirZ, 1, 99999);
	DDV_MinMaxFloat(pDX, m_LiftTime, 0.1, 99999);
	DDV_MinMaxFloat(pDX, m_Age, 0, 99999);
	DDV_MinMaxFloat(pDX, m_IDTag, 0, 99999);
	DDV_MinMaxInt(pDX, m_BaseCount, 1, 99999);


	DDX_Control(pDX, IDC_TREE1, m_ShaderPathTree);
	DDX_Control(pDX, IDC_TREE3, m_TextureTree);
	DDX_Control(pDX, IDC_CHECK1, m_CheckGravity);

	DDX_Control(pDX, IDC_RADIO_X, m_AxisXBtn);
	DDX_Control(pDX, IDC_RADIO_Y, m_AxisYBtn);
	DDX_Control(pDX, IDC_RADIO_Z, m_AxisZBtn);
	DDX_Control(pDX, IDC_RADIO_ALL, m_AxisAllBtn);
	DDX_Text(pDX, IDC_EDIT19, m_fRandomMinusDirX);
	DDX_Text(pDX, IDC_EDIT20, m_fRandomMinusDirY);
	DDX_Text(pDX, IDC_EDIT21, m_fRandomMinusDirZ);
	DDX_Text(pDX, IDC_EDIT4, m_EffectName);
	DDX_Text(pDX, IDC_EDIT10, m_RenderPassNum);
	DDX_Text(pDX, IDC_EDIT22, m_frame);
	DDX_Text(pDX, IDC_EDIT23, m_ImagecountX);
	DDX_Text(pDX, IDC_EDIT24, m_ImagecountY);
	DDX_Text(pDX, IDC_EDIT25, m_EffectPlaySpeed);
	DDX_Text(pDX, IDC_EDIT26, m_IDTag);
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
	ON_BN_CLICKED(IDC_BUTTON2, &CEffectTool_Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CEffectTool_Dlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CEffectTool_Dlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON8, &CEffectTool_Dlg::OnBnClickedButtonNameTag)
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
	
	::PathRemoveExtension(m_strPickFileName); //이게 있으면 확장자도 지워짐
	_tcscpy_s(m_EffectDesc.TextureTag, m_strPickFileName);
	m_EffectDesc.fMyPos = { 0.f,0.f,0.f }; //transform위치이동은안함
	m_EffectDesc.ParticleMat = XMMatrixIdentity();
	m_EffectDesc.ParticleMat.r[3] = { m_PositionX, m_PositionY, m_PositionZ, 1.f };
	m_EffectDesc.fParticleVelocity = m_Velocity;
	m_EffectDesc.fParticleSize = { m_ParticleSizeX,m_ParticleSizeY };
	m_EffectDesc.fParticleRandomPos = { m_fRandomPosX, m_fRandomPosY,m_fRandomPosZ };
	m_EffectDesc.fParticleMinusRandomDir = { m_fRandomMinusDirX, m_fRandomMinusDirY,m_fRandomMinusDirZ };
	m_EffectDesc.fParticleRandomDir = { m_fRandomDirX, m_fRandomDirY,m_fRandomDirZ };
	m_EffectDesc.iNumInstance = m_BaseCount;
	m_EffectDesc.fMaxLifeTime = m_LiftTime;
	m_EffectDesc.fCurTime = m_Age;
	m_EffectDesc.bUsingGravity = m_bCheck;
	m_EffectDesc.iAxis = 0;

	m_EffectDesc.fEffectPlaySpeed = m_EffectPlaySpeed;
	m_EffectDesc.fFrame = m_frame;
	m_EffectDesc.iImageCountX = m_ImagecountX;
	m_EffectDesc.iImageCountY = m_ImagecountY;
	m_EffectDesc.iRenderPassNum = m_RenderPassNum;
	m_EffectDesc.IDTag = m_IDTag;

	if (m_AxisXBtn.GetCheck() == BST_CHECKED)
	{
		m_EffectDesc.iAxis = 0;
	}
	else if (m_AxisYBtn.GetCheck() == BST_CHECKED)
	{
		m_EffectDesc.iAxis = 1;
	}
	else if (m_AxisZBtn.GetCheck() == BST_CHECKED)
	{
		m_EffectDesc.iAxis = 2;
	}
	else if (m_AxisAllBtn.GetCheck() == BST_CHECKED)
	{
		m_EffectDesc.iAxis = 3;
	}

	//Age아직안함
	wstring ShaderFullPath = ShaderFolderPath + ShaderFileName;
	_tcscpy_s(m_EffectDesc.ShaderFilePath, ShaderFolderPath.c_str());
	_tcscpy_s(m_EffectDesc.ShaderFileName, ShaderFileName.c_str());
	_tcscpy_s(m_EffectDesc.ShaderFullFilePath, ShaderFullPath.c_str());

	wstring Name = m_strPickFileName;
	wstring FullName = L"Prototype_GameObject_Effect"/* + Name*/;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_GAMEPLAY, L"Layer_Effect", FullName, &m_EffectDesc)))
	{
		ERR_MSG(L"Failed to Creating in CEffectTool_Dlg::OnBnClickedButtonApply()");
		return;
	}

	m_vecEffect.push_back(m_EffectDesc);

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
	CString strFilter = L".tga";

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
	UpdateData(TRUE);
	m_bCheck = m_CheckGravity.GetCheck();
	UpdateData(FALSE);
}

void CEffectTool_Dlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bReset = true;

	UpdateData(TRUE);

	::PathRemoveExtension(m_strPickFileName); //이게 있으면 확장자도 지워짐
	_tcscpy_s(m_EffectDesc.TextureTag, m_strPickFileName);
	m_EffectDesc.fMyPos = { 0.f,0.f,0.f }; //transform위치이동은안함
	m_EffectDesc.ParticleMat = XMMatrixIdentity();
	m_EffectDesc.ParticleMat.r[3] = { m_PositionX, m_PositionY, m_PositionZ, 1.f };
	m_EffectDesc.fParticleVelocity = m_Velocity;
	m_EffectDesc.fParticleSize = { m_ParticleSizeX,m_ParticleSizeY };
	m_EffectDesc.fParticleRandomPos = { m_fRandomPosX, m_fRandomPosY,m_fRandomPosZ };
	m_EffectDesc.fParticleMinusRandomDir = { m_fRandomMinusDirX, m_fRandomMinusDirY,m_fRandomMinusDirZ };
	m_EffectDesc.fParticleRandomDir = { m_fRandomDirX, m_fRandomDirY,m_fRandomDirZ };
	m_EffectDesc.iNumInstance = m_BaseCount;
	m_EffectDesc.fMaxLifeTime = m_LiftTime;
	m_EffectDesc.fCurTime = m_Age;
	m_EffectDesc.bUsingGravity = m_bCheck;
	m_EffectDesc.iAxis = 0;

	m_EffectDesc.fEffectPlaySpeed = m_EffectPlaySpeed;
	m_EffectDesc.fFrame = m_frame;
	m_EffectDesc.iImageCountX = m_ImagecountX;
	m_EffectDesc.iImageCountY = m_ImagecountY;
	m_EffectDesc.iRenderPassNum = m_RenderPassNum;
	m_EffectDesc.IDTag = m_IDTag;

	if (m_AxisXBtn.GetCheck() == BST_CHECKED)
	{
		m_EffectDesc.iAxis = 0;
	}
	else if (m_AxisYBtn.GetCheck() == BST_CHECKED)
	{
		m_EffectDesc.iAxis = 1;
	}
	else if (m_AxisZBtn.GetCheck() == BST_CHECKED)
	{
		m_EffectDesc.iAxis = 2;
	}
	else if (m_AxisAllBtn.GetCheck() == BST_CHECKED)
	{
		m_EffectDesc.iAxis = 3;
	}

	//Age아직안함
	wstring ShaderFullPath = ShaderFolderPath + ShaderFileName;

	_tcscpy_s(m_EffectDesc.ShaderFilePath, ShaderFolderPath.c_str());
	_tcscpy_s(m_EffectDesc.ShaderFileName, ShaderFileName.c_str());
	_tcscpy_s(m_EffectDesc.ShaderFullFilePath, ShaderFullPath.c_str());

	UpdateData(FALSE);
}


void CEffectTool_Dlg::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	list<CGameObject*> ListObj = *g_pGameInstance->getObjectList(1, L"Layer_Effect");

	//if (!ListObj.empty())
	//{
	//	for (auto pObj : ListObj)
	//	{
	//		CMFCEffect* pEffect = (CMFCEffect*)pObj;
	//		m_vecEffect.push_back(pEffect->Get_EffectDesc());
	//		int a = 0;
	//	}
	//}

	CFileDialog Dlg(false, L"dat", L"*.dat"); //저장, 디폴트확장자, 디폴트파일이름
	TCHAR szFilePath[MAX_PATH] = L"";

	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		wstring strFilePath = Dlg.GetPathName();
		g_pGameInstance->SaveFile<CMFCEffect::EFFECTDESC>(&m_vecEffect, strFilePath);
	}
}


void CEffectTool_Dlg::OnBnClickedButtonLoad()
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
		g_pGameInstance->LoadFile<CMFCEffect::EFFECTDESC>(m_vecEffect, strFilePath);
		int a = 0;
	}

	for (int i = 0; i < m_vecEffect.size(); ++i)
	{
		wstring Tag = m_vecEffect[i].TextureTag;
		wstring FullName = L"Prototype_GameObject_Effect"/* + Tag*/;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_GAMEPLAY, L"Layer_Effect", FullName, &m_vecEffect[i])))
		{
			ERR_MSG(L"Failed to Creating in CEffectTool_Dlg::OnBnClickedButtonLoad()");
			return;
		}

		//::PathRemoveExtension(m_strPickFileName); //이게 있으면 확장자도 지워짐
		//_tcscpy_s(m_EffectDesc.TextureTag, m_strPickFileName);
		UpdateData(TRUE);
		_tcscpy_s(m_strPickFileName, m_vecEffect[i].ShaderFilePath);
		m_EffectName = m_vecEffect[i].EffectName;
		m_PositionX = XMVectorGetX(m_vecEffect[i].ParticleMat.r[3]);
		m_PositionY = XMVectorGetY(m_vecEffect[i].ParticleMat.r[3]);
		m_PositionZ = XMVectorGetZ(m_vecEffect[i].ParticleMat.r[3]);
		m_Velocity = m_vecEffect[i].fParticleVelocity;
		m_ParticleSizeX = m_vecEffect[i].fParticleSize.x;
		m_ParticleSizeY = m_vecEffect[i].fParticleSize.y;
		m_fRandomPosX = m_vecEffect[i].fParticleRandomPos.x;
		m_fRandomPosY = m_vecEffect[i].fParticleRandomPos.y;
		m_fRandomPosZ = m_vecEffect[i].fParticleRandomPos.z;
		m_fRandomMinusDirX = m_vecEffect[i].fParticleMinusRandomDir.x;
		m_fRandomMinusDirY = m_vecEffect[i].fParticleMinusRandomDir.y;
		m_fRandomMinusDirZ = m_vecEffect[i].fParticleMinusRandomDir.z;
		m_fRandomDirX = m_vecEffect[i].fParticleRandomDir.x;
		m_fRandomDirY = m_vecEffect[i].fParticleRandomDir.y;
		m_fRandomDirZ = m_vecEffect[i].fParticleRandomDir.z;
		m_BaseCount = m_vecEffect[i].iNumInstance;
		m_LiftTime = m_vecEffect[i].fMaxLifeTime;
		m_Age = m_vecEffect[i].fCurTime;
		m_bCheck = m_vecEffect[i].bUsingGravity;

		m_EffectPlaySpeed = m_vecEffect[i].fEffectPlaySpeed;
		m_frame = m_vecEffect[i].fFrame;
		m_ImagecountX = m_vecEffect[i].iImageCountX;
		m_ImagecountY = m_vecEffect[i].iImageCountY;
		m_RenderPassNum = m_vecEffect[i].iRenderPassNum;
		m_IDTag = m_vecEffect[i].IDTag;
		
		if (0 == m_vecEffect[i].iAxis)
		{
			m_AxisXBtn.SetCheck(true);
		}
		else if (1 == m_vecEffect[i].iAxis)
		{
			m_AxisYBtn.SetCheck(true);
		}
		else if (2 == m_vecEffect[i].iAxis)
		{
			m_AxisZBtn.SetCheck(true);
		}
		else if (3 == m_vecEffect[i].iAxis)
		{
			m_AxisAllBtn.SetCheck(true);
		}

		//Age아직안함
		ShaderFolderPath = m_vecEffect[i].ShaderFilePath;
		ShaderFileName = m_vecEffect[i].ShaderFileName;
		UpdateData(FALSE);

	}
}


void CEffectTool_Dlg::OnBnClickedButtonNameTag()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (0 != m_EffectName.GetLength())
	{
		_tcscpy_s(m_EffectDesc.EffectName, m_EffectName);
		int a = 0;
	}
}
