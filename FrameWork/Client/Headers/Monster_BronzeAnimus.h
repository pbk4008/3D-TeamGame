#ifndef Monster_BronzeAnimus_h__
#define Monster_BronzeAnimus_h__

#include "Actor.h"

BEGIN(Engine)
class CAnimator;
class CAnimation;
class CCapsuleCollider;
END

BEGIN(Client)
class CBronzeAnimus_Sword;
class CMonster_BronzeAnimus : public CActor
{
public:
	enum class ANIM_TYPE {
		A_HEAD, A_IDLE, A_IDLE_BATTLE, A_DEATH, A_FLINCH, A_RICOCHET, A_ROAR,
		A_ATTACK_R1, A_ATTACK_R2, A_ATTACK_S1,
		A_WALK_FWD_ST, A_WALK_FWD, A_WALK_FWD_ED,
		A_WALK_BWD_ST, A_WALK_BWD, A_WALK_BWD_ED,
		A_STUN_ST, A_STUN, A_STUN_ED,
		A_TURN_45_LEFT, A_TURN_45_RIGHT,
		A_TURN_90_LEFT, A_TURN_90_RIGHT,
		A_TURN_135_LEFT, A_TURN_135_RIGHT,
		A_TURN_180_LEFT, A_TURN_180_RIGHT,
		A_EXCUTION,
		A_END
	};

private:
	explicit CMonster_BronzeAnimus(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_BronzeAnimus(const CMonster_BronzeAnimus& _rhs);
	virtual ~CMonster_BronzeAnimus() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

	virtual void Hit(const ATTACKDESC& _tAttackDesc) override;
	virtual void Parry(const PARRYDESC& _tParryDesc) override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Weapon(void);
	HRESULT Ready_AnimFSM(void);
	HRESULT Ready_StateFSM(void);
	HRESULT Render_Debug(void);
private:
	virtual HRESULT Set_SpawnPosition(const _float3 vPoint);

private:
	virtual void OnTriggerEnter(CCollision& collision);

public:
	void Set_IsAttack(const _bool _isAttack);
	void Set_Current_HP(_int _iHp) { m_fCurrentHp += _iHp; }
	void Set_GroggyGauge(_uint _Value) { m_fGroggyGauge += _Value; }
	void Set_Dead(void) { m_bDead = true;}
	void Remove_Collider();

public:
	void GroggyStart();
	void Hit(CCollision& collision);
public:
	void Set_Target(_bool bCheck) { m_bTarget = bCheck; }
	void Set_AttackOn(_bool bCheck) { m_bAttackOn = bCheck; }
	void Set_BattleOn(_bool bCheck) { m_bBattleOn = bCheck; }
	void Set_Find(_bool bCheck) { m_bFind = bCheck; }
public:
	_bool Get_Target() { return m_bTarget; }
	_bool Get_AttackOn() { return m_bAttackOn; }
	_bool Get_BattleOn() { return m_bBattleOn; }
	_bool Get_Find() { return m_bFind; }

private:
	_bool m_bTarget = false;
	_bool m_bAttackOn = false;
	_bool m_bBattleOn = false;
	_bool m_bFind = false;
private:
	CAnimation* m_pAnimation = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CStateController* m_pStateController = nullptr;
	CCapsuleCollider* m_pColliderCom = nullptr;
	CCharacterController* m_pCharacterController = nullptr;
private:
	CAnimator::ANIMATORDESC m_AanimDesc;
	
private: /* For.Weapon */
	CBronzeAnimus_Sword* m_pWeapon = nullptr;

private:
	class CUI_Monster_Panel* m_pPanel = nullptr;
	_bool m_bFirst = false;
public:
	_bool	m_isFall = false;
	_bool	m_bFirstHit = false; //¸ÇÃ³À½µé¾î¿Í¼­ ¸Â¾ÒÀ»¶§ ÆÇ³ÚÅ©±â¹Ù²ãÁà¾ßµÅ¼­

public:
	static CMonster_BronzeAnimus* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END

#endif // Monster_BronzeAnimus_h__