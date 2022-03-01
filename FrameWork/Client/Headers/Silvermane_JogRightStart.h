#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_JogRightStart final : public CState_Silvermane
{
protected:
	explicit CSilvermane_JogRightStart(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane_JogRightStart(const CSilvermane_JogRightStart& _rhs);
	virtual ~CSilvermane_JogRightStart() = default;

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
	static CSilvermane_JogRightStart* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END