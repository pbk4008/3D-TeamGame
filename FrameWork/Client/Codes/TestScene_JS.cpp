#include "pch.h"
#include "..\Headers\TestScene_JS.h"

CTestScene_JS::CTestScene_JS()
{
}

CTestScene_JS::CTestScene_JS(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CTestScene_JS::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Gameobject()))
		return E_FAIL;

	return S_OK;
}

_int CTestScene_JS::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		return -1;
	}

	return _int();
}

HRESULT CTestScene_JS::Render()
{
	return S_OK;
}

HRESULT CTestScene_JS::Ready_Gameobject()
{
 	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Plane", L"Plane_Test")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Silvermane", L"Silvermane")))
		return E_FAIL;
 	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Camera", L"Camera_Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Monster", L"Monster_Bastion_2HSword")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Monster", L"Monster_Bastion_Healer")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Monster", L"Monster_Bastion_Shooter")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Monster", L"Monster_Bastion_Spear")))
		return E_FAIL;
	return S_OK;
}

CTestScene_JS* CTestScene_JS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTestScene_JS* pInstance = new CTestScene_JS(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CTestScene_JS Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestScene_JS::Free()
{

	__super::Free();
}
