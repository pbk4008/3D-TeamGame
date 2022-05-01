#pragma once
#ifndef __CINEMA1_2_H__
#define __CINEMA1_2_H__
#include "Scenematic.h"

BEGIN(Client)
class CCinemaCam;
class CCinemaActor;
class CSubtitles;
class CCinema1_2 final : public CScenematic
{
private:
	explicit CCinema1_2();
	explicit CCinema1_2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCinema1_2() = default;
public:
	virtual HRESULT NativeContruct(_uint iSceneID);
	virtual _int Tick(_double dDeltaTime);
	virtual _int LateTick(_double dDeltaTime);
public:
	virtual void Set_Active(_bool bCheck);
private:
	HRESULT Ready_Components();
public:
	static CCinema1_2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneID);
private:
	virtual void Free() override;
private:
	CCinemaCam* m_pCam;
	CCinemaActor* m_pGrayeHwak;
	CCinemaActor* m_pPhoenix;
	CCinemaActor* m_pSilvermane;
	_bool m_bActorAnimOn;

private:
	CCinemaActor* m_pScree0;
	CCinemaActor* m_pScree1;
	CCinemaActor* m_pScree2;
	CCinemaActor* m_pScree3;
	CCinemaActor* m_pScree4;
private:
	CCinemaWeapon* m_pPhoenixWeapon;
	CCinemaWeapon* m_pGrayeHwakWeapon;
private:
	CSubtitles* m_pSubTitlePhoenix;
	CSubtitles* m_pSubTitleGrayHwak;
	CSubtitles* m_pSubTitleSilvermane;
private:
	_uint m_iSubTitleSequence = 0;
};
END
#endif