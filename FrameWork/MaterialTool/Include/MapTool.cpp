// MapTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Form.h"
#include "Layer.h"
#include "ToolMouse.h"
#include "ToolTerrain.h"

// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMapTool, pParent)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pTerrain(nullptr)
	, m_pMouse(nullptr)
	, m_tMouseRadius(0.f)
	, m_eTerrainDrawCheck(DRAWCHECK::DRAW_END)
	, m_eObjCheck(OBJCHECK::OBJ_END)
	, m_bSameHeight(false)
	, m_iSelectStaticIndex(0)
	, m_iSelectDynamicIndex(0)
	, m_iMouseSelectIndex(0)
{
	ZeroMemory(&m_tScale,sizeof(_float3));
	ZeroMemory(&m_tRotate,sizeof(_float3));
	ZeroMemory(&m_tPosition,sizeof(_float3));
}

CMapTool::CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, CToolMouse* pMouse, CWnd* pParent)
	: CDialog(IDD_CMapTool, pParent)
	, m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_pMouse(pMouse)
	, m_pTerrain(nullptr)
	, m_eTerrainDrawCheck(DRAWCHECK::DRAW_END)
	, m_eObjCheck(OBJCHECK::OBJ_END)
	, m_bSameHeight(false)
	, m_iSelectStaticIndex(0)
	, m_iSelectDynamicIndex(0)
	, m_iMouseSelectIndex(0)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pMouse);
	ZeroMemory(&m_tScale, sizeof(_float3));
	ZeroMemory(&m_tRotate, sizeof(_float3));
	ZeroMemory(&m_tPosition, sizeof(_float3));
}

CMapTool::~CMapTool()
{
	Release();
}

_int CMapTool::Update_MapTool(_float fDeltaTime)
{
	if (NullEditCheck())
		UpdateData(true);

	_int iIndex = 0;
	iIndex = m_tTerrainTextureList.GetCurSel();
	if (iIndex > -1)
	{
		CString str;
		m_tTerrainTextureList.GetText(iIndex, str);
		if (FAILED(Set_TerrainTexture(str)))
			return E_FAIL;
	}
	m_pTerrain->setUsingNaviTool(false);
	Update_Transform();

	for (auto& pStaticObj : m_vecStaticAddObjects)
		pStaticObj->Update_GameObject(fDeltaTime);

	for (auto& pDynamicObj : m_vecDynamicAddObjects)
		pDynamicObj->Update_GameObject(fDeltaTime);

	Update_Mouse();
	m_pTerrain->Update_Terrain(fDeltaTime);
	m_pTerrain->Check_Draw((_uint)m_eTerrainDrawCheck);

	return 0;
}

HRESULT CMapTool::Render_MapTool()
{
	return S_OK;
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_tScale.x);
	DDX_Text(pDX, IDC_EDIT2, m_tScale.y);
	DDX_Text(pDX, IDC_EDIT3, m_tScale.z);

	DDX_Text(pDX, IDC_EDIT4, m_tRotate.x);
	DDX_Text(pDX, IDC_EDIT5, m_tRotate.y);
	DDX_Text(pDX, IDC_EDIT6, m_tRotate.z);

	DDX_Text(pDX, IDC_EDIT7, m_tPosition.x);
	DDX_Text(pDX, IDC_EDIT8, m_tPosition.y);
	DDX_Text(pDX, IDC_EDIT9, m_tPosition.z);

	DDX_Control(pDX, IDC_EDIT1, m_tScaleEdit[0]);
	DDX_Control(pDX, IDC_EDIT2, m_tScaleEdit[1]);
	DDX_Control(pDX, IDC_EDIT3, m_tScaleEdit[2]);

	DDX_Control(pDX, IDC_EDIT4, m_tRotateEdit[0]);
	DDX_Control(pDX, IDC_EDIT5, m_tRotateEdit[1]);
	DDX_Control(pDX, IDC_EDIT6, m_tRotateEdit[2]);

	DDX_Control(pDX, IDC_EDIT7, m_tPostionEdit[0]);
	DDX_Control(pDX, IDC_EDIT8, m_tPostionEdit[1]);
	DDX_Control(pDX, IDC_EDIT9, m_tPostionEdit[2]);

	DDX_Control(pDX, IDC_TREE3, m_tStaticObject);
	DDX_Control(pDX, IDC_TREE4, m_tDynamicObejct);
	DDX_Control(pDX, IDC_LIST4, m_tTerrainTextureList);
	DDX_Control(pDX, IDC_RADIO2, m_tStaticCheck);
	DDX_Control(pDX, IDC_RADIO3, m_tDynamicCheck);
	DDX_Control(pDX, IDC_LIST2, m_tStaticObjAddList);
	DDX_Control(pDX, IDC_LIST1, m_tDynamicObjAddList);
	DDX_Control(pDX, IDC_PICTURE, m_tTexture);
	DDX_Text(pDX, IDC_EDIT10, m_tMouseRadius);
	DDX_Control(pDX, IDC_EDIT10, m_tMouseRadiusEdit);
	DDX_Control(pDX, IDC_RADIO5, m_tTerrainDrawCheck[0]);
	DDX_Control(pDX, IDC_RADIO6, m_tTerrainDrawCheck[1]);
	DDX_Control(pDX, IDC_CHECK1, m_tSameHeight);
}

HRESULT CMapTool::Ready_MapTool()
{
	//원본 객체 및 Mesh 및 Transform 기타 등등 컴포넌트 생성

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_tScale = { 1.f,1.f,1.f };
	m_iSelectStaticIndex = -1;
	m_iSelectDynamicIndex = -1;
	m_iMouseSelectIndex = -1;

	if (FAILED(ClassifyObjectList()))
		return E_FAIL;

	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();

	CLayer* pStatic = pForm->getLayer(TOOLLAYERID::TOOL_STATICCLONE);
	CLayer* pDynamic = pForm->getLayer(TOOLLAYERID::TOOL_DYNAMICCLONE);

	if(FAILED(Ready_StaticTree(pStatic)))
		return E_FAIL;
	if (FAILED(Ready_DynamicTree(pDynamic)))
		return E_FAIL;
	if (FAILED(Ready_TerrainsTexture()))
		return E_FAIL;

	m_pTerrain = CToolTerrain::Create(m_pDevice, m_pDeviceContext);

	if (!m_pTerrain)
		return E_FAIL;

	switch (m_eTerrainDrawCheck)
	{
	case CMapTool::DRAWCHECK::NO_DRAW:
		m_tTerrainDrawCheck[0].SetCheck(false);
		m_tTerrainDrawCheck[1].SetCheck(true);
		break;
	case CMapTool::DRAWCHECK::DRAW_HEIGHT:
		m_tTerrainDrawCheck[0].SetCheck(true);
		m_tTerrainDrawCheck[2].SetCheck(false);
		break;
	case CMapTool::DRAWCHECK::DRAW_END:
		break;
	}
	
	m_tSameHeight.SetCheck(false);
	m_bSameHeight = false;

	UpdateData(false);

	return S_OK;
}

HRESULT CMapTool::ClassifyObjectList()
{
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();

	CLayer* pProtoLayr = pForm->getLayer(TOOLLAYERID::TOOL_PROTOTYPE);
	if (!pProtoLayr)
		return E_FAIL;

	vector<CGameObject*>* pProtoVector = pProtoLayr->getGameObjects((_uint)TOOLLAYERID::TOOL_PROTOTYPE);
	if (!pProtoVector)
		return E_FAIL;

	for (auto& pObj : *pProtoVector)
	{
		Safe_AddRef(pObj);
		pForm->Init_CloneLayer(static_cast<CToolObject*>(pObj));
	}
	return S_OK;
}

void CMapTool::Release()
{
	for_each(m_vecStaticAddObjects.begin(), m_vecStaticAddObjects.end(), CDeleteObj());
	m_vecStaticAddObjects.clear();
	m_vecStaticAddObjects.shrink_to_fit();
	for_each(m_vecDynamicAddObjects.begin(), m_vecDynamicAddObjects.end(), CDeleteObj());
	m_vecDynamicAddObjects.clear();
	m_vecDynamicAddObjects.shrink_to_fit();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pMouse);
	Safe_Release(m_pTerrain);
}

_bool CMapTool::NullEditCheck()
{
	_bool m_bScale = true;
	_bool m_bRotate = true;
	_bool m_bPos = true;

	for (_int i = 0; i < 3; i++)
	{
		if (!m_tScaleEdit[i].GetWindowTextLengthW())
			return false;
		else
		{
			if (!Check_MinusSymbol(m_tScaleEdit[i]))
				return false;
		}
		if (!m_tRotateEdit[i].GetWindowTextLengthW())
			return false;
		else
		{
			if (!Check_MinusSymbol(m_tRotateEdit[i]))
				return false;
		}
		if (!m_tPostionEdit[i].GetWindowTextLengthW())
			return false;
		else
		{
			if (!Check_MinusSymbol(m_tPostionEdit[i]))
				return false;
		}
	}
	if(!m_tMouseRadiusEdit.GetWindowTextLengthW())
		return false;
	else
	{
		if (!Check_MinusSymbol(m_tMouseRadiusEdit))
			return false;
	}

	return true;
}

HRESULT CMapTool::Ready_StaticTree(CLayer* pLayer)
{
	HTREEITEM tStaticRoot = m_tStaticObject.InsertItem(_T("StaticObject"), NULL, NULL);
	m_tStaticObject.InsertItem(L"Tree", tStaticRoot, nullptr);
	m_tStaticObject.InsertItem(L"Rock", tStaticRoot, nullptr);
	m_tStaticObject.InsertItem(L"Grass", tStaticRoot, nullptr);
	vector<CGameObject*>* pStaticVector = pLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_STATICCLONE);

	if (!pStaticVector || pStaticVector->empty())
		return S_OK;

	CString str;
	for (auto& pObj : *pStaticVector)
	{
		CToolObject* pStatic = static_cast<CToolObject*>(pObj);
		str = ClassifyObjType((_uint)pStatic->getType());
		Insert_TreeNode(m_tStaticObject, str);
	}

	return S_OK;
}

HRESULT CMapTool::Ready_DynamicTree(CLayer* pLayer)
{
	HTREEITEM tDynamicRoot = m_tDynamicObejct.InsertItem(_T("DynamicObject"), NULL, NULL);
	m_tDynamicObejct.InsertItem(L"Character", tDynamicRoot, nullptr);
	m_tDynamicObejct.InsertItem(L"Monster", tDynamicRoot, nullptr);

	vector<CGameObject*>* pDynamicVec = pLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_DYNAMICCLONE);

	if (!pDynamicVec || pDynamicVec->empty())
		return S_OK;

	CString str;
	for (auto& pObj : *pDynamicVec)
	{
		CToolObject* pDynamic = static_cast<CToolObject*>(pObj);
		str = ClassifyObjType((_uint)pDynamic->getType());
		Insert_TreeNode(m_tDynamicObejct, str);
	}

	return S_OK;
}

HRESULT CMapTool::Set_TerrainTexture(CString pStr)
{
	if (pStr.IsEmpty())
		return E_FAIL;

	CString FilePath = L"../../Client/bin/Resources/Texture/Terrain/";
	FilePath += pStr;

	CRect rect;

	m_tTexture.GetWindowRect(rect);
	CDC* dc = nullptr;
	dc = m_tTexture.GetDC();
	CImage image;
	if (FAILED(image.Load(FilePath)))
		return E_FAIL;

	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	ReleaseDC(dc);


	switch (m_eTerrainDrawCheck)
	{
	case CMapTool::DRAWCHECK::DRAW_HEIGHT:
		break;
	case CMapTool::DRAWCHECK::NO_DRAW:
		m_pTerrain->Change_TerrainTexture(pStr);
		break;
	case CMapTool::DRAWCHECK::DRAW_END:
		break;
	default:
		break;

	}

	return S_OK;
}

HRESULT CMapTool::Ready_TerrainsTexture()
{
	CFileFind finder;
	
	CString ForderPath = L"../bin/Resource/Texture/Terrain/*.*";
	_bool bFind = finder.FindFile(ForderPath);
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	while (bFind)
	{
		bFind = finder.FindNextFile();
		CString FilePath = finder.GetFilePath();
		if (finder.IsDots()||finder.IsDirectory())
			continue;
		if (FAILED(pInstance->Add_Texture(m_pDevice, finder.GetFileName(), FilePath)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}
		m_tTerrainTextureList.AddString(finder.GetFileName());
	}
	
	if (FAILED(pInstance->Add_Texture(m_pDevice, L"TerrainFilter", L"../bin/Resource/Texture/Terrain/Filter/Filter.bmp")))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMapTool::Insert_CloneObject(CToolObject* pObj, CString strName, _int iMeshNum)
{
	if (!pObj)
		return E_FAIL;
	CToolObject* pClone = nullptr;
	pClone = static_cast<CToolObject*>(pObj->Clone());
	if(pClone->getModelType() == CModel::TYPE::TYPE_STATIC)
	{
		pClone->setMeshNum((_uint)iMeshNum);
		m_vecStaticAddObjects.emplace_back(pClone);
		m_tStaticObjAddList.AddString(strName);
	}
	else if (pClone->getModelType() == CModel::TYPE::TYPE_ANIM)
	{
		pClone->setMeshNum((_uint)iMeshNum);
		m_vecDynamicAddObjects.emplace_back(pClone);
		m_tDynamicObjAddList.AddString(strName);
	}
	UpdateData(false);
	return S_OK;
}

CToolObject* CMapTool::Load_CloneObject(CToolObject* pObj)
{
	if (!pObj)
		return nullptr;
	CToolObject* pClone = nullptr;
	pClone = static_cast<CToolObject*>(pObj->Clone());
	if (pClone->getModelType() == CModel::TYPE::TYPE_STATIC)
	{
		m_vecStaticAddObjects.emplace_back(pClone);
	}
	else if (pClone->getModelType() == CModel::TYPE::TYPE_ANIM)
	{
		m_vecDynamicAddObjects.emplace_back(pClone);
	}
	UpdateData(false);
	return pClone;
}

void CMapTool::Update_Mouse()
{
	if(m_tTerrainDrawCheck[0].GetCheck())
	{
		m_tTerrainDrawCheck[0].SetCheck(true);
		m_tTerrainDrawCheck[1].SetCheck(false);
		m_eTerrainDrawCheck = DRAWCHECK::DRAW_HEIGHT;
	}
	else if(m_tTerrainDrawCheck[1].GetCheck())
	{
		m_tTerrainDrawCheck[0].SetCheck(false);
		m_tTerrainDrawCheck[1].SetCheck(true);
		m_eTerrainDrawCheck = DRAWCHECK::NO_DRAW;
	}

	if (m_tSameHeight.GetCheck())
		m_bSameHeight = true;
	else
		m_bSameHeight = false;

	_int iIndex = -1;
	CToolTerrain::MOUSEDESC pMouseDesc;
	ZeroMemory(&pMouseDesc, sizeof(CToolTerrain::MOUSEDESC));

	pMouseDesc.fRadius = m_tMouseRadius;
	XMStoreFloat3(&pMouseDesc.vPos,m_pMouse->Terrain_Picking(m_pTerrain->getVertices(), m_pTerrain->getWorldMatrx(),m_pTerrain->getVtxX(), m_pTerrain->getVtxZ(), iIndex));

	m_pTerrain->setMouseDesc(pMouseDesc,iIndex);
	m_pTerrain->setSameHeight(m_bSameHeight);

	if (m_eTerrainDrawCheck == DRAWCHECK::NO_DRAW)
	{
		CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
		if (pInstance->GetMouseState(CInputDev::MOUSESTATE::MB_LBUTTON))
		{
			_int  iIndex = -1;
			if (m_eObjCheck == OBJCHECK::OBJ_STATIC)
			{
				iIndex = m_pMouse->Click_Object(&m_vecStaticAddObjects);
				m_iMouseSelectIndex = iIndex;
				if(iIndex!=9999)
					m_tStaticObjAddList.SetCurSel(iIndex);
			}
			else if (m_eObjCheck == OBJCHECK::OBJ_DYNAMIC)
			{
				iIndex = m_pMouse->Click_Object(&m_vecDynamicAddObjects);
				m_tDynamicObjAddList.SetCurSel(iIndex);
			}
		}
		if (pInstance->GetMouseState(CInputDev::MOUSESTATE::MB_RBUTTON))
		{
			if (m_iSelectStaticIndex < 0 && m_iSelectStaticIndex < 0)
				return;

			m_tPosition = pMouseDesc.vPos;
			UpdateData(false);
		}
		RELEASE_INSTANCE(CGameInstance);
	}
}

void CMapTool::Update_Transform()
{
	_int iIndex = 0;
	_vector vScale, vRotate, vPos;
	if (m_tStaticCheck.GetCheck())
	{
		iIndex = m_tStaticObjAddList.GetCurSel();
		if (iIndex < 0)
			return;

		if (iIndex != m_iSelectStaticIndex)
		{
			m_iSelectStaticIndex = iIndex;
			m_iMouseSelectIndex = -1;

			for (auto& pObj : m_vecStaticAddObjects)
				pObj->ResetColliderColor();

			CCollider* pCollider=m_vecStaticAddObjects[iIndex]->getClickCollider();
			pCollider->setColor(XMVectorSet(0.f, 1.f, 0.f, 0.f));
			m_vecStaticAddObjects[iIndex]->Change_TransformValue(vScale, vRotate, vPos);
			XMStoreFloat3(&m_tScale, vScale);
			XMStoreFloat3(&m_tRotate, vRotate);
			XMStoreFloat3(&m_tPosition, vPos);
			UpdateData(false);
		}

		vScale = XMLoadFloat3(&m_tScale);
		vRotate = XMLoadFloat3(&m_tRotate);
		vPos = XMLoadFloat3(&m_tPosition);
		vPos = XMVectorSetW(vPos, 1.f);

		m_vecStaticAddObjects[iIndex]->setScale(vScale);
		m_vecStaticAddObjects[iIndex]->setRotate(vRotate);
		m_vecStaticAddObjects[iIndex]->setPos(vPos);
	}
	else if (m_tDynamicCheck.GetCheck())
	{
		iIndex = m_tDynamicObjAddList.GetCurSel();
		if (iIndex < 0)
			return;
		if (iIndex != m_iSelectDynamicIndex)
		{
			m_iSelectDynamicIndex = iIndex;

			m_vecDynamicAddObjects[iIndex]->Change_TransformValue(vScale, vRotate, vPos);
			XMStoreFloat3(&m_tScale, vScale);
			XMStoreFloat3(&m_tRotate, vRotate);
			XMStoreFloat3(&m_tPosition, vPos);
			UpdateData(false);
		}
		vScale = XMLoadFloat3(&m_tScale);
		vRotate = XMLoadFloat3(&m_tRotate);
		vPos = XMLoadFloat3(&m_tPosition);
		vPos = XMVectorSetW(vPos, 1.f);

		m_vecDynamicAddObjects[iIndex]->setScale(vScale);
		m_vecDynamicAddObjects[iIndex]->setRotate(vRotate);
		m_vecDynamicAddObjects[iIndex]->setPos(vPos);
	}
}

BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnAddBtnClick)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::OnDeleteBtnClick)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnHeightMapSaveBtnClick)
	ON_BN_CLICKED(IDC_BUTTON12, &CMapTool::OnHeightMapLoadBtnClick)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTool::OnStaticObjectSaveBtnClick)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTool::OnStaticObjectLoadBtnClick)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTool::OnDynamicSaveBtnClick)
	ON_BN_CLICKED(IDC_BUTTON9, &CMapTool::OnDynamicLoadBtnClick)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(FAILED(Ready_MapTool()))
		return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTool::OnAddBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();
	CLayer* pLayer = nullptr;
	_int iIndex = 0;
	if (m_tStaticCheck.GetCheck())
	{
		m_eObjCheck = OBJCHECK::OBJ_STATIC;
		HTREEITEM pSelectItem=m_tStaticObject.GetSelectedItem();
		CString tSelItemName = m_tStaticObject.GetItemText(pSelectItem);
		iIndex = getIndex(m_tStaticObject, pSelectItem);
		if (iIndex>-1)
		{
			pLayer = pForm->getLayer(TOOLLAYERID::TOOL_STATICCLONE);
			vector<CGameObject*>* pStaticObjVector = pLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_STATICCLONE);

			if (!pStaticObjVector)
				return;
			if (FAILED(Insert_CloneObject(static_cast<CToolObject*>((*pStaticObjVector)[iIndex]), tSelItemName, iIndex)))
				return;
			m_tStaticObject.Select(nullptr, TVGN_CARET);
		}
	}
	else if (m_tDynamicCheck.GetCheck())
	{
		if (!m_tStaticObject.GetFocus())
			return;
		m_eObjCheck = OBJCHECK::OBJ_DYNAMIC;
		HTREEITEM pSelectItem = m_tDynamicObejct.GetSelectedItem();
		CString tSelItemName = m_tDynamicObejct.GetItemText(pSelectItem);
		iIndex = getIndex(m_tDynamicObejct, pSelectItem);
		if (iIndex >-1)
		{
			pLayer = pForm->getLayer(TOOLLAYERID::TOOL_DYNAMICCLONE);
			vector<CGameObject*>* pDynamicObject = pLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_DYNAMICCLONE);

			if (!pDynamicObject)
				return;
			if (FAILED(Insert_CloneObject(static_cast<CToolObject*>((*pDynamicObject)[iIndex]), tSelItemName,iIndex)))
				return;
			m_tDynamicObejct.Select(nullptr, TVGN_CARET);
		}
	}
}


void CMapTool::OnDeleteBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_int iIndex = 0;

	if (m_tStaticCheck.GetCheck())
	{
		iIndex = m_tStaticObjAddList.GetCurSel();
		if (iIndex!=-1)
		{
			m_tStaticObjAddList.DeleteString(iIndex);
			Safe_Release(m_vecStaticAddObjects[iIndex]);
			auto iter_begin = m_vecStaticAddObjects.begin();
			m_vecStaticAddObjects.erase(iter_begin + iIndex);
		}
	}
	else if (m_tDynamicCheck.GetCheck())
	{
		iIndex = m_tDynamicObjAddList.GetCurSel();
		if (iIndex != -1)
		{
			m_tDynamicObjAddList.DeleteString(iIndex);
			Safe_Release(m_vecDynamicAddObjects[iIndex]);
			auto iter_begin = m_vecDynamicAddObjects.begin();
			m_vecDynamicAddObjects.erase(iter_begin + iIndex);
		}
	}
}



void CMapTool::OnHeightMapSaveBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTerrain)
		return;

	CFileDialog Dlg(false, L"HeightMap", L"*.Height"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.Height", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	m_pTerrain->Save_HeightMap(Dlg.GetPathName());
}


void CMapTool::OnHeightMapLoadBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(true, L"HeightMap", L"*.Height"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.Height", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	m_pTerrain->Load_HeighMap(Dlg.GetPathName());
}


void CMapTool::OnStaticObjectSaveBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CFileDialog Dlg(false, L"StaticObject", L"*.Static"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.Static", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	vector<STATICDATA> vecStaticData;
	vecStaticData.reserve((_uint)m_vecStaticAddObjects.size());
	for (auto& pObj : m_vecStaticAddObjects)
	{
		STATICDATA tData;
		ZeroMemory(&tData, sizeof(STATICDATA));
		tData.iMeshNum = pObj->getMeshNum();

		_matrix matTransform=pObj->getMatrix();
		
		XMStoreFloat3(&tData.vRight, matTransform.r[0]);
		XMStoreFloat3(&tData.vUp, matTransform.r[1]);
		XMStoreFloat3(&tData.vLook, matTransform.r[2]);
		XMStoreFloat3(&tData.vPos, matTransform.r[3]);

		vecStaticData.emplace_back(tData);
	}
	pInstance->SaveFile<STATICDATA>(&vecStaticData, Dlg.GetPathName());
	RELEASE_INSTANCE(CGameInstance);
}


void CMapTool::OnStaticObjectLoadBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(true, L"StaticObject", L"*.Static"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.Static", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	vector<STATICDATA> vecStaticData;
	pInstance->LoadFile<STATICDATA>(vecStaticData, Dlg.GetPathName());
	RELEASE_INSTANCE(CGameInstance);
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();
	CLayer* pLayer = pForm->getLayer(TOOLLAYERID::TOOL_STATICCLONE);
	vector<CGameObject*>* pStaticObjVector = pLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_STATICCLONE);
	if (!pStaticObjVector)
		return;

	for (auto& pData : vecStaticData)
	{
		CToolObject* pClone = Load_CloneObject(static_cast<CToolObject*>((*pStaticObjVector)[pData.iMeshNum]));

		if (!pClone)
			return;
		
		_matrix matMatrix = XMMatrixIdentity();

		matMatrix.r[0] = XMLoadFloat3(&pData.vRight);
		matMatrix.r[1] = XMLoadFloat3(&pData.vUp);
		matMatrix.r[2] = XMLoadFloat3(&pData.vLook);
		_vector vPos = XMLoadFloat3(&pData.vPos);
		vPos = XMVectorSetW(vPos, 1.f);
		matMatrix.r[3] = vPos;

		pClone->setMatrix(matMatrix);

		CString str = ClassifyObjType((_uint)pClone->getType());
		wstring tmp = str.GetString();
		tmp += to_wstring(pData.iMeshNum);
		str.Format(tmp.c_str());
		m_tStaticObjAddList.AddString(str);
	}
}


void CMapTool::OnDynamicSaveBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CFileDialog Dlg(false, L"DynamicObject", L"*.Dynamic"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.Dynamic", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	vector<DYNAMICDATA> vecDynaimcData;
	vecDynaimcData.reserve((_uint)m_vecDynamicAddObjects.size());
	for (auto& pObj : m_vecDynamicAddObjects)
	{
		DYNAMICDATA tData;
		ZeroMemory(&tData, sizeof(DYNAMICDATA));
		tData.iMeshNum = pObj->getMeshNum();

		_matrix matTransform = pObj->getMatrix();

		XMStoreFloat3(&tData.vPos, matTransform.r[3]);

		vecDynaimcData.emplace_back(tData);
	}
	pInstance->SaveFile<DYNAMICDATA>(&vecDynaimcData, Dlg.GetPathName());
	RELEASE_INSTANCE(CGameInstance);
}


void CMapTool::OnDynamicLoadBtnClick()
{
	CFileDialog Dlg(true, L"DynamicObject", L"*.Dynamic"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.Dynamic", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	vector<DYNAMICDATA> vecDynamicData;
	pInstance->LoadFile<DYNAMICDATA>(vecDynamicData, Dlg.GetPathName());
	RELEASE_INSTANCE(CGameInstance);
	CMainFrame* pMainFrm = (CMainFrame*)GetParentFrame();
	CForm* pForm = pMainFrm->getForm();
	CLayer* pLayer = pForm->getLayer(TOOLLAYERID::TOOL_DYNAMICCLONE);
	vector<CGameObject*>* pDynamicObjVector = pLayer->getGameObjects((_uint)TOOLLAYERID::TOOL_DYNAMICCLONE);
	if (!pDynamicObjVector)
		return;

	for (auto& pData : vecDynamicData)
	{
		CToolObject* pClone = Load_CloneObject(static_cast<CToolObject*>((*pDynamicObjVector)[pData.iMeshNum]));

		if (!pClone)
			return;

		pClone->setPos(XMLoadFloat3(&pData.vPos));
		pClone->setMeshNum(pData.iMeshNum);
		CString str = ClassifyObjType((_uint)pClone->getType());
		wstring tmp = str.GetString();
		tmp += to_wstring(pData.iMeshNum);
		str.Format(tmp.c_str());
		m_tDynamicObjAddList.AddString(str);
	}
}
