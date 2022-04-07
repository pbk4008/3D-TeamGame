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
	CGameObject* AccquireObject(void); /* ���� ������Ʈ */
	void ReleaseObject(CGameObject* obj);
	/* ��뿡 ������ �� */
	void DestroyPool(void);

private:
	std::size_t chunkSize;
	std::queue<CGameObject*> freeList;
};
END
#endif // GameObjectPool_h__