#pragma once

#include "Monster_FSM.h"

BEGIN(Client)
class CShooter_Attack final : public CMonster_FSM
{
private:
	explicit CShooter_Attack(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShooter_Attack() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int	Tick(const _double& _dDeltaTime);
	virtual _int	LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	static CShooter_Attack* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void			Free() override;
};

END

