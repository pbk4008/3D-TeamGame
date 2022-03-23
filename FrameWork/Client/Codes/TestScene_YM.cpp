#include "pch.h"
#include "..\Headers\TestScene_YM.h"

#include "TestObj.h"

CTestScene_YM::CTestScene_YM()
{
}

CTestScene_YM::CTestScene_YM(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CTestScene_YM::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct())) return E_FAIL;

	if (FAILED(Ready_Gameobject())) return E_FAIL;

	return S_OK;
}

_int CTestScene_YM::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta)) return -1;

	return _int();
}

HRESULT CTestScene_YM::Render()
{
	return S_OK;
}

HRESULT CTestScene_YM::Ready_Gameobject()
{
	CGameObject* pObj = nullptr;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Plane", L"Proto_GameObject_Plane_Test")))
		return E_FAIL;

	//CTestObj::TESTDESC tTestDesc;
	//tTestDesc._eRigidType = ERigidType::Static;
	//tTestDesc.vPosition = { 2.f, 0.f, 2.f };
	//tTestDesc.isMove = false;
	//tTestDesc.isTrigger = false;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_TestObj", L"Proto_GameObject_TestObj", &tTestDesc, &pObj)))
	//	return E_FAIL;
	//tTestDesc._eRigidType = ERigidType::Dynamic;
	//tTestDesc.vPosition = { -2.f, 0.f, 2.f };
	//tTestDesc.isMove = true;
	//tTestDesc.isTrigger = true;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_TestObj", L"Proto_GameObject_TestObj", &tTestDesc, &pObj)))
	//	return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Silvermane", L"Proto_GameObject_Silvermane")))
		return E_FAIL;
 	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Monster", L"Monster_Bastion_2HSword")))
		return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Monster", L"Monster_Bastion_Healer")))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Monster", L"Monster_Bastion_Healer")))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Monster", L"Monster_Bastion_Shooter")))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_YM, L"Layer_Monster", L"Monster_Bastion_Spear")))
	//	return E_FAIL;
	return S_OK;
}

CTestScene_YM* CTestScene_YM::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTestScene_YM* pInstance = new CTestScene_YM(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CTestScene_YM Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestScene_YM::Free()
{

	__super::Free();
}
