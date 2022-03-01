#include "framework.h"
#include "pch.h"
#include "MFCLevel_Logo.h"
#include "MFCLevel_Play.h"
#include "GameInstance.h"
#include "Mouse.h"
#include "MainFrm.h"
#include "MyFormView.h"

CMFCLevel_Logo::CMFCLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice,pDeviceContext)
{
}

HRESULT CMFCLevel_Logo::NativeConstruct()
{
	cout << "Level Logo" << endl;


	if (FAILED(__super::NativeConstruct()))
	{
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_Camera", L"Prototype_GameObject_Camera")))
	{
		ERR_MSG(L"Failed to Creating in CMFCLevel_Logo::NativeConstruct()");
		return E_FAIL;
	}

	m_pMouse = CMouse::Create(m_pDevice, m_pDeviceContext);

	return S_OK;
}

_int CMFCLevel_Logo::Tick(_double TimeDelta)
{
	if ( 0 > (__super::Tick(TimeDelta)))
	{
		return -1;
	}

	if (g_pGameInstance->getkeyDown(DIK_RETURN))
	{
		if (FAILED(g_pGameInstance->Open_Level(1, CMFCLevel_Play::Create(m_pDevice, m_pDeviceContext))))
		{
			return -1;
		}
	}

	if (0 == g_pGameInstance->getCurrentLevel())
	{
		m_pMouse->Tick(g_hWnd,TimeDelta);
		
		if (nullptr != g_pGameInstance->getObjectList(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_UI"))
		{
			m_pMFCUI = (CMFCObject_UI*)m_pMouse->getCheckUI(g_pGameInstance->getObjectList(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_UI"));

			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CMyFormView* pForm = dynamic_cast<CMyFormView*>(pMain->m_SplitterWnd.GetPane(0, 0));

			if (nullptr != m_pMFCUI)
			{
				if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON))
				{
					pForm->m_UIDlg.m_pObject = m_pMFCUI;
					cout << "충돌됨" << endl;
				}
			}
			else
			{
				if (g_pGameInstance->getMouseKeyDown(CInputDev::MOUSESTATE::MB_LBUTTON) && g_pGameInstance->getkeyDown(DIK_LSHIFT))
				{
					pForm->m_UIDlg.m_pObject = nullptr;
					cout << "충돌안됨" << endl;
				}
			}
		}
	}
	
	return _int(0);
}

HRESULT CMFCLevel_Logo::Render()
{
 	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMFCLevel_Logo::Open_Level()
{
	return S_OK;
}

CMFCLevel_Logo* CMFCLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMFCLevel_Logo* pInstance = new CMFCLevel_Logo(pDevice,pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevelLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMFCLevel_Logo::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pMFCUI);
	__super::Free(); //부모의 Free 함수 호출 
}
