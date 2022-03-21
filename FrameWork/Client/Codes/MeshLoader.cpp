#include "pch.h"
#include "MeshLoader.h"

CMeshLoader::CMeshLoader()
	: m_bPause(false)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{
	ZeroMemory(&m_Critical, sizeof(CRITICAL_SECTION));
}

HRESULT CMeshLoader::Add_MeshLoader(MESHTYPE& tType)
{
	_uint iSize = (_uint)m_vecMeshLoader.size();
	for (_uint i = 0; i < iSize; i++)
	{
		tType.iLoaderIndex = i;
		if (!m_vecMeshLoader[i].hThread)
		{
			m_vecMeshLoader[i].bWorking = true;
			m_vecMeshLoader[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, Load_FBX, &tType, 0, nullptr);
			break;
		}
		else if (!m_vecMeshLoader[i].bWorking)
		{
			m_vecMeshLoader[i].bWorking = true;
			m_vecMeshLoader[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, Load_FBX, &tType, 0, nullptr);
			break;
		}
	}
	return S_OK;
}

HRESULT CMeshLoader::Reserve_MeshLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_vecMeshLoader.resize(10);
	InitializeCriticalSection(&m_Critical);
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

_bool CMeshLoader::Get_AllWorking()
{
	if (m_vecMeshLoader.empty())
		return false;

	for (auto& pLoader : m_vecMeshLoader)
	{
		if (!pLoader.bWorking)
			return false;
	}
	return true;
}

_uint CMeshLoader::Load_FBX(void* pArg)
{
	CMeshLoader* pMeshLoader = CMeshLoader::GetInstance();

	EnterCriticalSection(&pMeshLoader->m_Critical);

	HRESULT hr = S_OK;
	MESHTYPE tMeshType = (*(MESHTYPE*)pArg);

	_uint iMeshLoaderIndex = tMeshType.iLoaderIndex;
	switch (tMeshType.iType)
	{
	case 0://Static
		hr = g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, tMeshType.szFBXName
			, CModel::Create(pMeshLoader->m_pDevice, pMeshLoader->m_pDeviceContext, tMeshType.szFBXPath,
				CModel::TYPE_STATIC, true));
		break;
	case 1://Anim
		hr = g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, tMeshType.szFBXName
			, CModel::Create(pMeshLoader->m_pDevice, pMeshLoader->m_pDeviceContext, tMeshType.szFBXPath,
				CModel::TYPE_ANIM, true));
		break;
	case 2://Instanc_Static
		hr=g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, tMeshType.szFBXName
			, CInstancing_Mesh::Create(pMeshLoader->m_pDevice, pMeshLoader->m_pDeviceContext, tMeshType.szFBXPath,
				CInstancing_Mesh::INSTANCE_TYPE::STATIC));
		if (FAILED(hr))
			int i = 10;
		break;
	}
	if (FAILED(hr))
	{
		LeaveCriticalSection(&pMeshLoader->m_Critical);
		MSGBOX("Load_Fail FBX");
		return 0;
	}
	else
	{
		pMeshLoader->m_vecMeshLoader[iMeshLoaderIndex].bWorking = false;
		CloseHandle(pMeshLoader->m_vecMeshLoader[iMeshLoaderIndex].hThread);
		LeaveCriticalSection(&pMeshLoader->m_Critical);
	}

	return _uint();
}

void CMeshLoader::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	for (auto& pMeshLoade : m_vecMeshLoader)
	{
		WaitForSingleObject(pMeshLoade.hThread,INFINITE);
		CloseHandle(pMeshLoade.hThread);
	}
	m_vecMeshLoader.clear();
	DeleteCriticalSection(&m_Critical);
}
