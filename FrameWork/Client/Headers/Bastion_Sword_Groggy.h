#pragma once
#ifndef __BASTION_SWORD_GROGGY_H__
#define __BASTION_SWORD_GROGGY_H__

#include "Monster_FSM.h"
BEGIN(Client)

class CBastion_Sword_Groggy final : public CMonster_FSM
{
private:
	explicit CBastion_Sword_Groggy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CBastion_Sword_Groggy() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();
public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
private:
	void Stun(_double dDeltaTime);
public:
	static CBastion_Sword_Groggy* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
private:
	_bool m_bStunStart;
	_float m_fAccStunTime;
};
END
#endif