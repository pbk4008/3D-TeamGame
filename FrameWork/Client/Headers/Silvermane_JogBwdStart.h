#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_JogBwdStart final : public CState_Silvermane
{
protected:
	explicit CSilvermane_JogBwdStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane_JogBwdStart(const CSilvermane_JogBwdStart& _rhs);
	virtual ~CSilvermane_JogBwdStart() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double & TimeDelta) override;

public:
	static CSilvermane_JogBwdStart* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END