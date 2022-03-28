#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class C2H_HammerIdle final : public CState_Silvermane
{
protected:
	explicit C2H_HammerIdle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit C2H_HammerIdle(const C2H_HammerIdle& _rhs);
	virtual ~C2H_HammerIdle() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int Input(const _double & _dDeltaTime) override;

private:
	_float m_fHoldTime = 0.f;

public:
	static C2H_HammerIdle* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END