#include "EffectManager.h"
#include "GameInstance.h"

CEffectManager::CEffectManager()
	: m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
{
}


HRESULT CEffectManager::Ready_EffectManager(ID3D11Device* pDevcie, ID3D11DeviceContext* pDeviceContext)
{
	m_pDevice = pDevcie;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CEffectManager::Add_Effect(_uint iSceneID, const wstring& pLayerTag, CEffect* pEffect, _uint iCount)
{
	pEffect->setActive(false);
	Safe_AddRef(pEffect);
	list<CEffect*> listEffect;
	listEffect.emplace_back(pEffect);
	for (_uint i = 0; i < iCount - 1; i++)
	{
		CEffect* tmpEffect=pEffect->Copy();
		if (!tmpEffect)
			return E_FAIL;
		
		g_pGameInstance->Add_GameObjectToLayer(iSceneID, pLayerTag, tmpEffect);
		listEffect.emplace_back(tmpEffect);
		Safe_AddRef(tmpEffect);
	}
	m_vecEffect.emplace_back(listEffect);
	return S_OK;
}

CEffect* CEffectManager::Get_Effect(_uint iEffectIndex)
{
	CEffect* pEffect = Find_Effect(iEffectIndex);

	return pEffect;
}

CEffect* CEffectManager::Find_Effect(_uint iEffectIndex)
{
	if (iEffectIndex<0 && iEffectIndex>(_uint)m_vecEffect.size())
		return nullptr;

	if (0 == m_vecEffect.size())
		return nullptr;

	CEffect* pFindEffect = nullptr;

	list<CEffect*> pList = m_vecEffect[iEffectIndex];
	if (pList.empty())
		return nullptr;

	for (auto& pEffect : pList)
	{
		if (!pEffect->getActive())
		{
			pFindEffect = pEffect;
			break;
		}
	}

	return pFindEffect;
}

void CEffectManager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	for (auto& pList : m_vecEffect)
	{
		for (auto& pEffect : pList)
			Safe_Release(pEffect);
		pList.clear();
	}
	m_vecEffect.clear();
}
