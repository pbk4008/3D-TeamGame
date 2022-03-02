#include "..\public\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"
#include "Component.h"
#include "Level_Manager.h"

CObject_Manager::CObject_Manager()
{

}

CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pComponentTag, _uint iIndex)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring& pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype || 
		nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototype.insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));	

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObjectToLayer(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pPrototypeTag, void * pArg, CGameObject** ppOut/* = nullptr*/)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	if(nullptr != ppOut)
		*ppOut = pGameObject;

	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
	{
		CLayer*		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;

		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
		iter->second->Add_GameObject(pGameObject);

	return S_OK;
}

CGameObject* CObject_Manager::Clone_Gameobject(_uint iLevelIndex, const wstring& pPrototypeTag, void* pArg)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	CGameObject * pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject * pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

_int CObject_Manager::Tick(_double TimeDelta)
{
	_int		iProgress = 0;

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			iProgress = Pair.second->Tick(TimeDelta);

		if (0 > iProgress)
			return -1;
	}

	return _int();
}

_int CObject_Manager::LateTick(_double TimeDelta)
{
	_int		iProgress = 0;

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			iProgress = Pair.second->LateTick(TimeDelta);

		if (0 > iProgress)
			return -1;
	}


	return _int();
}

list<CGameObject*>* CObject_Manager::getObjectList(_uint iLevelIndex, const wstring& pLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (!pLayer)
		return nullptr;

	return pLayer->getObjList();
}

list<CGameObject*> CObject_Manager::getAllObjectList()
{
	list<CGameObject*> AllObjList;

	for (_uint i = 1; i < m_iNumLevels; i++)
	{
		for (auto& pLayer : m_pLayers[i])
		{
			list<CGameObject*>* pObjList = (pLayer.second)->getObjList();
			for (auto& pObj : *pObjList)
				AllObjList.emplace_back(pObj);
		}
	}
	return AllObjList;
}

HRESULT CObject_Manager::Clear_Object_Manager(_uint iLevelIndex)
{
	if (iLevelIndex > m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();

	return S_OK;
}

void CObject_Manager::Clear_Object_List(void)
{
	for (int i = 1; i < m_iNumLevels; i++)
		m_pLayers[i].clear();
}

CGameObject * CObject_Manager::Find_Prototype(const wstring& pPrototypeTag)
{
	auto	iter = find_if(m_Prototype.begin(), m_Prototype.end(), CTag_Finder(pPrototypeTag));
	if(iter == m_Prototype.end())		
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring& pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototype)
		Safe_Release(Pair.second);

	m_Prototype.clear();	
}
