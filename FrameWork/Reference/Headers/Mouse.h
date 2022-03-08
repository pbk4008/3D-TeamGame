#pragma once
#ifndef  __MOUSE_H__
#define __MOUSE_H__
#include "GameObject.h"

BEGIN(Engine)
class CUI;
class ENGINE_DLL CMouse : public CGameObject
{
private:
	explicit CMouse();
	explicit CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CMouse() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(HWND hWnd, _double TimeDelta);
public:
	void RayUpdate(const wstring& pCamTag);
	CUI* getCheckUI(list<CGameObject*>* pObjList);
	_fvector Terrain_Picking(void* pVertices, _fmatrix matWorld, _uint iVtxX, _uint iVtxZ, _int & iHitIndex);
public:
	_fvector getRayPos() { return XMLoadFloat3(&m_vRayPos); }
	_fvector getRayDir() { return XMLoadFloat3(&m_vRayDir); }
public:
	static CMouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
private:
	POINT m_tMousePos;
	_float3 m_vRayPos;
	_float3 m_vRayDir;
};
END
#endif // ! __MOUSE_H__
