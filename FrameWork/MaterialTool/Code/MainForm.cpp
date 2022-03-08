// MainForm.cpp: 구현 파일
//
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "pch.h"
#include "Tool_YASIC.h"
#include "MainForm.h"
#include "Material_Level.h"
#include "ModelObject.h"
#include "Mouse.h"
// CMainForm

IMPLEMENT_DYNCREATE(CMainForm, CFormView)

CMainForm::CMainForm()
	: CFormView(IDD_CMainForm)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pRenderer(nullptr)
	, m_pSelModel(nullptr)
	, m_bChange(false)
	, m_pMouse(nullptr)
{

}

CMainForm::~CMainForm()
{
	Free();
}

void CMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_FbxTree);
	DDX_Control(pDX, IDC_LIST2, m_TextureBox);
}

BEGIN_MESSAGE_MAP(CMainForm, CFormView)
END_MESSAGE_MAP()


// CMainForm 진단

#ifdef _DEBUG
void CMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainForm 메시지 처리기


void CMainForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pInstance->Initialize_Engine(g_hInst, g_hWnd, 1, CGraphic_Device::MODE_WIN, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDeviceContext)))
		return;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(g_pGameInstance->SetUpBaseComponent(m_pDevice, m_pDeviceContext)))
		return;

	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>(0, L"Renderer");
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(0, L"MaterialMouse", L"Mouse", nullptr, (CGameObject * *)& m_pMouse)))
		return;

	if (FAILED(Start_Level()))
		return;

	if (FAILED(Ready_Tree()))
		return;
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

_int CMainForm::Tick(_double dDeltaTime)
{
	//LevelUpdate 도는 중
	g_pGameInstance->Tick_Engine(dDeltaTime);

	m_pMouse->RayUpdate(L"MainCamera");
	Update_Tree();
	Update_TextureList();
	Focusing();

	return 0;
}

HRESULT CMainForm::Render()
{
	if (FAILED(g_pGameInstance->Clear_BackBuffer_View(XMFLOAT4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;

	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Render_Engine()))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Present()))
		return E_FAIL;


	return S_OK;
}

_int CMainForm::Update_Tree()
{
	m_tFbxPath = L"../FBX/";
	HTREEITEM tSelItem = m_FbxTree.GetSelectedItem();
	m_tFbxName = m_FbxTree.GetItemText(tSelItem);
	HTREEITEM tParent=m_FbxTree.GetParentItem(tSelItem);
	 m_tFbxPath+= m_FbxTree.GetItemText(tParent);
	 m_tFbxPath += "/";
	 if (m_tFbxPath == L"../FBX//" || m_tFbxPath == L"../FBX/FBX/")
		 return 0;
	m_pSelModel = Find_Model(m_tFbxName.operator LPCWSTR());
	if (!m_pSelModel)
	{
		wstring wstrFbxName = m_tFbxName.GetString();
		wstring wstrFbxPath = m_tFbxPath.GetString();

		string strFbxName;
		strFbxName.assign(wstrFbxName.begin(), wstrFbxName.end());
		string strFbxPath;
		strFbxPath.assign(wstrFbxPath.begin(), wstrFbxPath.end());

		//원형 만들고 만든 원형 바로 넣어주기?
		_matrix matPivot = XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));

		if(FAILED(g_pGameInstance->Add_Prototype(0, wstrFbxName
			, CModel::Create(m_pDevice, m_pDeviceContext, strFbxPath,strFbxName
			, L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot,CModel::TYPE::TYPE_STATIC))))
			return E_FAIL;

		CModelObject::MODELOBJDESC tDesc;
		tDesc.wstrFbxFolder = wstrFbxPath;
		tDesc.wstrFbxName = wstrFbxName;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer(0, L"Model", L"Object", &tDesc,(CGameObject**)&m_pSelModel)))
			return E_FAIL;

		m_bChange = false;
	}
	m_pSelModel->setDraw(true);

	return _int();
}

_int CMainForm::Update_TextureList()
{
	if (!m_pSelModel||m_bChange)
		return 0;


	list<wstring> TextureList;
	if(FAILED(m_pSelModel->Check_Texture(TextureList)))
		return 0;

	m_TextureBox.ResetContent();

	for (auto& pTextureTag : TextureList)
		m_TextureBox.AddString(pTextureTag.c_str());

	m_bChange = true;

	return _int();
}

HRESULT CMainForm::Start_Level()
{
	if (FAILED(g_pGameInstance->Open_Level(0, CMaterial_Level::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainForm::Ready_Tree()
{
	CString str = L"FBX";

	HTREEITEM tRoot = m_FbxTree.InsertItem(str, nullptr, nullptr);

	CFileFind fFinder;

	_bool bWorking = fFinder.FindFile(L"..\\FBX\\*.*");

	while (bWorking)
	{
		bWorking = fFinder.FindNextFile();
		if (fFinder.IsDots()) continue;
		if (fFinder.IsDirectory())
		{
			HTREEITEM tParent = m_FbxTree.InsertItem(fFinder.GetFileName(), tRoot);
			CString tmp = L"..\\FBX\\";
			tmp += fFinder.GetFileName();
			tmp += L"\\*.fbx";
			CFileFind fFileFinder;
			_bool bFileCheck = fFileFinder.FindFile(tmp);
			while (bFileCheck)
			{
				bFileCheck = fFileFinder.FindNextFile();
				if (fFileFinder.IsDots()) 
					continue;
				m_FbxTree.InsertItem(fFileFinder.GetFileName(), tParent);
			}
		}
	}
	return S_OK;
}

CModelObject* CMainForm::Find_Model(const wstring& pModelName)
{
	list<CGameObject*>* ObjList=g_pGameInstance->getObjectList(0, L"Model");

	if (!ObjList)
		return nullptr;

	for (auto& pObj : *ObjList)
	{
		CModelObject* pModelObj = static_cast<CModelObject*>(pObj);
		if (pModelObj->Check_Name(pModelName))
			return pModelObj;
	}

	return nullptr;
}

HRESULT CMainForm::Focusing()
{
	if (!m_pSelModel)
		return S_OK;

	if(g_pGameInstance->getkeyDown(DIK_F))
		m_pSelModel->Focusing();

	return S_OK;
}

HRESULT CMainForm::Picking()
{
	if (!m_pMouse || !m_pSelModel)
		return E_FAIL;

	_vector vRayPos = m_pMouse->getRayPos();
	_vector vRayDir = m_pMouse->getRayDir();

	if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
	{
		m_pSelModel->Picking_Face(vRayPos, vRayDir);
	}
	return S_OK;
}

void CMainForm::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pMouse);

	g_pGameInstance->Release_Engine();
}
