#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_Jog abstract : public CState_Silvermane
{
protected:
	explicit CSilvermane_Jog(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CSilvermane_Jog() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

	virtual void OnTriggerEnter(CCollision& collision);

protected:
	virtual _int Input(const _double& _dDeltaTime);

public:
	virtual void Free() override;
};

END