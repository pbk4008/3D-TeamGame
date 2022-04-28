#ifndef Player_Weapon_h__
#define Player_Weapon_h__

#include "Weapon.h"

BEGIN(Engine)
class CLight;
END	

BEGIN(Client)

class CTrailEffect_Normal;
class CTrailEffect_Distortion;

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
	virtual HRESULT	Render_Velocity() override;

	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void RangeAttack() override;

public: void	SetLightCheck(_bool check) { m_bLightCheck = check; }

private:
	HRESULT Ready_Components();
	HRESULT Ready_TrailEffects(const Desc& _tDesc);
	_int Attach_FixedBone(const _double& _dDeltaTime);
	_int Attach_Owner(const _double& _dDeltaTime);
	virtual void Set_Equip(const _bool _isEquip, void* _pArg = nullptr);

private: void	LightOnOff(_fvector pos, _fvector color, _float deltaspeed = 1.f);

private:
	_float4x4 m_matPivot;
	CBoxCollider*	  m_pBoxCollider = nullptr;
	CCapsuleCollider* m_pCapsuleCollider = nullptr;

	_float4 m_fWeaponColor = { 0.f, 0.552f, 0.384f, 1.f };
	std::wstring m_szModelName;
	std::wstring m_szMIname = L"Mtrl_";
	std::wstring m_szMIFilePath = L"../../Reference/ShaderFile/Shader_Weapon.hlsl";
	std::wstring m_szMiTexPath = L"../bin/Resources/Mesh/";

private: /* TrailEffect */
	CTrailEffect_Normal* m_pTrailEffect_Normal = nullptr;
	CTrailEffect_Distortion* m_pTrailEffect_Distortion = nullptr;

private: 
	_bool			m_bLightCheck = false;
	CLight*			m_pActiveLight = nullptr;
	_float			m_LightRange = 0.f;
	_float			m_OrigLightRange = 0.f;
	_vector			m_HitPosition = XMVectorZero();

public:
	static CPlayer_Weapon* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END

#endif // Player_Weapon_h__