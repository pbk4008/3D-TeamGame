#pragma once

#include "GameObject.h"

BEGIN(Client)

class CWeapon abstract : public CGameObject
{
public:
	enum class EType { None, Sword_1H, Sword_2H, Monster_Sword1H, Hammer_2H, Shield, Staff, Earth_Pick, BronzeAnimus_Sword, Polearm, Max};
protected:
	explicit CWeapon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CWeapon(const CWeapon& _rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;
	virtual HRESULT	Render_Velocity() override;
	virtual HRESULT BindConstantBuffer(const wstring& camTag, SCB* consbuffer = nullptr, RIM* rimlightbuffer = nullptr, MOTIONBLUR* motionbuffer = nullptr);
	virtual HRESULT	BindLightBuffer();
	// dissolve
	virtual HRESULT	WeaponToAppear();
	virtual HRESULT	WeaponToDisAppear();

public:
	const wstring& Get_Name() const;
	const EType Get_Type() const;
	CActor* Get_Owner() const;
	const _float Get_Damage() const;
	const _float3& Get_EndPos() const;
	CModel* Get_Model() { return m_pModel; }

	void Set_Owner(CActor* _pOwner);
	virtual void Set_OwnerPivotMatrix(const _fmatrix& _smatPivot);
	virtual void Set_Equip(const _bool _isEquip, void* _pArg = nullptr);
	void Set_FixedBone(CHierarchyNode* _pFixedBone);

	void Set_IsAttack(const _bool _isAttack);
	void Set_IsTrail(const _bool _isTrail);
	void Set_SwordTrail(_bool check) { m_bTrailOnOff = check; }

	void Set_WeaponAppear(_bool check) { m_bdissolveappear = check; }
	void Set_WeaponDisAppear(_bool check) { m_bdissolvedisappear = check; }

	const _bool IsAttack() const;
	const _bool IsTrail() const;

	// rimlight
	_bool GetRimCheck() { return m_rimcheck; }
	void RimlightCheck(_bool check);
	void SetRimIntensity(_float time);

public:
	virtual void RangeAttack();

protected:
	//매개변수로 받은 행렬 크기 없애주는 함수
	_fmatrix Remove_Scale(_fmatrix matTransform);

protected:
	CTransform* m_pLocalTransform = nullptr;
	CModel*		m_pModel = nullptr;
	_float4x4	m_matOwnerPivot;

	CActor* m_pOwner = nullptr;
	CHierarchyNode* m_pFixedBone = nullptr;

	_bool m_isEquip = false;
	_bool m_isAttack = false;
	_bool m_isTrail = false;
	EType m_eType = EType::Max;

	wstring m_wstrName = L"";

	_bool			m_bTrailOnOff = false;

	_float m_fDamage = 0.f;
	const LIGHTDESC* m_lightdesc;

	_bool			m_bdissolveappear = false;
	_bool			m_bdissolvedisappear = false;
	_float			m_lifetime = 0.f;
	CTexture*		m_dissolveTex = nullptr;
	_uint			m_dissolvepass = 1;

	// motion blur
	_float			m_timer = 0.f;

	//rimlight
	_bool			m_rimcheck = false;
	_float			m_rimintensity = 8.f;

	_float3			m_vEndPos{};

public:
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) PURE;
	virtual void Free() override;
};

END