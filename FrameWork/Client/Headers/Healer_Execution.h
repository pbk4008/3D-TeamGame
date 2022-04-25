#pragma once

#include "Bastion_Healer_State.h"

BEGIN(Client)

class CHealer_Execution : public CBastion_Healer_State
{
private:
	explicit CHealer_Execution(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CHealer_Execution() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;

public:
	static CHealer_Execution* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END