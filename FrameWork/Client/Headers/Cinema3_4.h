#pragma once
#ifndef __CINEMA3_4_H__
#define __CINEMA3_4_H__
#include "Scenematic.h"

BEGIN(Client)
class CCinemaCam;
class CCinemaActor;
class CCinemaWeapon;
class CCinema3_4 final : public CScenematic
{
private:
	explicit CCinema3_4();
	explicit CCinema3_4(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCinema3_4() = default;
public:
	virtual HRESULT NativeContruct(_uint iSceneID);
	virtual _int Tick(_double dDeltaTime);
	virtual _int LateTick(_double dDeltaTime);
public:
	virtual void Set_Active(_bool bCheck);
private:
	HRESULT Ready_Components();
public:
	static CCinema3_4* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID);
private:
	virtual void Free() override;
private:
	CCinemaCam* m_pCam;
	CCinemaActor* m_pSilvermane;
	CCinemaActor* m_pMidBoss;
private:
	CCinemaWeapon* m_pMidWeapon;
};
END
#endif