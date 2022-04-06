#pragma once

#include "Weapon.h"

BEGIN(Client)

class CTrailEffect;

class CNeedle final : public CWeapon
{
private:
	explicit CNeedle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CNeedle(const CNeedle& _rhs);
	virtual ~CNeedle() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnTriggerEnter(CCollision& collision) override;

	virtual void RangeAttack() override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);
	virtual void Set_Equip(const _bool _isEquip, void* _pArg = nullptr);

private:
	_float4x4 m_matPivot{};
	CCapsuleCollider* m_pCollider = nullptr;
	CTrailEffect* m_pTrailEffect = nullptr;

public:
	static CNeedle* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END