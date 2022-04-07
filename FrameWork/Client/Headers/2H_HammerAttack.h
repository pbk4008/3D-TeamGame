#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class C2H_HammerAttack abstract : public CState_Silvermane
{
protected:
	explicit C2H_HammerAttack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C2H_HammerAttack() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState() override;
	virtual HRESULT ExitState() override;

	virtual void Hit(const ATTACKDESC& _tAttackDesc) override;

protected:
	virtual _int Input(const _double& _dDeltaTime);

protected:
	_uint m_iAttackStartIndex = 0;
	_uint m_iAttackEndIndex = 0;
	_int m_iShakeIndex = -1;
	
	_bool m_isShake2 = false;
	_bool m_isRangeAttack = false;

	CCameraShake::SHAKEEVENT m_tShakeEvent;
	CCameraShake::SHAKEEVENT m_tShakeEvent2;

public:
	virtual void Free() override;
};

END