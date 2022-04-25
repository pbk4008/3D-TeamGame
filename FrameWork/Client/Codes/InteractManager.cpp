#include "pch.h"
#include "InteractManager.h"
#include "InteractableObject.h"
#include "Loot_Shield.h"

CInteractManager::CInteractManager(void)
{
}

HRESULT CInteractManager::NativeConstruct(void)
{
	return S_OK;
}

_int CInteractManager::Tick(_double _dTimeDelta)
{
	for (_int i = 0 ; i < m_vecInteractableObjects.size(); i++)
	{
		if (m_vecInteractableObjects[i]->IsAlive())
		{
			if (m_vecInteractableObjects[i]->IsInteractable())					/* 플레이어와의 거리 측정하여 true 리턴 */
			{
				if (false == m_vecInteractableObjects[i]->IsInteratcing())		/* First Interaction */
				{
					m_vecInteractableObjects[i]->BeginInteraction();
				}
				m_vecInteractableObjects[i]->UpdateInteraction(_dTimeDelta);	/* Update Interaction */
			}
			else
			{
				if (m_vecInteractableObjects[i]->IsInteratcing())
				{
					m_vecInteractableObjects[i]->EndInteraction();
				}
			}
		}
		else
		{
			m_vecInteractableObjects.erase(m_vecInteractableObjects.begin() + i);
		}
	}
	DropItemUpdate();

	return _int();
}

void CInteractManager::OnDropItem(void* pItemData)
{
}

void CInteractManager::DropItem(CItemData itemData, _fvector vecOriginPos)
{
	DropDesc desc;
	desc.itemData = itemData;
	desc.vecOriginPos = vecOriginPos;
	_fvector pMin = XMVectorSet((XMVectorGetX(vecOriginPos) - m_scatteredRadius), XMVectorGetY(vecOriginPos), (XMVectorGetZ(vecOriginPos) - m_scatteredRadius), 1.0f);
	_fvector pMax = XMVectorSet((XMVectorGetX(vecOriginPos) + m_scatteredRadius), XMVectorGetY(vecOriginPos), (XMVectorGetZ(vecOriginPos) + m_scatteredRadius), 1.0f);

	MathUtils::GetRandomVector(&desc.vecScatteredPos, &pMin, &pMax);
	
	desc.elapsed = 0.0f;
	desc.duration = m_dropDuration;

	m_dropDescriptons.push_back(desc);
}

void CInteractManager::DropItemUpdate(void)
{
}

void CInteractManager::Remove_Interactable()
{
	if(!m_vecInteractableObjects.empty())
		m_vecInteractableObjects.clear();
}

void CInteractManager::AddInteractObject(CInteractableObject* pInteractableObject)
{
	assert(pInteractableObject);
	m_vecInteractableObjects.push_back(pInteractableObject);
}

void CInteractManager::Free()
{
	Remove_Interactable();
}
