#pragma once
#ifndef __SHOOTER_WALK_H__
#define __SHOOTER_WALK_H__

#include "Monster_FSM.h"

BEGIN(Client)

class CShooter_Walk final : public CMonster_FSM
{
private:
	explicit CShooter_Walk(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShooter_Walk() = default;

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
public:
	static CShooter_Walk* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
};
END
#endif