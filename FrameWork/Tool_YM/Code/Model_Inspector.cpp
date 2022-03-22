// ../Code/Model_Inspector.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Model_Inspector.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Menu_Form.h"
#include "Inspector_Form.h"
#include "GameInstance.h"
#include "Observer.h"
#include "Cell.h"
#include "NavSphere.h"
#include "Navigation.h"
#include "MeshCollider.h"
#include "Trigger.h"

// CModel_Inspector 대화 상자

IMPLEMENT_DYNAMIC(CModel_Inspector, CDialogEx)
 
CModel_Inspector::CModel_Inspector(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CModel_Inspector, pParent)
{

}

CModel_Inspector::~CModel_Inspector()
{
}


BOOL CModel_Inspector::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Ready_Level_Combo();

	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pInspec_Form = dynamic_cast<CInspector_Form*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 2));

	m_pObserver = GET_INSTANCE(CObserver);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HRESULT CModel_Inspector::Get_ModelInfo(const FILEINFO& _FileInfo)
{
	if(L"" == _FileInfo.cstrFileName)
		return E_FAIL;

	m_FileInfo = _FileInfo;

	m_StaticTxt_FileName.SetWindowTextW(m_FileInfo.cstrFileName.c_str());
	m_EditText_Model_Tag.SetWindowTextW(m_FileInfo.cstrFileName.c_str());

	return S_OK;
}

_int CModel_Inspector::Update_Model_Inspector(_double _dTimeDelta)
{
	if (false == m_pObserver->m_bPick)
	{
		UpdateData(TRUE);
		m_ModelPosX.Format(_T("%.3f"), m_pObserver->m_fPickPos.x);
		m_ModelPosY.Format(_T("%.3f"), m_pObserver->m_fPickPos.y);
		m_ModelPosZ.Format(_T("%.3f"), m_pObserver->m_fPickPos.z);
		UpdateData(FALSE);
	}
	else if(true == m_pObserver->m_bPick)
	{
		UpdateData(TRUE);
		m_ModelPosX.Format(_T("%.3f"), m_pObserver->m_fModelPos.x);
		m_ModelPosY.Format(_T("%.3f"), m_pObserver->m_fModelPos.y);
		m_ModelPosZ.Format(_T("%.3f"), m_pObserver->m_fModelPos.z);

		m_ModelScaleX.Format(_T("%.3f"), m_pObserver->m_fModelScale.x);
		m_ModelScaleY.Format(_T("%.3f"), m_pObserver->m_fModelScale.y);
		m_ModelScaleZ.Format(_T("%.3f"), m_pObserver->m_fModelScale.z);

		UpdateData(FALSE);
	}
	return _int();
}

void CModel_Inspector::Ready_Level_Combo(void)
{
	m_Combo_Level.AddString(_T("Level_Static"));
	m_Combo_Level.AddString(_T("Level_Logo"));
	m_Combo_Level.AddString(_T("Level_Stage_1"));
	m_Combo_Level.AddString(_T("Level_Stage_2"));
	m_Combo_Level.AddString(_T("Level_Stage_Boss"));

	m_Combo_Level.SetCurSel(0);
	m_TriggerCombo.AddString(_T("for Lod"));
	m_TriggerCombo.AddString(_T("for Scene"));
	m_TriggerCombo.AddString(_T("for Light"));
	m_TriggerCombo.AddString(_T("for Monster"));
	m_TriggerCombo.AddString(_T("for Quest"));

	m_TriggerCombo.AddString(_T("땅벌레"));
	m_TriggerCombo.AddString(_T("힐러"));
	m_TriggerCombo.AddString(_T("한손검"));
	m_TriggerCombo.AddString(_T("두손검"));
	m_TriggerCombo.AddString(_T("대지"));
	m_TriggerCombo.AddString(_T("중간보스"));

	m_TriggerCombo.SetCurSel(0);
}

HRESULT CModel_Inspector::Delete_Clone_ModelList(wstring _ModelName)
{
	auto nameFinder = find(m_CloneMode_NameList.begin(), m_CloneMode_NameList.end(), _ModelName);

	if (nameFinder == m_CloneMode_NameList.end())
		return E_FAIL;

	m_CloneMode_NameList.erase(nameFinder);

	return S_OK;
}

void CModel_Inspector::Clear_Clone_ModelList(void)
{
	m_CloneMode_NameList.clear();
}

HRESULT CModel_Inspector::Add_GameObjectToLayer(const MESHDESC& ModelDesc)
{
	MODELDESC MeshDesc;
	
	MeshDesc.strFolder = ModelDesc.FolderName;
	MeshDesc.strFileName = ModelDesc.FileName;
	MeshDesc.strTag = ModelDesc.Tag	;
	MeshDesc.strName = ModelDesc.Name;
	MeshDesc.fInitPos = ModelDesc.fInitPos;
	MeshDesc.WorldMat = ModelDesc.WorldMat;
	MeshDesc.iType = ModelDesc.iType;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TAB_MAP, MeshDesc.strTag, L"Prototype_GameObject_StaticMesh", &MeshDesc)))
		return E_FAIL;

	m_pInspec_Form->m_pMenu_Form->InitHierarchyTree();
	if (FAILED(m_pInspec_Form->m_pMenu_Form->Create_HierarchyTree(MeshDesc)))
		return E_FAIL;

	m_CloneMode_NameList.push_back(MeshDesc.strName);

	return S_OK;
}

void CModel_Inspector::CCW_Sort(_float3* pPoints[])
{
	_int temp = (pPoints[0]->x * pPoints[1]->z) +
		(pPoints[1]->x * pPoints[2]->z) +
		(pPoints[2]->x * pPoints[0]->z);

	temp = temp - (pPoints[0]->z * pPoints[1]->x) -
		(pPoints[1]->z * pPoints[2]->x) -
		(pPoints[2]->z * pPoints[0]->x);

	if (temp > 0)
	{
		_float3 fPointTemp = *pPoints[1];
		*pPoints[1] = *pPoints[2];
		*pPoints[2] = fPointTemp;
		CCW_Sort(pPoints);
	}
	else if (temp < 0)
		return;
}

void CModel_Inspector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_StaticTxt_FileName);
	DDX_Control(pDX, IDC_EDIT2, m_EditTxt_Model_Name);
	DDX_Control(pDX, IDC_EDIT3, m_EditText_Model_Tag);
	DDX_Control(pDX, IDC_COMBO2, m_Combo_Level);
	DDX_Text(pDX, IDC_Model2, m_ModelPosX);
	DDX_Text(pDX, IDC_Model3, m_ModelPosY);
	DDX_Text(pDX, IDC_Model4, m_ModelPosZ);
	DDX_Text(pDX, IDC_Model5, m_ModelScaleX);
	DDX_Text(pDX, IDC_Model6, m_ModelScaleY);
	DDX_Text(pDX, IDC_Model7, m_ModelScaleZ);
	DDX_Control(pDX, IDC_COMBO1, m_TriggerCombo);
}


BEGIN_MESSAGE_MAP(CModel_Inspector, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CModel_Inspector::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_RADIO1, &CModel_Inspector::OnBnClickedModelSetMode)
	ON_BN_CLICKED(IDC_RADIO2, &CModel_Inspector::OnBnClickedNaveSetMode)
	ON_BN_CLICKED(IDC_BUTTON2, &CModel_Inspector::OnBnClickedNavSaveButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CModel_Inspector::OnBnClickedNavLoadButton)
	ON_BN_CLICKED(IDC_RADIO4, &CModel_Inspector::OnBnClickedRotXButton)
	ON_BN_CLICKED(IDC_RADIO5, &CModel_Inspector::OnBnClickedRotYButton)
	ON_BN_CLICKED(IDC_RADIO6, &CModel_Inspector::OnBnClickedZRotButton)
	ON_BN_CLICKED(IDC_RADIO7, &CModel_Inspector::OnBnClickedRotNoneButton)
	ON_BN_CLICKED(IDC_BUTTON4, &CModel_Inspector::OnBnClickedCreateNavMesh)
	ON_BN_CLICKED(IDC_BUTTON5, &CModel_Inspector::OnBnClickedTriggerAdd)
	ON_BN_CLICKED(IDC_BUTTON6, &CModel_Inspector::OnBnClickedSaveTrigger)
	ON_BN_CLICKED(IDC_BUTTON7, &CModel_Inspector::OnBnClickedLodTrigger)
END_MESSAGE_MAP()


// CModel_Inspector 메시지 처리기

void CModel_Inspector::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	RELEASE_INSTANCE(CObserver);

	delete this;
	CDialogEx::PostNcDestroy();
}


void CModel_Inspector::OnBnClickedAddButton()
{
	// TODO: 모델의 사본을 생성합니다.
	CString strSelData;
	CString strSelLevel;
	CString strTag;
	CString strName;
	UpdateData(TRUE);

	GetDlgItemText(IDC_EDIT3, strTag);
	GetDlgItemText(IDC_EDIT2, strName);

	m_ModelDesc.strTag = strTag.operator LPCWSTR();
	m_ModelDesc.strName = strName.operator LPCWSTR();

	if ("AddTag" != strSelData)
	{
		m_ModelDesc.eLevel = (LEVEL_ID)m_Combo_Level.GetCurSel();
		m_ModelDesc.strFolder = m_FileInfo.cstrFolder;
		m_ModelDesc.strFileName = m_FileInfo.cstrFileName;
		m_ModelDesc.fInitPos = m_pObserver->m_fPickPos;
		m_ModelDesc.iType = m_FileInfo.cstrFBX_Type;

		/* ##2. 사본 모델의 Layer Tag = 모델의 이름  */
		/* ##3. 사본 모델의 ProtoType Tag = 모델의 파일명 */

		auto nameFinder = find(m_CloneMode_NameList.begin(), m_CloneMode_NameList.end(), m_ModelDesc.strName);

		if (nameFinder == m_CloneMode_NameList.end())
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TAB_MAP, m_ModelDesc.strTag, L"Prototype_GameObject_StaticMesh", &m_ModelDesc)))
				return;
		}
		if (FAILED(m_pInspec_Form->m_pMenu_Form->Create_HierarchyTree(m_ModelDesc)))
			return;
		else 
			m_CloneMode_NameList.push_back(m_ModelDesc.strName);
	} 
	UpdateData(FALSE);
}


void CModel_Inspector::OnBnClickedModelSetMode()
{
	// TODO: 맵 설치 모드
	m_pObserver->m_eMode = CObserver::MODE_MAP;
}


void CModel_Inspector::OnBnClickedNaveSetMode()
{
	// TODO: 네비 셀 설치 모드
	m_pObserver->m_eMode = CObserver::MODE_NAV;
}


void CModel_Inspector::OnBnClickedNavSaveButton()
{
	// TODO: 네비 매쉬 정보를 저장합니다
	CNavigation* pPlaneNav = dynamic_cast<CNavigation*>(g_pGameInstance->Get_Component(TAB_STATIC, L"Layer_Plane", L"Com_Navigation"));

	if (nullptr == pPlaneNav)
		return;

	m_NavMeshList_Pos.clear();
	m_vecCells.clear();
	m_vecCells = pPlaneNav->Get_vecCells();
	
	for (auto iter : m_vecCells)
	{
		//CCW_Sort(iter->m_pPoint);
		for (int i = 0; i < 3; ++i)
		{
			m_NavMesh.Point[i] = (iter->m_vPoint[i]);
		}
		m_NavMeshList_Pos.push_back(m_NavMesh);
	}

	CFileDialog Dlg(false, L"dat", L"*.dat"); //저장, 디폴트확장자, 디폴트파일이름
	TCHAR szFilePath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\NavMesh\\");
	
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		wstring strFilePath = Dlg.GetPathName();
		g_pGameInstance->SaveFile<NAVMESHDESC>(&m_NavMeshList_Pos, strFilePath);
	}
}

void CModel_Inspector::OnBnClickedNavLoadButton()
{
	// TODO: 네비 매쉬 정보를 불러옵니다
	CFileDialog Dlg(true, L"dat", L"*.dat");
	TCHAR szFilePath[MAX_PATH] = L"";
	HRESULT hr = E_FAIL;

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\NavMesh\\");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	m_vecCells.clear();

	CNavigation* pPlaneNav = dynamic_cast<CNavigation*>(g_pGameInstance->Get_Component(TAB_STATIC, L"Layer_Plane", L"Com_Navigation"));

	if (nullptr == pPlaneNav)
		return;

	m_NavMeshList_Pos.clear();
	pPlaneNav->m_Cells.clear();
	list<CGameObject*>* SphereList = g_pGameInstance->getObjectList(TAB_STATIC, L"Layer_NaveSphere");
	if(nullptr != SphereList)
		SphereList->clear();

	if (IDOK == Dlg.DoModal())
	{
		wstring strFilePath = Dlg.GetPathName();
		g_pGameInstance->LoadFile<NAVMESHDESC>(m_NavMeshList_Pos, strFilePath);
	}

	for (int i = 0; i < m_NavMeshList_Pos.size(); ++i)
	{
		CGameObject* pSphere = nullptr;
		CCell* pCell = CCell::Create(m_pInspec_Form->m_pDevice, m_pInspec_Form->m_pDeviceContext, m_NavMeshList_Pos[i].Point, pPlaneNav->m_Cells.size());
		pPlaneNav->m_Cells.push_back(pCell);
	
		for (int j = 0; j < 3; j++)
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TAB_STATIC, L"Layer_NaveSphere", L"Prototype_GameObject_NavSphere", &m_NavMeshList_Pos[i].Point[j], &pSphere)))
				MessageBox(L"Faild to Create Nav Sphere!", MB_OK);

			pCell->m_pPoint[j] = &(dynamic_cast<CNavSphere*>(pSphere)->m_fPostion);
		}
	}
	pPlaneNav->SetUp_Neighbor();
}

void CModel_Inspector::OnBnClickedCreateNavMesh()
{
	// TODO: 네비 매쉬 파일을 불러와 매쉬를 쿠킹합니다.
	CFileDialog Dlg(true, L"dat", L"*.dat");
	TCHAR szFilePath[MAX_PATH] = L"";
	HRESULT hr = E_FAIL;

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\NavMesh\\");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		wstring strFilePath = Dlg.GetPathName();
		g_pGameInstance->LoadFile<NAVMESHDESC>(m_NavMeshList_Pos, strFilePath);
	}

	CMeshCollider::MESHDESC NavMeshDesc;
	CPhysicsXSystem::COLDESC	ColDesc;

	ColDesc.eType = CPhysicsXSystem::ERigidType::Static;
	ColDesc.fPos = { 0.0f, 0.0f, 0.0f };//Collider Local
	ColDesc.bGravity = FALSE;
	ColDesc.bKinematic = FALSE;

	for (int i = 0; i < m_NavMeshList_Pos.size(); ++i)
		NavMeshDesc.vecPoints.push_back(m_NavMeshList_Pos[i].Point);

	XMStoreFloat4x4(&NavMeshDesc.matTransform, XMMatrixIdentity());
	NavMeshDesc.pParent = nullptr;
	NavMeshDesc.tColDesc = ColDesc;

	m_pMeshCollider = (CMeshCollider*)g_pGameInstance->Clone_Component(TAB_MAP, L"Prototype_Component_MeshCollider", &NavMeshDesc);
	

	m_pObserver->m_pMeshCollider = m_pMeshCollider;

	if (nullptr == m_pMeshCollider)
	{
		MessageBox(L"네비 매쉬 파일을 불러오는 도중 쿠킹에 실패했습니다.", MB_OK);
		return;
	}
}

void CModel_Inspector::OnBnClickedRotXButton()
{
	// TODO: X축 스케일 조정 활성화
	m_pObserver->m_eScaleMode = CObserver::SCALE_X;
}


void CModel_Inspector::OnBnClickedRotYButton()
{
	// TODO: Y축 스케일 조정 활성화
	m_pObserver->m_eScaleMode = CObserver::SCALE_Y;
}


void CModel_Inspector::OnBnClickedZRotButton()
{
	// TODO: Z축 스케일 조정 활성화
	m_pObserver->m_eScaleMode = CObserver::SCALE_Z;
}

void CModel_Inspector::OnBnClickedRotNoneButton()
{
	// TODO: 스케일 조정 기능 비활성화
	m_pObserver->m_eScaleMode = CObserver::SCALE_END;

}


void CModel_Inspector::OnBnClickedTriggerAdd()
{
	// TODO: 트리거를 추가합니다.
	TRIGGER TriggerDesc;

	/* TRIGGERTYPE { TRIGGER_LOD, TRIGGER_SCENE, TRIGGER_LIGHT, TRIGGER_MONSTER, TRIGGER_QUEST, TRIGGER_END } */

	TriggerDesc.eTrigger_Type = (TRIGGERTYPE)m_TriggerCombo.GetCurSel();
	void* temp = &XMVector3TransformCoord(XMLoadFloat3(&m_pObserver->m_fPickPos), m_pObserver->m_pPlane->Get_Transform()->Get_WorldMatrix());
	TriggerDesc.fTrigger_Point = *(_float3*)temp;

	switch (TriggerDesc.eTrigger_Type)
	{
	case TRIGGERTYPE::TRIGGER_LOD:
		TriggerDesc.iIndex = m_iLodIndex++;
		break;
	case TRIGGERTYPE::TRIGGER_LIGHT:
		TriggerDesc.iIndex = m_iLightIndex++;
		break;
	case TRIGGERTYPE::TRIGGER_SCENE:
		TriggerDesc.iIndex = m_iSceneIndex++;
		break;
	case TRIGGERTYPE::TRIGGER_MONSTER:
		TriggerDesc.iIndex = m_iMonsterIndex++;
		break;
	case TRIGGERTYPE::TRIGGER_QUEST:
		TriggerDesc.iIndex = m_iQuestIndex++;
		break;
	}

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TAB_MAP, L"Layer_Trigger", L"Prototype_GameObject_Trigger", &TriggerDesc)))
	{
		MessageBox(L"Failed to Create Trigger!!!");
		return;
	}
}

void CModel_Inspector::OnBnClickedSaveTrigger()
{
	// TODO: 트리거 데이터를 저장합니다.
	list<CGameObject*> TriggerList = *(g_pGameInstance->getObjectList(TAB_MAP, L"Layer_Trigger"));

	m_vecTrigger.clear();

	if (!TriggerList.empty())
	{
		for (auto& pObj : TriggerList)
		{
			CTrigger* pTrigger = (CTrigger*)pObj;
			m_vecTrigger.push_back(pTrigger->m_TriggerDesc);
		}
	}

	CFileDialog Dlg(false, L"dat", L"*.dat"); //저장, 디폴트확장자, 디폴트파일이름
	TCHAR szFilePath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Trigger\\");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		wstring strFilePath = Dlg.GetPathName();
		g_pGameInstance->SaveFile<TRIGGER>(&m_vecTrigger, strFilePath);
	}
}

void CModel_Inspector::OnBnClickedLodTrigger()
{
	// TODO: 트리거 데이터를 불러옵니다.

	CFileDialog Dlg(true, L"dat", L"*.dat");
	TCHAR szFilePath[MAX_PATH] = L"";
	HRESULT hr = E_FAIL;

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data\\Trigger\\");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	m_vecTrigger.clear();

	if (IDOK == Dlg.DoModal())
	{
		wstring strFilePath = Dlg.GetPathName();
		g_pGameInstance->LoadFile<TRIGGER>(m_vecTrigger, strFilePath);
	}
	for (int i = 0; i < m_vecTrigger.size(); ++i)
	{
		TRIGGER TriggerDesc;
		TriggerDesc.eTrigger_Type = m_vecTrigger[i].eTrigger_Type;
		TriggerDesc.fTrigger_Point = m_vecTrigger[i].fTrigger_Point;
		TriggerDesc.iIndex = m_vecTrigger[i].iIndex;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TAB_MAP, L"Layer_Trigger", L"Prototype_GameObject_Trigger", &TriggerDesc)))
		{
			MessageBox(L"Failed to Create Trigger!!!");
			return;
		}
	}
}