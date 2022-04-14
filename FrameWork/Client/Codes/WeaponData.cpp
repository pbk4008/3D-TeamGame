#include "pch.h"
#include "WeaponData.h"


CWeaponData::CWeaponData(void)
{
	isValid = false;
}

CWeaponData::CWeaponData(Desc desc)
{
	weaponName = desc.weaponName;
	damage = desc.damage;
	weaponType = desc.weaponType;

	isValid = true;
}

CWeaponData::~CWeaponData(void)
{
}

CWeapon* CWeaponData::Get_Weapon(void)
{
	//assert(IsValid());

	return g_pWeaponGenerator->GetWeapon(weaponName);
}

_bool CWeaponData::IsValid(void)
{
	return isValid;
}

