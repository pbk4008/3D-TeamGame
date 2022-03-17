#pragma once

#include "Level.h"

class CTestScene_Sb final : public CLevel
{
private:
	explicit CTestScene_Sb();
	explicit CTestScene_Sb(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CTestScene_Sb() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Gameobject();

public:
	static CTestScene_Sb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

private:
	virtual void Free() override;
};