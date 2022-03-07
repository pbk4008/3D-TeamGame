// ObjectTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Form.h"
#include "Layer.h"
#include "ToolMouse.h"
#include "Animation.h"
#include "HierarchyNode.h"


// CObjectTool 대화 상자

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CObjectTool, pParent)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_bCollisionCheck(false)
	, m_fSphereRadius(0.f)
	, m_iColliderSelectIndex(-1)
	, m_iSelectObjectIndex(-1)
	, m_iSelectAnimIndex(-1)
	, m_tSelectItem(nullptr)
{
	ZeroMemory(&m_vSpherePos, sizeof(_float3));
	ZeroMemory(&m_vBoxScale, sizeof(_float3));
	ZeroMemory(&m_vBoxRotate, sizeof(_float3));
	ZeroMemory(&m_vBoxPos, sizeof(_float3));
}

CObjectTool::CObjectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse, CWnd* pParent)
	: CDialog(IDD_CObjectTool, pParent)
	, m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_pMouse(pMouse)
	, m_bCollisionCheck(false)
	, m_fSphereRadius(0.f)
	, m_iColliderSelectIndex(-1)
	, m_iSelectObjectIndex(-1)
	, m_iSelectAnimIndex(-1)
	, m_tSelectItem(nullptr)
{
	ZeroMemory(&m_vSpherePos, sizeof(_float3));
	ZeroMemory(&m_vBoxScale, sizeof(_float3));
	ZeroMemory(&m_vBoxRotate, sizeof(_float3));
	ZeroMemory(&m_vBoxPos, sizeof(_float3));
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pMouse);
}

CObjectTool::~CObjectTool()
{
	Release();
}

void CObjectTool::Release()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pMouse);
}

HRESULT CObjectTool::Deform_Model(OBJDESC tDesc, _uint iType, CToolObject** pObj)
{
	CGameObject::COMINDEX* pIndex = new CGameObject::COMINDEX[(_uint)COMPONENTID::COMPONENT_END];
	_uint iIndexNum = 0;

	pIndex[(_uint)COMPONENTID::RENDERER].iStartIndex = 0;
	pIndex[(_uint)COMPONENTID::TRANSFORM].iStartIndex = 0;
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	pInstance->Init_ComDeForm((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::MODEL, &iIndexNum,&tDesc.tModelDesc);
	pIndex[(_uint)COMPONENTID::MODEL].iStartIndex = iIndexNum;

	pInstance->Init_ComDeForm((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::COLLIDER, &iIndexNum, &tDesc.tColliderDesc);
	pIndex[(_uint)COMPONENTID::COLLIDER].iStartIndex = iIndexNum;

	CToolObject* pToolObj = CToolObject::Create(m_pDevice, m_pDeviceContext, pIndex);
	pToolObj->setType((CToolObject::TYPE)iType);
	*pObj = pToolObj;
	if (tDesc.tModelDesc.eType == CModel::TYPE::TYPE_STATIC)
		pInstance->Init_GameObject((_uint)TOOLOBJID::TOOL_STATIC, pToolObj);
	else if (tDesc.tModelDesc.eType == CModel::TYPE::TYPE_ANIM)
		pInstance->Init_GameObject((_uint)TOOLOBJID::TOOL_DYNAMIC, pToolObj);

	Safe_Delete_Arr(pIndex);
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CObjectTool::Init_Object()
{
	CMainFrame* pMain = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMain->getForm();

	CToolObject* pObj = nullptr;

	_matrix pMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	if (FAILED(Ready_Tree()))
		return E_FAIL;
	if (FAILED(Ready_Rock()))
		return E_FAIL;
	if (FAILED(Ready_Grass()))
		return E_FAIL;

	OBJDESC tObjDesc;
	ZeroMemory(&tObjDesc, sizeof(OBJDESC));
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Character/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Man_Mesh1.fbx");
	tObjDesc.tModelDesc.eType = CModel::TYPE::TYPE_ANIM;
	XMStoreFloat4x4(&tObjDesc.tModelDesc.matPivot, pMatrix);
	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::CHARACTER, &pObj)))
	{
		MSG_BOX(L"Character0 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Character/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Man_Mesh2.fbx");
	tObjDesc.tModelDesc.eType = CModel::TYPE::TYPE_ANIM;
	XMStoreFloat4x4(&tObjDesc.tModelDesc.matPivot, pMatrix);
	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::CHARACTER, &pObj)))
	{
		MSG_BOX(L"Character1 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Pumpkin/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Pumpkin.fbx");
	tObjDesc.tModelDesc.eType = CModel::TYPE::TYPE_ANIM;
	XMStoreFloat4x4(&tObjDesc.tModelDesc.matPivot, pMatrix);
	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::MONSTER, &pObj)))
	{
		MSG_BOX(L"Monster1 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/MushRoom/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "MushRoom.fbx");
	tObjDesc.tModelDesc.eType = CModel::TYPE::TYPE_ANIM;
	XMStoreFloat4x4(&tObjDesc.tModelDesc.matPivot, pMatrix);
	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::MONSTER, &pObj)))
	{
		MSG_BOX(L"Monster1 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	if (FAILED(Add_ObjTree()))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjectTool::Add_ObjTree()
{
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();

	CLayer* pProtoLayer = pForm->getLayer(TOOLLAYERID::TOOL_PROTOTYPE);
	if (!pProtoLayer)
		return E_FAIL;

	vector<CGameObject*>* pObjVector = pProtoLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_PROTOTYPE);

	if (!pObjVector)
		return E_FAIL;

	CString str;
	_uint iIndex = 0;
	for (auto& pObj : *pObjVector)
	{
		str = ClassifyObjType((_uint)static_cast<CToolObject*>(pObj)->getType());
		Insert_TreeNode(m_ObjTree,str);

		CToolObject* pToolObj = static_cast<CToolObject*>(pObj);
		pToolObj->setObjName(str);
		Add_Bone(pToolObj);
	}

	return S_OK;
}

HRESULT CObjectTool::Ready_Root()
{
	CString str = L"Objects";
	HTREEITEM tRoot = m_ObjTree.InsertItem(str, nullptr, nullptr);
	for (_uint i = 0; i < (_uint)CToolObject::TYPE::TYPE_END; i++)
	{
		str = ClassifyObjType(i);
		m_ObjTree.InsertItem(str, tRoot, nullptr);
	}
	return S_OK;
}
HRESULT CObjectTool::Ready_Tree()
{
	CMainFrame* pMain = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMain->getForm();

	CToolObject* pObj = nullptr;

	_matrix pMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//Tree0
	OBJDESC tObjDesc;
	ZeroMemory(&tObjDesc, sizeof(OBJDESC));
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Tree/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Tree0.fbx");
	tObjDesc.tModelDesc.eType = CModel::TYPE::TYPE_STATIC;
	XMStoreFloat4x4(&tObjDesc.tModelDesc.matPivot, pMatrix);

	tObjDesc.tColliderDesc.eType = CCollider::COLLIDER_TYPE::TYPE_AABB;
	tObjDesc.tColliderDesc.vScale = XMFLOAT3(1.f, 1.f, 1.f);
	tObjDesc.tColliderDesc.vRotate = XMFLOAT3(0.f, 0.f, 0.f);
	tObjDesc.tColliderDesc.vPosition = XMFLOAT3(0.f, 0.f, 0.f);

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::TREE, &pObj)))
	{
		MSG_BOX(L"Tree0 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	//Tree1
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Tree/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Tree1.fbx");

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::TREE, &pObj)))
	{
		MSG_BOX(L"Tree1 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	//Tree2
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Tree/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Tree2.fbx");

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::TREE, &pObj)))
	{
		MSG_BOX(L"Tree2 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);


	return S_OK;
}
HRESULT CObjectTool::Ready_Rock()
{
	CMainFrame* pMain = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMain->getForm();

	CToolObject* pObj = nullptr;

	_matrix pMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//RockGroup
	OBJDESC tObjDesc;
	ZeroMemory(&tObjDesc, sizeof(OBJDESC));
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Rock/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Rock Group.fbx");
	tObjDesc.tModelDesc.eType = CModel::TYPE::TYPE_STATIC;
	XMStoreFloat4x4(&tObjDesc.tModelDesc.matPivot, pMatrix);

	tObjDesc.tColliderDesc.eType = CCollider::COLLIDER_TYPE::TYPE_AABB;
	tObjDesc.tColliderDesc.vScale = XMFLOAT3(1.f, 1.f, 1.f);
	tObjDesc.tColliderDesc.vRotate = XMFLOAT3(0.f, 0.f, 0.f);
	tObjDesc.tColliderDesc.vPosition = XMFLOAT3(0.f, 0.f, 0.f);

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::ROCK, &pObj)))
	{
		MSG_BOX(L"Rock Group Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	//Rock0
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Rock/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Rock0.fbx");

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::ROCK, &pObj)))
	{
		MSG_BOX(L"Rock0 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	//Rock1
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Rock/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Rock1.fbx");

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::ROCK, &pObj)))
	{
		MSG_BOX(L"Rock1 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	//Rock2
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Rock/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Rock2.fbx");

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::ROCK, &pObj)))
	{
		MSG_BOX(L"Rock2 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);


	//Rock3
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Rock/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Rock3.fbx");

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::ROCK, &pObj)))
	{
		MSG_BOX(L"Rock3 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	//Rock4
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Rock/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Rock4.fbx");

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::ROCK, &pObj)))
	{
		MSG_BOX(L"Rock4 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	return S_OK;
}
HRESULT CObjectTool::Ready_Grass()
{
	CMainFrame* pMain = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMain->getForm();

	CToolObject* pObj = nullptr;

	_matrix pMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//RockGroup
	OBJDESC tObjDesc;
	ZeroMemory(&tObjDesc, sizeof(OBJDESC));
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Grass/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Grass0.fbx");
	tObjDesc.tModelDesc.eType = CModel::TYPE::TYPE_STATIC;
	XMStoreFloat4x4(&tObjDesc.tModelDesc.matPivot, pMatrix);

	tObjDesc.tColliderDesc.eType = CCollider::COLLIDER_TYPE::TYPE_AABB;
	tObjDesc.tColliderDesc.vScale = XMFLOAT3(1.f, 1.f, 1.f);
	tObjDesc.tColliderDesc.vRotate = XMFLOAT3(0.f, 0.f, 0.f);
	tObjDesc.tColliderDesc.vPosition = XMFLOAT3(0.f, 0.f, 0.f);

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::GRASS, &pObj)))
	{
		MSG_BOX(L"Grass0 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	//Rock0
	strcpy_s(tObjDesc.tModelDesc.szMeshPATH, "../../Client/bin/Resources/Meshes/Grass/");
	strcpy_s(tObjDesc.tModelDesc.szMeshName, "Grass1.fbx");

	if (FAILED(Deform_Model(tObjDesc, (_uint)CToolObject::TYPE::GRASS, &pObj)))
	{
		MSG_BOX(L"Grass1 Deform Fail");
		return E_FAIL;
	}
	pForm->Init_ProtoTypeLayer(pObj);
	Safe_AddRef(pObj);

	return S_OK;
}
HRESULT CObjectTool::Add_Bone(CToolObject* pObj)
{
	CString str = ClassifyObjType((_uint)pObj->getType());
	_uint iCnt = getChildCnt(m_ObjTree, getParent(m_ObjTree, m_ObjTree.GetRootItem(),str));

	wstring tmp = str.GetString();
	tmp += to_wstring(iCnt - 1);

	//Tree0
	str.Format(tmp.c_str());
	
	HTREEITEM ObjTree = getParent(m_ObjTree, m_ObjTree.GetRootItem(),str);
	

	vector<CHierarchyNode*>* vecNodes = pObj->getBones();

	for (auto& pNode : *vecNodes)
	{
		_tchar szBoneName[MAX_PATH] =L"";
		MultiByteToWideChar(CP_ACP, 0, pNode->Get_Name(), (_int)strlen(pNode->Get_Name()), szBoneName, MAX_PATH);

		CString tmpString = m_ObjTree.GetItemText(ObjTree);
		tmpString += L" ";
		tmpString += szBoneName;
		if(pNode->Get_Depth()==0)
			m_ObjTree.InsertItem(tmpString, ObjTree, nullptr);
		else
		{
			_tchar szParentName[MAX_PATH] = L"";
			CHierarchyNode* pParent=pNode->getParent();
			MultiByteToWideChar(CP_ACP, 0, pParent->Get_Name(), (_int)strlen(pParent->Get_Name()), szParentName, MAX_PATH);

			CString tmpParentNodeName = m_ObjTree.GetItemText(ObjTree);
			tmpParentNodeName += L" ";
			tmpParentNodeName += szParentName;
			HTREEITEM pParentNode = getParent(m_ObjTree, ObjTree, tmpParentNodeName);

			m_ObjTree.InsertItem(tmpString, pParentNode, nullptr);
		}
	}

	return S_OK;
}
HRESULT CObjectTool::Update_ColliderList(_int iSelectIndex, CToolObject* pObj)
{
	if (m_iSelectObjectIndex == iSelectIndex)
		return S_OK;
	m_tCollisionList.ResetContent();
	vector<CToolObject::COLLIDER*>* vecCollider = pObj->getVecCollider();

	if (!vecCollider || vecCollider->empty())
		return S_OK;

	for (_uint i = 0; i < vecCollider->size(); i++)
	{
		CString str;
		switch ((*vecCollider)[i]->pCollider->getType())
		{
		case CCollider::COLLIDER_TYPE::TYPE_AABB:
			str = L"AAbb";
			break;
		case CCollider::COLLIDER_TYPE::TYPE_OBB:
			str = L"Obb";
			break;
		case CCollider::COLLIDER_TYPE::TYPE_SPHERE:
			str = L"Sphere";
			break;
		}
		wstring tmpString = str.GetString();
		tmpString += to_wstring(i);

		str.Format(tmpString.c_str());
		m_tCollisionList.AddString(str);
	}

	Change_ColliderValue(pObj);

	return S_OK;
}

HRESULT CObjectTool::Update_ColliderValue(CToolObject* pObj)
{
	_int iIndex = 0;

	iIndex = m_tCollisionList.GetCurSel();
	if (iIndex < 0)
		return S_OK;

	pObj->ResetColliderColor();

	_vector vScale, vRotate, vPos;

	vScale = XMLoadFloat3(&m_vBoxScale);
	vRotate = XMLoadFloat3(&m_vBoxRotate);
	vPos = XMLoadFloat3(&m_vBoxPos);
	vPos=XMVectorSetW(vPos, 1.f);

	pObj->setColliderValue(iIndex, vScale, vRotate, vPos);
	CCollider* pCollider = pObj->getClickCollider(iIndex);
	pCollider->setColor(XMVectorSet(0.f, 1.f, 0.f, 1.f));

	return S_OK;
}

void CObjectTool::Update_AnimationList(_int iIndex,CToolObject* pObj)
{
	if (iIndex == m_iSelectObjectIndex)
		return;
	m_iSelectObjectIndex = iIndex;
	m_tAnimationList.ResetContent();

	if (pObj->getModelType() == CModel::TYPE::TYPE_STATIC)
		return;

	
	vector<CAnimation*>* vecAnimation = pObj->getAnimations();
	CString str;
	for (auto& pAnim : *vecAnimation)
	{
		_tchar tmp[MAX_PATH] = L"";
		pAnim->getName(tmp);
		str.Format(tmp);
		m_tAnimationList.AddString(str);
	}
}

void CObjectTool::Change_ColliderValue(CToolObject* pObj)
{
	_int iIndex = -1;
	iIndex=m_tCollisionList.GetCurSel();

	if (iIndex < 0)
		return;
	if (m_iColliderSelectIndex == iIndex)
		return;

	m_iColliderSelectIndex = iIndex;
	
	_vector vScale, vRotate, vPos;
	pObj->Change_ColliderValue(iIndex, vScale, vRotate, vPos);
	if (pObj->getColliderType(iIndex) == CCollider::COLLIDER_TYPE::TYPE_AABB
		|| pObj->getColliderType(iIndex) == CCollider::COLLIDER_TYPE::TYPE_OBB)
	{
		m_bCollisionCheck = true;
		XMStoreFloat3(&m_vBoxScale, vScale);
		XMStoreFloat3(&m_vBoxRotate, vRotate);
		XMStoreFloat3(&m_vBoxPos, vPos);
	}
	else if (pObj->getColliderType(iIndex) == CCollider::COLLIDER_TYPE::TYPE_SPHERE)
	{
		m_bCollisionCheck = false;
		m_fSphereRadius = XMVectorGetX(vScale);
		XMStoreFloat3(&m_vSpherePos, vPos);
	}
	UpdateData(false);
}

void CObjectTool::Change_Animation(CToolObject* pObj)
{
	_int iIndex = -1;
	iIndex=m_tAnimationList.GetCurSel();
	if (iIndex < 0)
		return;
	if (m_iSelectAnimIndex == iIndex)
		return;
	m_iSelectAnimIndex = iIndex;

	pObj->setAnimationIndex(m_iSelectAnimIndex);
}

_int CObjectTool::getObjSelectIndex(HTREEITEM tSelItem)
{
	HTREEITEM tRoot = m_ObjTree.GetRootItem();//StaticObject
	HTREEITEM tParent = m_ObjTree.GetChildItem(tRoot);//Tree
	HTREEITEM tObj = m_ObjTree.GetChildItem(tParent);//Tree0

	_uint iIndex = 0;

	while (true)
	{
		//그리고 다시 검사
		CString tObjString = m_ObjTree.GetItemText(tObj);
		_bool bFind = getHasChild(m_ObjTree, tObj, tSelItem);//자식에게 선택한 아이템이 있는지 없는지 판단
		if (bFind)//있으면 나가
			return iIndex;
		tObj = m_ObjTree.GetNextSiblingItem(tObj);//없으면  옆에 놈 
		if (!tObj)//옆에놈이 비어있는 놈이면
		{
			tParent = m_ObjTree.GetNextSiblingItem(tParent);//부모를 옆으로 이동
			if (!tParent)//부모도 없으면 완전 없어
				return -1;
			else//부모 있으면 부모의 자식으로 바꿔
			{
				//근데 부모한테 자식이 있는지 없는지 확인
				if(!m_ObjTree.ItemHasChildren(tParent))//부모한테 자식이 없으면
				{
					while (true)
					{
						tParent = m_ObjTree.GetNextSiblingItem(tParent);//다음으로 부모로 넘겨
						if (!tParent)//근데 다음 부모로 넘길 부모가 없으면 나가
							return -1;
						else
							break;
					}
				}
				iIndex++;
				tObj = m_ObjTree.GetChildItem(tParent);//찾은 부모의 자식을 불러와
			}
		}
		else
			iIndex++;
	}
}

HTREEITEM CObjectTool::getNode(HTREEITEM RootItem, HTREEITEM SelItem)
{
	HTREEITEM hItem = m_ObjTree.GetChildItem(RootItem);
	if (RootItem == SelItem)
		return SelItem;

	if (!hItem)
		return NULL;

	HTREEITEM hItemFind, hItemChile, hItemSibling;
	hItemFind = hItemChile = hItemSibling = NULL;

	if (m_ObjTree.GetItemText(hItem) == m_ObjTree.GetItemText(SelItem))
	{
		hItemFind = hItem;
	}
	else
	{
		hItemChile = m_ObjTree.GetChildItem(hItem);

		if (hItemChile)
			hItemFind = getNode(hItem,SelItem);

		hItemSibling = m_ObjTree.GetNextSiblingItem(hItem);

		if (hItemFind == NULL && hItemSibling)
			hItemFind = getNode(hItemSibling, SelItem);
	}

	return hItemFind;
}

_bool CObjectTool::Check_ObjNameWithItem()
{
	if (!m_tSelectItem)
		return false;

	CString SelName = m_ObjTree.GetItemText(m_tSelectItem);

	HTREEITEM tParent = m_ObjTree.GetChildItem(m_ObjTree.GetRootItem());//Tree
	HTREEITEM tObj = m_ObjTree.GetChildItem(tParent);//Tree0
	while(true)
	{
		CString str1 = m_ObjTree.GetItemText(tParent);
		CString str2 = m_ObjTree.GetItemText(tObj);
		if (SelName == m_ObjTree.GetItemText(tObj))
			return true;
		tObj = m_ObjTree.GetNextSiblingItem(tObj);
		while (!tObj)
		{
			tParent = m_ObjTree.GetNextSiblingItem(tParent);
			if (!tParent)
				return false;
			if (m_ObjTree.ItemHasChildren(tParent))
				tObj = m_ObjTree.GetChildItem(tParent);		
		}
	}
	return true;
}

_int CObjectTool::Update_ObjectTool(_float fDeltaTime)
{
	CheckCollisionBtn();
	if(EditNullCheck())
		UpdateData(true);

	m_tSelectItem = m_ObjTree.GetSelectedItem();
	_int iIndex = getObjSelectIndex(m_tSelectItem);
	if (iIndex < 0)
		return 0;
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();

	CLayer* pProtoLayer = pForm->getLayer(TOOLLAYERID::TOOL_PROTOTYPE);
	if (!pProtoLayer)
		return -1;

	vector<CGameObject*>* pObjVector = pProtoLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_PROTOTYPE);

	if (!pObjVector)
		return -1;
	CToolObject* pToolObj = static_cast<CToolObject*>((*pObjVector)[iIndex]);

	pToolObj->Update_GameObject(fDeltaTime);

	if (FAILED(Update_ColliderList(iIndex, pToolObj)))
		return -1;
	Update_AnimationList(iIndex, pToolObj);

	Change_Animation(pToolObj);

	Change_ColliderValue(pToolObj);//보내는거
	
	Update_ColliderValue(pToolObj);//받는거
	return 0;
}

HRESULT CObjectTool::Render_ObjectTool()
{
	return S_OK;
}

void CObjectTool::CheckCollisionBtn()
{
	if (!m_bCollisionCheck)
	{
		m_tSphereBtn.SetCheck(true);
		m_tSphereGroup.ShowWindow(true);
		m_tSphereRadius.ShowWindow(true);
		for (_int i = 0; i < 3; i++)
		{
			m_tSpherePos[i].ShowWindow(true);
			m_tSpherePosEdit[i].ShowWindow(true);
			m_tSpherePosEdit[i].EnableWindow(true);
		}
		m_tSphereRadiusEdit.EnableWindow(true);
		m_tSphereRadiusEdit.ShowWindow(true);
		//////////////////////////////////////////
		m_tBoxBtn.SetCheck(false);
		for (_int i = 0; i < 3; i++)
		{
			m_tBoxGroups[i].ShowWindow(false);
			m_tBoxScale[i].ShowWindow(false);
			m_tBoxRotate[i].ShowWindow(false);
			m_tBoxPos[i].ShowWindow(false);
			m_tBoxScaleEdit[i].EnableWindow(false);
			m_tBoxScaleEdit[i].ShowWindow(false);
			m_tBoxRotateEdit[i].EnableWindow(false);
			m_tBoxRotateEdit[i].ShowWindow(false);
			m_tBoxPosEdit[i].EnableWindow(false);
			m_tBoxPosEdit[i].ShowWindow(false);
		}
	}
	else
	{
		m_tSphereBtn.SetCheck(false);
		m_tSphereGroup.ShowWindow(false);
		m_tSphereRadius.ShowWindow(false);
		for (_int i = 0; i < 3; i++)
		{
			m_tSpherePos[i].ShowWindow(false);
			m_tSpherePosEdit[i].ShowWindow(false);
			m_tSpherePosEdit[i].EnableWindow(false);
		}
		m_tSphereRadiusEdit.EnableWindow(false);
		m_tSphereRadiusEdit.ShowWindow(false);

		////////////////////////////////////
		m_tBoxBtn.SetCheck(true);
		for (_int i = 0; i < 3; i++)
		{
			m_tBoxGroups[i].ShowWindow(true);
			m_tBoxScale[i].ShowWindow(true);
			m_tBoxRotate[i].ShowWindow(true);
			m_tBoxPos[i].ShowWindow(true);
			m_tBoxScaleEdit[i].EnableWindow(true);
			m_tBoxScaleEdit[i].ShowWindow(true);
			m_tBoxRotateEdit[i].EnableWindow(true);
			m_tBoxRotateEdit[i].ShowWindow(true);
			m_tBoxPosEdit[i].EnableWindow(true);
			m_tBoxPosEdit[i].ShowWindow(true);
		}
	}
}

_bool CObjectTool::EditNullCheck()
{
	if (!m_bCollisionCheck)
	{
		for (_int i = 0; i < 3; i++)
		{
			if (!m_tSpherePosEdit[i].GetWindowTextLengthW())
				return false;
			else
			{
				if (!Check_MinusSymbol(m_tSpherePosEdit[i]))
					return false;
			}
		}
		if (m_tSphereRadiusEdit.GetWindowTextLengthW())
		{
			if (!Check_MinusSymbol(m_tSphereRadiusEdit))
				return false;

			return true;
		}
		else
			return false;
	}
	else
	{
		for (_int i = 0; i < 3; i++)
		{
			if (!m_tBoxScaleEdit[i].GetWindowTextLengthW())
				return false;
			else
			{
				if (!Check_MinusSymbol(m_tBoxScaleEdit[i]))
					return false;
			}
			if (!m_tBoxRotateEdit[i].GetWindowTextLengthW())
				return false;
			else
			{
				if (!Check_MinusSymbol(m_tBoxRotateEdit[i]))
					return false;
			}
			if (!m_tBoxPosEdit[i].GetWindowTextLengthW())
				return false;
			else
			{
				if (!Check_MinusSymbol(m_tBoxPosEdit[i]))
					return false;
			}
		}
		return true;
	}
	return true;
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_tSphereBtn);
	DDX_Control(pDX, IDC_RADIO2, m_tBoxBtn);
	DDX_Control(pDX, SphereGroup0, m_tSphereGroup);
	DDX_Control(pDX, SphereGroup1, m_tSpherePos[0]);
	DDX_Control(pDX, ShpereGroup2, m_tSpherePos[1]);
	DDX_Control(pDX, ShpereGroup3, m_tSpherePos[2]);
	DDX_Control(pDX, ShpereGroup4, m_tSphereRadius);
	DDX_Text(pDX, IDC_EDIT2, m_vSpherePos.x);
	DDX_Text(pDX, IDC_EDIT3, m_vSpherePos.y);
	DDX_Text(pDX, IDC_EDIT10, m_vSpherePos.z);
	DDX_Text(pDX, IDC_EDIT14, m_fSphereRadius);
	DDX_Control(pDX, IDC_EDIT2, m_tSpherePosEdit[0]);
	DDX_Control(pDX, IDC_EDIT3, m_tSpherePosEdit[1]);
	DDX_Control(pDX, IDC_EDIT10, m_tSpherePosEdit[2]);
	DDX_Control(pDX, IDC_EDIT14, m_tSphereRadiusEdit);
	DDX_Control(pDX, BoxGroup0, m_tBoxGroups[0]);
	DDX_Control(pDX, BoxGroup10, m_tBoxGroups[1]);
	DDX_Control(pDX, BoxGroup11, m_tBoxGroups[2]);
	DDX_Control(pDX, BoxGroup1, m_tBoxScale[0]);
	DDX_Control(pDX, BoxGroup2, m_tBoxScale[1]);
	DDX_Control(pDX, BoxGroup3, m_tBoxScale[2]);
	DDX_Control(pDX, BoxGroup4, m_tBoxRotate[0]);
	DDX_Control(pDX, BoxGroup5, m_tBoxRotate[1]);
	DDX_Control(pDX, BoxGroup6, m_tBoxRotate[2]);
	DDX_Control(pDX, BoxGroup7, m_tBoxPos[0]);
	DDX_Control(pDX, BoxGroup8, m_tBoxPos[1]);
	DDX_Control(pDX, BoxGroup9, m_tBoxPos[2]);
	DDX_Text(pDX, IDC_EDIT11, m_vBoxScale.x);
	DDX_Text(pDX, IDC_EDIT12, m_vBoxScale.y);
	DDX_Text(pDX, IDC_EDIT15, m_vBoxScale.z);
	DDX_Text(pDX, IDC_EDIT16, m_vBoxRotate.x);
	DDX_Text(pDX, IDC_EDIT17, m_vBoxRotate.y);
	DDX_Text(pDX, IDC_EDIT18, m_vBoxRotate.z);
	DDX_Text(pDX, IDC_EDIT8, m_vBoxPos.x);
	DDX_Text(pDX, IDC_EDIT9, m_vBoxPos.y);
	DDX_Text(pDX, IDC_EDIT13, m_vBoxPos.z);
	DDX_Control(pDX, IDC_EDIT11, m_tBoxScaleEdit[0]);
	DDX_Control(pDX, IDC_EDIT12, m_tBoxScaleEdit[1]);
	DDX_Control(pDX, IDC_EDIT15, m_tBoxScaleEdit[2]);
	DDX_Control(pDX, IDC_EDIT16, m_tBoxRotateEdit[0]);
	DDX_Control(pDX, IDC_EDIT17, m_tBoxRotateEdit[1]);
	DDX_Control(pDX, IDC_EDIT18, m_tBoxRotateEdit[2]);
	DDX_Control(pDX, IDC_EDIT8, m_tBoxPosEdit[0]);
	DDX_Control(pDX, IDC_EDIT9, m_tBoxPosEdit[1]);
	DDX_Control(pDX, IDC_EDIT13, m_tBoxPosEdit[2]);
	DDX_Control(pDX, IDC_TREE1, m_ObjTree);
	DDX_Control(pDX, IDC_LIST1, m_tCollisionList);
	DDX_Control(pDX, IDC_COMBO1, m_tAnimationList);
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CObjectTool::OnSphereBtnClick)
	ON_BN_CLICKED(IDC_RADIO2, &CObjectTool::OnBoxBtnClick)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjectTool::OnColliderInsertBtnClick)
	ON_BN_CLICKED(IDC_BUTTON11, &CObjectTool::OnCollisionDeleteBtnClick)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjectTool::OnColliderSaveBtnClick)
	ON_BN_CLICKED(IDC_BUTTON10, &CObjectTool::OnCollliderLoadBtnClick)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리


BOOL CObjectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (FAILED(Ready_Root()))
		return false;
	if (FAILED(Init_Object()))
		return false;


	m_tSphereBtn.SetCheck(BST_CHECKED);
	m_tBoxBtn.SetCheck(BST_UNCHECKED);
	m_bCollisionCheck = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectTool::OnSphereBtnClick()
{
	m_bCollisionCheck = !m_bCollisionCheck;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CObjectTool::OnBoxBtnClick()
{
	m_bCollisionCheck = !m_bCollisionCheck;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CObjectTool::OnColliderInsertBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();

	CLayer* pProtoLayer = pForm->getLayer(TOOLLAYERID::TOOL_PROTOTYPE);
	if (!pProtoLayer)
		return;

	vector<CGameObject*>* pObjVector = pProtoLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_PROTOTYPE);

	if (!pObjVector || pObjVector->empty())
		return;

	CToolObject* pObj = static_cast<CToolObject*>((*pObjVector)[m_iSelectObjectIndex]);

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	CCollider::COLLIDERDESC tDesc;
	ZeroMemory(&tDesc, sizeof(CCollider::COLLIDERDESC));
	_bool bBoneCheck = true;
	if (!Check_ObjNameWithItem())//true : obj일때 false : Bone일때
	{
		bBoneCheck = false;
		CString str = m_ObjTree.GetItemText(m_tSelectItem);

		str.Replace(pObj->getObjName(), L"");
		str.Replace(L" ", L"");
		CStringA tmp = CStringA(str.GetBuffer());
		pObj->AddBoneCollider(tmp);
	}

	if (m_bCollisionCheck)//Box
	{
		tDesc.eType = CCollider::COLLIDER_TYPE::TYPE_AABB;
		tDesc.vPosition = m_vBoxPos;
		tDesc.vScale = m_vBoxScale;
		tDesc.vRotate = m_vBoxRotate;	
	}
	else//Sphere
	{
		tDesc.eType = CCollider::COLLIDER_TYPE::TYPE_SPHERE;
		tDesc.vPosition = m_vSpherePos;
		tDesc.vScale = _float3(m_fSphereRadius, m_fSphereRadius, m_fSphereRadius);
	}
	_uint iIndexNum = 0;
	CCollider* pCollider = Init_ComDeForm<CCollider>(TOOLSCENEID::TOOL_DEFORM, COMPONENTID::COLLIDER, &iIndexNum, &tDesc);
	if (!pCollider)
		return;

	pObj->Add_Collider(pCollider, bBoneCheck);
	m_tCollisionList.ResetContent();
	vector<CToolObject::COLLIDER*>* vecCollider = pObj->getVecCollider();

	if (!vecCollider || vecCollider->empty())
		return;

	for (_uint i = 0; i < vecCollider->size(); i++)
	{
		CString str;
		switch ((*vecCollider)[i]->pCollider->getType())
		{
		case CCollider::COLLIDER_TYPE::TYPE_AABB:
			str = L"AAbb";
			break;
		case CCollider::COLLIDER_TYPE::TYPE_OBB:
			str = L"Obb";
			break;
		case CCollider::COLLIDER_TYPE::TYPE_SPHERE:
			str = L"Sphere";
			break;
		}
		wstring tmpString = str.GetString();
		tmpString+=to_wstring(i);

		str.Format(tmpString.c_str());
		m_tCollisionList.AddString(str);
	}
	RELEASE_INSTANCE(CGameInstance);
}


void CObjectTool::OnCollisionDeleteBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();

	CLayer* pProtoLayer = pForm->getLayer(TOOLLAYERID::TOOL_PROTOTYPE);
	if (!pProtoLayer)
		return;

	vector<CGameObject*>* pObjVector = pProtoLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_PROTOTYPE);

	if (!pObjVector || pObjVector->empty())
		return;

	CToolObject* pObj = static_cast<CToolObject*>((*pObjVector)[m_iSelectObjectIndex]);
	
	pObj->DeleteCollider(m_iColliderSelectIndex);

	m_tCollisionList.DeleteString(m_iColliderSelectIndex);

	m_iColliderSelectIndex = -1;
	m_tCollisionList.SetCurSel(-1);
}


void CObjectTool::OnColliderSaveBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CFileDialog Dlg(false, L"Collider", L"*.Collider"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.Collider", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();

	CLayer* pProtoLayer = pForm->getLayer(TOOLLAYERID::TOOL_PROTOTYPE);
	if (!pProtoLayer)
		return;

	vector<CGameObject*>* pObjVector = pProtoLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_PROTOTYPE);

	if (!pObjVector || pObjVector->empty())
		return;

	CToolObject* pObj = static_cast<CToolObject*>((*pObjVector)[m_iSelectObjectIndex]);
	//각 파일이 자체가 오브젝트를 나타냄
	//파일에는 Collider타입/콜리더 자체 Matrix/뼈에 붙는 다면 어떤 뼈에 붙는지
	if (FAILED(pObj->Save_Collider(Dlg.GetPathName())))
		return;
}


void CObjectTool::OnCollliderLoadBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(true, L"Collider", L"*.Collider"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.Collider", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	HTREEITEM tObj = getParent(m_ObjTree, m_ObjTree.GetRootItem(), Dlg.GetFileTitle());
	if (!tObj)
		return;

	_uint iIndex = getObjSelectIndex(tObj);

	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();

	CLayer* pProtoLayer = pForm->getLayer(TOOLLAYERID::TOOL_PROTOTYPE);
	if (!pProtoLayer)
		return;

	vector<CGameObject*>* pObjVector = pProtoLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_PROTOTYPE);

	if (!pObjVector || pObjVector->empty())
		return;

	CToolObject* pObj = static_cast<CToolObject*>((*pObjVector)[iIndex]);
	if (FAILED(pObj->Load_Collider(Dlg.GetPathName())))
		return;
}
