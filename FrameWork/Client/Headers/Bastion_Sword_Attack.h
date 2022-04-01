#pragma once
#ifndef __BASTION_SWORD_ATTACK_H__
#define __BASTION_SWORD_ATTACK_H__

#include "Monster_FSM.h"

BEGIN(Client)

class CBastion_Sword_Attack final : public CMonster_FSM
{
private:
	enum class ATTACK_TYPE {ATTACK_SINGLE, ATTACK_DOUBLE, ATTACK_JUMP, ATTACK_END};
private:
	explicit CBastion_Sword_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CBastion_Sword_Attack() = default;
public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);
	virtual HRESULT ExitState(void* _pArg);

private:
	void Play_Sound(void);

public:
	static CBastion_Sword_Attack* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);

private:
	_uint m_iSingleAtt = 113;
	_uint m_iDoubleAtt = 83;
	_uint m_iJumpAtt = 68;

	_bool m_bSingle = false;
	_bool m_bDouble = false;
	_bool m_bJump = false;

	_float m_fDoubleDelay = 5.f;
private:
	virtual void Free() override;
private:
	ATTACK_TYPE m_eAttackType;
};
END
#endif