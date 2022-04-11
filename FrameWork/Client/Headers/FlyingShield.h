#pragma once

#include "Weapon.h"

BEGIN(Client)

class CFlyingShield : public CWeapon
{
private:
	explicit CFlyingShield(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CFlyingShield(const CFlyingShield& _rhs);
	virtual ~CFlyingShield() = default;

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

private:
	CBoxCollider* m_pCollider = nullptr;

public:
	static CFlyingShield* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(_uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END