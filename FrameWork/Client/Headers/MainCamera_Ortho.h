#pragma once
#ifndef __MainCamera_Ortho_H__
#define __MainCamera_Ortho_H__

#include "GameObject.h"
BEGIN(Client)
class CMainCamera_Ortho final : public CGameObject
{
private:
	explicit CMainCamera_Ortho();
	explicit CMainCamera_Ortho(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMainCamera_Ortho(const CMainCamera_Ortho& rhs);
	virtual ~CMainCamera_Ortho() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg) override;
	virtual _int Tick(_double fDeltaTime) override;
	virtual _int LateTick(_double fDeltaTime) override;
	virtual HRESULT Render() override;
public:
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg) override;
	static CMainCamera_Ortho* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	HRESULT Ready_GameObject(void* pArg);
private:
	virtual void Free() override;
private:
	CCamera* m_pCamera;
};
END
#endif