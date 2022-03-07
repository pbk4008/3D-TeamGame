#include "pch.h"
#include "Loading.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Stage1.h"
#include "TestObj.h"

#include "TestScene_JS.h"

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

	if (FAILED(Ready_GameObject()))
		return E_FAIL;

	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, m_eSceneID);

	if (!m_pLoader)
		return E_FAIL;
	
	return S_OK;
}

_int CLoading::Tick(_double TimeDelta)
{
	if (0>CLevel::Tick(TimeDelta))
		return -1;
	if (m_pLoader->getFinish())
	{
		if (g_pGameInstance->getkeyDown(DIK_SPACE))
		{
			if (FAILED(Move_Scene()))
				return -1;
		}
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
	case SCENEID::SCENE_TEST_JS:
		pLevel = CTestScene_JS::Create(m_pDevice, m_pDeviceContext);
	}
	if (FAILED(g_pGameInstance->Open_Level((_uint)m_eSceneID, pLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading::Ready_GameObject()
{
	wstring strTag = L"BackGround";
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_LOADING, L"Loading_Background", L"Prototype_GameObject_BackGround", &strTag)))
		return E_FAIL;

	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"BoxCollider", CBoxCollider::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"SphereCollider", CSphereCollider::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	//if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"CapsuleCollider", CCapsuleCollider::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;
	/*if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, L"Model", CModel::Create(m_pDevice, m_pDeviceContext,L"../bin/SaveData/Sphere.fbx",L"../../Reference/ShaderFile/Shader_Mesh.hlsl",CModel::TYPE_STATIC))))
			return E_FAIL;



	if (FAILED(g_pGameInstance->Add_Prototype(L"TestObj", CTestObj::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	CTestObj::TESTDESC tDesc;
	ZeroMemory(&tDesc, sizeof(tDesc));


	_matrix matIdentity = XMMatrixIdentity();
	XMStoreFloat4x4(&tDesc.tCapsuleDesc.matTransform, matIdentity);
	tDesc.tCapsuleDesc.tColDesc.bGravity = false;
	tDesc.tCapsuleDesc.tColDesc.bKinematic = false;
	tDesc.tCapsuleDesc.tColDesc.eType = CPhysicsXSystem::ACTORTYPE::ACTOR_DYNAMIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_LOADING, L"Test", L"TestObj", &tDesc)))
		return E_FAIL;

	ZeroMemory(&tDesc, sizeof(tDesc));

	XMStoreFloat4x4(&tDesc.tBoxDesc.matTransform, matIdentity);
	tDesc.tBoxDesc.tColDesc.bGravity = false;
	tDesc.tBoxDesc.tColDesc.bKinematic = false;
	tDesc.tBoxDesc.tColDesc.eType = CPhysicsXSystem::ACTORTYPE::ACTOR_STATIC;

	if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)SCENEID::SCENE_LOADING, L"Test", L"TestObj" ,&tDesc)))
		return E_FAIL;*/


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
