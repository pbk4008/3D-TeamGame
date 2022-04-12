#ifndef Monster_Bastion_Spear_h__
#define Monster_Bastion_Spear_h__

#include "Actor.h"

BEGIN(Engine)
class CAnimator;
class CAnimation;
class CCapsuleCollider;
END

BEGIN(Client)
class CPolearm;
class CMonster_Bastion_Spear : public CActor
{
public:
	enum class ANIM_TYPE {
		A_HEAD, A_IDLE, A_DEATH, A_FLINCH, A_RICOCHET, A_GUARD, A_BWD_DASH,
		A_ATTACK_CHARGE_ST, A_ATTACK_CHARGE, A_ATTACK_CHARGE_ED,
		A_ATTACK_R1, A_ATTACK_R2,
		A_WALK_FWD_ST, A_WALK_FWD, A_WALK_FWD_ED,
		A_WALK_BWD_ST, A_WALK_BWD, A_WALK_BWD_ED,
		A_STUN_ST, A_STUN, A_STUN_ED,
		A_TURN_45_LEFT, A_TURN_45_RIGHT,
		A_TURN_90_LEFT, A_TURN_90_RIGHT,
		A_TURN_135_LEFT, A_TURN_135_RIGHT,
		A_TURN_180_LEFT, A_TURN_180_RIGHT,
		A_END
	};

private:
	explicit CMonster_Bastion_Spear(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_Spear(const CMonster_Bastion_Spear& _rhs);
	virtual ~CMonster_Bastion_Spear() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

private:
	HRESULT Ready_Components(_uint iSceneID);
	HRESULT Ready_Weapon(void);
	HRESULT Ready_AnimFSM(void);
	HRESULT Ready_StateFSM(void);
	HRESULT Render_Debug(void);

private:
	virtual void OnTriggerEnter(CCollision& collision);

public:
	void Set_IsAttack(const _bool _isAttack);
	void Set_Current_HP(_int _iHp) { m_fCurrentHp += _iHp; }
	void Set_GroggyGauge(_uint _Value) { m_fGroggyGauge += _Value; }
	void Set_Dead(void) { m_bDead = true; }
	void Remove_Collider();
	void Groggy_Start();
public:
	virtual void Hit(const ATTACKDESC& _tAttackDesc) override;
	virtual void Parry(const PARRYDESC& _tParryDesc) override;
	void Hit(CCollision& collision);
	void Attack(_bool bCheck);
public:
	void Set_Target(_bool bCheck) { m_bTargetOn = bCheck; }
	void Set_Attack(_bool bCheck) { m_bAttackOn = bCheck; }
	void Set_Guard(_bool bCheck) { m_bGuard = bCheck; }
	void Set_Half(_bool bCheck) { m_bHalf = bCheck; }
	void Set_ChargeOn(_bool bCheck) { m_bChargeOn = bCheck; }
	void Set_GuardCount(_int iCount) { m_iGuardCount = iCount;  }

	_bool Get_Target() { return m_bTargetOn; }
	_bool Get_Attack() { return m_bAttackOn; }
	_bool Get_Guard() { return m_bGuard; }
	_bool Get_Half() { return m_bHalf; }
	_bool Get_ChargeOn() {return m_bChargeOn;}

	_int Get_GuardCount() { return m_iGuardCount; }
private:
	_bool m_bTargetOn = false;
	_bool m_bAttackOn = false;//공격 한다는 애님 체크
	_bool m_bGuard = false;
	_bool m_bHalf = false;
	_bool m_bChargeOn = false;
	_int m_iGuardCount = 7;

private:
	CAnimation* m_pAnimation = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CStateController* m_pStateController = nullptr;
	CCapsuleCollider* m_pColliderCom = nullptr;
	CCharacterController* m_pCharacterController = nullptr;
private:
	CAnimator::ANIMATORDESC m_AanimDesc;
	
private: /* For.Weapon */
	CPolearm* m_pWeapon = nullptr;

private:
	class CUI_Monster_Panel* m_pPanel = nullptr;
	_bool m_bFirst = false;

private:
	_bool	m_isFall = false;
	_bool	m_bFirstHit = false; //맨처음들어와서 맞았을때 판넬크기바꿔줘야돼서
	_bool m_bIsAttack = false;//공격 판단 체크
public:
	static CMonster_Bastion_Spear* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END

#endif // Monster_Bastion_Spear_h__
