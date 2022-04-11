#ifndef EquipmentData_h__
#define EquipmentData_h__

#include "ScriptableData.h"
#include "ItemData.h"

BEGIN(Client)

class CEquipmentData final : public CScriptableData
{
public:
	CEquipmentData(void);

public:
	_bool IsExistEquip(EEquipSlot slot);
	_bool ExistItem(CItemData item);
	_bool CompareEquipment(EEquipSlot slot, CItemData item);
	void ChangeEquipment(EEquipSlot slot, CItemData item);
	void RemoveEquipment(EEquipSlot slot);
	void RemoveEquipment(CItemData item);

public: /* Get */
	CItemData GetEquipment(EEquipSlot slot);

public: /* Set */
	void SetEquipment(EEquipSlot slot, CItemData _data);

public:
	std::vector<CItemData> m_vecEquipmentItems;
};

END
#endif // EquipmentData_h__
