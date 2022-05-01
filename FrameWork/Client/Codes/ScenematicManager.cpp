#include "pch.h"
#include"ScenematicManager.h"
#include "Scenematic.h"

CScenematicManager::CScenematicManager()
	:m_pCurCinema(nullptr)
	, m_iCurCinema(-1)
	, m_bCinemaEnd(false)
{
}

HRESULT CScenematicManager::Add_Scenema(CScenematic* pScenema)
{
	pScenema->Set_Active(false);
	m_vecScenema.emplace_back(pScenema);

	return S_OK;
}

HRESULT CScenematicManager::Active_Scenema(_uint iScenemaIndex)
{
	_uint iSize = (_uint)m_vecScenema.size();
	if (iScenemaIndex < 0 || iScenemaIndex >= iSize)
		return E_FAIL;

	m_vecScenema[iScenemaIndex]->Set_Active(true);
	m_pCurCinema = m_vecScenema[iScenemaIndex];
	m_iCurCinema = iScenemaIndex;
	m_bCinemaEnd = true;

	return S_OK;
}

HRESULT CScenematicManager::Change_Cinema(_uint iCinemaIndex)
{
	if (iCinemaIndex < 0 && (_uint)m_vecScenema.size() < iCinemaIndex)
		return E_FAIL;

	m_pCurCinema->Set_Active(false);
	m_pCurCinema = m_vecScenema[iCinemaIndex];
	m_pCurCinema->Set_Active(true);
	m_iCurCinema = iCinemaIndex;

	return S_OK;
}

_bool CScenematicManager::Get_EventCinema(_uint iCinemaIndex)
{
	if (m_iCurCinema != -1&&m_iCurCinema == iCinemaIndex)
	{
		if (m_bCinemaEnd)
			return true;
	}
	return false;
}

void CScenematicManager::ResetCinema()
{
	m_iCurCinema = -1;
	m_bCinemaEnd = false;
}

_uint CScenematicManager::Tick(_double TimeDelta)
{
	_uint iProgress = 0;
	if (m_pCurCinema)
	{
		if (m_pCurCinema->Get_Active())
			iProgress =m_pCurCinema->Tick(TimeDelta);
	}
	return iProgress;
}

_uint CScenematicManager::LateTick(_double TimeDelta)
{
	if (m_pCurCinema)
	{
		if (m_pCurCinema->Get_Active())
		{
			_uint iProgress =m_pCurCinema->LateTick(TimeDelta);
				return iProgress;
		}
		else
		{
			m_pCurCinema = nullptr;
			m_bCinemaEnd = true;
		}
	}

	return _uint();
}

void CScenematicManager::Free()
{
	for (auto& pScenema : m_vecScenema)
		Safe_Release(pScenema);

	m_vecScenema.clear();
}
