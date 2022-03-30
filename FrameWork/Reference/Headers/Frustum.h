#pragma once

#include "SingleTon.h"

BEGIN(Engine)

class CFrustum final : public CSingleTon<CFrustum>
{
	friend CSingleTon;
private:
	NO_COPY(CFrustum);
	explicit CFrustum();
	virtual ~CFrustum() = default;
public:
	HRESULT Ready_FrustumInProjSpace();
	HRESULT Transform_ToWorldSpace(const wstring& pCameraTag);
	HRESULT Transform_ToLocalSpace(_fmatrix WorldMatrixInverse);

	_bool isInWorld(_fvector vPosition, _float fRange = 0.f);
	_bool isInWorld(_float4* vPoints, _float fRange = 0.f);
	_bool isInLocal(_fvector vPosition, _float fRange = 0.f);

private:
	_float3			m_vPoint[8];	
	_float3			m_vPointInWorld[8];

	_float4			m_PlaneInWorld[6];	
	_float4			m_PlaneInLocal[6];

private:
	void Make_PlaneInWorld(_fvector* pPoints);
	void Make_PlaneInLocal(_fvector* pPoints);

public:
	virtual void Free() override;
};

END