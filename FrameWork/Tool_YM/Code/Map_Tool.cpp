// ../Code/Map_Tool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Map_Tool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Menu_Form.h"

// CMap_Tool 대화 상자
IMPLEMENT_DYNAMIC(CMap_Tool, CDialogEx)

CMap_Tool::CMap_Tool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMap_Tool, pParent)
{
	m_pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pMenuForm = dynamic_cast<CMenu_Form*>(m_pMain->m_tMainSplitter.GetPane(0, 0));
}

CMap_Tool::~CMap_Tool()
{
}

BOOL CMap_Tool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Map Tool Tab 초기화

	InitAssetsTree();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMap_Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE1, m_AssetTree);
}

BEGIN_MESSAGE_MAP(CMap_Tool, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMap_Tool::OnTvnSelchangedTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CMap_Tool::OnNMDblclkTree1)
END_MESSAGE_MAP()


void CMap_Tool::InitAssetsTree(void)
{
	HTREEITEM hItem = m_AssetTree.InsertItem(_T("..\\Bin\\Resources\\FBX\\", hItem));

	CFileFind fFinder;

	BOOL bWorking = fFinder.FindFile(_T("\\*.fbx"));
	
	while (bWorking)
	{
		bWorking = fFinder.FindNextFile();

		if (fFinder.IsDots()) continue;
		if (fFinder.IsDirectory())
			m_AssetTree.InsertItem(fFinder.GetFileName(), hItem);
	}
	m_AssetTree.EnsureVisible(hItem);
}

// CMap_Tool 메시지 처리기

void CMap_Tool::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialogEx::PostNcDestroy();
}

void CMap_Tool::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Asset List 아이템 클릭
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem;
	HTREEITEM hParentItem = hSelected;

	CString	  strExt;
	CString   strPath;

	/* 노드 선택시 펼쳐짐 */
	if (0 == m_AssetTree.GetItemData(hSelected))
	{
		CString pathSelectd;

		while (hParentItem != NULL)
		{
			pathSelectd = _T("\\") + pathSelectd;
			pathSelectd = m_AssetTree.GetItemText(hParentItem) + pathSelectd;
			hParentItem = m_AssetTree.GetParentItem(hParentItem);
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
				m_AssetTree.InsertItem(finder.GetFileName(), hSelected);
		}
	}
	m_AssetTree.SetItemData(hSelected, 1);

	/* 선택한 fbx 파일에 대한 정보를 가져온다 */
	CString strSelectItem = m_AssetTree.GetItemText(hSelected);
	CString strFilter = L".fbx";

	if (-1 != strSelectItem.Find(strFilter))
	{
		m_FileInfo.cstrFileName = strSelectItem;

		HTREEITEM temp = m_AssetTree.GetParentItem(hSelected);
		if (NULL != temp)
		{
			m_FileInfo.cstrFolder = m_AssetTree.GetItemText(temp);
			m_bSelect_FBXFile = true;
		}
	}
	else
		m_bSelect_FBXFile = false;

	*pResult = 0;
}

void CMap_Tool::OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: FBX 파일을 더블클릭 한 경우 원본 모델 생성
	HRESULT hr = E_FAIL;
	if (m_bSelect_FBXFile)
	{
		auto iter = find(m_ProtoTag.begin(), m_ProtoTag.end(), m_FileInfo.cstrFileName);

		if(iter == m_ProtoTag.end())
			hr = m_pMenuForm->Create_Model_Prototype(m_FileInfo);

		if (FAILED(m_pMenuForm->m_pInspec_Form->Get_ModelInfo(m_FileInfo)))
			return;
	}
	if (SUCCEEDED(hr))
		m_ProtoTag.push_back(m_FileInfo.cstrFileName);



	*pResult = 0;
}
