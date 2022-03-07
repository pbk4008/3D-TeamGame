#include "pch.h"
#include "Material_Level.h"

CMaterial_Level::CMaterial_Level()
{
}

CMaterial_Level::CMaterial_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CMaterial_Level::NativeConstruct()
{
	return S_OK;
}

_uint CMaterial_Level::Tick(_float fDeltaTime)
{
	return _uint();
}

HRESULT CMaterial_Level::Render()
{
	return S_OK;
}

CMaterial_Level* CMaterial_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CMaterial_Level* pInstance = new CMaterial_Level(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CMaterial_Level Create Fail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMaterial_Level::Free()
{
	CLevel::Free();
}
