#pragma once
#ifndef __CINEMA4_1_H__
#define __CINEMA4_1_H__
#include "Scenematic.h"

BEGIN(Client)
class CCinemaCam;
class CCinemaActor;
class CCinema4_1 final : public CScenematic
{
private:
	explicit CCinema4_1();
	explicit CCinema4_1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCinema4_1() = default;
public:
	virtual HRESULT NativeContruct(_uint iSceneID);
	virtual _int Tick(_double dDeltaTime);
	virtual _int LateTick(_double dDeltaTime);
public:
	virtual void Set_Active(_bool bCheck);
private:
	HRESULT Ready_Components();
public:
	static CCinema4_1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID);
private:
	virtual void Free() override;
private:
	CCinemaCam* m_pCam;
	CCinemaActor* m_pSilvermane;
	CCinemaActor* m_pBoss;
};
END
#endif