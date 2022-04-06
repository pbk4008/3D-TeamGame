#ifndef InteractManager_h__
#define InteractManager_h__

#include "SingleTon.h"
#include "ItemData.h"

BEGIN(Client)
class CInteractableObject;
class CInteractManager final : public CSingleTon<CInteractManager>
{
	friend CSingleTon;
	
private:
	NO_COPY(CInteractManager);
	explicit CInteractManager(void);
	virtual ~CInteractManager(void) = default;

public:
	struct DropDesc
	{
		CItemData itemData;
		_vector vecOriginPos;
		_vector vecScatteredPos; // Èð¾îÁö´Â À§Ä¡
		_float elapsed = 0.0f;   // Èå¸£´Ù?
		_float duration = 0.3f;
	};

public:
	HRESULT NativeConstruct(void);
	_int    Tick(_double _dTimeDelta);

public:
	void OnDropItem(void* pItemData);

public:
	void DropItem(CItemData itemData, _fvector vecOriginPos);
	void DropItemUpdate(void);

public:
	void AddInteractObject(CInteractableObject* pInteractableObject);

private:
	std::vector< CInteractableObject*> m_vecInteractableObjects;

	/* for. Drop Item */
private:
	_float m_scatteredRadius = 4.0f;
	_float m_dropDuration = 0.5f;
	std::vector<DropDesc> m_dropDescriptons;

private:
	virtual void Free() override;
};

END
#endif // InteractManager_h__