#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class CSilvermane_Death : public CState_Silvermane
{
private:
	explicit CSilvermane_Death(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CSilvermane_Death() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState(void* _pArg = nullptr) override;
	virtual HRESULT ExitState() override;

public:
	static CSilvermane_Death* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END