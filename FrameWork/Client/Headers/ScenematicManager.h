#pragma once
#ifndef __SCENEMATICE_MANAGER_H__
#define __SCENEMATICE_MANAGER_H__

#include "SingleTon.h"
BEGIN(Client)
class CScenematic;
class CScenematicManager final : public CSingleTon<CScenematicManager>
{
	friend CSingleTon;
private:
	NO_COPY(CScenematicManager);
	explicit CScenematicManager();
	virtual ~CScenematicManager() = default;
public:
	_uint Tick(_double TimeDelta);
	_uint LateTick(_double TimeDelta);
public:
	HRESULT Add_Scenema(CScenematic* pScenema);
	HRESULT Active_Scenema(_uint iScenemaIndex);
	HRESULT Change_Cinema(_uint iCinemaIndex);
public:
	_bool Get_EventCinema(_uint iCinemaIndex);
public:
	void ResetCinema();
private:
	virtual void Free() override;
private:
	vector<CScenematic*> m_vecScenema;
	CScenematic* m_pCurCinema;
	_uint m_iCurCinema;
	_bool m_bCinemaEnd;
};
END
#endif