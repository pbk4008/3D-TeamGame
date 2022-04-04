#pragma once

#include "Shield_Block.h"

class CShield_BlockEnd : public CShield_Block
{
protected:
	explicit CShield_BlockEnd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShield_BlockEnd() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double& _dDeltaTime);
	virtual _int LateTick(const _double& _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

	virtual void Block(const ATTACKDESC& _tAttackDesc);

public:
	virtual _int Input(const _double& _dDeltaTime) override;

public:
	static CShield_BlockEnd* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

