#pragma once
#ifndef __SCENEMATICE_MANAGER_H__
#define __SCENEMATICE_MANAGER_H__

#include "SingleTon.h"
BEGIN(Client)
class CScenematic;
class CSceneMaticManager final : public CSingleTon<CSceneMaticManager>
{
	friend CSingleTon;
private:
	NO_COPY(CSceneMaticManager);
	explicit CSceneMaticManager();
	virtual ~CSceneMaticManager() = default;
public:
	HRESULT Add_Scenema(CScenematic* pScenema);
	HRESULT Active_Scenema(_uint iScenemaIndex);
private:
	virtual void Free() override;
private:
	vector<CScenematic*> m_vecScenema;
};
END
#endif