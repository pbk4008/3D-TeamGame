#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
class CAnimator;
END

BEGIN(Client)
class CBoss_Bastion_Judicator final : public CActor
{
	enum M_BossAnimState
	{
		HEAD, ATTACK_JOG_H, ATTACK_LEGACY_H, ATTACK_R1_H, ATTACK_R2_H, ATTACK_SPRINT_R1_H, ATTACK_STRONG_R1_H, MENU_IDLE_LOOP, MENU_IDLE_START,
		RICOCHET_REACTION, SPRINT_ATTACK_H, BATTLE_CRY, BATTLE_CRY_START, BATTLE_CRY_STOP, ATTACK_R1, ATTACK_R2, DASH_BACK, DASH_LEFT, 
		DASH_RIGHT, DEATH, FLINCH_LEFT, FLINCH_RIGHT, IDLE, KNEEL_END, KNEEL_LOOP, KNEEL_START, RICOCHET, RUN, RUN_START, RUN_STOP, 
		STAGGER_LEFT, STAGGER_RIGHT, STUN, STUN_START, STUN_STOP, WALK_BWD, WALK_BWD_START, WALK_BWD_STOP, WALK_FWD, WALK_FWD_START, 
		WALK_FWD_STOP, WALK_LEFT, WALK_LEFT_START, WALK_LEFT_STOP, WALK_RIGHT, WALK_RIGHT_START, WALK_RIGHT_STOP, STATE_END
	};

	enum M_BossState
	{

	};

private:
	explicit CBoss_Bastion_Judicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Bastion_Judicator(const CBoss_Bastion_Judicator& rhs);
	virtual ~CBoss_Bastion_Judicator() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT SetUp_Components();
	HRESULT Set_Animation_FSM();

private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CCapsuleCollider* m_pCollider = nullptr;
	
	class CShieldBreaker* m_pWeapon = nullptr;

private:
	_uint itest = 0;
public:
	static CBoss_Bastion_Judicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
