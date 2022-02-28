#include "pch.h"
#include "Loader.h"


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

void CLoader::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Critical);
}
