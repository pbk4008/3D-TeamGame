#include "pch.h"
#include "Loading.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"

#include "TestObj.h"
#include "TestScene_JS.h"
#include "BackGround.h"

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

HRESULT CLoading::NativeConstruct(SCENEID eID, _bool bDebug)
{
	if (FAILED(CLevel::NativeConstruct()))
		return E_FAIL;

	m_eSceneID = eID;

	if (FAILED(Ready_GameObject()))
		return E_FAIL;

	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, m_eSceneID, bDebug);

	if (!m_pLoader)
		return E_FAIL;

	m_pobj->Set_FadeInCheck(true);
	m_pobj->setActive(true);

	return S_OK;
}

_int CLoading::Tick(_double TimeDelta)
{
	if (0>CLevel::Tick(TimeDelta))
		return -1;
	if (m_pLoader->getFinish() && m_pobj->Get_FadeIn() == false && m_eSceneID == SCENEID::SCENE_LOGO)
	{
		if(m_eSceneID == SCENEID::SCENE_LOGO)
			m_pobj->Set_FadeOutCheck(true);
	}

	if (m_eSceneID != SCENEID::SCENE_LOGO)
	{
		if (m_pLoader->getFinish())
		{
			if (FAILED(Move_Scene()))
				return -1;
		}
	}
	else if (m_pobj->Get_OutNextCheck() == true)
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
		pLevel = CStage1::Create(m_pDevice, m_pDeviceContext);
		break;
	case SCENEID::SCENE_STAGE2:
		pLevel = CStage2::Create(m_pDevice, m_pDeviceContext);
		break;
	case SCENEID::SCENE_STAGE3:
		pLevel = CStage3::Create(m_pDevice, m_pDeviceContext);
		break;
	case SCENEID::SCENE_TEST_JS:
		pLevel = CTestScene_JS::Create(m_pDevice, m_pDeviceContext);
		break;
	}

	if (FAILED(g_pGameInstance->Open_Level((_uint)m_eSceneID, pLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading::Ready_GameObject()
{
	wstring strTag = L"Texture_BackGround";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_LOADING, L"Layer_Background", L"Proto_GameObject_BackGround", &strTag,&m_pobj)))
		return E_FAIL;

	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"BoxCollider", CBoxCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"SphereCollider", CSphereCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"CapsuleCollider", CCapsuleCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;*/

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model", CModel::Create(m_pDevice, m_pDeviceContext,L"../bin/SaveData/Sphere.fbx",L"../../Reference/ShaderFile/Shader_Mesh.hlsl",CModel::TYPE_STATIC))))
	//		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype(L"TestObj", CTestObj::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	return S_OK;
}

CLoading* CLoading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext, SCENEID eID, _bool bDebug)
{
	CLoading* pInstance = new CLoading(pDevice, pDevcieContext);
	if (FAILED(pInstance->NativeConstruct(eID, bDebug)))
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
