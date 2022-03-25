#include "pch.h"
#include "..\Headers\TestScene_JS.h"

#include "JumpNode.h"
#include "JumpTrigger.h"

CTestScene_JS::CTestScene_JS()
{
}

CTestScene_JS::CTestScene_JS(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CTestScene_JS::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct())) return E_FAIL;

	if (FAILED(Ready_Gameobject())) return E_FAIL;

	return S_OK;
}

_int CTestScene_JS::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta)) return -1;

	return _int();
}

HRESULT CTestScene_JS::Render()
{
	return S_OK;
}

HRESULT CTestScene_JS::Ready_Gameobject()
{
 	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Plane", L"Proto_GameObject_Plane_Test")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Silvermane", L"Proto_GameObject_Silvermane")))
		return E_FAIL;
 	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;


	// ���� ����
	CJumpNode::DESC tJumpNodeDesc;
	tJumpNodeDesc.vPosition = { 25.f, 5.f, 84.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -176.f, 50.f, 335.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;

	// ���� Ʈ���ŵ�
	CJumpTrigger::DESC tJumpTriggerDesc;
	tJumpTriggerDesc.vPosition = { -25.f, 6.f, 100.f };
	tJumpTriggerDesc.vRotation = { 0.f, 0.f, 0.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpTrigger", L"Proto_GameObject_JumpTrigger", &tJumpTriggerDesc)))
		return E_FAIL;
	tJumpTriggerDesc.vPosition = { -47.f, 4.5f, 81.f };
	tJumpTriggerDesc.vRotation = { 0.f, 90.f, 0.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpTrigger", L"Proto_GameObject_JumpTrigger", &tJumpTriggerDesc)))
		return E_FAIL;
	tJumpTriggerDesc.vPosition = { -136.f, 18.5f, 236.f };
	tJumpTriggerDesc.vRotation = { 0.f, 0.f, 0.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpTrigger", L"Proto_GameObject_JumpTrigger", &tJumpTriggerDesc)))
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
