#pragma once

#include "2H_HammerAttack.h"

BEGIN(Engine)

class C2H_HammerAttackSprintR1 final : public C2H_HammerAttack
{
private:
	explicit C2H_HammerAttackSprintR1(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C2H_HammerAttackSprintR1() = default;

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
	_bool m_isShake3 = false;

public:
	static C2H_HammerAttackSprintR1* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END