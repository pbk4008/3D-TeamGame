// Form.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "Form.h"

#include "MapTool.h"
#include "ObjectTool.h"
#include "NavigationTool.h"
#include "ActionCamTool.h"
#include "UITool.h"
#include "Layer.h"
#include "ToolCamera.h"
#include "ToolMouse.h"
#include "ToolObject.h"


// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_CForm)
	, m_pTools()
	, m_iIndex(0)
	, m_pGameInstance(CGameInstance::GetInstance())
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pRenderer(nullptr)
	, m_pCamera(nullptr)
	, m_pMouse(nullptr)
{
	Safe_AddRef(m_pGameInstance);
}

CForm::~CForm()
{
	for (_int i = 0; i < (_int)TOOLS::TOOL_END; i++)
		Safe_Delete(m_pTools[i]);

	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
	Safe_Release(m_pCamera);
	Safe_Release(m_pMouse);
	Safe_Release(m_pRenderer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pGameInstance);
}

_int CForm::SelectUpdate(_float fDeltaTime)
{
	switch (m_iIndex)
	{
	case(_int)TOOLS::TOOL_OBJECT:
		return static_cast<CObjectTool*>(m_pTools[m_iIndex])->Update_ObjectTool(fDeltaTime);
	case (_int)TOOLS::TOOL_MAP:
		return static_cast<CMapTool*>(m_pTools[m_iIndex])->Update_MapTool(fDeltaTime);
	case (_int)TOOLS::TOOL_NAVIGATION:
		return static_cast<CNavigationTool*>(m_pTools[m_iIndex])->Update_NavigationTool(fDeltaTime);
	case (_int)TOOLS::TOOL_CAM:
		return static_cast<CActionCamTool*>(m_pTools[m_iIndex])->Update_ActionCamTool(fDeltaTime);
	case (_int)TOOLS::TOOL_UI:
		return static_cast<CUITool*>(m_pTools[m_iIndex])->Update_UITool(fDeltaTime);
	}
	return 0;
}

HRESULT CForm::SelectRender()
{
	switch (m_iIndex)
	{
	case(_int)TOOLS::TOOL_OBJECT:
		return static_cast<CObjectTool*>(m_pTools[m_iIndex])->Render_ObjectTool();
	case (_int)TOOLS::TOOL_MAP:
		return static_cast<CMapTool*>(m_pTools[m_iIndex])->Render_MapTool();
	case (_int)TOOLS::TOOL_NAVIGATION:
		return static_cast<CNavigationTool*>(m_pTools[m_iIndex])->Render_NavigationTool();
	case (_int)TOOLS::TOOL_CAM:
		return static_cast<CActionCamTool*>(m_pTools[m_iIndex])->Render_ActionCamTool();
	case (_int)TOOLS::TOOL_UI:
		return static_cast<CUITool*>(m_pTools[m_iIndex])->Render_UITool();
	}

	return S_OK;
}

_int CForm::Update_Form(_float fDeltaTime)
{
	if (!m_pMouse)
		return -1;
	m_pMouse->Update(fDeltaTime);

	_int iExit = SelectUpdate(fDeltaTime);
	if (!m_pCamera)
		return -1;

	if (m_iIndex != (_uint)TOOLS::TOOL_UI)
		m_pCamera->Update_GameObject(fDeltaTime);
	m_pGameInstance->Update_PipeLine();
	return iExit;
}

HRESULT CForm::Render_Form()
{
	m_pGameInstance->Clear_BackBuffer(XMFLOAT4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil();

	if (FAILED(SelectRender()))
		return E_FAIL;
	m_pRenderer->Render_Group();

	m_pGameInstance->Present();
	return S_OK;
}

HRESULT CForm::Init_ProtoTypeLayer(CGameObject* pObj)
{
	if (!pObj)
		return E_FAIL;
	auto iter=m_mapLayer.find(TOOLLAYERID::TOOL_PROTOTYPE);
	if (iter == m_mapLayer.end())
		return E_FAIL;
	
	return iter->second->Add_GameObject((_uint)TOOLLAYERID::TOOL_PROTOTYPE, pObj);
}

HRESULT CForm::Init_CloneLayer(CToolObject* pObj)
{
	if (!pObj)
		return E_FAIL;

	if (pObj->getModelType() == CModel::TYPE::TYPE_STATIC)
	{
		auto iter = m_mapLayer.find(TOOLLAYERID::TOOL_STATICCLONE);
		if (iter == m_mapLayer.end())
			return E_FAIL;

	return iter->second->Add_GameObject((_uint)TOOLLAYERID::TOOL_STATICCLONE, pObj);
	}
	else if (pObj->getModelType() == CModel::TYPE::TYPE_ANIM)
	{
		auto iter = m_mapLayer.find(TOOLLAYERID::TOOL_DYNAMICCLONE);
		if (iter == m_mapLayer.end())
			return E_FAIL;

		return iter->second->Add_GameObject((_uint)TOOLLAYERID::TOOL_DYNAMICCLONE, pObj);
	}
}

CLayer* CForm::getLayer(TOOLLAYERID eLayerID)
{
	auto iter = m_mapLayer.find(eLayerID);
	if (iter == m_mapLayer.end())
		return nullptr;
	return iter->second;
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tTab);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CForm::OnChangeTab)
END_MESSAGE_MAP()


// CForm 진단

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (FAILED(m_pGameInstance->Init_GraphicDevice(g_ToolHwnd, g_iToolCnX, g_iToolCnY, CGraphicDevice::WINMODE::MODE_WIN,&m_pDevice,&m_pDeviceContext)))
		return;
	if (FAILED(m_pGameInstance->Init_InputDevice(g_ToolHInst,g_ToolHwnd)))
		return;

	CLayer* pLayer = CLayer::Create();
	m_mapLayer.emplace(TOOLLAYERID::TOOL_PROTOTYPE, pLayer);
	pLayer = CLayer::Create();
	m_mapLayer.emplace(TOOLLAYERID::TOOL_STATICCLONE, pLayer);
	pLayer = CLayer::Create();
	m_mapLayer.emplace(TOOLLAYERID::TOOL_DYNAMICCLONE, pLayer);

	m_pGameInstance->Reserve_Component((_uint)TOOLSCENEID::TOOL_END);
	m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext);
	m_pRenderer->setUsingDeferrd(false);
	m_pGameInstance->Init_ComProto(COMPONENTID::RENDERER, m_pRenderer);
	m_pGameInstance->SetUp_ComProto(m_pDevice, m_pDeviceContext);


	CGameObject::COMINDEX* pIndex = new CGameObject::COMINDEX[(_uint)COMPONENTID::COMPONENT_END];
	_uint iStartIndex = 0;

	CCamera::CAMERADESC pDesc;
	ZeroMemory(&pDesc, sizeof(CCamera::CAMERADESC));
	lstrcpy(pDesc.pCameraTag, L"MainCamera");
	pDesc.eType = CCamera::CAMERATYPE::CAMERA_PROJECTION;
	pDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	pDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	pDesc.vAxisY = _float4(0.f, 1.f, 0.f, 1.f);

	pDesc.fFovy = XMConvertToRadians(60.f);
	pDesc.fAspect = (_float)g_iToolCnX / g_iToolCnY;
	pDesc.fFar = 300.f;
	pDesc.fNear = 0.2f;

	m_pGameInstance->Init_ComDeForm((_uint)TOOLSCENEID::TOOL_DEFORM, COMPONENTID::CAMERA, &iStartIndex,&pDesc);
	pIndex[(_uint)COMPONENTID::CAMERA].iStartIndex = iStartIndex;
	m_pGameInstance->Init_GameObject((_uint)TOOLOBJID::TOOL_CAMERA, CToolCamera::Create(m_pDevice, m_pDeviceContext, pIndex));

	m_pCamera=static_cast<CToolCamera*>(m_pGameInstance->Clone_GameObject((_uint)TOOLOBJID::TOOL_CAMERA));

	Safe_Delete_Arr(pIndex);

	m_pMouse = CToolMouse::Create(m_pDevice, m_pDeviceContext);
	if (!m_pMouse)
		return;

	m_tTab.DeleteAllItems();
	m_tTab.InsertItem(0, _T("Object"));
	m_tTab.InsertItem(1, _T("Map"));
	m_tTab.InsertItem(2, _T("Navigation"));
	m_tTab.InsertItem(3, _T("ActionCam"));
	m_tTab.InsertItem(4, _T("UI"));

	m_tTab.SetCurSel(0);
	CRect winRect = {};
	m_tTab.GetWindowRect(&winRect);

	m_pTools[(_int)TOOLS::TOOL_OBJECT] = new CObjectTool(m_pDevice, m_pDeviceContext,m_pMouse);
	m_pTools[(_int)TOOLS::TOOL_OBJECT]->Create(IDD_CObjectTool, &m_tTab);
	m_pTools[(_int)TOOLS::TOOL_OBJECT]->MoveWindow(5, 25, winRect.Width(), winRect.Height());
	m_pTools[(_int)TOOLS::TOOL_OBJECT]->ShowWindow(SW_SHOW);

	m_pTools[(_int)TOOLS::TOOL_MAP] = new CMapTool(m_pDevice, m_pDeviceContext, m_pMouse);
	m_pTools[(_int)TOOLS::TOOL_MAP]->Create(IDD_CMapTool, &m_tTab);
	m_pTools[(_int)TOOLS::TOOL_MAP]->MoveWindow(5, 25, winRect.Width(), winRect.Height());
	m_pTools[(_int)TOOLS::TOOL_MAP]->ShowWindow(SW_HIDE);

	m_pTools[(_int)TOOLS::TOOL_NAVIGATION] = new CNavigationTool(m_pDevice, m_pDeviceContext, m_pMouse);
	m_pTools[(_int)TOOLS::TOOL_NAVIGATION]->Create(IDD_CNavigationTool, &m_tTab);
	m_pTools[(_int)TOOLS::TOOL_NAVIGATION]->MoveWindow(5, 25, winRect.Width(), winRect.Height());
	m_pTools[(_int)TOOLS::TOOL_NAVIGATION]->ShowWindow(SW_HIDE);

	m_pTools[(_int)TOOLS::TOOL_CAM] = new CActionCamTool(m_pDevice, m_pDeviceContext, m_pMouse);
	m_pTools[(_int)TOOLS::TOOL_CAM]->Create(IDD_CActionCamTool,&m_tTab);
	m_pTools[(_int)TOOLS::TOOL_CAM]->MoveWindow(5, 25, winRect.Width(), winRect.Height());
	m_pTools[(_int)TOOLS::TOOL_CAM]->ShowWindow(SW_HIDE);

	m_pTools[(_int)TOOLS::TOOL_UI] = new CUITool(m_pDevice, m_pDeviceContext,m_pMouse);
	m_pTools[(_int)TOOLS::TOOL_UI]->Create(IDD_CUITool, &m_tTab);
	m_pTools[(_int)TOOLS::TOOL_UI]->MoveWindow(5, 25, winRect.Width(), winRect.Height());
	m_pTools[(_int)TOOLS::TOOL_UI]->ShowWindow(SW_HIDE);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CForm::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	m_iIndex = m_tTab.GetCurSel();

	for (int i = 0; i<int(TOOLS::TOOL_END); i++)
		m_pTools[i]->ShowWindow(SW_HIDE);

	m_pTools[m_iIndex]->ShowWindow(SW_SHOW);

	*pResult = 0;
}
