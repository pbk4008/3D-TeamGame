#pragma once

#include "1H_SwordAttack.h"

class C1H_SwordAttackNormalR2_Loop final : public C1H_SwordAttack
{
private:
	explicit C1H_SwordAttackNormalR2_Loop(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordAttackNormalR2_Loop() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int Input(const _double& _dDeltaTime) override;

private:
	_float m_fHoldTime = 0.f;

public:
	static C1H_SwordAttackNormalR2_Loop* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

