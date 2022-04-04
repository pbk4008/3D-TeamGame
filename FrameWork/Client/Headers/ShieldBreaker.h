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

	virtual void OnTriggerEnter(CCollision& collision) override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& Timedelta);
	_int Attach_Owner(const _double& Timedelta);

private:
	_float4x4 m_matPivot;
	CCapsuleCollider* m_pCollider = nullptr;

public:
	static CShieldBreaker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg = nullptr) override;
	virtual void Free() override;
};

END