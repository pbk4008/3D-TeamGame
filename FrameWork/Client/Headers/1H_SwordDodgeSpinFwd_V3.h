#pragma once

#include "1H_Dash.h"

BEGIN(Engine)

class C1H_SwordDodgeSpinFwd_V3 final : public C1H_Dash
{
private:
	explicit C1H_SwordDodgeSpinFwd_V3(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordDodgeSpinFwd_V3() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int Input(const _double& _dDeltaTime) override;

public:
	static C1H_SwordDodgeSpinFwd_V3* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END