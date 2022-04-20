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
	//무기 = 데미지
	//장신구 = 근력, 스피릿, 바이탈 등등
	//생명석 = 풀 Hp 등등등

	/* for. Weapon */
public:
	EWeaponType	   weaponType = EWeaponType::Weapon_End;
	EEquipmentType equipmentType = EEquipmentType::Equipment_Type_End;
	_float fCrit = -1.f;		    /* 크리티컬 확률    */
	_float fUItiChargingSpd = -1.f; /* 궁극기 차징 속도 */
	_float fAddKnockDown = -1.f;	/* 스턴 수치 증가량 */

	/*for. Amulet 장신구 */
public:

	/*for. Ring */
public:

	/* for. LifeStone */
public:
	_int iHealth = -1;

	/* for. Charm 매력 */
public:

	/* for. Banner */
public:

	/* for. Resource */
public:
	_int iResourceCount;
};

END
#endif // ItemData_h__
