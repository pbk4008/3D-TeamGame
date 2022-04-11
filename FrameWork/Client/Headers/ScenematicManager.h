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
	HRESULT Add_Scenema(CScenematic* pScenema);
	HRESULT Active_Scenema(_uint iScenemaIndex, CScenematic** pOutCineam);
private:
	virtual void Free() override;
private:
	vector<CScenematic*> m_vecScenema;
};
END
#endif