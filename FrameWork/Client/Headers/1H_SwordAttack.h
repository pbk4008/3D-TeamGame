#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class C1H_SwordAttack abstract : public CState_Silvermane
{
protected:
	explicit C1H_SwordAttack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordAttack() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

protected:
	virtual _int KeyCheck(const _double& _dDeltaTime);

public:
	virtual void Free() override;
};

END