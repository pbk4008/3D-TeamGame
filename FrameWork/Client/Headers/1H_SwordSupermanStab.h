#pragma once

#include "State_Silvermane.h"

BEGIN(Engine)

class C1H_SwordSupermanStab final : public CState_Silvermane
{
private:
	explicit C1H_SwordSupermanStab(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordSupermanStab() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double& _dDeltaTime) override;

public:
	static C1H_SwordSupermanStab* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END