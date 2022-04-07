#pragma once

#include "Shield_Block.h"

BEGIN(Client)

class CShield_BlockSkidStrong : public CShield_Block
{
private:
	explicit CShield_BlockSkidStrong(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CShield_BlockSkidStrong() = default;

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
	static CShield_BlockSkidStrong* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END