#pragma once

#include "Weapon.h"
#include "Light.h"

BEGIN(Client)

class CTrailEffect_Normal;
class CTrailEffect_Distortion;

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
	virtual HRESULT	Render_Shadow() override;
	virtual HRESULT	Render_Velocity() override;

	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void OnTriggerExit(CCollision& collision) override;

	virtual void RangeAttack(const _float _fForce = 1.f) override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);
	virtual void Set_Equip(const _bool _isEquip, void* _pArg = nullptr);

private:
	_float4x4 m_matPivot{};
	CCapsuleCollider* m_pCollider = nullptr;
	CTrailEffect_Normal* m_pTrailEffect_Normal = nullptr;
	CTrailEffect_Distortion* m_pTrailEffect_Distortion = nullptr;
	CLight* m_pLight = nullptr;

	_bool m_bLight = false;
	_float m_fLightRange = 0.f;
public:
	static CNeedle* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END