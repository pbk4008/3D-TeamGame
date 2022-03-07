#pragma once
#ifndef __TOOL_FUNCTION_H__
#define __TOOL_FUNCTION_H__

BEGIN(Tool)
template<typename T>
T* Clone_ComProto(TOOLSCENEID eScene, COMPONENTID eID, _uint iIndex, void* pArg = nullptr)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CComponent* pCom = pInstance->Clone_ComProto((_uint)eScene,eID, iIndex, pArg);

	RELEASE_INSTANCE(CGameInstance);

	if (!pCom)
		return nullptr;
	return static_cast<T*>(pCom);
}
template<typename T>
T* Init_ComDeForm(TOOLSCENEID eScene, COMPONENTID eID, _uint* iIndex, void* pArg = nullptr)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CComponent* pCom = pInstance->Init_ComDeForm((_uint)eScene,eID, iIndex, pArg);

	RELEASE_INSTANCE(CGameInstance);

	if (!pCom)
		return nullptr;
	return static_cast<T*>(pCom);
}
static CString ClassifyObjType(_uint eType)
{
	//TREE, ROCK, CHARACTER, GRASS
	CString str;
	switch (eType)
	{
	case 0:
		str = L"Tree";
		break;
	case  1:
		str = L"Rock";
		break;
	case  2:
		str = L"Grass";
		break;
	case 3:
		str = L"Character";
		break;
	case 4:
		str = L"Monster";
		break;
	}
	return str;
}
static HTREEITEM getParent(CTreeCtrl& tTree, HTREEITEM hItem, CString str)
{
	HTREEITEM hItemFind, hItemChile, hItemSibling;
	hItemFind = hItemChile = hItemSibling = NULL;

	if (tTree.GetItemText(hItem) == str)
	{
		hItemFind = hItem;
	}
	else
	{
		hItemChile = tTree.GetChildItem(hItem);

		if (hItemChile)
			hItemFind = getParent(tTree, hItemChile, str);

		hItemSibling = tTree.GetNextSiblingItem(hItem);

		if (hItemFind == NULL && hItemSibling)
			hItemFind = getParent(tTree, hItemSibling, str);
	}

	return hItemFind;
}

static _bool getHasChild(CTreeCtrl& tTree, HTREEITEM tParent, HTREEITEM tSelect)
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
			if (getHasChild(tTree, tObj, tSelect))
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

static _uint getIndex(CTreeCtrl& tTree, HTREEITEM tSelect)
{
	if (!tSelect)
		return -1;
	_uint iCnt = 0;
	HTREEITEM tRoot = tTree.GetRootItem();
	HTREEITEM tParent = tTree.GetChildItem(tRoot);
	HTREEITEM tChild = tTree.GetChildItem(tParent);
	_bool bFind = false;
	while (tParent)
	{
		while (tChild)
		{
			if (tChild == tSelect)
				return iCnt;
			tChild = tTree.GetNextItem(tChild, TVGN_NEXT);
			iCnt++;
		}
		tParent = tTree.GetNextItem(tParent, TVGN_NEXT);
		tChild = tTree.GetChildItem(tParent);
	}
	return -1;
}
static _uint getChildCnt(CTreeCtrl& tTree, HTREEITEM tParent)
{
	if (!tParent)
		return -1;
	HTREEITEM tChild = tTree.GetChildItem(tParent);
	_int count = 0;
	while (tChild)
	{
		count++;
		tChild = tTree.GetNextSiblingItem(tChild);
	}
	return count;
}
static HRESULT Insert_TreeNode(CTreeCtrl& tTree, CString& str)
{
	HTREEITEM tParent = getParent(tTree,tTree.GetRootItem(),str);
	_uint iCnt = getChildCnt(tTree,tParent);
	str += L"%d";
	str.Format(str.GetString(), iCnt);
	tTree.InsertItem(str, tParent, nullptr);
	return S_OK;
}
static _bool Check_MinusSymbol(const CEdit& pEdit)
{
	CString str;
	pEdit.GetWindowTextW(str);
	_uint iLen = pEdit.GetWindowTextLengthW();
	if (iLen > 1)
		return true;
	if (str[0] == '-')
		return false;

	return true;
}
END
#endif