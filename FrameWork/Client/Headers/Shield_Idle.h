#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class Cshield_Idle final : public CState_Silvermane
{
protected:
	explicit Cshield_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~Cshield_Idle() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double & _dDeltaTime) override;

private:
	_float m_fHoldTime = 0.f;

public:
	static Cshield_Idle* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END