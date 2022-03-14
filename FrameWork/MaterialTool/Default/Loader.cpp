#include "pch.h"
#include "Loader.h"

CLoader::CLoader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	: m_pDeivce(_pDevice)
	, m_pDeviceContext(_pDeviceContext)
{
	Safe_AddRef(m_pDeivce);
	Safe_AddRef(m_pDeviceContext);
}

_uint APIENTRY Thread_Main(void* _pArg)
{
	CLoader* pLoader = (CLoader*)_pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	HRESULT			hr = 0;

	switch (pLoader->Get_NextLevelID())
	{
	case ELevel::AnimationTool:
		hr = pLoader->Loading_ForAnimationTool();
		break;
	}

	if (FAILED(hr))
		return -1;

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

CRITICAL_SECTION CLoader::Get_CriticalSection() const
{
	return m_CS;
}

const ELevel CLoader::Get_NextLevelID() const
{
	return m_eNextLevel;
}

const _bool CLoader::Get_Finished() const
{
	return m_isFinished;
}

HRESULT CLoader::NativeConstruct(ELevel _eLevel)
{
	m_eNextLevel = _eLevel;

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForAnimationTool()
{

	m_isFinished = true;
	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, const ELevel _eLevel)
{
	CLoader* pInstance = new CLoader(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(_eLevel)))
	{
		MSGBOX("Failed to Creating CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CS);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDeivce);
}
