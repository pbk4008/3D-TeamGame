#pragma once

#include "Actor.h"

BEGIN(Engine)
class CAnimator;
class CCharacterController;
class CCapsuleCollider;
END

BEGIN(Client)
class CBoss_Solaris final : public CActor
{
public:
	enum M_BossAnimState
	{
		HEAD, ATTACK_AGG_SPIN_360, ATTACK_DEF_BEAM_CENTER, EXECUTION, ATTACK_R1, ATTACK_R2, ATTACK_S2_VARIANT, ATTACK_S3, ATTACK_S5_SKEWER_PROTOCOL,
		ATTACK_S6, CLOSING, DASH_BACK_FLIP, DASH_BWD, DASH_LEFT, DASH_RIGHT, DEATH, X1, FLINCH_LEFT, X2, FLINCH_RIGHT, IDLE, OPENING, X3, X4,
		REFERENCE_POSE, REPLICATION, RICHOCHET, RUN, RUN_START, RUN_STOP, STAGGER_LEFT, STAGGER_RIGHT, STUN_END, STUN_LOOP, STUN, STUN_START, TAUNT_CURIOUS,
		TAUNT_FIST, TAUNT_SWORD, TURN_135LEFT, TURN_135RIGHT, TURN_180LEFT, TURN_180RIGHT, TURN_45LEFT, TURN_45RIGHT, TURN_90LEFT, TURN_90RIGHT,
		WALK_BWD, WALK_BWD_START, WALK_BWD_STOP, WALK_FWD, WALK_FWD_START, WALK_FWD_STOP, ANIM_END
	};

private:
	explicit CBoss_Solaris(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Solaris(const CBoss_Solaris& rhs);
	virtual ~CBoss_Solaris() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg = nullptr) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow() override;

	virtual void setActive(_bool bActive) override;

private:
	virtual HRESULT SetUp_Components();
	HRESULT Set_Animation_FSM();
	HRESULT Set_State_FSM();
	HRESULT Set_Weapon();
	HRESULT Set_Boss_Effect();

public:
	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void Hit(const ATTACKDESC& _tAttackDesc) override;
	virtual void Execution() override;


public:
	void Set_IsAttack(const _bool _isAttack);
	void OnWeaponEffect();
	void Set_HitMotion(_bool bHitMotion) { m_bHitMotion = bHitMotion; }
	void Set_Random_AttackAnim();

public:
	void Active_Light(); //라이트켜줄때 
	void Set_LightDisTime(_float DisTime); //사라질 시간 정해주는 함수
	void Setting_Light(); //라이트 정보세팅 

public:
	void OnEff_MeshExplosion(_bool Active);
	void OnEff_MeshRazer(_bool Active);
	void OnEff_MeshShield(_bool Active);
	void OnEff_MeshEyeRazer(_bool Active);
	void Set_RazerAngle(_bool Check);

private:
	CAnimator* m_pAnimator = nullptr;
	CStateController* m_pStateController = nullptr;
	CCharacterController* m_pCharacterController = nullptr;

	class CBoss_Weapon* m_pWeapon = nullptr;

	//effect
	class CMeshEffect_Boss_Explosion* m_pEff_Explosion = nullptr;
	class CMeshEffect_Razer* m_pEff_Razer = nullptr;
	class CMeshEffect_Boss_Shield* m_pEff_Shield = nullptr;
	class CMeshEffect_EyeRazer* m_pEff_EyeRazer = nullptr;

private:
	_uint m_iHitCount = 0;
	_bool m_bHitMotion = false;

	_uint m_iPreAnim = 0;
	_bool m_bFirstAnim = false;

	_bool m_bFirstGroggy = false; //첫번째그로기 
	_bool m_bSecondGroggy = false; //두번쨰그로기
	_bool m_bFillShield = false; //실드다시채워졌는지 

	_float m_fDisTime = 5.f;

private:
	_bool	m_bIsFall = false;
public:
	static CBoss_Solaris* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg = nullptr) override;
	virtual void Free() override;
};
END
