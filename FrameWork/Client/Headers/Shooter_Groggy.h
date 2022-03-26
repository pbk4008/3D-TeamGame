#pragma once
#ifndef __SHOOTER_GROGGY_H__
#define __SHOOTER_GROGGY_H__

#include "Monster_FSM.h"
BEGIN(Client)

class CShooter_Groggy final : public CMonster_FSM
{
private:
	explicit CShooter_Groggy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShooter_Groggy() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState(void* pArg);
private:
	void Stun(_double dDeltaTime);
public:
	static CShooter_Groggy* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
private:
	_bool m_bStunStart;
	_float m_fAccStunTime;
};
END

#endif