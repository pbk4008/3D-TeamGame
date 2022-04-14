#include "pch.h"
#include "Loot_Equipment.h"

CLoot_Equipment::CLoot_Equipment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CHud(pDevice, pDeviceContext)
{
}

CLoot_Equipment::CLoot_Equipment(const CHud& rhs)
	:CHud(rhs)
{
}

HRESULT CLoot_Equipment::NativeConstruct_Prototype(void)
{
	if (FAILED(CHud::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoot_Equipment::NativeConstruct(const _uint iSceneID, void* pArg)
{
	if (FAILED(CHud::NativeConstruct(iSceneID, pArg)))
		return E_FAIL;

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

_int CLoot_Equipment::Tick(_double dDeltaTime)
{
	return _int();
}

_int CLoot_Equipment::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CLoot_Equipment::Render()
{
	return S_OK;
}

void CLoot_Equipment::setActive(_bool bActive)
{
}

HRESULT CLoot_Equipment::Ready_Component(void)
{
	return S_OK;
}

HRESULT CLoot_Equipment::Ready_UIObject(void)
{
	return S_OK;
}

CLoot_Equipment* CLoot_Equipment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	return nullptr;
}

CGameObject* CLoot_Equipment::Clone(const _uint iSceneID, void* pArg)
{
	return nullptr;
}

void CLoot_Equipment::Free()
{
}
