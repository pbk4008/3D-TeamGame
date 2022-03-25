#pragma once
#ifndef __SHOOTER_HIT_H__
#define __SHOOTER_HIT_H__

#include "Monster_FSM.h"

BEGIN(Client)

class CShooter_Hit final : public CMonster_FSM
{
public:
	typedef struct tagHitData
	{
		_uint iHitType;
		_float fCurHp;
	}HITDATA;
private:
	explicit CShooter_Hit(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShooter_Hit() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* _pArg);
	virtual HRESULT ExitState(void* _pArg);

public:
	static CShooter_Hit* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
private:
	_float m_fCurHp;
};
END
#endif