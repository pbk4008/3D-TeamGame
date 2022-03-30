#pragma once
#ifndef __SHOOTER_DEATH_H__
#define __SHOOTER_DEATH_H__

#include "Monster_FSM.h"

BEGIN(Client)

class CShooter_Death final : public CMonster_FSM
{
private:
	explicit CShooter_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShooter_Death() = default;
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
	CCameraShake::SHAKEEVENT m_tShakeEvent;

public:
	static CShooter_Death* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
};
END
#endif