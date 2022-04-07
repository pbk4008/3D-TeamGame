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
		Safe_AddRef(pObj);
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
	Safe_Release(obj);

	return obj;
}

void CGameObjectPool::ReleaseObject(CGameObject* obj)
{
	obj->setActive(false);
	Safe_AddRef(obj);
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


