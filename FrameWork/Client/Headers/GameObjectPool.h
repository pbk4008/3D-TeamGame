#ifndef GameObjectPool_h__
#define GameObjectPool_h__

#include <queue>

static const int DefaultChunkSize = 1;

BEGIN(Client)

class CGameObjectPool
{
public:
	CGameObjectPool(std::function<CGameObject*()> createFunc, std::size_t chunkSize = DefaultChunkSize);

public:
	CGameObject* AccquireObject(void); /* 습득 오브젝트 */
	void ReleaseObject(CGameObject* obj);
	/* 사용에 주의할 것 */
	void DestroyPool(void);

private:
	std::size_t chunkSize;
	std::queue<CGameObject*> freeList;
};
END
#endif // GameObjectPool_h__