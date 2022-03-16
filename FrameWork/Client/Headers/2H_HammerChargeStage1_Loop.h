#pragma once

#include "2H_HammerAttack.h"

class C2H_HammerChargeStage1_Loop final : public C2H_HammerAttack
{
private:
	explicit C2H_HammerChargeStage1_Loop(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C2H_HammerChargeStage1_Loop() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double& _dDeltaTime) override;

private:
	_float m_fHoldTime = 0.f;

public:
	static C2H_HammerChargeStage1_Loop* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

