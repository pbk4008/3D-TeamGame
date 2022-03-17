#ifndef Monster_Bastion_2HSword_h__
#define Monster_Bastion_2HSword_h__

#include "Actor.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CWeapon;
class CMonster_Bastion_2HSword final : public CActor
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

private:
	HRESULT Ready_Components(void);
	HRESULT Ready_Weapon(void);
	HRESULT Ready_AnimFSM(void);
	HRESULT Ready_StateFSM(void);

private:
	CModel*				  m_pModel = nullptr;
	CAnimator*			  m_pAnimator = nullptr;
	CStateController*	  m_pStateController = nullptr;

private:
	CAnimator::ANIMATORDESC m_AanimDesc;

private: /* For.Weapon */
	CWeapon* m_pCurWeapon = nullptr;
	_bool m_isEquipWeapon = false;
	unordered_map<wstring, CWeapon*> m_umapWeapons;

public:
	static CMonster_Bastion_2HSword* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END


#endif // Monster_Bastion_2HSword_h__