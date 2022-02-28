#pragma once

#ifndef MFCCamera_Proj_h__
#define MFCCamera_Proj_h__

#include "GameObject.h"
#include "UI_Tool_Define.h"

BEGIN(Engine)
class CCamera;
END

class CMFCCamera_Proj final : public CGameObject
{
private:
	explicit CMFCCamera_Proj(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMFCCamera_Proj(const CMFCCamera_Proj& rhs);
	virtual ~CMFCCamera_Proj() = default;


public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CMFCCamera_Proj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	CCamera* m_pCameraCom = nullptr;
};

#endif // MFCCamera_Proj_h__

