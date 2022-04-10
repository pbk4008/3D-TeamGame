#include "pch.h"
#include "DebugSystem.h"
#include "Loading.h"

CDebugSystem::CDebugSystem()
	: m_hThread(nullptr)
	, m_iNumber(-1)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_bExit(false)
	, m_bLevelMove(false)
	, m_iSelectLevel(-1)
{
	ZeroMemory(&m_Critical, sizeof(CRITICAL_SECTION));
}

HRESULT CDebugSystem::Init_DebugSystem(ID3D11Device* pDevice, ID3D11DeviceContext* pDevcieContext, CRenderer* pRenderer)
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDevcieContext;
	m_pRenderer = pRenderer;
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	InitializeCriticalSection(&m_Critical);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Update_DebugSystem, nullptr, 0, nullptr);

	return S_OK;
}

HRESULT CDebugSystem::Input_Number()
{
	cout << "기능 선택" << endl;
	cout << "0. 레벨 이동 1. 오브젝트 특정 좌표 이동 2. God Mode" << endl;
	cin >> m_iNumber;
	system("cls");

	if (FAILED(Debug_Functions()))
		return E_FAIL;


	return S_OK;
}

void CDebugSystem::Set_LevelcMoveCheck(_bool bCheck)
{
	m_bLevelMove = bCheck;
	m_iSelectLevel = -1;
}


_bool CDebugSystem::Get_LevelMoveCheck(_int& bLevel)
{
	bLevel = m_iSelectLevel;

	return m_bLevelMove;
}

HRESULT CDebugSystem::Debug_Functions()
{
	HRESULT hr = 0;
	switch (m_iNumber)
	{
	case 0:
		hr = Level_Move();
		break;
	case 1:
		break;
	default:
		hr = S_OK;
		break;
	}
	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CDebugSystem::Level_Move()
{
	_int iSelect = 0;
	cout << "현재 레벨 : "; 
	Print_Level();
	cout << "1. 로고    2. Stage1     3. Stage2     4.Stage3" << endl;
	cout <<"이동하고자 하는 레벨을 선택하세요(0 입력시 뒤로 이동) : ";
	cin >> iSelect;
	system("cls");
	if (iSelect == 0)
		return S_OK;

	m_bLevelMove = true;
	m_iSelectLevel = iSelect+1;
	m_pRenderer->Remove_RenderGroup();
	if (FAILED(g_pGameInstance->Open_Level((_uint)SCENEID::SCENE_LOADING, CLoading::Create(m_pDevice, m_pDeviceContext, (SCENEID)(iSelect + 1)))))
		return E_FAIL;

	return S_OK;
}

void CDebugSystem::Print_Level()
{
	_uint iLevel = g_pGameInstance->getCurrentLevel();

	switch (iLevel)
	{
	case (_uint)SCENEID::SCENE_STATIC:
		cout << "Static"<<endl;
		return;
	case (_uint)SCENEID::SCENE_LOADING:
		cout << "Loading" << endl;
		return;
	case (_uint)SCENEID::SCENE_LOGO:
		cout << "Logo" << endl;
		return;
	case (_uint)SCENEID::SCENE_STAGE1:
		cout << "Stage1" << endl;
		return;
	}
}



_uint CDebugSystem::Update_DebugSystem(void* pArg)
{
	_uint iFlag = 0;

	EnterCriticalSection(m_pInstance->getCritical());

	while (1)
	{
		if (m_pInstance->m_bExit)
			break;
		if (FAILED(m_pInstance->Input_Number()))
			 iFlag = E_FAIL;
		if (m_pInstance->m_iNumber == -1)
			break;
	}
	LeaveCriticalSection(m_pInstance->getCritical());
	return iFlag;
}

void CDebugSystem::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pRenderer);

	LeaveCriticalSection(&m_Critical);
	//_ulong exitCode;
	//GetExitCodeThread(m_hThread, &exitCode);
	//ExitThread(exitCode)
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Critical);
}
