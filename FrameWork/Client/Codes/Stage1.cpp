#include "pch.h"
#include "Loading.h"
#include "Stage1.h"
#include "Environment.h"

#include "Effect_DashDust.h"
#include "UI_Ingame.h"
#include "UI_Player_HpBar.h"
#include "UI_Tuto_Base.h"
#include "UI_Tuto_Font.h"

#include "JumpNode.h"
#include "JumpTrigger.h"

CStage1::CStage1()
{
}

CStage1::CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CStage1::NativeConstruct()
{
	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Light())) return E_FAIL;

	if (FAILED(Ready_MapObject()))
	{
		return E_FAIL;
	}
	
	if (FAILED(Ready_Trigger_Jump()))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
	{
		return E_FAIL;
	}

	//if (FAILED(Ready_Monster(L"Layer_Monster")))
	//{
	//	return E_FAIL;
	//}

	if (FAILED(Ready_Boss(L"Layer_Boss")))
	{
		return E_FAIL;
	}


	//Data
	if (FAILED(Ready_Data_UI(L"../bin/SaveData/UI/UI.dat")))
	{
		return E_FAIL;
	}
	
	//if (FAILED(Ready_Data_Effect(L"../bin/SaveData/Effect/Effect_Explosion.dat")))
	//{
	//	return E_FAIL;
	//}

	if (FAILED(Ready_UI(L"Layer_UI")))
	{
		return E_FAIL;
	}

	//if (FAILED(Ready_Trigger_Lod(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Light(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Monster(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Scene(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Quest(L"../bin/SaveData/Trigger/Stage1_QuestTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Treasure_Chest()))
	//	return E_FAIL;

	g_pGameInstance->Change_BaseCamera(L"Camera_Silvermane");


	return S_OK;
}

_int CStage1::Tick(_double TimeDelta)
{
#ifdef  _DEBUG
	_int iLevel = 0;
	if (g_pDebugSystem->Get_LevelMoveCheck(iLevel))
	{
		CLoading* pLoading = CLoading::Create(m_pDevice, m_pDeviceContext, (SCENEID)iLevel);
		if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, pLoading)))
			return -1;
		g_pDebugSystem->Set_LevelcMoveCheck(false);
	}
#endif //  _DEBUG

	return _int();
}

HRESULT CStage1::Render()
{
	return S_OK;
}

HRESULT CStage1::Ready_MapObject()
{
	vector<ENVIRONMENTLOADDATA> vecEnvironmentData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecEnvironmentData, L"../bin/SaveData/Stage_1.dat")))
		return E_FAIL;

	vector<CEnvironment::ENVIRONMENTDESC> tEnvironmentDesc;
	tEnvironmentDesc.resize(1000);
	_uint iIndex = 0;
	tEnvironmentDesc[iIndex].wstrInstaneTag = vecEnvironmentData[0].FileName;
	for (auto& pData : vecEnvironmentData)
	{
		if (lstrcmp(tEnvironmentDesc[iIndex].wstrInstaneTag.c_str(), pData.FileName))
		{
			iIndex++;
			tEnvironmentDesc[iIndex].wstrInstaneTag = pData.FileName;
			tEnvironmentDesc[iIndex].tInstanceDesc.vecMatrix.emplace_back(pData.WorldMat);
		}
		else
			tEnvironmentDesc[iIndex].tInstanceDesc.vecMatrix.emplace_back(pData.WorldMat);
	}
	_uint iTmpIndx = 0;
	for (auto& pDesc : tEnvironmentDesc)
	{
		if (pDesc.wstrInstaneTag == L"")
			break;
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Environment", L"Proto_GameObject_Environment", &pDesc)))
		{
			return E_FAIL;
		}
	}

	//wstring strTag = L"StageBackGround";
	//g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Stage1_Back", L"Prototype_GameObject_BackGround", &strTag);

	return S_OK;
}

HRESULT CStage1::Ready_Camera(const _tchar* LayerTag)
{
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Camera_Silvermane")))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CStage1::Ready_Player(const _tchar* LayerTag)
{
	//// 네비메쉬
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Plane", L"Proto_GameObject_Plane_Test")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Silvermane")))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Camera", L"Proto_GameObject_Camera_Silvermane")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Boss(const _tchar* LayerTag)
{
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Boss_Bastion")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Monster(const _tchar* LayerTag)
{
	for (int i = 0; i < 3; ++i)
	{
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_Crawler")))
			return E_FAIL;
	}

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Monster", L"Monster_Bastion_2HSword")))
		return E_FAIL;
	
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_EarthAberrant")))
	//	return E_FAIL;
	//
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_BronzeAnimus")))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_Bastion_Healer")))
	//	return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Model_Monster_Bastion_Sword")))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_UI(const _tchar* LayerTag)
{
	//Player HpBar Green
	CUI_Player_HpBar::UIDESC Desc;
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar");
	Desc.bMinus = true;
	Desc.fAngle = 0.3f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI_Green", L"Proto_GameObject_UI_Player_HpBar", &Desc)))
		return E_FAIL;

	//Player HpBar Red
	ZeroMemory(&Desc, sizeof(CUI_Player_HpBar::UIDESC));
	_tcscpy_s(Desc.TextureTag, L"Texture_Player_HpBar_Red");
	Desc.bMinus = true;
	Desc.fAngle = 0.3f;
	Desc.fPos = { 0.f, 0.f, 0.f };
	Desc.fSize = { 200.f , 30.f };
	Desc.IDTag = (_uint)GAMEOBJECT::UI_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Player_HpBar_Red", &Desc)))
		return E_FAIL;


	//Tuto Base
	CUI_Tuto_Base::UIACTIVEDESC Desc1;
	ZeroMemory(&Desc1, sizeof(CUI_Tuto_Base::UIACTIVEDESC));
	_tcscpy_s(Desc1.UIDesc.TextureTag, L"Texture_Tuto_Base");
	Desc1.UIDesc.bMinus = false;
	Desc1.UIDesc.fAngle = 0.f;
	Desc1.UIDesc.fPos = { 1150.f, 360.f, 0.2f };
	Desc1.UIDesc.fSize = { 333.f , 105.f };
	Desc1.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Tuto_Base", &Desc1)))
		return E_FAIL;

	//Tuto Font
	CUI_Tuto_Font::UIACTIVEDESC Desc2;
	ZeroMemory(&Desc2, sizeof(CUI_Tuto_Font::UIACTIVEDESC));
	_tcscpy_s(Desc2.UIDesc.TextureTag, L"Texture_Tuto_Font");
	Desc2.UIDesc.bMinus = false;
	Desc2.UIDesc.fAngle = 0.f;
	Desc2.UIDesc.fPos = { 1130.f, 360.f, 0.1f };
	Desc2.UIDesc.fSize = { 73.f , 73.f };
	Desc2.UIDesc.IDTag = (_uint)GAMEOBJECT::UI_STATIC;
	Desc2.iTextureNum = 0;
	
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", L"Proto_GameObject_UI_Tuto_Font", &Desc2)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Light()
{
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float3(-1.f, -1.f, 1.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);

	_vector up = { 0, 1.f, 0,0 };
	_vector lookat = { 0.f, 1.f, 0.f, 1.f };

	LightDesc.mOrthinfo[0] = 50.f;

	XMStoreFloat3(&LightDesc.vPosition, ((XMLoadFloat3(&LightDesc.vDirection) * LightDesc.mOrthinfo[0] * -1.f) + lookat));
	LightDesc.mLightView = XMMatrixLookAtLH(XMLoadFloat3(&LightDesc.vPosition), lookat, up);

	_vector origin = { 0,0,0,0 };
	_float3	forigin;

	origin = XMVector3TransformCoord(origin, LightDesc.mLightView);
	XMStoreFloat3(&forigin, origin);

	LightDesc.mOrthinfo[1] = forigin.x - LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[2] = forigin.x + LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[3] = forigin.y - LightDesc.mOrthinfo[0];
	LightDesc.mOrthinfo[4] = forigin.y + LightDesc.mOrthinfo[0];

	LightDesc.mLightProj = XMMatrixOrthographicLH(LightDesc.mOrthinfo[2] - LightDesc.mOrthinfo[1], LightDesc.mOrthinfo[4] - LightDesc.mOrthinfo[3], 0.1f, 300.f);

	if (FAILED(g_pGameInstance->Add_Light(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STATIC, L"Layer_SkyBox", L"Proto_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Data_Effect(const _tchar* pDataFilePath)
{
	vector<CEffect_DashDust::EFFECTDESC> vecEffect;
	g_pGameInstance->LoadFile<CEffect_DashDust::EFFECTDESC>(vecEffect, pDataFilePath);
	
	for (int i = 0; i < vecEffect.size(); ++i)
	{
		wstring Tag = vecEffect[i].TextureTag;
		wstring FullName = L"Proto_GameObject_Effect";

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect", FullName, &vecEffect[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_Data_UI(const _tchar* pDataFilePath)
{
	//UI_Ingame_Static
	vector<CUI::UIDESC> vecUI;
	g_pGameInstance->LoadFile<CUI::UIDESC>(vecUI, pDataFilePath);

	for (int i = 0; i < vecUI.size(); ++i)
	{
		wstring Tag = vecUI[i].TextureTag;
		wstring FullName = L"Proto_GameObject_UI_" + Tag;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", FullName, &vecUI[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_UI()");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_Trigger_Lod(const _tchar* pDataFilePath)
{
	//트리거를 벡터로 받는다
	vector<TRIGGER> vecTrigger;
	
	g_pGameInstance->LoadFile<TRIGGER>(vecTrigger, pDataFilePath);
	 
	for (int i = 0; i < vecTrigger.size(); ++i)
	{
		TRIGGER TriggerDesc;

		TriggerDesc.eTrigger_Type = vecTrigger[i].eTrigger_Type;
		TriggerDesc.fTrigger_Point = vecTrigger[i].fTrigger_Point;
		TriggerDesc.iIndex = vecTrigger[i].iIndex;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Trigger_Lod", L"Prototype_GameObject_Trigger", &TriggerDesc)))
		{
			MSGBOX("트리거 파일을 불러오는 도중 오류가 발생했습니다. Stage1.cpp Line 204");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_Trigger_Light(const _tchar* pDataFilePath)
{
	return E_NOTIMPL;
}

HRESULT CStage1::Ready_Trigger_Scene(const _tchar* pDataFilePath)
{
	return E_NOTIMPL;
}

HRESULT CStage1::Ready_Trigger_Monster(const _tchar* pDataFilePath)
{
	return E_NOTIMPL;
}

HRESULT CStage1::Ready_Trigger_Quest(const _tchar* pDataFilePath)
{
	vector<TRIGGER> vecTrigger;

	g_pGameInstance->LoadFile<TRIGGER>(vecTrigger, pDataFilePath);

	for (int i = 0; i < vecTrigger.size(); ++i)
	{
		TRIGGER TriggerDesc;

		TriggerDesc.eTrigger_Type = vecTrigger[i].eTrigger_Type;
		TriggerDesc.fTrigger_Point = vecTrigger[i].fTrigger_Point;
		TriggerDesc.iIndex = vecTrigger[i].iIndex;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Trigger_Quest", L"Prototype_GameObject_Trigger", &TriggerDesc)))
		{
			MSGBOX("트리거 파일을 불러오는 도중 오류가 발생했습니다. Stage1.cpp Line 306");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_Trigger_Jump()
{
	// 점프 노드들
	CJumpNode::DESC tJumpNodeDesc;
	tJumpNodeDesc.vPosition = { 25.f, 5.f, 84.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;
	tJumpNodeDesc.vPosition = { -176.f, 50.f, 335.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpNode", L"Proto_GameObject_JumpNode", &tJumpNodeDesc)))
		return E_FAIL;

	// 점프 트리거들
	CJumpTrigger::DESC tJumpTriggerDesc;
	tJumpTriggerDesc.vPosition = { -25.f, 6.f, 100.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpTrigger", L"Proto_GameObject_JumpTrigger", &tJumpTriggerDesc)))
		return E_FAIL;
	tJumpTriggerDesc.vPosition = { -47.f, 5.f, 81.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpTrigger", L"Proto_GameObject_JumpTrigger", &tJumpTriggerDesc)))
		return E_FAIL;
	tJumpTriggerDesc.vPosition = { -136.f, 18.f, 236.f };
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_TEST_JS, L"Layer_JumpTrigger", L"Proto_GameObject_JumpTrigger", &tJumpTriggerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage1::Ready_Treasure_Chest()
{
	vector<ENVIRONMENTLOADDATA> vecMapObjectData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecMapObjectData, L"../bin/SaveData/Treasure_Chest/Stage1_Treasure_Chest.dat")))
		return E_FAIL;

	vector<_float4x4> vecObject;

	vector<CEnvironment::ENVIRONMENTDESC> tChestDesc;
	tChestDesc.resize(10);
	_uint iIndex = 0;
	tChestDesc[iIndex].wstrInstaneTag = vecMapObjectData[0].FileName;

	for (auto& pData : vecMapObjectData)
	{
		vecObject. emplace_back(pData.WorldMat);
	}

	for (int i = 0; i < vecObject.size(); ++i)
	{
		MABOBJECT MapObjectDesc;

		MapObjectDesc.WorldMat = vecObject[i];

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Treasure_Chest", L"Proto_GameObject_Treasure_Chest", &MapObjectDesc)))
		{
			MSGBOX("Treasure_Chest 파일을 불러오는 도중 오류가 발생했습니다. Stage1.cpp Line 306");
			return E_FAIL;
		}
	}
	return S_OK;
}

CStage1* CStage1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CStage1* pInstance = new CStage1(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CStage1 Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStage1::Free()
{
	CLevel::Free();
}
