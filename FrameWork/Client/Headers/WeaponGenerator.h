#ifndef WeaponGenerator_h__
#define WeaponGenerator_h__

#include "SingleTon.h"

BEGIN(Client)
class CWeapon;
class CWeaponData;
class CWeaponGenerator final : public CSingleTon<CWeaponGenerator>
{
	friend CSingleTon;

private:
	NO_COPY(CWeaponGenerator);
	CWeaponGenerator(void);
	~CWeaponGenerator(void) = default;

public:
	HRESULT NativeConstruct(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _uint _iSceneID, CModel* _pModel);

public:
	CWeapon*		GetWeapon(std::wstring weaponName);
	CWeaponData		GetWeaponData(std::wstring weaponName);

private:
	std::vector<std::pair<CWeaponData, CWeapon*> > m_vecWeapons;


private:
	virtual void Free() override;
};

END
#endif // WeaponGenerator_h__

