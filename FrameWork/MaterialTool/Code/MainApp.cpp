#include "pch.h"
#include "MainApp.h"

CMainApp::CMainApp()
{
}

HRESULT CMainApp::NativeConstruct()
{
	if (FAILED(g_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, 0, CGraphic_Device::MODE_WIN, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDeviceContext)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Component_Prototype()))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_GameObject_Prototype()))
	{
		return E_FAIL;
	}


	return S_OK;
}

_int CMainApp::Tick(_double _dDeltaTime)
{
	return _int();
}

HRESULT CMainApp::Render()
{
	return S_OK;
}

HRESULT CMainApp::SetUp_StartScene()
{
	HRESULT hr = 0;

	return S_OK;
}

HRESULT CMainApp::Ready_Component_Prototype()
{
	return S_OK;
}

HRESULT CMainApp::Ready_GameObject_Prototype()
{
	return S_OK;
}


CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
