#include "pch.h"
#include "GameObjectPool.h"

CGameObjectPool::CGameObjectPool(std::function<CGameObject* ()> createFunc, std::size_t chunkSize)
{
	assert(0 <= chunkSize);

	this->chunkSize = chunkSize;

	for (_uint i = 0; i < this->chunkSize; i++)
	{
		CGameObject* pObj = createFunc();
		pObj->setActive(false);
		freeList.push(pObj);
	}
}

CGameObject* CGameObjectPool::AccquireObject(void)
{
	if (freeList.empty())
		return nullptr;
	auto obj = freeList.front();
	freeList.pop();

	obj->setActive(true);

	return obj;
}

void CGameObjectPool::ReleaseObject(CGameObject* obj)
{
	obj->setActive(false);
	Safe_Release(obj);
	freeList.push(obj);
}

void CGameObjectPool::DestroyPool(void)
{
	while (0 != freeList.size())
	{
		CGameObject* obj = freeList.front();
		freeList.pop();
		Safe_Release(obj);
	}
}

void ComponentPool::DestroyPool(void)
{
}


ComponentPool::ComponentPool(std::function<CComponent* ()> createFunc, std::size_t chunkSize)
{
	assert(0 <= chunkSize);

	this->chunkSize = chunkSize;

	for (_uint i = 0; i < this->chunkSize; i++)
	{
		CComponent* pCom = createFunc();
		pCom->setActive(false);
		freeList.push(pCom);
	}
}

CComponent* ComponentPool::AccquireObject(void)
{
	if (freeList.empty())
		return nullptr;
	auto obj = freeList.front();
	freeList.pop();

	obj->setActive(true);

	return obj;
}

void ComponentPool::ReleaseObject(CComponent* component)
{
	component->setActive(false);
	freeList.push(component);
}



