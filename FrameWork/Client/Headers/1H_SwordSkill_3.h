#pragma once

#include "State_Silvermane.h"

BEGIN(Client)

class C1H_SwordSkill_3 : public CState_Silvermane
{
private:
	explicit C1H_SwordSkill_3(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~C1H_SwordSkill_3() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int Input(const _double& _dDeltaTime) override;

private:
	_bool m_isAttack = false;
	_bool m_isAttack2 = false;
	_float3 m_vDir{};

public:
	static C1H_SwordSkill_3* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END