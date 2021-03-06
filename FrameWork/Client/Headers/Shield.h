#pragma once

#include "Weapon.h"

BEGIN(Client)

class CShield : public CWeapon
{
private:
	explicit CShield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CShield(const CShield& _rhs);
	virtual ~CShield() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

	virtual void OnTriggerEnter(CCollision& collision) override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);

public:
	virtual void Set_Equip(const _bool _isEquip, void* _pArg = nullptr);
	virtual void Set_EquipAnim(const _bool _isEquip);
	void Add_TrackAcc(const _double& _dTrackAcc);
	void Set_TrackAcc(const _double& _dTrackAcc);

private:
	_float4x4 m_matPivot;
	CAnimationController* m_pAnimationController = nullptr;
	CBoxCollider* m_pCollider = nullptr;

public:
	static CShield* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END