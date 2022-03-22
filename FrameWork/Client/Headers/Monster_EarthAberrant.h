#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
class CCapsuleCollider;
class CAnimator;
END

BEGIN(Client)
class CMonster_EarthAberrant final : public CActor
{
public:
	enum MON_STATE {
		HEAD, ATTACK_EXECUTION, ATTACK_R1, ATTACK_R2, DASH_BWD, DASH_LEFT, DASH_RIGHT, DEATH, FLINCH_LEFT,
		FLINCH_RIGHT, GET_UP, IDLE, KNOCKBACK, RICOCHET, RUN_FWD, RUN_FWD_START, RUN_FWD_STOP, STAGGER_LEFT, STAGGER_RIGHT,
		STUN_END, STUN_LOOP, STUN_START, WALK_BWD, WALK_BWD_START, WALK_BWD_STOP, WALK_FWD, WALK_FWD_START, WALK_FWD_STOP,
		WALK_LEFT, WALK_LEFT_START, WALK_LEFT_STOP, WALK_RIGHT, WALK_RIGHT_START, WALK_RIGHT_STOP, MON_STATE_END
	};
private:
	explicit CMonster_EarthAberrant(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_EarthAberrant(const CMonster_EarthAberrant& _rhs);
	virtual ~CMonster_EarthAberrant() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT SetUp_Components();
	HRESULT Set_Animation_FSM();
	HRESULT Set_State_FSM();

private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimatorCom = nullptr;
	CCapsuleCollider* m_pColliderCom = nullptr;
	CStateController* m_pStateController = nullptr;

	class CUI_Monster_Panel* m_pPanel = nullptr;

	class CEarthAberrant_Pick* m_pWeapon = nullptr;

public:
	static CMonster_EarthAberrant* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};
END
