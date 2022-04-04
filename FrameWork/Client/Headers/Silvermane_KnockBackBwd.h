#pragma once

#include "Silvermane_Hit.h"

BEGIN(Client)

class CSilvermane_KnockBackBwd : public CSilvermane_Hit
{
private:
	explicit CSilvermane_KnockBackBwd(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual ~CSilvermane_KnockBackBwd() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState() override;
	virtual HRESULT ExitState() override;

protected:
	virtual _int Input(const _double & _dDeltaTime);

public:
	static CSilvermane_KnockBackBwd* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END