#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_JogLeftStart final : public CState_Silvermane
{
protected:
	explicit CSilvermane_JogLeftStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane_JogLeftStart(const CSilvermane_JogLeftStart& _rhs);
	virtual ~CSilvermane_JogLeftStart() = default;

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
	static CSilvermane_JogLeftStart* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END