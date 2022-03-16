#pragma once

#include "Shield_Walk.h"

BEGIN(Client)

class CShield_WalkBwdStop final : public CShield_Walk
{
protected:
	explicit CShield_WalkBwdStop(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CShield_WalkBwdStop(const CShield_WalkBwdStop& _rhs);
	virtual ~CShield_WalkBwdStop() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual _int KeyCheck(const _double & _dDeltaTime) override;

public:
	static CShield_WalkBwdStop* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END