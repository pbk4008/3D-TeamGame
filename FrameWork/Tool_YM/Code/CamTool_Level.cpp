#include "pch.h"
#include "CamTool_Level.h"
#include "Tab_Changer.h"

CCamTool_Level::CCamTool_Level(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	:CLevel(_pDevice, _pDeviceContext)
{
}

HRESULT CCamTool_Level::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	return S_OK;
}

_int CCamTool_Level::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

HRESULT CCamTool_Level::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CCamTool_Level* CCamTool_Level::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	CCamTool_Level* pInstance = new CCamTool_Level(_pDevice, _pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Createing CCamTool_Level");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamTool_Level::Free(void)
{
	__super::Free();
}