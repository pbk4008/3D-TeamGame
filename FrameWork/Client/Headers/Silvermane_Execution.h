#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_Execution : public CState_Silvermane
{
protected:
	explicit CSilvermane_Execution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CSilvermane_Execution() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState() override;
	virtual HRESULT ExitState() override;

	virtual void Hit(const ATTACKDESC & _tAttackDesc) override;

protected:
	virtual _int Input(const _double & _dDeltaTime);

public:
	static CSilvermane_Execution* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END