#ifndef WeaponData_h__
#define WeaponData_h__

#include "ScriptableData.h"

BEGIN(Client)

class CWeaponData : public CScriptableData
{
public:
	struct WeaponDesc
	{
		std::wstring weaponName;
		_float damage;
		_float minMag;
		_float maxMag;
		EWeaponType weaponType;
	};

public:
	CWeaponData(void);
	CWeaponData(WeaponDesc desc);
	~CWeaponData(void);
};

END
#endif // WeaponData_h__
