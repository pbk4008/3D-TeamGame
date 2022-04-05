#pragma once

#include "Silvermane_Hit.h"

BEGIN(Client)

class C1H_Stagger : public CSilvermane_Hit
{
private:
	explicit C1H_Stagger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_Stagger() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState() override;
	virtual HRESULT EnterState(void* _pArg = nullptr) override;
	virtual HRESULT ExitState() override;

protected:
	virtual _int Input(const _double& _dDeltaTime);

public:
	static C1H_Stagger* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END