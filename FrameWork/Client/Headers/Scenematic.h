#pragma once
#ifndef __SCENEMATIC_H__
#define __SCENEMATIC_H__

#include "Base.h"
BEGIN(Engine)
class CComponent;
END
BEGIN(Client)
class CScenematic abstract : public CBase 
{
protected:
	explicit CScenematic();
	explicit CScenematic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CScenematic() = default;
public:
	virtual HRESULT NativeContruct();
	virtual _int Tick(_double dDeltaTime);
	virtual _int LateTick(_double dDeltaTime);
public:
	virtual void Set_Active(_bool bCheck);
	virtual void End_Cinema();
public:
	_bool Get_Active() { return m_bActive; }
protected:
	HRESULT Set_UpComponents(CComponent* pComponent);
	HRESULT Ready_Actor(class CCinemaActor** pOut, _uint iActorTag);
protected:
	virtual void Free() override;
protected:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	_bool m_bActive;
	vector<CComponent*> m_vecScenemaComponents;
	_bool m_bCinemaEnd;
};
END
#endif