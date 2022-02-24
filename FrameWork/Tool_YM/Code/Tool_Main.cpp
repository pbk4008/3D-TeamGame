#include "pch.h"
#include "Tool_Main.h"
#include "Tool_YMView.h"
#include "Tool_Defines.h"

#include "GameInstance.h"
#include "Menu_Form.h"
#include "Inspector_Form.h"


CTool_Main::CTool_Main(void)
	: m_pGameInst(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInst);
}

HRESULT CTool_Main::Tool_Main_Ready(void)
{
	if (nullptr == m_pGameInst)
		return E_FAIL;

	if(FAILED(m_pGameInst->Initialize_Engine(g_hInst, g_hWnd, CMenu_Form::TAB_END, CGraphic_Device::MODE_WIN, g_iWINCX, g_iWINCY, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	if(FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	/*if (FAILED(Set_Start_Tab(CMenu_Form::TAB_MAP)))
		return E_FAIL;*/

	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pToolView = dynamic_cast<CToolYMView*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 1));
	m_pMenu_Form = dynamic_cast<CMenu_Form*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 0));
	m_pInspec_Form = dynamic_cast<CInspector_Form*>(m_pMainFrm->m_tMainSplitter.GetPane(0, 2));

	m_pInspec_Form->Get_Device(m_pDevice, m_pDeviceContext);

	return S_OK;
}

_int CTool_Main::Tool_Main_Update(const _double _dTimeDelta)
{
	return _int();
}

HRESULT CTool_Main::Tool_Main_Render(void)
{
	return E_NOTIMPL;
}

HRESULT CTool_Main::Ready_Component_Prototype(void)
{
	return E_NOTIMPL;
}

HRESULT CTool_Main::Ready_GameObject_Prototype(void)
{
	return E_NOTIMPL;
}

//HRESULT CTool_Main::Set_Start_Tab(CMenu_Form::TAB_ID _eTab)
//{
//	return S_OK;
//}

void CTool_Main::Free()
{
	Safe_Release(m_pGameInst);

	CGameInstance::Release_Engine();
}
