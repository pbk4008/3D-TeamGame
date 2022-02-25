#pragma once

#ifndef MFCCamera_h__
#define MFCCamera_h__

#include "GameObject.h"
#include "UI_Tool_Define.h"

BEGIN(Engine)
class CCamera;
END

class CMFCCamera final : public CGameObject
{
private:
	explicit CMFCCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMFCCamera(const CMFCCamera& rhs);
	virtual ~CMFCCamera() = default;


public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CMFCCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CCamera* m_pCameraCom = nullptr;
};

#endif // MFCCamera_h__

