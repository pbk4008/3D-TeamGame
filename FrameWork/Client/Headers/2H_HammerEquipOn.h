#pragma once

#include "State_Silvermane.h"

class C2H_HammerEquipOn final : public CState_Silvermane
{
private:
	explicit C2H_HammerEquipOn(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C2H_HammerEquipOn() = default;

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
	static C2H_HammerEquipOn* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

