#include "..\public\Layer.h"
#include "GameObject.h"
#include "Component.h"
#include "Actor.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const wstring& pComponentTag, _uint iIndex)
{
	auto	iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double TimeDelta)
{
	_int		iProgress = 0;

	for (auto& pGameObject : m_Objects)
	{
		if (nullptr != pGameObject && pGameObject->getActive() == true)
			iProgress = pGameObject->Tick(TimeDelta);			

		if (0 > iProgress)
			return -1;
	}	
	return _int(0);
}

_int CLayer::LateTick(_double TimeDelta)
{
	_int		iProgress = 0;

	auto& iter = m_Objects.begin();

	for (; iter != m_Objects.end();)
	{
		if (nullptr != (*iter) && (*iter)->getActive() == true)
			iProgress = (*iter)->LateTick(TimeDelta);

		if (!Delete_GameObject(iter))
			iter++;

		if (0 > iProgress)
			return -1;
	}

	/*for (auto& pGameObject : m_Objects)
	{
		if (nullptr != pGameObject && pGameObject->getActive() == true)
			iProgress = pGameObject->LateTick(TimeDelta);
		cout << typeid(pGameObject).name() << endl;

		if (0 > iProgress)
			return -1;
	}*/
	return _int(0);
}

_bool CLayer::Delete_GameObject(list<class CGameObject*>::iterator& iter)
{
	if ((*iter)->getRemove())
	{
		Safe_Release(*iter);
		iter = m_Objects.erase(iter);
		return true;
	}
	return false;
}



CLayer * CLayer::Create()
{
	CLayer*	pInstance = new CLayer();
	
	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
		Safe_Release(pGameObject);

	m_Objects.clear();
}
