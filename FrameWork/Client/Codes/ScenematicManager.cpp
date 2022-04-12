#include "pch.h"
#include"ScenematicManager.h"
#include "Scenematic.h"

CScenematicManager::CScenematicManager()
{
}

HRESULT CScenematicManager::Add_Scenema(CScenematic* pScenema)
{
	pScenema->Set_Active(false);
	m_vecScenema.emplace_back(pScenema);

	return S_OK;
}

HRESULT CScenematicManager::Active_Scenema(_uint iScenemaIndex, CScenematic** pOutCineam)
{
	_uint iSize = (_uint)m_vecScenema.size();
	if (iScenemaIndex < 0 || iScenemaIndex >= iSize)
	{
		*pOutCineam = nullptr;
		return E_FAIL;
	}

	m_vecScenema[iScenemaIndex]->Set_Active(true);
	*pOutCineam = m_vecScenema[iScenemaIndex];
	
	return S_OK;
}

void CScenematicManager::Free()
{
	for (auto& pScenema : m_vecScenema)
		Safe_Release(pScenema);

	m_vecScenema.clear();
}
