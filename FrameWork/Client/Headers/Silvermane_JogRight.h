#pragma once

#include "Silvermane_Jog.h"

BEGIN(Client)

class CSilvermane_JogRight final : public CSilvermane_Jog
{
protected:
	explicit CSilvermane_JogRight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane_JogRight(const CSilvermane_JogRight& _rhs);
	virtual ~CSilvermane_JogRight() = default;

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
	static CSilvermane_JogRight* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END