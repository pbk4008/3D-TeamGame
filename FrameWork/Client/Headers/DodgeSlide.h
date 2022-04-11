#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CDodgeSlide : public CState_Silvermane
{
private:
	explicit CDodgeSlide(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CDodgeSlide() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState(void* _pArg = nullptr) override;
	virtual HRESULT ExitState() override;

private:
	virtual _int Input(const _double& _dDeltaTime);
	const _int Reset();
	const _int ToDash();

public:
	static CDodgeSlide* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END