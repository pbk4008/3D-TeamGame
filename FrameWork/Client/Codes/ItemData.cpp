#include "pch.h"
#include "ItemData.h"
#include "UIDGen.h"

CItemData::CItemData(void)
{
	uid = UIDGen::GetInstance()->GetUID();
	bEquiped = false;
	UIDGen::DestroyInstance();
}

_bool CItemData::Compare(CItemData data)
{
	if (uid = data.uid)
		return true;
	else
		return false;
}
