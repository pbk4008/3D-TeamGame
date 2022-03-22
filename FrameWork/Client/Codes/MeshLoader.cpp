#include "pch.h"
#include "MeshLoader.h"

CMeshLoader::CMeshLoader()
	: m_bClear(false)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{
	ZeroMemory(&m_Critical, sizeof(CRITICAL_SECTION));
}

HRESULT CMeshLoader::Add_MeshLoader(MESHTYPE* tType,_bool& bCheck)
{
	_uint iSize = (_uint)m_vecWorking.size();
	for (_uint i = 0; i < iSize; i++)
	{
		tType->iLoaderIndex = i;
		if (!m_HandleArr[i])//쓰레드가 없을때
		{
			bCheck = false;
			m_vecWorking[i] = true;
			m_HandleArr[i] = (HANDLE)_beginthreadex(nullptr, 0, Load_FBX, tType, 0, nullptr);
			return S_OK;
		}
		else if (!m_vecWorking[i])//쓰레드는 있는데 일을 안할때
		{
			bCheck = false;
			m_vecWorking[i] = true;
			m_HandleArr[i] = (HANDLE)_beginthreadex(nullptr, 0, Load_FBX, tType, 0, nullptr);
			return S_OK;
		}
	}
	_uint iResult = 0;
	for (_uint i = 0; i < iSize; i++)
	{
		 iResult= ResumeThread(m_HandleArr[i]);
	}
	if(iResult!=0)
		WaitForMultipleObjects(iSize, m_HandleArr, true, INFINITE);
	bCheck = true;

	return S_OK;
}

HRESULT CMeshLoader::Reserve_MeshLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	InitializeCriticalSection(&m_Critical);
	m_vecWorking.resize(2);
	_uint iSize = (_uint)m_vecWorking.size();
	for (_uint i = 0; i < iSize; i++)
		m_HandleArr[i] = nullptr;

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

void CMeshLoader::Update()
{
	_bool bCheck = false;
	for (auto bWork : m_vecWorking)
	{
		if (bWork)
			bCheck = true;
	}
	if (!bCheck)
		m_bClear = true;
}


_bool CMeshLoader::Get_AllWorking()
{
	for (auto pWorking : m_vecWorking)
	{
		if (!pWorking)
			return false;
	}
	return true;
}

void CMeshLoader::Resume_Thread()
{
	_uint iSize = (_uint)m_vecWorking.size();
	_uint iResult = 0;
	for (_uint i = 0; i < iSize; i++)
	{
		if (m_vecWorking[i])
			iResult = ResumeThread(m_HandleArr[i]);
	}
	if(iResult != 0)
		WaitForMultipleObjects(iSize, m_HandleArr, true, INFINITE);
}

void CMeshLoader::Wait_AllThread()
{
	_uint iSize = (_uint)m_vecWorking.size();
	for(_uint i=0; i< iSize; i++)
	{
		if (m_vecWorking[i])
			WaitForSingleObject(m_HandleArr[i], INFINITE);
	}
}

_uint CMeshLoader::Load_FBX(void* pArg)
{
	CMeshLoader* pMeshLoader = CMeshLoader::GetInstance();

	HRESULT hr = S_OK;

	MESHTYPE* tMeshType = (MESHTYPE*)pArg;

	_uint iMeshLoaderIndex = tMeshType->iLoaderIndex;

	//EnterCriticalSection(&pMeshLoader->m_Critical);
	switch (tMeshType->iType)
	{
	case 0://Static
		hr = g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, tMeshType->szFBXName
			, CModel::Create(pMeshLoader->m_pDevice, pMeshLoader->m_pDeviceContext, tMeshType->szFBXPath,
				CModel::TYPE_STATIC, true));
		break;
	case 1://Anim
		hr = g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, tMeshType->szFBXName
			, CModel::Create(pMeshLoader->m_pDevice, pMeshLoader->m_pDeviceContext, tMeshType->szFBXPath,
				CModel::TYPE_ANIM, true));
		break;
	case 2://Instanc_Static
		hr=g_pGameInstance->Add_Prototype((_uint)SCENEID::SCENE_STATIC, tMeshType->szFBXName
			, CInstancing_Mesh::Create(pMeshLoader->m_pDevice, pMeshLoader->m_pDeviceContext, tMeshType->szFBXPath, 
				CInstancing_Mesh::INSTANCE_TYPE::STATIC));
		if (FAILED(hr))
			int i = 10;
		break;
	}

	if (FAILED(hr))
	{
		//LeaveCriticalSection(&pMeshLoader->m_Critical);
		MSGBOX("Load_Fail FBX");
		return 0;
	}
	else
	{
		Safe_Delete_Array(tMeshType->szFBXName);
		Safe_Delete_Array(tMeshType->szFBXPath);
		Safe_Delete(tMeshType);
		pMeshLoader->m_vecWorking[iMeshLoaderIndex] = false;
		//LeaveCriticalSection(&pMeshLoader->m_Critical);
	}

	return _uint();
}

void CMeshLoader::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	_uint iSize = (_uint)m_vecWorking.size();
	for (_uint i = 0; i < iSize; i++)
		CloseHandle(m_HandleArr[i]);

	m_vecWorking.clear();
	DeleteCriticalSection(&m_Critical);
}
