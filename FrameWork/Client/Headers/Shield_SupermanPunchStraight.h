#pragma once

#include "State_Silvermane.h"

BEGIN(Engine)

class CShield_SupermanPunchStraight final : public CState_Silvermane
{
private:
	explicit CShield_SupermanPunchStraight(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShield_SupermanPunchStraight() = default;

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
	static CShield_SupermanPunchStraight* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END