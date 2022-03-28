#pragma once

#include "Level.h"

class CTestScene_JS final : public CLevel
{
private:
	explicit CTestScene_JS();
	explicit CTestScene_JS(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CTestScene_JS() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();

private:

	HRESULT Ready_Light();
	HRESULT Ready_Gameobject();

public:
	static CTestScene_JS* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);

private:
	virtual void Free() override;
};

