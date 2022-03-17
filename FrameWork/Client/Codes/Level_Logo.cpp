#include "pch.h"
#include "Level_Logo.h"
#include "Loading.h"


CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_GameObejct()))
		return E_FAIL;

	return S_OK;
}

_int CLevel_Logo::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (g_pGameInstance->getkeyDown(DIK_RETURN))
	{
		if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext,SCENEID::SCENE_STAGE1))))
			return -1;

		return 0;
	}
	else if (g_pGameInstance->getkeyDown(DIK_0))
	{
		if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_TEST_JS))))
			return -1;

		return 0;
	}
	else if (g_pGameInstance->getkeyDown(DIK_NUMPAD7))
	{
		if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_TEST_YM))))
			return -1;

		return 0;
	}
	else if (g_pGameInstance->getkeyDown(DIK_F9))
	{
		if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_TEST_SB))))
			return -1;

		return 0;
	}

	return _int(0);
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Logo::Ready_GameObejct()
{
	wstring strTag = L"Texture_Title";
	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_LOGO, L"Layer_Background", L"Proto_GameObject_BackGround", &strTag);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
