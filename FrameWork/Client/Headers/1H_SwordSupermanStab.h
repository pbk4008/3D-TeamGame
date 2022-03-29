#pragma once

#include "1H_SwordAttack.h"

BEGIN(Engine)

class C1H_SwordSupermanStab final : public C1H_SwordAttack
{
private:
	explicit C1H_SwordSupermanStab(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordSupermanStab() = default;

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
	CCameraShake::SHAKEEVENT m_tShakeEvent2;
	_bool m_isShake2 = false;

public:
	static C1H_SwordSupermanStab* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END