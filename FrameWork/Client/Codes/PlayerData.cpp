#include "pch.h"
#include "PlayerData.h"

CPlayerData::CPlayerData(void)
{
}

void CPlayerData::Reset(void)
{
	CurHp = Life;
}

_float CPlayerData::GetExp(void)
{
	return CurExp;
}

void CPlayerData::SetExp(_float iExp)
{
	if (CurExp < MaxExp)
		CurExp += iExp;
}
