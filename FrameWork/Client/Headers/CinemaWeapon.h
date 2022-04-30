#pragma once
#ifndef __CINEMAWEAPON_H__
#define __CINEMAWEAPON_H__

BEGIN(Client)
class CCinemaWeapon final : public CWeapon 
{
public:
	enum class WEAPONTYPE {TYPE_SPEAR, TYPE_SWORD, TYPE_END};
private:
	explicit CCinemaWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCinemaWeapon(const CCinemaWeapon& rhs);
	virtual ~CCinemaWeapon() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
public:
	void ResetBone();
private:
	HRESULT Ready_Model();
	HRESULT Ready_PoenixMaterial();
	HRESULT Ready_GrayHwakMaterial();
	HRESULT Ready_ShieldBreaker();
private:
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);
public:
	void set_OwerMatrix(_fmatrix matWorld);
public:
	static CCinemaWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint iSceneID, void* pArg);
private:
	virtual void Free();
private:
	_uint m_iType=0;
	_float4x4 m_matOwerWorld;
};
END
#endif