#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CShield_Throw : public CState_Silvermane
{
private:
	explicit CShield_Throw(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShield_Throw() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

private:
	virtual _int Input(const _double& _dDeltaTime) override;
	void RaycastForThrow();

private:
	_bool m_isThrow = false;
	_bool m_isTrigger = false;
	_bool m_isRecive = false;

public:
	static CShield_Throw* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END