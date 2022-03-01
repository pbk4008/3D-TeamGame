#include "pch.h"
#include "Static.h"
#include "Tab_Changer.h"
#include "GameInstance.h"
#include "Plane.h"
#include "Observer.h"

CStatic_Level::CStatic_Level(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CLevel(_pDevice, _pDeviceContext)
{
}

HRESULT CStatic_Level::NativeConstruct(void)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	return S_OK;
}

_int CStatic_Level::Tick(_double _dTimeDelta)
{
	if (0 > __super::Tick(_dTimeDelta))
		return -1;

	if (GetKeyState(VK_SPACE) < 0)
	{
  		if (FAILED(g_pGameInstance->Open_Level(TAB_STATIC, CTab_Changer::Create(m_pDevice, m_pDeviceContext, TAB_MAP))))
			return -1;
	}

	return _int(0);
}

HRESULT CStatic_Level::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CStatic_Level* CStatic_Level::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CStatic_Level* pInstance = new CStatic_Level(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Createing CLevel Static");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStatic_Level::Free(void)
{
	__super::Free();
}
