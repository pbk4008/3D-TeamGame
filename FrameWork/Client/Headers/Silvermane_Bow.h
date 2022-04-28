#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_Bow : public CState_Silvermane
{
private:
	explicit CSilvermane_Bow(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CSilvermane_Bow() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState() override;
	virtual HRESULT ExitState() override;

	virtual void Hit(const ATTACKDESC& _tAttackDesc) override;

private:
	_float4x4 m_matPrePivot{};

public:
	static CSilvermane_Bow* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END