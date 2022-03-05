#include "pch.h"
#include "Loader.h"

#include "Material.h"

#pragma region TestScene_JS

#include "Plane_Test.h"
#include "Silvermane.h"
#include "Camera_Silvermane.h"

#pragma endregion


CLoader::CLoader() 
	: m_hThread(nullptr)
	, m_bFinish(false)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_eNextScene(SCENEID::SCENE_END)
{
	ZeroMemory(&m_Critical, sizeof(CRITICAL_SECTION));
}

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) 
	: m_hThread(nullptr)
	, m_bFinish(false)
	, m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_eNextScene(SCENEID::SCENE_END)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	ZeroMemory(&m_Critical, sizeof(CRITICAL_SECTION));
}
HRESULT CLoader::Init_Loader(SCENEID eID)
{
	InitializeCriticalSection(&m_Critical);
	m_eNextScene = eID;
	
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0,nullptr);
	
	if (!m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::LoadForScene()
{
	HRESULT hr = 0;
	switch (m_eNextScene)
	{
	case SCENEID::SCENE_LOGO:
		hr = Ready_Logo();
		break;
	case SCENEID::SCENE_STAGE1:
		hr = Ready_Stage1();
		break;
	case SCENEID::SCENE_TEST_JS:
		hr = Ready_Test_JS();
		break;
	default:
		return E_FAIL;
	}

	if (FAILED(hr))
		return E_FAIL;
	return S_OK;
}

_uint CLoader::Thread_Main(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;
	_uint iFlag = 0;

	EnterCriticalSection(pLoader->getCritical());
	if (FAILED(pLoader->LoadForScene()))
		iFlag = E_FAIL;

	pLoader->m_bFinish = true;
	LeaveCriticalSection(pLoader->getCritical());
	return iFlag;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, SCENEID eID)
{
	CLoader* pInsance = new CLoader(pDevice, pDeviceContext);
	if (FAILED(pInsance->Init_Loader(eID)))
	{
		MSGBOX("Loader Create Fail");
		Safe_Release(pInsance);
	}
	return pInsance;
}

HRESULT CLoader::Ready_Logo()
{
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Title", L"../bin/Resources/Texture/penguin000.jpg")))
		return E_FAIL;

	


	return S_OK;
}

HRESULT CLoader::Ready_Stage1()
{
	if (FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"StageBackGround", L"../bin/Resources/Texture/penguin001.jpg")))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CLoader::Ready_Test_JS()
{
	cout << "TestScene_JS 로딩 시작..." << endl;

#pragma region 리소스

	if(FAILED(g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Texture/Terrain/Plane_Default.png")))
		return E_FAIL;

	cout << "TestScene_JS 리소스 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;


	CTexture* pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Hairs_d_new.tga", 1);
	CMaterial* pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Hair", L"../../Reference/ShaderFile/Shader_MeshAnim.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Silvermane_Hair", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Cloak_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Cloak", L"../../Reference/ShaderFile/Shader_MeshAnim.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Silvermane_Cloak", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Down_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Down", L"../../Reference/ShaderFile/Shader_MeshAnim.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Silvermane_Down", pMtrl);

	pTexture = CTexture::Create(m_pDevice, m_pDeviceContext);
	pTexture->NativeConstruct_Prototype(L"../Bin/Resources/Mesh/Silvermane/T_Silvermane_Top_D.tga", 1);
	pMtrl = CMaterial::Create(m_pDevice, m_pDeviceContext, L"Silvermane_Top", L"../../Reference/ShaderFile/Shader_MeshAnim.hlsl", CMaterial::EType::Anim);
	pMtrl->Set_Texture("g_DiffuseTexture", aiTextureType_DIFFUSE, pTexture, 0);
	g_pGameInstance->Add_Material(L"Silvermane_Top", pMtrl);

#pragma endregion


#pragma region 컴포넌트

	cout << "TestScene_JS 컴포넌트 프로토타입 생성중..." << endl;
	_matrix matPivot = XMMatrixIdentity();
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"Model_Silvermane", CModel::Create(m_pDevice, m_pDeviceContext,
		"../bin/Resources/Mesh/Silvermane/", "Silvermane.fbx",
		L"../../Reference/ShaderFile/Shader_MeshAnim.hlsl",
		matPivot,
		CModel::TYPE_ANIM))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"AnimationController", CAnimationController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_TEST_JS, L"StateController", CStateController::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma endregion
#pragma region 오브젝트

	cout << "TestScene_JS 오브젝트 프로토타입 생성중..." << endl;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Plane_Test", CPlane_Test::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Silvermane", CSilvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(g_pGameInstance->Add_Prototype(L"Camera_Silvermane", CCamera_Silvermane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

#pragma endregion

	cout << "TestScene_JS 로딩 완료..." << endl;
	return S_OK;
}

void CLoader::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Critical);
}
