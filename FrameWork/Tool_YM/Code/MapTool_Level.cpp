#include "pch.h"
#include "Tool_Defines.h"
#include "MapTool_Level.h"
#include "Tab_Changer.h"
#include "GameInstance.h"

CMapTool_Level::CMapTool_Level(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CLevel(_pDevice, _pDeviceContext)
{
}

HRESULT CMapTool_Level::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Plane(L"Layer_Plane")))  
		return E_FAIL;  

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	return S_OK;
}

_int CMapTool_Level::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

HRESULT CMapTool_Level::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;  
}

HRESULT CMapTool_Level::Ready_Layer_Plane(const wstring& _pLayerTag)
{
	if(FAILED(g_pGameInstance->Add_GameObjectToLayer(TAB_MAP, _pLayerTag, L"Prototype_GameObject_Plane")))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapTool_Level::Ready_Layer_Camera(const wstring& _pLayerTag)
{
	if (FAILED(g_pGameInstance->Add_GameObjectToLayer(TAB_MAP, _pLayerTag, L"Prototype_GameObject_Camera")))
		return E_FAIL;

	return S_OK;
}

CMapTool_Level* CMapTool_Level::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CMapTool_Level* pInstance = new CMapTool_Level(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Createing CMapTool_Level");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMapTool_Level::Free(void)
{
	__super::Free();
}
