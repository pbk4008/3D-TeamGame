#pragma once

#include "Actor.h"
#include "Weapon.h"

BEGIN(Engine)

END

BEGIN(Client)
class CWeapon;
class CCamera_Silvermane;
class CJumpNode;

class CSilvermane final : public CActor
{
private:
	explicit CSilvermane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CSilvermane(const CSilvermane& _rhs);
	virtual ~CSilvermane() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	HRESULT Render_Debug();

private:
	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Ready_Weapons();

public:
	CTransform* Get_Transform() const;
	CModel* Get_Model() const;
	const _float Get_PlusAngle() const;
	const _float Get_Angle() const;

	void Set_Move(const _bool _isMove);
	void Set_TrasceCamera(const _bool _isTraceCamera);
	void Set_Camera(CCamera_Silvermane* _pCamera);
	void Set_PlusAngle(const _float _fAngle);

	void Add_PlusAngle(const _float _fDeltaAngle);

public: /* For.Weapon */
	const _bool Is_EquipWeapon() const;
	const _bool Is_EquipShield() const;
	const CWeapon::EType Get_WeaponType() const;
	void Set_EquipWeapon(const _bool _isEquipWeapon);
	void Set_WeaponFixedBone(const string& _wstrFixedBoneTag);
	void Set_WeaponFixedBone(CHierarchyNode* _pFixedBone);
	const _bool Change_Weapon(const wstring& _name);
	HRESULT Change_State(const wstring& _wstrStateTag);

public: /* For.Shield */
	void Set_EquipShield(const _bool _isEquipShield);
	void Set_EquipShieldAnim(const _bool _isEquipShield);

public: /* For.JumpNode */
	CJumpNode* Get_TargetJumpNode() const;
	const _bool Raycast_JumpNode(const _double& _dDeltaTime);

private:
	const _int Trace_CameraLook(const _double& _dDeltaTime);
	const _int Fall(const _double& _dDeltaTime);

private:
	CModel* m_pModel = nullptr;
	CStateController* m_pStateController = nullptr;
	CAnimationController* m_pAnimationController = nullptr;
	CCamera_Silvermane* m_pCamera = nullptr;
	CCharacterController* m_pCharacterController = nullptr;

	_bool m_isFall = false;
	_bool m_isMove = false;
	_bool m_isTraceCamera = true;

	_float m_fAngle = 0.f;
	_float m_fPlusAngle = 0.f;

private: /* For.Weapon */
	CWeapon* m_pCurWeapon = nullptr;
	CWeapon* m_pShield = nullptr;
	_bool m_isEquipWeapon = false;
	_bool m_isEquipShield = false;
	unordered_map<wstring, CWeapon*> m_umapWeapons;

private: /* For.JumpNode */
	CJumpNode* m_pTargetJumpNode = nullptr;
	_float m_fJumpNodeLookTime = 0.f;

public:
	static CSilvermane* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END