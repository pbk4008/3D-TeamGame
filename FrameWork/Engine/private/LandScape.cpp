#include "LandScape.h"

CLandScape::CLandScape()
{
}

CLandScape::CLandScape(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CLandScape::CLandScape(const CLandScape& rhs)
	: CGameObject(rhs)
{
}

HRESULT CLandScape::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLandScape::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CLandScape::Tick(_double TimeDelta)
{
	return _int();
}

_int CLandScape::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CLandScape::Render()
{
	return S_OK;
}

void CLandScape::Free()
{
	CGameObject::Free();
}
