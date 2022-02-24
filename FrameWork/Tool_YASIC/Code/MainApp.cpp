#include "pch.h"
#include "MainApp.h"

#include "Level_Loading.h"

CMainApp::CMainApp()
{
}

HRESULT CMainApp::NativeConstruct()
{
	if (nullptr == m_pGameInstance)
	{
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, (_int)ELevel::Max, CGraphic_Device::MODE_WIN, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDeviceContext)))
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


	if (FAILED(SetUp_StartScene(ELevel::AnimationTool)))
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

HRESULT CMainApp::SetUp_StartScene(ELevel _eScene)
{
	if (nullptr == m_pGameInstance)
	{
		return E_FAIL;
	}

	HRESULT hr = 0;

	switch (_eScene)
	{
	case ELevel::AnimationTool:
		hr = m_pGameInstance->Open_Level((_int)ELevel::Loading, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, _eScene));
		break;
	default:
		return E_FAIL;
	}

	if (FAILED(hr))
	{
		return E_FAIL;
	}

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
