#pragma once
#ifndef __STAGE1_H__
#define __STAGE1_H__

#include "Level.h"
#include "Defines.h"

BEGIN(Example)
class CStage1 final : public CLevel
{
private:
	explicit CStage1();
	explicit CStage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CStage1() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT	Ready_LightDesc();
	HRESULT Ready_Gameobject();

public:
	static CStage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
};
END

#endif