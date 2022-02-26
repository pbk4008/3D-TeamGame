#include "pch.h"
#include "Loading.h"
#include "Loader.h"
#include "Level_Logo.h"


CLoading::CLoading()
	: m_eSceneID(SCENEID::SCENE_END)
	, m_pLoader(nullptr)
{
}

CLoading::CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext)
	:CLevel(pDevice,pDevcieContext)
	, m_eSceneID(SCENEID::SCENE_END)
	, m_pLoader(nullptr)
{
}

HRESULT CLoading::NativeConstruct(SCENEID eID)
{
	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	m_eSceneID = eID;

	if(FAILED(Ready_GameObject()))
		return E_FAIL;

	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, m_eSceneID);

	if (!m_pLoader)
		return E_FAIL;
	
	return S_OK;
}

_int CLoading::Tick(_float fDeltaTime)
{
	if (0>CLevel::Tick(fDeltaTime))
		return -1;
	if (m_pLoader->getFinish())
	{
 		if (FAILED(Move_Scene()))
			return -1;
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
	case SCENEID::SCENE_LOGO:
		pLevel = CLevel_Logo::Create(m_pDevice, m_pDeviceContext);
		break;
	case SCENEID::SCENE_STAGE1:
		break;
	}
	if (FAILED(g_pGameInstance->Open_Level((_uint)m_eSceneID, pLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading::Ready_GameObject()
{
	if (FAILED(g_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), ))))
		return E_FAIL;

	wstring 
	g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_LOADING, L"Prototype_GameObject_BackGround", L"Logo_BackGround",
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
