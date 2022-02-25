#include "pch.h"
#include "Tool_Main.h"
#include "Tool_YMView.h"
#include "Tool_Defines.h"

#include "GameInstance.h"
#include "Menu_Form.h"
#include "Inspector_Form.h"
#include "Tab_Changer.h"

CTool_Main::CTool_Main(void)
{
}

HRESULT CTool_Main::Tool_Main_Ready(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if(FAILED(pGameInstance->Initialize_Engine(g_hInst, g_hWnd, TAB_END, CGraphic_Device::MODE_WIN, g_iWINCX, g_iWINCY, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(g_pGameInstance->SetUpBaseComponent(m_pDevice, m_pDeviceContext)))
		return E_FAIL;

	m_pRenderer = g_pGameInstance->Clone_Component<CRenderer>(TAB_STATIC, L"Renderer");
	if (nullptr == m_pRenderer)
		return E_FAIL;

	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	if(FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Set_Start_Tab(TAB_STATIC)))
		return E_FAIL;

	m_pMainFrm     = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pToolView    = dynamic_cast<CToolYMView*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 1));
	m_pMenu_Form   = dynamic_cast<CMenu_Form*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 0));
	m_pInspec_Form = dynamic_cast<CInspector_Form*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 2));

	m_pInspec_Form->Get_Device(m_pDevice, m_pDeviceContext);

	return S_OK;
}

_int CTool_Main::Tool_Main_Update(const _double _dTimeDelta)
{
	g_pGameInstance->Tick_Engine(_dTimeDelta);

	m_pMenu_Form->Update_Menu_Form(_dTimeDelta);
	m_pInspec_Form->Update_Inspector_Form(_dTimeDelta);

	return _int();
}

HRESULT CTool_Main::Tool_Main_Render(void)
{
	if (FAILED(g_pGameInstance->Clear_BackBuffer_View(XMFLOAT4(0.21f, 0.21f, 0.21f, 1.f))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;
	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Present()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Main::Ready_Component_Prototype(void)
{
	return S_OK;
}

HRESULT CTool_Main::Ready_GameObject_Prototype(void)
{
	return S_OK;
}

HRESULT CTool_Main::Set_Start_Tab(TAB_ID _eTab)
{
	HRESULT hr = 0;

	switch (_eTab)
	{
	case TAB_STATIC: case TAB_MAP:
		hr = g_pGameInstance->Open_Level(TAB_STATIC, CTab_Changer::Create(m_pDevice, m_pDeviceContext, _eTab));
		break;
	default:
		return E_FAIL;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

CTool_Main* CTool_Main::Create(void)
{
	CTool_Main* pInstance = new CTool_Main();

	if (FAILED(pInstance->Tool_Main_Ready()))
	{
		MSGBOX("Tool Main Ready Failed!!!");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTool_Main::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	CGameInstance::Release_Engine();
}
