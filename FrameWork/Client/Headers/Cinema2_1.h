#pragma once
#ifndef __CINEMA2_1_H__
#define __CINEMA2_1_H__
#include "Scenematic.h"

BEGIN(Client)
class CCinemaCam;
class CCinemaActor;
class CCinemaWeapon;
class CSubtitles;
class CCinema2_1 final : public CScenematic
{
private:
	explicit CCinema2_1();
	explicit CCinema2_1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCinema2_1() = default;
public:
	virtual HRESULT NativeContruct(_uint iSceneID);
	virtual _int Tick(_double dDeltaTime);
	virtual _int LateTick(_double dDeltaTime);
public:
	virtual void Set_Active(_bool bCheck);
private:
	HRESULT Ready_Components();
public:
	static CCinema2_1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID);
private:
	virtual void Free() override;
private:
	CCinemaCam* m_pCam;
	CCinemaActor* m_pMidBoss;
	CCinemaActor* m_pSilvermane;
	_bool m_bActorAnimOn;
private:
	CCinemaWeapon* m_pMidWeapon;
private:
	CSubtitles* m_pSubTitleMidBoss;
	_uint m_iSubTitleSequence =0;
};
END
#endif