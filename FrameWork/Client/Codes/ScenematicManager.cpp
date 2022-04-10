#include "pch.h"
#include"ScenematicManager.h"
#include "Scenematic.h"

HRESULT CScenematicManager::Add_Scenema(CScenematic* pScenema)
{
	pScenema->Set_Active(false);
	m_vecScenema.emplace_back(pScenema);

	return S_OK;
}

HRESULT CScenematicManager::Active_Scenema(_uint iScenemaIndex)
{
	_uint iSize = m_vecScenema.size();
	if (iScenemaIndex < 0 || iScenemaIndex >= iSize)
		return E_FAIL;

	m_vecScenema[iScenemaIndex]->Set_Active(true);

	return S_OK;
}

void CScenematicManager::Free()
{
	for (auto& pScenema : m_vecScenema)
		Safe_Release(pScenema);

	m_vecScenema.clear();
}
