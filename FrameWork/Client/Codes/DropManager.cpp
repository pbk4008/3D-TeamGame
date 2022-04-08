#include "pch.h"
#include "DropManager.h"
#include "InteractableObject.h"
#include "DropObject.h"
#include "GameObjectPool.h"

CDropManager::CDropManager(void)
{
}

HRESULT CDropManager::NativeConstruct(SCENEID _iSceneID)
{
	m_pDropObjectPool = new CGameObjectPool([&]()->CGameObject*
		{
			CGameObject* pObj;
			if (FAILED(g_pGameInstance->Add_GameObjectToLayer((_uint)_iSceneID, L"Layer_DropObject", L"Proto_GameObject_DropObject", nullptr ,&pObj)))
			{
				PRINT_LOG(L"Alert", L"Drop Item 생성에 실패했어요...");
				return nullptr;
			}
			return pObj;
		}, 30);

	if (nullptr == m_pDropObjectPool)
		return E_FAIL;

	m_curSceneId = (_uint)_iSceneID;

	m_pPlayer = *g_pGameInstance->getObjectList(m_curSceneId, L"Layer_Silvermane")->begin();

	return S_OK;
}

void CDropManager::Tick(void)
{
	if (nullptr == m_pPlayer)
		m_pPlayer = *g_pGameInstance->getObjectList(m_curSceneId, L"Layer_Silvermane")->begin();

	for (_int i = 0; i < m_dropObjects.size(); i++)
	{
		if (false == m_dropObjects[i]->IsDrop())
		{
			m_pDropObjectPool->ReleaseObject(m_dropObjects[i]);
			m_dropObjects.erase(m_dropObjects.begin() + i);
			i--;
			continue;
		}
		_float dist = MathUtils::Length(m_dropObjects[i], m_pPlayer);
		if (50 <= dist)
		{
			m_pDropObjectPool->ReleaseObject(m_dropObjects[i]);
			m_dropObjects.erase(m_dropObjects.begin() + i);
			i--;
			continue;
		}
	}
}

void CDropManager::DropItem(CItemData itemData, _fvector centerPos, EScatterType scatterType, CTransform* pSender)
{
	CDropObject* pDropObject = static_cast<CDropObject*>(m_pDropObjectPool->AccquireObject());

	if (nullptr == pDropObject)
		return;

	pDropObject->Get_Transform()->Set_State(CTransform::STATE_POSITION, centerPos);
	pDropObject->Drop(itemData, centerPos, scatterType, pSender);
	m_dropObjects.push_back(pDropObject);
}

void CDropManager::Free()
{
	if (0 != m_dropObjects.size())
	{
		for (auto iter : m_dropObjects)
			m_pDropObjectPool->ReleaseObject(iter);
	}
	m_dropObjects.clear();

	m_pDropObjectPool->DestroyPool();
	Safe_Delete(m_pDropObjectPool);
}
