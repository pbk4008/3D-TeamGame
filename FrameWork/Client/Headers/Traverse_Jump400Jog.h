#pragma once

#include "State_Silvermane.h"

class CTraverse_Jump400Jog : public CState_Silvermane
{
private:
	explicit CTraverse_Jump400Jog(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CTraverse_Jump400Jog() = default;

public:
	HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double& _dDeltaTime) override;

private:
	_bool m_isJumpEnd = false;

	_float m_fMoveSpeed = 0.f;
	_float3 m_vTargetPos = { 0.f, 0.f, 0.f };
	_float3 m_vDir = { 0.f, 0.f, 0.f };


public:
	static CTraverse_Jump400Jog* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

