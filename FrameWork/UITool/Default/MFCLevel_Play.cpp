#include "framework.h"
#include "pch.h"
#include "MFCLevel_Play.h"
#include "GameInstance.h"
#include "MainFrm.h"
#include "MyFormView.h"

CMFCLevel_Play::CMFCLevel_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice,pDeviceContext)
{
}

HRESULT CMFCLevel_Play::NativeConstruct()
{
	cout << "Level Play" << endl;
	if (FAILED(__super::NativeConstruct()))
	{
		return E_FAIL;
	}
	
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_GAMEPLAY, L"Layer_Camera", L"Prototype_GameObject_Terrain")))
	{
		ERR_MSG(L"Failed to Creating in CMFCLevel_Play::NativeConstruct()");
		return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TOOL_LEVEL::TOOL_LEVEL_GAMEPLAY, L"Layer_Background", L"Prototype_GameObject_Camera_Proj")))
	{
		ERR_MSG(L"Failed to Creating in CMFCLevel_Play::NativeConstruct()");
		return E_FAIL;
	}

	return S_OK;
}

_int CMFCLevel_Play::Tick(_double TimeDelta)
{
	if ( 0 > (__super::Tick(TimeDelta)))
	{
		return -1;
	}

	if (nullptr != g_pGameInstance->getObjectList(1, L"Layer_Effect"))
	{
		int b = g_pGameInstance->getObjectList(1, L"Layer_Effect")->size();
		int a = 0;
	}

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


	return _int(0);
}

HRESULT CMFCLevel_Play::Render()
{
 	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMFCLevel_Play::Open_Level()
{
	return S_OK;
}

CMFCLevel_Play* CMFCLevel_Play::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMFCLevel_Play* pInstance = new CMFCLevel_Play(pDevice,pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevelLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMFCLevel_Play::Free()
{
	__super::Free(); //부모의 Free 함수 호출 
}
