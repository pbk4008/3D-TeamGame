#include "pch.h"
#include "Stage1.h"
#include "Environment.h"

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

	if (FAILED(Ready_Gameobject()))
		return E_FAIL;

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

HRESULT CStage1::Ready_Gameobject()
{
	/*wstring strTag = L"StageBackGround";
	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_STAGE1, L"Stage1_Back", L"Prototype_GameObject_BackGround", &strTag);*/
	vector<ENVIRONMENTLOADDATA> vecEnvironmentData;
	if (FAILED(g_pGameInstance->LoadFile<ENVIRONMENTLOADDATA>(vecEnvironmentData, L"../bin/SaveData/test.dat")))
		return E_FAIL;


	vector<CEnvironment::ENVIRONMENTDESC> tEnvironmentDesc;
	tEnvironmentDesc.resize(50);
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
