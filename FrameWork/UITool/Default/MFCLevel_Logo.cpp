#include "framework.h"
#include "pch.h"
#include "MFCLevel_Logo.h"
#include "GameInstance.h"

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

	return S_OK;
}

_int CMFCLevel_Logo::Tick(_double TimeDelta)
{
	if ( 0 > (__super::Tick(TimeDelta)))
	{
		return -1;
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
