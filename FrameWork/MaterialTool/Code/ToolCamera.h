#pragma once
#ifndef __TOOLMAINCAMERA_H__
#define __TOOLMAINCAMERA_H__
#include "GameObject.h"
BEGIN(Engine)
class CCamera;
END
BEGIN(Tool)
class CToolCamera final : public CGameObject
{
private:
	explicit CToolCamera();
	explicit CToolCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CToolCamera(const CToolCamera& rhs);
	virtual ~CToolCamera()=default;
public:
	virtual HRESULT Init_GameObject(COMINDEX* pIndex);
	virtual HRESULT Init_Clone(void* pArg);
	virtual _int Update_GameObject(_float fDeltaTime);
	virtual HRESULT Render_GameObject();
private:
	void CameraMove(_float fDeltaTime);
public:
	static CToolCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, COMINDEX* pIndex);
	virtual CGameObject* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	CCamera* m_pCamera;
};
END
#endif