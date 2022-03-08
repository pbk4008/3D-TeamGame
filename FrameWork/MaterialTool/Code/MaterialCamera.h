#pragma once
#ifndef __MATERIALCAMERA_H__
#define  __MATERIALCAMERA_H__
#include "GameObject.h"

BEGIN(Engine)
class CCamera;
END
BEGIN(Tool)
class CMaterialCamera final : public CGameObject
{
private:
	explicit CMaterialCamera();
	explicit CMaterialCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMaterialCamera(const CMaterialCamera& rhs);
	virtual ~CMaterialCamera() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double dDeltaTime);
public:
	static CMaterialCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
private:
	virtual void Free() override;
private:
	CCamera* m_pCamera;
};
END
#endif