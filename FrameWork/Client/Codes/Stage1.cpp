#include "pch.h"
#include "Stage1.h"
#include "Environment.h"

#include "Effect_DashDust.h"
#include "UI_Ingame.h"
#include "UI_Player_HpBar.h"

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

	/*if (FAILED(Ready_MapObject()))
	{
		return E_FAIL;
	}*/

	if (FAILED(Ready_Player(L"Layer_Silvermane")))
	{
		return E_FAIL;
	}

	//if (FAILED(Ready_Boss(L"Layer_Boss")))
	//{
	//	return E_FAIL;
	//}
	if (FAILED(Ready_Monster(L"Layer_Monster")))
	{
		return E_FAIL;
	}
	//if (FAILED(Ready_UI(L"Layer_UI")))
	//{
	//	return E_FAIL;
	//}

	////Data
	//if (FAILED(Ready_Data_UI(L"../bin/SaveData/UI/UI.dat")))
	//{
	//	return E_FAIL;
	//}
	//if (FAILED(Ready_Data_Effect(L"../bin/SaveData/Effect/Effect_Explosion.dat")))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(Ready_Trigger_Lod(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Light(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Monster(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;
	//if (FAILED(Ready_Trigger_Scene(L"../bin/SaveData/Trigger/Stage1_LodTri.dat")))
	//	return E_FAIL;

	return S_OK;
}

_int CStage1::Tick(_double TimeDelta)
{
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
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Plane", L"Proto_GameObject_Plane_Test")))
	//	return E_FAIL;

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
	//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Proto_GameObject_Monster_Crawler")))
	//	return E_FAIL;
	
	/*if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Monster_EarthAberrant")))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Monster_BronzeAnimus")))
		return E_FAIL;*/

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Monster_Bastion_Sword")))
		return E_FAIL;

		//if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Monster_Bastion_Shooter")))
		//return E_FAIL;

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
	vector<TRIGGER> vecTrigger;
	
	g_pGameInstance->LoadFile<TRIGGER>(vecTrigger, pDataFilePath);

	for (int i = 0; i < vecTrigger.size(); ++i)
	{
		TRIGGER TriggerDesc;

		TriggerDesc.eTrigger_Type = vecTrigger[i].eTrigger_Type;
		TriggerDesc.fTrigger_Point = vecTrigger[i].fTrigger_Point;

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
