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

	for (auto& pGameObject : m_Objects)
	{
		if (nullptr != pGameObject && pGameObject->getActive() == true)
			iProgress = pGameObject->LateTick(TimeDelta);

		if (0 > iProgress)
			return -1;
	}
	return _int(0);
}

HRESULT CLayer::Delete_Object()
{
	if (m_Objects.empty())
		return S_OK;

	auto iter_begin = m_Objects.begin();

	for (; iter_begin != m_Objects.end();)
	{
		if ((*iter_begin)->getRemove())
		{
			Safe_Release((*iter_begin));
			iter_begin=m_Objects.erase(iter_begin);
		}
		else
			iter_begin++;
	}
	return S_OK;
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
