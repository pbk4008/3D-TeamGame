#pragma once
#ifndef __BASTION_SWORD_CHASE_H__
#define __BASTION_SWORD_CHASE_H__

#include "Monster_FSM.h"

BEGIN(Client)

class CBastion_Sword_Chase final : public CMonster_FSM
{
private:
	explicit CBastion_Sword_Chase(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CBastion_Sword_Chase() = default;
public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();
public:
	//상태 변경시 호출하는 이벤트(이 상태로 바꿀때)
	virtual HRESULT EnterState();
	//상태 변경시 호출하는 이벤트(다른 상태로 바뀔 때)
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState(void* pArg);
public:
	static CBastion_Sword_Chase* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
private:
	virtual void Free() override;
private:
	_float m_fAngle;
};
END
#endif