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
#include "Material.h"

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
	, m_pSelMaterial(nullptr)
	, m_bTextureChange(false)
	, m_iSelMaterialIndex(-1)
	, m_iPreMaterialIndex(-1)
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
	DDX_Control(pDX, IDC_LIST1, m_MaterialBox);
	DDX_Control(pDX, IDC_Model2, m_tMaterialNameEdit);
	DDX_Control(pDX, IDC_COMBO1, m_ShaderFileComboBox);
	DDX_Control(pDX, IDC_RADIO4, m_tStaticBtn);
	DDX_Control(pDX, IDC_RADIO5, m_tAnimBtn);
	DDX_Control(pDX, IDC_RADIO6, m_tInstance_StaticBtn);
	DDX_Control(pDX, IDC_RADIO7, m_tInstance_AnimBtn);
	DDX_Control(pDX, IDC_Model3, m_tTextureTypeEdit);
	DDX_Control(pDX, IDC_LIST3, m_AddTextureList);
}

BEGIN_MESSAGE_MAP(CMainForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainForm::OnMaterialAddBtnClick)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainForm::OnTextureAddBtnClick)
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
	
	if (FAILED(Start_Level()))
		return;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(0, L"MaterialMouse", L"Mouse", nullptr, (CGameObject * *)& m_pMouse)))
		return;

	if (FAILED(Ready_Tree()))
		return;

	if (FAILED(Ready_ComboBox()))
		return;
	if (FAILED(Ready_CommonTexture()))
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
	Update_Material();
	Update_AddTextureList();
	Focusing();
	Picking();

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
			, L"../../Reference/ShaderFile/Shader_StaticMesh.hlsl", matPivot,CModel::TYPE::TYPE_STATIC,true))))
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

	if (FAILED(Ready_CommonTexture()))
		return -1;
	for (auto& pTextureTag : TextureList)
		m_TextureBox.AddString(pTextureTag.c_str());

	m_bChange = true;

	return _int();
}

_int CMainForm::Update_Material()
{
	m_iSelMaterialIndex = m_MaterialBox.GetCurSel();
	if (m_iSelMaterialIndex == -1 || m_iPreMaterialIndex == m_iSelMaterialIndex)
		return 0;

	CString strMaterialName=L"";
	_uint iSelectIndex = m_MaterialBox.GetCurSel();
	m_MaterialBox.GetText(iSelectIndex, strMaterialName);

	m_pSelMaterial=g_pGameInstance->Get_Material(strMaterialName.GetString());
	if (!m_pSelMaterial)
		return -1;
	m_iPreMaterialIndex = m_iSelMaterialIndex;
	m_bTextureChange = true;
	return _int();
}

_int CMainForm::Update_AddTextureList()
{
	if (!m_pSelMaterial||!m_bTextureChange)
		return 0;

	m_AddTextureList.ResetContent();
	list<wstring> TextureNameList;
	TextureNameList = m_pSelMaterial->Get_TextureName();

	for (auto& pName : TextureNameList)
		m_AddTextureList.AddString(pName.c_str());

	m_bTextureChange = false;
	return 0;
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

	CMaterial* pMaterial=g_pGameInstance->Get_Material();
	wstring wstrMaterialName=pMaterial->Get_Name();

	m_MaterialBox.AddString(wstrMaterialName.c_str());
	return S_OK;
}

HRESULT CMainForm::Ready_ComboBox()
{
	CFileFind fFinder;

	_bool bWorking = fFinder.FindFile(L"..\\..\\Reference\\ShaderFile\\*.hlsl");

	while (bWorking)
	{
		bWorking = fFinder.FindNextFile();
		if (fFinder.IsDots()) continue;
			m_ShaderFileComboBox.AddString(fFinder.GetFileName());
	}
	fFinder.Close();

	return S_OK;
}

HRESULT CMainForm::Ready_CommonTexture()
{
	CFileFind fFinder;

	_bool bWorking = fFinder.FindFile(L"..\\Common_Texture\\*.*");
	

	while (bWorking)
	{
		CString strPath = L"../Common_Texture/";
		bWorking = fFinder.FindNextFile();
		if (fFinder.IsDots()) continue;
		strPath += fFinder.GetFileName();
		if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, fFinder.GetFileName().GetString(), strPath.GetString())))
			return E_FAIL;
		m_TextureBox.AddString(fFinder.GetFileName());
	}
	fFinder.Close();


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

_int CMainForm::Check_RadioBtn()
{
	_int iResult = 0;
	if (!m_tStaticBtn.GetCheck() && !m_tAnimBtn.GetCheck() &&
		!m_tInstance_StaticBtn.GetCheck() && !m_tInstance_AnimBtn.GetCheck())
		iResult = -1;
	else
	{
		if (m_tStaticBtn.GetCheck())
			iResult = 0;
		else if (m_tAnimBtn.GetCheck())
			iResult = 1;
		else if (m_tInstance_StaticBtn.GetCheck())
			iResult = 2;
		else if (m_tInstance_AnimBtn.GetCheck())
			iResult = 3;
	}



	return iResult;
}

void CMainForm::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pMouse);

	g_pGameInstance->Release_Engine();
}


void CMainForm::OnMaterialAddBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_int iResult = Check_RadioBtn();
	if (0!=m_tMaterialNameEdit.GetSel()&&-1!= m_ShaderFileComboBox.GetCurSel() && iResult!=-1)
	{
		CString strMaterialName = L"";
		m_tMaterialNameEdit.GetWindowTextW(strMaterialName);

		m_MaterialBox.AddString(strMaterialName);
		m_tMaterialNameEdit.SetWindowTextW(L"");

		CString strShaderFile = L"../../Reference/ShaderFile/";
		CString strShaderFileName = L"";
		m_ShaderFileComboBox.GetWindowTextW(strShaderFileName);
		strShaderFile += strShaderFileName;
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pDeviceContext, strMaterialName.GetString()
			, strShaderFile.GetString(), (CMaterial::EType)iResult);

		g_pGameInstance->Add_Material(strMaterialName.GetString(), pMaterial);
	}
}


void CMainForm::OnTextureAddBtnClick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_int iSelTextureIndex = m_TextureBox.GetCurSel();
	if (m_pSelMaterial && iSelTextureIndex>=0 && m_tTextureTypeEdit.GetSel() !=0)
	{
		CString strSelTexture = L"";
		m_TextureBox.GetText(iSelTextureIndex, strSelTexture);

		CString strTextureType;
		m_tTextureTypeEdit.GetWindowTextW(strTextureType);
		_uint iTextureType = _ttoi(strTextureType);

		m_tFbxPath;
		CString strSelTexturePath = m_tFbxPath + strSelTexture.GetString();
		if(FAILED(m_pSelMaterial->Set_Texture((TEXTURETYPE)iTextureType, strSelTexture.GetString(), strSelTexturePath.GetString())))
			return;

		m_AddTextureList.InsertString(iTextureType, strSelTexture);
	}
}
