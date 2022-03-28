#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class C1H_Dash abstract : public CState_Silvermane
{
protected:
	explicit C1H_Dash(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_Dash() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState() override;
	virtual HRESULT ExitState() override;

protected:
	virtual _int Input(const _double& _dDeltaTime);

public:
	virtual void Free() override;
};

END