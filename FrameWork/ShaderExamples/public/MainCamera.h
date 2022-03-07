#pragma once
#ifndef __MAINCAMERA_H__
#define __MAINCAMERA_H__

#include "GameObject.h"
#include "Defines.h"

BEGIN(Engine)
class CCamera;
END

BEGIN(Example)
class CMainCamera final : public CGameObject
{
private:
	explicit CMainCamera();
	explicit CMainCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMainCamera(const CMainCamera& rhs);
	virtual ~CMainCamera() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double fDeltaTime) override;
	virtual _int LateTick(_double fDeltaTime) override;
	virtual HRESULT Render() override;
public:
	virtual CGameObject* Clone(void* pArg) override;
	static CMainCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	HRESULT Ready_GameObject(void* pArg);
private:
	virtual void Free() override;
private:
	 CCamera* m_pCamera;
};
END
#endif