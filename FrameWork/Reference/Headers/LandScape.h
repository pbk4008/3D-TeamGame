#pragma once
#ifndef __LANDSCAPE_H__
#define __LANDSCAPE_H__
#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CLandScape abstract : public CGameObject
{
protected:
	explicit CLandScape();
	explicit CLandScape(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CLandScape(const CLandScape& rhs);
	virtual ~CLandScape() = default;
protected:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
protected:
	virtual void Free() override;
};
END
#endif