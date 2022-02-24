// ../Code/Map_Tool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Map_Tool.h"
#include "afxdialogex.h"


// CMap_Tool 대화 상자
IMPLEMENT_DYNAMIC(CMap_Tool, CDialogEx)

CMap_Tool::CMap_Tool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMap_Tool, pParent)
{
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

	*pResult = 0;
}
