#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_Idle final : public CState_Silvermane
{
protected:
	explicit CSilvermane_Idle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane_Idle(const CSilvermane_Idle& _rhs);
	virtual ~CSilvermane_Idle() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

	virtual void OnTriggerEnter(CCollision& collision);

public:
	virtual _int Input(const _double & _dDeltaTime) override;

private:
	_float m_fHoldTime = 0.f;

public:
	static CSilvermane_Idle* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END