#ifndef DropManager_h__
#define DropManager_h__

#include "SingleTon.h"
#include "ItemData.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CGameObjectPool;
class CDropObject;
class CDropManager final : public CSingleTon<CDropManager>
{
	friend CSingleTon;

	struct DropDesc
	{
		CDropObject* dropObject;
		_float3 centerPos;
		_float3 scatteredPos; //흩어지는 위치
		_float elapsed = 0.0f;
		_float duration = 0.3f;
	};

private:
	NO_COPY(CDropManager);
	CDropManager(void);
	~CDropManager(void) = default;

public:
	HRESULT NativeConstruct(SCENEID _iSceneID);
	void    Tick(void);

public:
	void DropItem(CItemData itemData, _fvector centerPos, EScatterType scatterType = EScatterType::Quad, CTransform* pSender = nullptr);

private:
	CGameObjectPool*			  m_pDropObjectPool = nullptr;
	std::vector<CDropObject*>	  m_dropObjects;
	CGameObject*				  m_pPlayer = nullptr;
	_uint						  m_curSceneId = 0;

private:
	virtual void Free() override;
};

END
#endif // DropManager_h__
