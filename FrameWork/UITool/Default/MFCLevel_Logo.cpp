#include "framework.h"
#include "pch.h"
#include "MFCLevel_Logo.h"
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

	m_pMouse->Tick(g_hWnd,TimeDelta);
	//if (GetKeyState(VK_SPACE) < 0)
	//{
	//	cout << "Space" << endl;
	//	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//	if (FAILED(pGameInstance->Open_Level((_uint)TOOL_LEVEL::TOOL_LEVEL_LOADING, CMFCLevel_Loading::Create(m_pDevice, m_pDeviceContext, TOOL_LEVEL::TOOL_LEVEL_GAMEPLAY))))
	//	{
	//		RELEASE_INSTANCE(CGameInstance);
	//		return -1;
	//	}

	//	RELEASE_INSTANCE(CGameInstance);
	//	return 0;
	//}


	if (nullptr != g_pGameInstance->getObjectList(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_UI"))
	{
		m_pMFCUI = (CMFCObject_UI*)m_pMouse->getCheckUI(g_pGameInstance->getObjectList(TOOL_LEVEL::TOOL_LEVEL_LOGO, L"Layer_UI"));

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMyFormView* pForm = dynamic_cast<CMyFormView*>(pMain->m_SplitterWnd.GetPane(0, 0));

		if (nullptr != m_pMFCUI)
		{
			pForm->m_UIDlg.m_pObject = m_pMFCUI;
			int a = 0;
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
	__super::Free(); //부모의 Free 함수 호출 
}
