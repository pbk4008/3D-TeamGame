#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class C1H_SwordAttack abstract : public CState_Silvermane
{
protected:
	explicit C1H_SwordAttack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordAttack() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState() override;
	virtual HRESULT ExitState() override;

	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void Hit(const ATTACKDESC& _tAttackDesc) override;

protected:
	virtual _int Input(const _double& _dDeltaTime);

protected:
	_uint m_iAttackStartIndex = 0;
	_uint m_iAttackEndIndex = 0;

	_uint m_iTrailStartIndex = 0;
	_uint m_iTrailEndIndex = 0;

	_bool	m_btest = true;

	CCameraShake::SHAKEEVENT m_tShakeEvent;

public:
	virtual void Free() override;
};

END