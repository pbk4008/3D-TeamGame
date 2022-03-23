#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
class CAnimator;
class CCharacterController;
END

BEGIN(Client)
class CBoss_Bastion_Judicator final : public CActor
{
public:
	enum M_BossAnimState
	{
		HEAD, ATTACK_JOG_H, ATTACK_LEGACY_H, ATTACK_R1_H, CHARGE_STAGE1_END_H, CHARGE_STAGE1_LOOP_H, CHARGE_STAGE1_START_H, FALLING_PEDALLING_H, JOG_BWD_H,
		JOG_BWD_START_H, JOG_FWD_H, JOG_FWD_START_H, WALK_BWD_H, WALK_BWD_START_H, WALK_BWD_STOP_H, WALK_FWD_H, WALK_FWD_STOP_H, IDLE_LOOP_H, IDLE_START_H,
		RICOCHET_H, SPRINT_ATTACK_H, TURN_135LEFT_H, TURN_135RIGHT_H, TURN_180LEFT_H, TURN_180RIGHT_H, TURN_45LEFT_H, TURN_45RIGHT_H, TURN_90LEFT_H, TURN_90RIGHT_H,
		ATTACK_R1, ATTACK_S1, ATTACK_S2, BATTLECRY_END, BATTLECRY_LOOP ,BATTLECRY_START, DASH_BWD, DASH_LEFT, DASH_RIGHT, DEATH, RAGE, STUN_END, STUN_LOOP, STUN_START, ANIMSTATE_END
	};

private:
	explicit CBoss_Bastion_Judicator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Bastion_Judicator(const CBoss_Bastion_Judicator& rhs);
	virtual ~CBoss_Bastion_Judicator() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* pArg = nullptr) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT SetUp_Components();
	HRESULT Set_Animation_FSM();
	HRESULT Set_State_FSM();

private:
	const _int Fall(const _double& TimeDelta);
	virtual void OnTriggerEnter(CCollision& collision) override;

public:
	void Set_Groggy(_float fGroggy) { m_fGroggy = fGroggy; }

public:
	const _bool IsAttack() { return m_bIsAttack; }
	void Set_IsAttack(const _bool _isAttack);

private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CStateController* m_pStateController = nullptr;
	CCharacterController* m_pCharacterController = nullptr;

	class CShieldBreaker* m_pWeapon = nullptr;
	class CUI_Monster_Panel* m_pPanel = nullptr;

private:
	_uint itest = 0;
	_float fTime = 0.f;
	_float m_fGroggy = 0.f;
	_float m_fMaxGroggy = 0.f;

private:
	_bool	m_isFall = false;
	_bool m_bFirstHit = false; //��ó�����ͼ� �¾����� �ǳ�ũ��ٲ���ߵż�
	_bool m_bGroggy = false;
	_bool m_bIsAttack = false;

public:
	static CBoss_Bastion_Judicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* pArg = nullptr) override;
	virtual void Free() override;
};
END
