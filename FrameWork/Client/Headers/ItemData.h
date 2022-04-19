#ifndef ItemData_h__
#define ItemData_h__

#include "ScriptableData.h"
#include "WeaponData.h"

BEGIN(Client)

class CItemData final : public CScriptableData
{
public:
	CItemData(void);

public:
	_bool Compare(CItemData data);

public:
	_uint	  uid;
	EItemType ItemType = EItemType::NONE;

	/*for. Equipment */
public:
	std::wstring iconTexName = L"None_Icon";
	std::wstring szWeaponName = L"";
	std::wstring szItemName = L"";
	std::wstring szStatusName = L"";

	CWeaponData			   weaponData;
	EEquipmentGrade		   equipmentGrade		 = EEquipmentGrade::EQUIPGRADE_END;
	EEquipmentMainStatType equipmentMainStatType = EEquipmentMainStatType::MainStatType_End;
	EEquipmentName		   equipmentName		 = EEquipmentName::Equipment_Name_End;

	_bool bEquiped = false;
	_int iDmg = 1;
	_int iLevel = -1;
	_int iMainStat = -1;
	//���� = ������
	//��ű� = �ٷ�, ���Ǹ�, ����Ż ���
	//���� = Ǯ Hp ����

	/* for. Weapon */
public:
	EWeaponType	   weaponType = EWeaponType::Weapon_End;
	EEquipmentType equipmentType = EEquipmentType::Equipment_Type_End;
	_float fCrit = -1.f;		    /* ũ��Ƽ�� Ȯ��    */
	_float fUItiChargingSpd = -1.f; /* �ñر� ��¡ �ӵ� */
	_float fAddKnockDown = -1.f;	/* ���� ��ġ ������ */

	/*for. Amulet ��ű� */
public:

	/*for. Ring */
public:

	/* for. LifeStone */
public:
	_int iHealth = -1;

	/* for. Charm �ŷ� */
public:

	/* for. Banner */
public:

	/* for. Resource */
public:
	_int iResourceCount;
};

END
#endif // ItemData_h__
