#ifndef Player_Weapon_h__
#define Player_Weapon_h__

#include "Weapon.h"

BEGIN(Client)
class CPlayer_Weapon final : public CWeapon
{
public:
	struct Desc
	{
		std::wstring weaponName;
		std::wstring equipBone = L"spine_03";
		std::wstring modelName;
		const std::wstring* arrMaterial = nullptr;
		_float colliderHeight;
		_float colliderRadius;
		_float4x4 colliderOffset;
		_float4 weaponColor = _float4(0.f, 0.552f, 0.384f, 1.f);
		_float damage = 10.f;
		_uint  NumMat = 3;
		CHierarchyNode* pHierarchyNode = nullptr;
		CWeapon::EType EWeaponType = EType::Max;
	};

private:
	explicit CPlayer_Weapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CPlayer_Weapon(const CPlayer_Weapon& _rhs);
	virtual ~CPlayer_Weapon() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, Desc _desc);
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void RangeAttack() override;

private:
	HRESULT Ready_Components();
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);
	virtual void Set_Equip(const _bool _isEquip, void* _pArg = nullptr);

private:
	_float4x4 m_matPivot;
	CBoxCollider*	  m_pBoxCollider = nullptr;
	CCapsuleCollider* m_pCapsuleCollider = nullptr;

	_float4 m_fWeaponColor = { 0.f, 0.552f, 0.384f, 1.f };
	std::wstring m_szModelName;
	std::wstring m_szMIname = L"Mtrl_";
	std::wstring m_szMIFilePath = L"../../Reference/ShaderFile/Shader_Weapon.hlsl";
	std::wstring m_szMiTexPath = L"../bin/Resources/Mesh/";

	CWeapon::EType m_EWeaponType = CWeapon::EType::Max;

public:
	static CPlayer_Weapon* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END

#endif // Player_Weapon_h__