#pragma once
#ifndef __TOOL_MOUSE_H__
#define __TOOL_MOUSE_H__
#include "Base.h"

BEGIN(Tool)
class CToolObject;
class CToolMouse final : public CBase 
{
private:
	explicit CToolMouse();
	explicit CToolMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CToolMouse() = default;
public:
	HRESULT Init_Mouse();
	_uint Update(_float fDeltaTime);
	_fvector UI_Picking();
	_fvector Terrain_Picking(void* pVertices, _fmatrix matWorld,_uint iVtxX, _uint iVtxZ, _int& iHitIndex);
	_int Click_Object(vector<CToolObject*>* vecObjects);
public:
	static CToolMouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free();
public:
	_fvector getRayPos() { return XMLoadFloat3(&m_vRayPos); }
	_fvector getRayDir() { return XMLoadFloat3(&m_vRayDir); }
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
private:
	_float2 m_vMousePos;
	_float3 m_vRayPos;
	_float3 m_vRayDir;
};
END
#endif