#ifndef PlayerData_h__
#define PlayerData_h__

#include "ScriptableData.h"

BEGIN(Client)

class CPlayerData final : public CScriptableData
{
public:
	CPlayerData(void);


public:
	void Reset(void);
	_float GetExp(void);
	_bool SetExp(_float iExp);
	_int  Get_Level(void);
public:
	_int Level	    = 0;
	_float MaxExp   = 100;
	_float CurExp	  = 0;

	_float CurHp = 200.f;
	_float Life = 200.f;

	_float Power = 100.f;

	_bool m_bEquipShield = false;
	_int  EquipedSlot = 1;
};

END

#endif // PlayerData_h__
