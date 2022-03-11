#pragma once

#include "Silvermane_Jog.h"

BEGIN(Client)

class CSilvermane_JogFwdPivot180 final : public CSilvermane_Jog
{
protected:
	explicit CSilvermane_JogFwdPivot180(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane_JogFwdPivot180(const CSilvermane_JogFwdPivot180& _rhs);
	virtual ~CSilvermane_JogFwdPivot180() = default;

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
	static CSilvermane_JogFwdPivot180* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END