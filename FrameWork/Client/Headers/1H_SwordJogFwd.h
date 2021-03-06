#pragma once

#include "1H_SwordJog.h"

BEGIN(Client)

class C1H_SwordJogFwd final : public C1H_SwordJog
{
protected:
	explicit C1H_SwordJogFwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit C1H_SwordJogFwd(const C1H_SwordJogFwd& _rhs);
	virtual ~C1H_SwordJogFwd() = default;

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
	static C1H_SwordJogFwd* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END