#pragma once

#include "1H_SwordAttack.h"

BEGIN(Engine)

class C1H_SwordAttackNormalR1_02 final : public C1H_SwordAttack
{
private:
	explicit C1H_SwordAttackNormalR1_02(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordAttackNormalR1_02() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

private:
	_bool m_bEffectCheck = false;

public:
	virtual _int Input(const _double& _dDeltaTime) override;

public:
	static C1H_SwordAttackNormalR1_02* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END