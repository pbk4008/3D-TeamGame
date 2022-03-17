#include "pch.h"
#include "TestScene_Sb.h"

CTestScene_Sb::CTestScene_Sb()
{
}

CTestScene_Sb::CTestScene_Sb(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CTestScene_Sb::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct())) return E_FAIL;

	if (FAILED(Ready_Gameobject())) return E_FAIL;

	return S_OK;
}

_int CTestScene_Sb::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta)) return -1;

	return _int();
}

HRESULT CTestScene_Sb::Render()
{
	return S_OK;
}

HRESULT CTestScene_Sb::Ready_Gameobject()
{
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_SB, L"Layer_Plane", L"Plane_Test")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_SB, L"Layer_Silvermane", L"Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_SB, L"Layer_Camera", L"Camera_Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_SB, L"Layer_Monster", L"Monster_Bastion_Marksman")))
		return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Monster", L"Monster_Bastion_Healer")))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Monster", L"Monster_Bastion_Shooter")))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Monster", L"Monster_Bastion_Spear")))
	//	return E_FAIL;
	return S_OK;
}

CTestScene_Sb* CTestScene_Sb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTestScene_Sb* pInstance = new CTestScene_Sb(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CTestScene_Sb Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestScene_Sb::Free()
{

	__super::Free();
}