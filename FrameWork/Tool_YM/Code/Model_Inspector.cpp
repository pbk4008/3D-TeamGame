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

	Ready_Tag_Combo();
	Ready_Level_Combo();

	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pInspec_Form = dynamic_cast<CInspector_Form*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 2));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HRESULT CModel_Inspector::Get_ModelInfo(const FILEINFO& _FileInfo)
{
	if(L"" == _FileInfo.cstrFileName)
		return E_FAIL;

	m_FileInfo = _FileInfo;

	m_StaticTxt_FileName.SetWindowTextW(m_FileInfo.cstrFileName.c_str());

	return S_OK;
}

void CModel_Inspector::Ready_Tag_Combo(void)
{
	m_Combo_Tag.AddString(_T("Stage_1"));
	m_Combo_Tag.AddString(_T("Stage_2"));
	m_Combo_Tag.AddString(_T("Stage_3"));
	m_Combo_Tag.AddString(_T("Stage_Boss"));
	m_Combo_Tag.AddString(_T("AddTag"));

	m_Combo_Tag.SetCurSel(0);
}

void CModel_Inspector::Ready_Level_Combo(void)
{
	m_Combo_Level.AddString(_T("Level_Static"));
	m_Combo_Level.AddString(_T("Level_Logo"));
	m_Combo_Level.AddString(_T("Level_Stage_1"));
	m_Combo_Level.AddString(_T("Level_Stage_2"));
	m_Combo_Level.AddString(_T("Level_Stage_Boss"));

	m_Combo_Level.SetCurSel(0);
}

HRESULT CModel_Inspector::Clear_Clone_ModelList(wstring _ModelName)
{
	auto nameFinder = find(m_CloneMode_NameList.begin(), m_CloneMode_NameList.end(), _ModelName);

	if (nameFinder == m_CloneMode_NameList.end())
		return E_FAIL;

	m_CloneMode_NameList.erase(nameFinder);

	return S_OK;
}

void CModel_Inspector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_StaticTxt_FileName);
	DDX_Control(pDX, IDC_EDIT2, m_EditTxt_Model_Name);
	DDX_Control(pDX, IDC_COMBO1, m_Combo_Tag);
	DDX_Control(pDX, IDC_COMBO2, m_Combo_Level);
}


BEGIN_MESSAGE_MAP(CModel_Inspector, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CModel_Inspector::OnBnClickedAddButton)
END_MESSAGE_MAP()


// CModel_Inspector 메시지 처리기

void CModel_Inspector::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	delete this;
	CDialogEx::PostNcDestroy();
}


void CModel_Inspector::OnBnClickedAddButton()
{
	// TODO: 모델의 사본을 생성합니다.
	CString strSelData;
	CString strSelLevel;
	CString strName;
	UpdateData(TRUE);

	GetDlgItemText(IDC_EDIT2, strName);

	m_ModelDesc.strName = strName.operator LPCWSTR();

	if ("AddTag" != strSelData)
	{
		m_Combo_Tag.GetLBText(m_Combo_Tag.GetCurSel(), strSelData);
		m_ModelDesc.strTag = strSelData;
		m_ModelDesc.eLevel = (LEVEL_ID)m_Combo_Level.GetCurSel();
		m_ModelDesc.strFileName = m_FileInfo.cstrFileName;
		/* ##2. 사본 모델의 Layer Tag = 모델의 이름  */
		/* ##3. 사본 모델의 ProtoType Tag = 모델의 파일명 */

		auto nameFinder = find(m_CloneMode_NameList.begin(), m_CloneMode_NameList.end(), m_ModelDesc.strName);

		if (nameFinder == m_CloneMode_NameList.end())
		{
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TAB_MAP, m_ModelDesc.strTag, L"Prototype_GameObject_StaticMesh", &m_ModelDesc)))
				return;

			if (FAILED(m_pInspec_Form->m_pMenu_Form->Create_HierarchyTree(m_ModelDesc)))
				return;

			m_CloneMode_NameList.push_back(m_ModelDesc.strName);
		}
		/*CMesh_Save* pSave = CMesh_Save::Create(m_ModelDesc,- )*/
	} 
	UpdateData(FALSE);


}
