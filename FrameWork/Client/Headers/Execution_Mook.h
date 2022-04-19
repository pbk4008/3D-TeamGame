#pragma once

#include "Silvermane_Execution.h"

BEGIN(Client)

class CExecution_Mook : public CSilvermane_Execution
{
private:
	explicit CExecution_Mook(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CExecution_Mook() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState() override;
	virtual HRESULT ExitState() override;

public:
	static CExecution_Mook* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END