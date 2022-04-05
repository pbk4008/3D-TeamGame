#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CShield_ParryStunback : public CState_Silvermane
{
private:
	explicit CShield_ParryStunback(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShield_ParryStunback() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int Input(const _double & _dDeltaTime) override;

public:
	static CShield_ParryStunback* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END