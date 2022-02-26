#include "pch.h"
#include "Changer.h"
#include "GameInstance.h"

#include "Mouse.h"
#include "Plane.h"
#include "Dyanmic_Camera.h"
#include "Static_Mesh.h"

CChanger::CChanger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:m_pDevice(_pDevice)
	, m_pDeviceContext(_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

_uint APIENTRY Thread_Main(void* pArg)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	CChanger* pChanger = (CChanger*)pArg;

	EnterCriticalSection(&pChanger->Get_CriticalSection());

	HRESULT hr = 0;

	switch (pChanger->Get_NextTabID())
	{
	case TAB_STATIC:
		hr = pChanger->Loading_For_Static();
		break;
	case TAB_MAP:
		hr = pChanger->Loading_ForMap_Tool();
		break;
	case TAB_CAM:
		hr = pChanger->Loading_ForCam_Tool();
		break;
	}

	if (FAILED(hr))
		return -1;

	LeaveCriticalSection(&pChanger->Get_CriticalSection());

	CoUninitialize();
	return 0;
}

HRESULT CChanger::Ready_Changer(TAB_ID _eNextTab)
{
	m_eNextTab = _eNextTab;

	InitializeCriticalSection(&m_CS);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CChanger::Loading_For_Static(void)
{
	/* Prototype_Component_VIBuffer_Terrain */
	if (FAILED(g_pGameInstance->Add_Prototype(TAB_STATIC, L"Prototype_Component_VIBuffer_Plane", CVIBuffer_Plane::Create(m_pDevice, m_pDeviceContext, L"../../Reference/ShaderFile/Shader_Plane.hlsl", 100, 100))))
		return E_FAIL;

	/* Prototype_Component_Texture_Terrain */
	g_pGameInstance->Add_Texture(m_pDevice, L"Plane_Texture", L"../Bin/Resources/Textures/Terrain/Plane_Default.png");

	m_isFinished = TRUE;

	return S_OK;
}

HRESULT CChanger::Loading_ForMap_Tool(void)
{
	/* Create Component Prototype */


	/* Create Object Prototype */

	/* Prototype_GameObject_Terrain */
	if (FAILED(g_pGameInstance->Add_Prototype(L"Prototype_GameObject_Plane", CPlane::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* Prototype_GameObject_Mouse */
	if (FAILED(g_pGameInstance->Add_Prototype(L"Prototype_GameObject_Mouse", CMouse::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* Prototype_GameObject_Camera */
	if (FAILED(g_pGameInstance->Add_Prototype(L"Prototype_GameObject_Camera", CDynamic_Camera::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* Prototype_GameObject_StaticMesh */
	if (FAILED(g_pGameInstance->Add_Prototype(L"Prototype_GameObject_StaticMesh", CStatic_Mesh::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	m_isFinished = TRUE;

	return S_OK;
}

HRESULT CChanger::Loading_ForCam_Tool(void)
{

	m_isFinished = TRUE;
	return S_OK;
}

CChanger* CChanger::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, TAB_ID _eNextTab)
{
	CChanger* pInstance = new CChanger(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->Ready_Changer(_eNextTab)))
	{
		MSGBOX("Failed to Creating CChanger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChanger::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CS);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
