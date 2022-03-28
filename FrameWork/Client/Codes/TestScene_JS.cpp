#include "pch.h"
#include "..\Headers\TestScene_JS.h"

#include "JumpNode.h"
#include "JumpTrigger.h"
#include "JumpBox.h"

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


	// 점프 노드들
	CJumpNode::DESC tJumpNodeDesc;
	tJumpNodeDesc.vPosition = { 25.f, 5.f, 84.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -25.f, 8.f, 98.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -176.f, 50.f, 335.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;

	// 점프 박스들
	CJumpBox::DESC tJumpBoxrDesc;
	tJumpBoxrDesc.vPosition = { -42.5f, 4.5f, 82.f };
	tJumpBoxrDesc.vRotation = { 0.f, 0.f, 0.f };
	tJumpBoxrDesc.vScale = { 7.f, 2.f, 8.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpBox", L"Proto_GameObject_JumpBox", &tJumpBoxrDesc)))
		return E_FAIL;
	tJumpBoxrDesc.vPosition = { -136.5f, 19.f, 231.5f };
	tJumpBoxrDesc.vRotation = { 0.f, 0.f, 0.f };
	tJumpBoxrDesc.vScale = { 7.f, 2.f, 6.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpBox", L"Proto_GameObject_JumpBox", &tJumpBoxrDesc)))
		return E_FAIL;



	///////////////////// 스테이지 2용
	// 
	// 점프 노드들
	tJumpNodeDesc.vPosition = { 30.f , 23.f, 202.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { 27.f, 18.f, 228.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -2.f, 15.f, 235.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { 39.f, 15.f, 268.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
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
