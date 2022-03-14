// ../Code/MenuForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Menu_Form.h"
#include "GameInstance.h"

#include "MainFrm.h"
#include "Inspector_Form.h"
#include "Map_Tool.h"
#include "Cam_Tool.h"

// CMenu_Form
IMPLEMENT_DYNCREATE(CMenu_Form, CFormView)

CMenu_Form::CMenu_Form()
	: CFormView(IDD_CMenu_Form)
{
	
}

void CMenu_Form::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: MenuForm 초기화

	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pInspec_Form = dynamic_cast<CInspector_Form*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 2));

	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	Ready_Tab();
}

CMenu_Form::~CMenu_Form()
{
}

void CMenu_Form::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MenuTab);
}

HRESULT CMenu_Form::Ready_Tab(void)
{
	CString tabOne = _T("Map");
	CString tabTwo = _T("Camera");

	m_MenuTab.SetCurSel(0);

	m_MenuTab.InsertItem(1, tabOne);
	m_MenuTab.InsertItem(2, tabTwo);

	CRect rtTemp;
	m_MenuTab.GetWindowRect(&rtTemp);

	//첫번째 탭
	m_pMap_Tool = new CMap_Tool;
	m_pMap_Tool->Create(IDD_CMap_Tool, &m_MenuTab);
	m_pMap_Tool->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pMap_Tool->ShowWindow(SW_SHOW);

	//두번째 탭
	m_pCam_Tool = new CCam_Tool;
	m_pCam_Tool->Create(IDD_CCam_Tool, &m_MenuTab);
	m_pCam_Tool->MoveWindow(5, 25, rtTemp.Width() - 10, rtTemp.Height() - 30);
	m_pCam_Tool->ShowWindow(SW_HIDE);

	return S_OK;
}

_int CMenu_Form::Update_Menu_Form(const _double& _dTimeDelta)
{
	m_pMap_Tool->Update_MapTool(_dTimeDelta);

	return _int();
}

HRESULT CMenu_Form::Create_Model_Prototype(const FILEINFO& _fileInfo)
{
	wstring strPath = L"../bin/Resources/FBX/";
	wstring strPullPath = strPath + _fileInfo.cstrFolder + L'/';

	string PullPath;
	PullPath.assign(strPullPath.begin(), strPullPath.end());
	string FileName;
	FileName.assign(_fileInfo.cstrFileName.begin(), _fileInfo.cstrFileName.end());
	string FolderName;
	FolderName.assign(_fileInfo.cstrFolder.begin(), _fileInfo.cstrFolder.end());
	
	wstring Static_ShaderFilePath = L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl";
	wstring Anim_ShaderFilePath = L"../../Reference/ShaderFile/Shader_MeshAnim.hlsl";

	_matrix  PivotMatrix;

	if (0 == _fileInfo.cstrFBX_Type)
	{
		PivotMatrix = /*XMMatrixIdentity();*/XMMatrixRotationX(XMConvertToRadians(90.0f)) *  XMMatrixRotationY(XMConvertToRadians(180.0f));

		if (FAILED(g_pGameInstance->Add_Prototype(TAB_MAP, _fileInfo.cstrFileName,
			CModel::Create(m_pDevice, m_pDeviceContext, PullPath.c_str(), FileName.c_str(),
				Static_ShaderFilePath, PivotMatrix, (CModel::TYPE)_fileInfo.cstrFBX_Type))))
			return E_FAIL;
	}
	else
	{
		PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(g_pGameInstance->Add_Prototype(TAB_MAP, _fileInfo.cstrFileName,
			CModel::Create(m_pDevice, m_pDeviceContext, PullPath.c_str(), FileName.c_str(),
				Anim_ShaderFilePath, PivotMatrix, (CModel::TYPE)_fileInfo.cstrFBX_Type))))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMenu_Form::Create_HierarchyTree(const MODELDESC& _ModelInfo)
{
	if (nullptr == m_pMap_Tool)
		return E_FAIL;
	
	return m_pMap_Tool->Create_HierarchyTree(_ModelInfo);
}

void CMenu_Form::InitHierarchyTree(void)
{
	m_pMap_Tool->InitHierarchyTree();
}

BEGIN_MESSAGE_MAP(CMenu_Form, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDCANCEL, &CMenu_Form::OnTcnSelchangeCancel)
END_MESSAGE_MAP()


// CMenu_Form 진단
#ifdef _DEBUG
void CMenu_Form::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMenu_Form::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CMenu_Form 메시지 처리기

void CMenu_Form::OnTcnSelchangeCancel(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Tab 선택

	m_eSelectTab = (TAB_ID)m_MenuTab.GetCurSel();

	switch (m_eSelectTab)
	{
	case TAB_MAP:
		m_pMap_Tool->ShowWindow(SW_SHOW);
		m_pCam_Tool->ShowWindow(SW_HIDE);
		break;
	case TAB_CAM:
		m_pMap_Tool->ShowWindow(SW_HIDE);
		m_pCam_Tool->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}


void CMenu_Form::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	CFormView::PostNcDestroy();
}
