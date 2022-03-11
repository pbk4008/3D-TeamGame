#include "pch.h"
#include "Stage1.h"
#include "Environment.h"

#include "Effect_DashDust.h"
#include "UI_Ingame.h"

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

	if (FAILED(Ready_MapObject()))
	{
		return E_FAIL;
	}
	if (FAILED(Ready_Monster(L"Layer_Monster")))
	{
		return E_FAIL;
	}


	if (FAILED(Ready_UI(L"../bin/SaveData/UI/UI.dat")))
	{
		return E_FAIL;
	}


	if (FAILED(Ready_Effect(L"../bin/SaveData/Effect/Effect_Explosion.dat")))
	{
		return E_FAIL;
	}




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
	tEnvironmentDesc.resize(350);
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
		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Environment", L"Environment", &pDesc)))
		{
			int a = 0;
		}
	}
	//wstring strTag = L"StageBackGround";
	//g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Stage1_Back", L"Prototype_GameObject_BackGround", &strTag);

	return S_OK;
}

HRESULT CStage1::Ready_Monster(const _tchar* LayerTag)
{
	/*if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Monster_Crawler")))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Monster_EarthAberrant")))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Monster_BronzeAnimus")))
		return E_FAIL;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, LayerTag, L"Monster_Bastion_Sword")))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CStage1::Ready_Effect(const _tchar* pDataFilePath)
{
	vector<CEffect_DashDust::EFFECTDESC> vecEffect;
	g_pGameInstance->LoadFile<CEffect_DashDust::EFFECTDESC>(vecEffect, pDataFilePath);
	
	for (int i = 0; i < vecEffect.size(); ++i)
	{
		wstring Tag = vecEffect[i].TextureTag;
		wstring FullName = L"Prototype_GameObject_Effect";

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_Effect", FullName, &vecEffect[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_Effect()");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CStage1::Ready_UI(const _tchar* pDataFilePath)
{
	vector<CUI::UIDESC> vecUI;
	g_pGameInstance->LoadFile<CUI::UIDESC>(vecUI, pDataFilePath);

	for (int i = 0; i < vecUI.size(); ++i)
	{
		wstring Tag = vecUI[i].TextureTag;
		wstring FullName = L"Prototype_GameObject_UI_" + Tag;

		if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Layer_UI", FullName, &vecUI[i])))
		{
			MSGBOX("Failed to Creating in CStage1::Ready_UI()");
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
