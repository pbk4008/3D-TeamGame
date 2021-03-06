#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_Sprint abstract : public CState_Silvermane
{
protected:
	explicit CSilvermane_Sprint(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CSilvermane_Sprint() = default;

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
	_float m_fFootStepTime = 0.f;

public:
	virtual void Free() override;
};

END