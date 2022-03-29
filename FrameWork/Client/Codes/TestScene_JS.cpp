#include "pch.h"
#include "..\Headers\TestScene_JS.h"

#include "JumpNode.h"
#include "JumpTrigger.h"
#include "JumpBox.h"

#include "Effect_DashDust.h"
#include "Effect_HitParticle.h"
#include "Effect_HitFloating.h"

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

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Gameobject())) 
		return E_FAIL;

	if (FAILED(Ready_Effect()))
		return E_FAIL;

	return S_OK;
}

_int CTestScene_JS::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	CGameObject* pMonster = nullptr;
	if (g_pGameInstance->getkeyDown(DIK_COLON))
	{
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Monster", L"Proto_GameObject_Monster_Bastion_2HSword", nullptr, &pMonster)))
			return -1;
		pMonster->setActive(true);
	}
	if (g_pGameInstance->getkeyDown(DIK_SEMICOLON))
	{
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Monster", L"Proto_GameObject_Monster_Crawler", nullptr, &pMonster)))
			return -1;
		pMonster->setActive(true);
	}
	return _int();
}

HRESULT CTestScene_JS::Render()
{
	return S_OK;
}

HRESULT CTestScene_JS::Ready_Light()
{
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(0.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);

	_vector up = { 0, 1.f, 0,0 };
	_vector lookat = { -1.f,1.f,1.f,0.f };

	LightDesc.mOrthinfo[0] = 40.f;

	_float3 dir = _float3(-1.f, -1.f, 1.f);
	_vector vdir = XMVector3Normalize(XMLoadFloat3(&LightDesc.vDirection));
	XMStoreFloat3(&LightDesc.vPosition, (vdir * LightDesc.mOrthinfo[0] * -1.f) + lookat);
	LightDesc.mLightView = XMMatrixLookAtLH(XMLoadFloat3(&LightDesc.vPosition), lookat, up);

	_vector origin = { 0,0,0,0 };
	_float3	forigin;
	//LightDesc.vPosition = _float3(20.f,100.f, -20.f);

	//_float3 up = _float3(0, 1.f, 0);
	//_float3 lookat = _float3(-10.f, 1.f, 5.f);

	//_vector		vPosition = XMLoadFloat3(&LightDesc.vPosition);
	//vPosition = XMVectorSetW(vPosition, 1.f);

	//_vector		vLook = XMLoadFloat3(&lookat) - XMLoadFloat3(&LightDesc.vPosition);
	//vLook = XMVector3Normalize(vLook);

	///*XMStoreFloat3(&LightDesc.vDirection, vLook);*/

	//_vector		vRight = XMVector3Cross(XMLoadFloat3(&up), vLook);
	//vRight = XMVector3Normalize(vRight);

	//_vector		vUp = XMVector3Cross(vLook, vRight);
	//vUp = XMVector3Normalize(vUp);

	//_matrix lightcam;
	//lightcam.r[0] = vRight;
	//lightcam.r[1] = vUp;
	//lightcam.r[2] = vLook;
	//lightcam.r[3] = vPosition;

	//_vector origin = { 0,0,0,0 };
	//_float3	forigin;

	//LightDesc.mLightView = XMMatrixInverse(nullptr, lightcam);

	origin = XMVector3TransformCoord(origin, LightDesc.mLightView);
	XMStoreFloat3(&forigin, origin);

	//LightDesc.mOrthinfo[0] = 30.f;

	LightDesc.mOrthinfo[1] = forigin.x - LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[2] = forigin.x + LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[3] = forigin.y - LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[4] = forigin.y + LightDesc.mOrthinfo[0];

	LightDesc.mLightProj = XMMatrixOrthographicLH(LightDesc.mOrthinfo[2] - LightDesc.mOrthinfo[1], LightDesc.mOrthinfo[4] - LightDesc.mOrthinfo[3], 0.1f, 500.f);

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CTestScene_JS::Ready_Gameobject()
{
	wstring wstrNaviFile = L"../Data/NavMesh/Stage_1_Nav.dat";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_Plane", L"Proto_GameObject_Plane_Test", &wstrNaviFile)))
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
	//tJumpNodeDesc.vPosition = { 30.f , 23.f, 202.f };
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
	//	return E_FAIL;
	//tJumpNodeDesc.vPosition = { 27.f, 18.f, 228.f };
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
	//	return E_FAIL;
	//tJumpNodeDesc.vPosition = { -2.f, 15.f, 235.f };
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
	//	return E_FAIL;
	//tJumpNodeDesc.vPosition = { 39.f, 15.f, 268.f };
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CTestScene_JS::Ready_Effect()
{
	/*vector<CEffect_DashDust::EFFECTDESC> vecEffect;
	g_pGameInstance->LoadFile<CEffect_DashDust::EFFECTDESC>(vecEffect, L"../bin/SaveData/Effect/Effect_Dash.dat");

	for (int i = 0; i < vecEffect.size(); ++i)
	{
		wstring FullName = L"Proto_GameObject_Effect_DashDust";

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect", FullName, &vecEffect[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
			return E_FAIL;
		}
	}*/


	//이펙트생성
	vector<CEffect_HitParticle::EFFECTDESC> vecEffect;
	g_pGameInstance->LoadFile<CEffect_HitParticle::EFFECTDESC>(vecEffect, L"../bin/SaveData/Effect/Effect_Player_Attack1.dat");

	for (int i = 0; i < vecEffect.size(); ++i)
	{
		wstring FullName = L"Proto_GameObject_Effect_Hit";
		//_tcscpy_s(vecEffect1[i].ShaderFullFilePath, L"../../Reference/ShaderFile/Shader_PointInstance.hlsl");
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Hit", FullName, &vecEffect[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
			return E_FAIL;
		}
	}

	//이펙트생성
	vector<CEffect_HitFloating::EFFECTDESC> vecEffect1;
	g_pGameInstance->LoadFile<CEffect_HitFloating::EFFECTDESC>(vecEffect1, L"../bin/SaveData/Effect/Effect_Player_Attack2_Floating_2.dat");

	for (int i = 0; i < vecEffect1.size(); ++i)
	{
		wstring FullName = L"Proto_GameObject_Effect_Floating";
		//_tcscpy_s(vecEffect1[i].ShaderFullFilePath, L"../../Reference/ShaderFile/Shader_PointInstance.hlsl");
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_Effect_Floating", FullName, &vecEffect1[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
			return E_FAIL;
		}
	}
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
