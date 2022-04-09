#ifndef Monster_Bastion_2HSword_h__
#define Monster_Bastion_2HSword_h__

#include "Actor.h"

BEGIN(Engine)
class CAnimator;
class CAnimation;
class CCapsuleCollider;
END

BEGIN(Client)
class CRetributionBlade;
class CMonster_Bastion_2HSword : public CActor
{
public:
	enum class ANIM_TYPE {
		A_HEAD, A_IDLE, A_DEATH, A_RICOCHET, A_FLINCH_LEFT,
		A_WALK_FWD_ST, A_WALK_FWD, A_WALK_FWD_ED,
		A_WALK_BWD_ST, A_WALK_BWD, A_WALK_BWD_ED,
		A_WALK_LEFT_ST, A_WALK_LEFT, A_WALK_LEFT_ED,
		A_WALK_RIGHT_ST, A_WALK_RIGHT, A_WALK_RIGHT_ED,
		A_ATTACK_R1, A_ATTACK_R2, A_ATTACK_S1, A_ATTACK_S3,
		A_BATTLECRY_ST, A_BATTLECRY, A_BATTLECRY_ED, A_TAUNT_ROAR,
		A_KNEEL_ST, A_KNEEL, A_KNEEL_ED,
		A_DASH_BWD, A_DASH_LEFT, A_DASH_RIGHT,
		A_STUN_ST, A_STUN, A_STUN_ED,
		A_TURN_45_LEFT, A_TURN_45_RIGHT,
		A_TURN_90_LEFT, A_TURN_90_RIGHT,
		A_TURN_135_LEFT, A_TURN_135_RIGHT,
		A_TURN_180_LEFT, A_TURN_180_RIGHT,
		A_END
	};
private:
	explicit CMonster_Bastion_2HSword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_2HSword(const CMonster_Bastion_2HSword& _rhs);
	virtual ~CMonster_Bastion_2HSword() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

private:
	HRESULT Ready_Components(void);
	HRESULT Ready_Weapon(void);
	HRESULT Ready_AnimFSM(void);
	HRESULT Ready_StateFSM(void);
	HRESULT Render_Debug(void);
public:
	void Groggy_Start();
	void Hit(CCollision& pCol);
	virtual void Hit(const ATTACKDESC& _tAttackDesc) override;
	void Remove_Collider();
private:
	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void OnTriggerExit(CCollision& collision) override;

public:
	void Set_IsAttack(const _bool _isAttack);
	void Set_Current_HP(_int _iHp) { m_fCurrentHp += _iHp; }
	void Set_GroggyGauge(_uint _Value) { m_fGroggyGauge += _Value; }
	void Set_Dead(void) { m_bDead = true; }
	virtual void Set_Remove(_bool bCheck) override;
public:
	void set_Target(_bool bCheck) { m_bTargetOn = bCheck; }
	void set_Attack(_bool bCheck) { m_bAttackOn = bCheck; }
	void set_RageOn(_bool bCheck) { m_bRageOn = bCheck; }
	void set_FirstAttack(_bool bCheck) { m_bFirstAttack = bCheck; }
	void set_RandAttack(_int iNum) { m_iRandAtt = iNum; }
	

	_bool get_Target() { return m_bTargetOn; }
	_bool get_Attack() { return m_bAttackOn; }
	_bool get_RageOn() { return m_bRageOn; }
	_bool get_FirstAttack() { return m_bFirstAttack; }
	_int get_RandAttack() { return m_iRandAtt; }

private:
	_bool m_bTargetOn = false;
	_bool m_bAttackOn = false;
	_bool m_bRageOn = false;
	_bool m_bFirstAttack = false;
	_int m_iRandAtt = -1;
private:
	CAnimation*			  m_pAnimation = nullptr;
	CAnimator*			  m_pAnimator = nullptr;
	CStateController*	  m_pStateController = nullptr;
	CCharacterController* m_pCharacterController = nullptr;

private:
	CAnimator::ANIMATORDESC m_AanimDesc;

private: /* For.Weapon */
	CRetributionBlade* m_pWeapon = nullptr;
	_bool m_bFirst = false;

private:
	class CUI_Monster_Panel* m_pPanel = nullptr;

private:
	_bool	m_isFall = false;
	_bool	m_bFirstHit = false; //¸ÇÃ³À½µé¾î¿Í¼­ ¸Â¾ÒÀ»¶§ ÆÇ³ÚÅ©±â¹Ù²ãÁà¾ßµÅ¼­

public:
	static CMonster_Bastion_2HSword* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END


#endif // Monster_Bastion_2HSword_h__