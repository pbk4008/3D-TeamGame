﻿// ../Code/Map_Tool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_YM.h"
#include "../Code/Map_Tool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Menu_Form.h"
#include "GameInstance.h"
#include "GameObject.h"

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
	InitHierarchyTree();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMap_Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE1, m_AssetTree);
	DDX_Control(pDX, IDC_TREE2, m_HierarchyTree);
}

BEGIN_MESSAGE_MAP(CMap_Tool, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMap_Tool::OnTvnSelchangedTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CMap_Tool::OnNMDblclkTree1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE2, &CMap_Tool::OnNMRRClickHierTree)
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

void CMap_Tool::InitHierarchyTree(void)
{
	if (L"Clone Models" != m_HierarchyTree.GetItemText(m_HierarchyTree.GetRootItem()))
		m_TreeRoot = m_HierarchyTree.InsertItem(L"Clone Models", 0, 0, TVI_ROOT, TVI_LAST);
}

void CMap_Tool::Clear_Tree(void)
{
	m_vTag.clear();
	m_vModelName.clear();
	m_HierarchyTree.DeleteAllItems();
}

HTREEITEM CMap_Tool::Find_TreeData(HTREEITEM& hItem, CString findItem)
{
	HTREEITEM hFindItem = NULL, hChildItem = NULL, hSiblingItem = NULL;

	if (m_AssetTree.GetItemText(hItem) == findItem)
		hFindItem = hItem;
	else
	{
		if (m_AssetTree.ItemHasChildren(hItem))
		{
			hChildItem = m_AssetTree.GetChildItem(hItem);
			if (NULL == hFindItem && NULL != hChildItem)
				hFindItem = Find_TreeData(hChildItem, findItem);
		}

		hSiblingItem = m_AssetTree.GetNextSiblingItem(hItem);
		if (NULL == hFindItem && NULL != hSiblingItem)
			hFindItem = Find_TreeData(hSiblingItem, findItem);
	}

	return hFindItem;
}

HRESULT CMap_Tool::Delete_Item(_uint _iLevelIndex, const MODELDESC& _ModelDesc)
{
	//if(FAILED(g_pGameInstance->Delete_Clone_Object()))
	if (FAILED(Delete_HierarchyTreeItem(m_ModelDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMap_Tool::Create_HierarchyTree(const MODELDESC& _ModelInfo)
{
	auto TagFinder = find(m_vTag.begin(), m_vTag.end(), _ModelInfo.strTag);
	auto NameFinder = find(m_vModelName.begin(), m_vModelName.end(), _ModelInfo.strName);

	if (TagFinder == m_vTag.end())
	{
		if (NameFinder != m_vModelName.end())
		{
			MessageBox(L"트리에 중복된 이름의 모델이 존재합니다", L"경고", MB_OK);
			return E_FAIL;
		}
		else
		{
			HTREEITEM Parent = NULL;
			m_vTag.push_back(_ModelInfo.strTag);
			Parent = m_HierarchyTree.InsertItem(m_vTag.back().c_str(), 0, 0, m_TreeRoot, TVI_LAST);

			HTREEITEM Child = NULL;
			m_vModelName.push_back(_ModelInfo.strName);
			Child = m_HierarchyTree.InsertItem(m_vModelName.back().c_str(), 0, 0, Parent, TVI_LAST);

			return S_OK;
		}
	}
	else
	{
		HTREEITEM Parent = m_HierarchyTree.GetRootItem();
		Parent = Search_ParentItemInTree(&m_HierarchyTree, _ModelInfo.strTag.c_str(), m_HierarchyTree.GetChildItem(Parent));
		CString strTemp = m_HierarchyTree.GetItemText(Parent);
		if (NULL == Parent)
			return E_FAIL;

		auto NameFinder = find(m_vModelName.begin(), m_vModelName.end(), _ModelInfo.strName);
		if (NameFinder == m_vModelName.end())
		{
			m_vModelName.push_back(_ModelInfo.strName);
			m_HierarchyTree.InsertItem(m_vModelName.back().c_str(), 0, 0, Parent, TVI_LAST);
		}
		else
		{
			MessageBox(L"트리에 중복된 이름의 모델이 존재합니다", L"경고", MB_OK);
			return E_FAIL;
		}

	}

	return S_OK;
}

HRESULT CMap_Tool::Delete_HierarchyTreeItem(const MODELDESC& _ModelDesc)
{
	auto tagFinder = find(m_vTag.begin(), m_vTag.end(), _ModelDesc.strName);

	if (tagFinder == m_vTag.end())
		return E_FAIL;

	CString strFindItem = _ModelDesc.strTag.c_str();

	HTREEITEM treeItem = NULL;
	HTREEITEM parent = m_HierarchyTree.GetRootItem();

	treeItem = Find_TreeData(parent, strFindItem);

	if (NULL == treeItem)
		return E_FAIL;

	_int SelIndx = Find_TreeItem_Indx(treeItem);

	parent = m_HierarchyTree.GetParentItem(treeItem);
	/* Delete Name in Tree */
	strFindItem = m_HierarchyTree.GetItemText(treeItem);
	m_HierarchyTree.DeleteItem(treeItem);
	
	auto nameFinder = find(m_vModelName.begin(), m_vModelName.end(), _ModelDesc.strTag);

	if (nameFinder == m_vModelName.end())
		return E_FAIL;
	else
		m_vModelName.erase(nameFinder);

	if (m_HierarchyTree.GetItemText(m_HierarchyTree.GetRootItem()) != m_HierarchyTree.GetItemText(parent))
	{
		if (!m_HierarchyTree.ItemHasChildren(parent))
		{
			m_HierarchyTree.DeleteItem(parent);
			m_vTag.erase(tagFinder);
		}
	}

	list<CGameObject*>* ObjectList;
	ObjectList = g_pGameInstance->getObjectList(TAB_MAP, _ModelDesc.strName);

	auto& iter = ObjectList->begin();

	std::advance(iter, SelIndx);
	ObjectList->erase(iter);
	
	m_pMenuForm->m_pInspec_Form->Clear_Clone_ModelList(_ModelDesc.strTag);

	return S_OK;
}

_int CMap_Tool::Find_TreeItem_Indx(HTREEITEM hSelectItem)
{
	HTREEITEM tRoot = m_HierarchyTree.GetRootItem();//StaticObject
	HTREEITEM tParent = m_HierarchyTree.GetChildItem(tRoot);//Tree
	HTREEITEM tObj = m_HierarchyTree.GetChildItem(tParent);//Tree0

	_uint iIndex = 0;

	while (true)
	{
		//그리고 다시 검사
		CString tObjString = m_HierarchyTree.GetItemText(tObj);
		_bool bFind = Get_HasChild(m_HierarchyTree, tObj, hSelectItem);//자식에게 선택한 아이템이 있는지 없는지 판단
		if (bFind)//있으면 나가
			return iIndex;
		tObj = m_HierarchyTree.GetNextSiblingItem(tObj);//없으면  옆에 놈 
		if (!tObj)//옆에놈이 비어있는 놈이면
		{
			tParent = m_HierarchyTree.GetNextSiblingItem(tParent);//부모를 옆으로 이동
			if (!tParent)//부모도 없으면 완전 없어
				return -1;
			else//부모 있으면 부모의 자식으로 바꿔
			{
				//근데 부모한테 자식이 있는지 없는지 확인
				if (!m_HierarchyTree.ItemHasChildren(tParent))//부모한테 자식이 없으면
				{
					while (true)
					{
						tParent = m_HierarchyTree.GetNextSiblingItem(tParent);//다음으로 부모로 넘겨
						if (!tParent)//근데 다음 부모로 넘길 부모가 없으면 나가
							return -1;
						else
							break;
					}
				}
				iIndex++;
				tObj = m_HierarchyTree.GetChildItem(tParent);//찾은 부모의 자식을 불러와
			}
		}
		else
			iIndex++;
	}
}

_bool CMap_Tool::Get_HasChild(CTreeCtrl& tTree, HTREEITEM tParent, HTREEITEM tSelect)
{
	if (!tSelect)
		return false;
	if (tParent == tSelect)
		return true;

	HTREEITEM tObj = tTree.GetChildItem(tParent);

	if (!tObj)
		return false;
	else
	{
		while (tTree.GetItemText(tObj) != tTree.GetItemText(tSelect))
		{
			if (Get_HasChild(tTree, tObj, tSelect))
			{
				CString tObjString = (tTree.GetItemText(tObj));
				CString tSelectString = (tTree.GetItemText(tSelect));

				return true;
			}
			else
			{
				tObj = tTree.GetNextSiblingItem(tObj);
				if (!tObj)
					return false;
			}
		}
		return true;
	}

}

static HTREEITEM FoundedTreeItem;

HTREEITEM CMap_Tool::Search_ParentItemInTree(CTreeCtrl* _TreeCtrl, CString _FindStr, HTREEITEM _pParent)
{
	if (nullptr != _pParent)
	{
		if (_TreeCtrl->GetItemText(_pParent) == _FindStr)
		{
			FoundedTreeItem = _pParent;
			return _pParent;
		}
		_pParent = _TreeCtrl->GetNextSiblingItem(_pParent);
		Search_ParentItemInTree(_TreeCtrl, _FindStr, _pParent);
	}
	return FoundedTreeItem;
}

//void CMap_Tool::Clear_CloneName(wstring _deleteItem)
//{
//	auto tagFinder = find(m_vModelName.begin(), m_vModelName.end(), _deleteItem);
//
//	if (tagFinder != m_vModelName.end())
//		m_vModelName.erase(tagFinder);
//}

// CMap_Tool 메시지 처리기

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


void CMap_Tool::OnNMRRClickHierTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Hierarchy Tree 아이템 오른쪽 클릭

	CPoint ptlnTree, ptlnScreen;

	GetCursorPos(&ptlnScreen);
	ptlnTree = ptlnScreen;

	/* Check Out Point in Tree */
	m_HierarchyTree.ScreenToClient(&ptlnTree);
	HTREEITEM hit = m_HierarchyTree.HitTest(ptlnTree);
	m_HierarchyTree.SelectItem(hit);

	if (NULL == hit) return;

	CMenu menu;
	CMenu* pMenu = NULL;
	menu.LoadMenu(IDR_MENU1);

	if (hit)
	{
		pMenu = menu.GetSubMenu(0);
		CString Tag = m_HierarchyTree.GetItemText(m_HierarchyTree.GetParentItem(hit));
		CString Parent = m_HierarchyTree.GetItemText(hit);

		m_ModelDesc.strTag = Parent;
		m_ModelDesc.strName = Tag;
	}
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, ptlnScreen.x, ptlnScreen.y, this);

	*pResult = 0;
}

BOOL CMap_Tool::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Clone Model 삭제 버튼 클릭 
	int wmId, wmEvent;
	wmId = LOWORD(wParam);
	wmEvent = LOWORD(wParam);

	switch (wmId)
	{
	case ID_CloneDeleteMenu:
		if (FAILED(Delete_Item(TAB_MAP, m_ModelDesc)))
			MessageBox(L"Clone Model Delete Fail", MB_OK);
		break;
	}


	return CDialogEx::OnCommand(wParam, lParam);
}


void CMap_Tool::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialogEx::PostNcDestroy();
}