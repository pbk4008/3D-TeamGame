#pragma once
#ifndef __BASTION_SWORD_DEATH_H__
#define __BASTION_SWORD_DEATH_H__

#include "Monster_FSM.h"

BEGIN(Client)

class CBastion_Sword_Death final : public CMonster_FSM
{
private:
	explicit CBastion_Sword_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CBastion_Sword_Death() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

private:
	CCameraShake::SHAKEEVENT m_tShakeEvent;

public:
	static CBastion_Sword_Death* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
};
END
#endif