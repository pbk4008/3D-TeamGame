#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_JogLeft final : public CState_Silvermane
{
protected:
	explicit CSilvermane_JogLeft(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane_JogLeft(const CSilvermane_JogLeft& _rhs);
	virtual ~CSilvermane_JogLeft() = default;

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
	static CSilvermane_JogLeft* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END