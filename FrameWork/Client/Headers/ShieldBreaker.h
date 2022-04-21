#pragma once

#include "Weapon.h"

BEGIN(Client)

class CShieldBreaker final : public CWeapon
{
private:
	explicit CShieldBreaker(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CShieldBreaker(const CShieldBreaker& rhs);
	virtual ~CShieldBreaker() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg = nullptr);
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double Timedelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void setActive(_bool bActive) override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& Timedelta);
	_int Attach_Owner(const _double& Timedelta);
	void OnHitGroundEffect();

public:
	void Set_HitGround(_bool bCheck) { m_bHitGround = bCheck; }


private:
	_float4x4 m_matPivot;
	CCapsuleCollider* m_pCollider = nullptr;
	_bool m_bHitGround = false;

public:
	static CShieldBreaker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg = nullptr) override;
	virtual void Free() override;
};

END