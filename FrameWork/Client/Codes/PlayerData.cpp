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

_bool CPlayerData::SetExp(_float iExp)
{
	if (CurExp < MaxExp)
	{
		CurExp += iExp;
	}
	if (CurExp >= MaxExp)
	{
		g_pInvenUIManager->SetLevelBG(++Level);
		CurExp = 0;

		if (10 < Level)
			Level = 9;

		return true;
	}

	return false;
}
