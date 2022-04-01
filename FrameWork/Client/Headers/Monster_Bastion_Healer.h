#ifndef Monster_Bastion_Healer_h__
#define Monster_Bastion_Healer_h__

#include "Actor.h"

BEGIN(Engine)
class CAnimator;
class CAnimation;
class CCapsuleCollider;
END

BEGIN(Client)
class CStaff;
class CMonster_Bastion_Healer : public CActor
{
public:
	enum class ANIM_TYPE {
		A_HEAD, A_IDLE, A_DEATH, A_CAST_PROTECT, A_FLINCH_LEFT, A_ATTACK_BLIND,
		A_WALK_FWD_ST, A_WALK_FWD, A_WALK_FWD_ED,
		A_WALK_BWD_ST, A_WALK_BWD, A_WALK_BWD_ED,
		A_WALK_LEFT_ST, A_WALK_LEFT, A_WALK_LEFT_ED,
		A_STUN_ST, A_STUN, A_STUN_ED,
		A_TURN_45_LEFT, A_TURN_45_RIGHT,
		A_TURN_90_LEFT, A_TURN_90_RIGHT,
		A_TURN_135_LEFT, A_TURN_135_RIGHT,
		A_TURN_180_LEFT, A_TURN_180_RIGHT,
		A_END
	};

private:
	explicit CMonster_Bastion_Healer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_Healer(const CMonster_Bastion_Healer& _rhs);
	virtual ~CMonster_Bastion_Healer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void Groggy_Start();
	void Hit(CCollision& pCol);
	void Remove_Collider();
private:
	HRESULT Ready_Components();
	HRESULT Ready_Weapon(void);
	HRESULT Ready_AnimFSM(void);
	HRESULT Ready_StateFSM(void);
	HRESULT Render_Debug(void);

private:
	virtual void OnTriggerEnter(CCollision& collision);
	virtual void OnTriggerExit(CCollision& collision);

public:
	void Set_IsAttack(const _bool _isAttack);
	void Set_Current_HP(_int _iHp) { m_fCurrentHp += _iHp; }
	void Set_GroggyGauge(_uint _Value) { m_fGroggyGauge += _Value; }
	void Set_Dead(void) { m_bDead = true; }
	virtual void Set_Remove(_bool bCheck) override;

private:
	CModel* m_pModel = nullptr;
	CAnimation* m_pAnimation = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CStateController* m_pStateController = nullptr;
	CCapsuleCollider* m_pColliderCom = nullptr;
	CCharacterController* m_pCharacterController = nullptr;
private:
	CAnimator::ANIMATORDESC m_AanimDesc;
	
private: /* For.Weapon */
	CStaff* m_pWeapon = nullptr;

private:
	class CUI_Monster_Panel* m_pPanel = nullptr;
	_bool m_bFirst = false;

private:
	_int	m_iHp = 3;
	_bool	m_bRender = true;

private:
	_bool	m_isFall = false;
	_bool	m_bFirstHit = false; //맨처음들어와서 맞았을때 판넬크기바꿔줘야돼서

public:
	static CMonster_Bastion_Healer* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END

#endif // Monster_Bastion_Healer_h__