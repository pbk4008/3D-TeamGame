#pragma once
#ifndef __CIRCLE_FLOOR_H__
#define __CIRCLE_FLOOR_H__

#include "LandScape.h"
BEGIN(Engine)
class CModel;
END
BEGIN(Client)
class CCircle_Floor final : public CLandScape
{
public:
	explicit CCircle_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCircle_Floor(const CCircle_Floor& rhs);
	virtual ~CCircle_Floor() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render()override;
	virtual HRESULT	Render_Shadow() override;
private:
	HRESULT Ready_Component();
public:
	static CCircle_Floor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CCircle_Floor* Clone(const _uint _iSceneID, void* pArg);
private:
	virtual void Free() override;
private:
	const LIGHTDESC* m_LightDesc = nullptr;
	_ulong				m_Nummeshcontainer = 0;
	CModel* m_pModel;
};
END
#endif