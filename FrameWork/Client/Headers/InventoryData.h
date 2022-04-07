#ifndef InventoryData_h__
#define InventoryData_h__

#include "ScriptableData.h"
#include "ItemData.h"

BEGIN(Client)

class CInventoryData : public CScriptableData
{
public:
	CInventoryData(void);

public:
	_bool PushItem(CItemData itemData);
	void AddResource(_int iResource);

public:
	void RemoveItem(CItemData itemData);
	void RemoveResource(_int iResource);

public:
	_bool ExistItem(_int iModalIndex);

public: /* Get Item */
	_bool TryGetItem(_int iIndex, _Out_ CItemData* pItemData);
	CItemData GetItem(_int iIndex);
	CItemData GetItem(void);
	CItemData* GetItemByID(_ulong _uid);
	_int GetCount(void);

public: /* Set Item */
	_bool IsFull(void);
	void SetEquiped(_int iIndex, _bool bEquiped);
	void SetResource(_int iResource);

public:
	std::vector<CItemData> InventoryItems;
	_int m_iResources = 0;
	const _int ModalMaxCount = 32;
};

END
#endif // InventoryData_h__