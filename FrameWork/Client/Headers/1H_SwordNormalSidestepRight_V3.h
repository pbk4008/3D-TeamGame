#pragma once

#include "State_Silvermane.h"

BEGIN(Engine)

class C1H_SwordNormalSidestepRight_V3 final : public CState_Silvermane
{
private:
	explicit C1H_SwordNormalSidestepRight_V3(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordNormalSidestepRight_V3() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double& _dDeltaTime) override;

private:
	EDir m_eDir = EDir::Forward;

public:
	static C1H_SwordNormalSidestepRight_V3* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END