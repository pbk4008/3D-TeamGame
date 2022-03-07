#pragma once
#ifndef __MATERIAL_LEVEL_H__
#define __MATERIAL_LEVEL_H__

#include "Level.h"
BEGIN(Tool)
class CMaterial_Level final : public CLevel
{
private:
	explicit CMaterial_Level();
	explicit CMaterial_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CMaterial_Level() = default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual _uint Tick(_float fDeltaTime);
	virtual HRESULT Render();
public:
	static CMaterial_Level* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
};
END
#endif