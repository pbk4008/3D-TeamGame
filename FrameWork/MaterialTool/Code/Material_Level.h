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
	virtual _int Tick(_double fDeltaTime);
	virtual HRESULT Render();
private:
	HRESULT Init_Camera();
	HRESULT Init_MaterialObject();
	HRESULT Init_Mouse();
public:
	static CMaterial_Level* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Free() override;
};
END
#endif