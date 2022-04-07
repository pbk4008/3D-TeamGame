#include "IResource.h"
#include "Graphic_Device.h"

IResource::IResource(void)
{
}

void IResource::NativeConstruct(std::wstring _fg)
{
	m_pDevice = CGraphic_Device::GetInstance()->Get_Device();
	m_pDeviceContext = CGraphic_Device::GetInstance()->Get_DeviceContext();
	m_bStatic = false;
}
