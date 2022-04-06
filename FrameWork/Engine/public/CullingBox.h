#pragma once
#ifndef __CULLINGBOX_H__
#define __CULLINGBOX_H__

#include "Component.h"
BEGIN(Engine)
class CGizmo;
class ENGINE_DLL CCullingBox final : public CComponent
{
private:
	explicit CCullingBox();
	explicit CCullingBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCullingBox(const CCullingBox& rhs);
	virtual ~CCullingBox() = default;
public:
	HRESULT NativeConstruct_Prototype();
	HRESULT NativeConstruct(void* pArg);
	HRESULT Render(const wstring& pCameraTag);
public:
	_uint Update_Matrix(_fmatrix matTransform);
	HRESULT CreateWithPoints(_fvector vMin, _fvector vMax);
	HRESULT Set_Length(_float fWidth, _float fHeight, _float fDepth);

public:
	_float4* Get_Points() { return m_pPoints; }
public:
	static CCullingBox* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual CComponent* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	CGizmo* m_pGizmo;
	_float4 m_pPoints[8];
	_float4 m_pCreatePoints[8];
};
END
#endif