#pragma once

#include "Silvermane_Sprint.h"

BEGIN(Client)

class CSilvermane_SprintFwd final : public CSilvermane_Sprint
{
private:
	explicit CSilvermane_SprintFwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CSilvermane_SprintFwd() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int Input(const _double& _dDeltaTime) override;

public:
	static CSilvermane_SprintFwd* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END