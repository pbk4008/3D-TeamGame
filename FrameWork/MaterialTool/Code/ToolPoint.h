#pragma once
#ifndef __TOOL_POINT_H__
#include "Base.h"
BEGIN(Engine)
class CTransform;
class CCollider;
class CRenderer;
END
BEGIN(Tool)
class CToolPoint final : public CBase
{
private:
	explicit CToolPoint();
	explicit CToolPoint(ID3D11Device* pDevcie, ID3D11DeviceContext* pDeviceContext);
	explicit CToolPoint(const CToolPoint& rhs);
	virtual ~CToolPoint() = default;
public:
	HRESULT Init_Point(_fvector vPos);
	_int Update(_float fDeltaTime);
	HRESULT Render();
	_fvector getPos();
	_bool Collision_Point(_fvector vRayPos, _fvector vRayDir);
	void Change_Pos(_fvector vPos);
public:
	static CToolPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _fvector vPos);
	CToolPoint* Clone();
public:
	void Click_Point();
	void Reset_Point();
private:
	virtual void Free();
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	CTransform* m_pTransform;
	CCollider* m_pCollider;
};
END
#endif // !__TOOL_POINT_H__
