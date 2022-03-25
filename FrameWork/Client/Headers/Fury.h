#pragma once

#include "Weapon.h"

BEGIN(Client)

class CFury final : public CWeapon
{
private:
	explicit CFury(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CFury(const CFury& _rhs);
	virtual ~CFury() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);
	virtual void Set_Equip(const _bool _isEquip, void* _pArg = nullptr);

private:
	_float4x4 m_matPivot;
	CBoxCollider* m_pCollider = nullptr;

public:
	static CFury* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END