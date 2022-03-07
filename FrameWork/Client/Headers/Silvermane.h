#pragma once

#include "Actor.h"

BEGIN(Engine)
END

BEGIN(Client)
class CWeapon;

class CSilvermane final : public CActor
{
private:
	explicit CSilvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane(const CSilvermane& _rhs);
	virtual ~CSilvermane() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_States();

public:
	CTransform* Get_Transform() const;
	CModel* Get_Model() const;
	const _float3& Get_Rotation() const;
	const _fvector& Get_Dir() const;

	void Set_Dir(const _fvector& _svDir);
	void Set_Rotation(const _float3& _vRotation);
	void Set_EquipWeapon(const _bool _isEquipWeapon);
	void Set_WeaponFixedBone(CHierarchyNode* _pFixedBone);

	const _bool Is_EquipWeapon() const;

public:
	CModel* m_pModel = nullptr;
	CStateController* m_pStateController = nullptr;
	CAnimationController* m_pAnimationController = nullptr;

	CWeapon* m_pWeapon = nullptr;
	_bool m_isEquipWeapon = false;

	_float3 m_vRotation = { 0.f, 0.f, 0.f };
	_float3 m_vDir = { 0.f, 0.f, 0.f };

public:
	static CSilvermane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(void* _pArg = nullptr) override;
	virtual void Free() override;
};

END