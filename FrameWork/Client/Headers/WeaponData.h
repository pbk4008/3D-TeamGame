#ifndef WeaponData_h__
#define WeaponData_h__

#include "ScriptableData.h"

BEGIN(Client)
class CWeaponData : public CScriptableData
{
public:
	struct Desc
	{
		std::wstring weaponName;
		_float damage;
		_float minMag;
		_float maxMag;
		EWeaponType weaponType;
	};

public:
	CWeaponData(void);
	CWeaponData(Desc desc);
	~CWeaponData(void);

public:
	CWeapon* Get_Weapon(void);
	_bool	 IsValid(void);

public:
	std::wstring weaponName;
	_float damage;

public:
	EWeaponType weaponType = EWeaponType::Weapon_End;

private:
	_bool isValid = false;

};

END
#endif // WeaponData_h__
