#pragma once
#ifndef __CINEMA1_1_H__
#define __CINEMA1_1_H__
#include "Scenematic.h"

BEGIN(Client)
class CCinemaCam;
class CCinemaActor;
class CCinemaWeapon;
class CWall;
class CSubtitles;
class CCinema1_1 final : public CScenematic
{
private:
	explicit CCinema1_1();
	explicit CCinema1_1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCinema1_1() = default;
public:
	virtual HRESULT NativeContruct(_uint iSceneID);
	virtual _int Tick(_double dDeltaTime);
	virtual _int LateTick(_double dDeltaTime);
public:
	virtual void Set_Active(_bool bCheck);
private:
	HRESULT Ready_Components();
public:
	static CCinema1_1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID);
private:
	virtual void Free() override;
private:
	CCinemaCam* m_pCam;
	CCinemaActor* m_pGrayeHwak;
	CCinemaActor* m_pPhoenix;
	_bool m_bActorAnimOn;
private:
	CCinemaActor* m_pScree0;
	CCinemaActor* m_pScree1;
	CCinemaActor* m_pScree2;
	CCinemaActor* m_pScree3;
	CCinemaActor* m_pScree4;
private:
	CCinemaWeapon* m_pGrayHwakSpear;
private:
	CSubtitles* m_pSubTitlePhoenix;
	CSubtitles* m_pSubTitleGrayeHwak;
	_uint m_iSubTitleSequence=0;
};
END
#endif