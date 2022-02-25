#pragma once
#ifndef  __MOUSE_H__
#define __MOUSE_H__
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CMouse abstract : public CGameObject
{
protected:
	explicit CMouse();
	explicit CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CMouse() = default;
protected:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(HWND hWnd, _double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
protected:
	_fvector Terrain_Picking(void* pVertices, _fmatrix matWorld, _uint iVtxX, _uint iVtxZ, _int& iHitIndex);
protected:
	_fvector getRayPos() { return XMLoadFloat3(&m_vRayPos); }
	_fvector getRayDir() { return XMLoadFloat3(&m_vRayDir); }
protected:
	_float3 m_vRayPos;
	_float3 m_vRayDir;
};
END
#endif // ! __MOUSE_H__
