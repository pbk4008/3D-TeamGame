#include "..\public\Level.h"
#include "Component_Manager.h"


CLevel::CLevel()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{
}

CLevel::CLevel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}
HRESULT CLevel::NativeConstruct()
{
	return S_OK;
}

_int CLevel::Tick(_double TimeDelta)
{
	return _int();
}

_int CLevel::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CLevel::Render()
{
	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
