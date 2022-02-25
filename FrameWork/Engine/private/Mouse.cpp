#include "Mouse.h"

CMouse::CMouse()
{
	ZeroMemory(&m_vRayPos, sizeof(_float3));
	ZeroMemory(&m_vRayDir, sizeof(_float3));
}

CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
	ZeroMemory(&m_vRayPos, sizeof(_float3));
	ZeroMemory(&m_vRayDir, sizeof(_float3));
}

HRESULT CMouse::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMouse::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CMouse::Tick(HWND hWnd, _double TimeDelta)
{
	POINT pt = {};

	GetCursorPos(&pt);
	ScreenToClient(hWnd,&pt);

	return _int(); 
}

_int CMouse::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CMouse::Render()
{
	return S_OK;
}

_fvector CMouse::Terrain_Picking(void* pVertices, _fmatrix matWorld, _uint iVtxX, _uint iVtxZ, _int& iHitIndex)
{
	return _fvector();
}
