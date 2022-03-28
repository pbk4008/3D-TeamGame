#pragma once

#include "2H_HammerJog.h"

BEGIN(Client)

class C2H_HammerJogBwd final : public C2H_HammerJog
{
protected:
	explicit C2H_HammerJogBwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit C2H_HammerJogBwd(const C2H_HammerJogBwd& _rhs);
	virtual ~C2H_HammerJogBwd() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int Input(const _double & _dDeltaTime) override;

public:
	static C2H_HammerJogBwd* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END