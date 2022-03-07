#include "framework.h"
#include "Loading.h"
#include "Loader.h"
#include "Stage1.h"
#include "GameInstance.h"

CLoading::CLoading()
	: m_eSceneID(SCENEID::SCENE_END)
	, m_pLoader(nullptr)
{
}

CLoading::CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext)
	: CLevel(pDevice, pDevcieContext)
	, m_eSceneID(SCENEID::SCENE_END)
	, m_pLoader(nullptr)
{
}

HRESULT CLoading::NativeConstruct(SCENEID eID)
{
	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	m_eSceneID = eID;

	if (FAILED(Ready_GameObject()))
		return E_FAIL;

	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, m_eSceneID);

	if (!m_pLoader)
		return E_FAIL;

	return S_OK;
}

_int CLoading::Tick(_double TimeDelta)
{
	if (0 > CLevel::Tick(TimeDelta))
		return -1;
	if (m_pLoader->getFinish())
	{
		if (FAILED(Move_Scene()))
			return -1;
		//if (g_pGameInstance->getkeyDown(DIK_SPACE))
		//{
		//}
	}
	return 0;
}


HRESULT CLoading::Render()
{
	if (FAILED(CLevel::Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoading::Move_Scene()
{
	CLevel* pLevel = nullptr;
	switch (m_eSceneID)
	{
	case SCENEID::SCENE_STAGE1:
		pLevel = CStage1::Create(m_pDevice, m_pDeviceContext);
		break;
	}
	if (FAILED(g_pGameInstance->Open_Level((_uint)m_eSceneID, pLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading::Ready_GameObject()
{
	wstring strTag = L"BackGroundTex";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_LOADING, L"Loading_Background", L"Prototype_GameObject_BackGround", &strTag)))
		return E_FAIL;

	return S_OK;
}

CLoading* CLoading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext, SCENEID eID)
{
	CLoading* pInstance = new CLoading(pDevice, pDevcieContext);
	if (FAILED(pInstance->NativeConstruct(eID)))
	{
		MSGBOX("Loading Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free()
{
	CLevel::Free();
	Safe_Release(m_pLoader);
}
