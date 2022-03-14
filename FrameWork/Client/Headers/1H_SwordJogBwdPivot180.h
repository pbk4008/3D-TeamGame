#pragma once

#include "1H_SwordJog.h"

BEGIN(Client)

class C1H_SwordJogBwdPivot180 final : public C1H_SwordJog
{
protected:
	explicit C1H_SwordJogBwdPivot180(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit C1H_SwordJogBwdPivot180(const C1H_SwordJogBwdPivot180& _rhs);
	virtual ~C1H_SwordJogBwdPivot180() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double & _dDeltaTime) override;

public:
	static C1H_SwordJogBwdPivot180* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END