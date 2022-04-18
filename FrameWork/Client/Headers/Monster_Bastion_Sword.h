#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
class CAnimator;
class CCharacterController;
END

BEGIN(Client)
class CStargazer;
class CMonster_Bastion_Sword final : public CActor
{
public:
	enum class ANIM_TYPE { HEAD, IDLE, RUN_START, RUN_END, RUN_LOOP
		, ATTACK_JUMPSTART,ATTACK_JUMPLOOP, ATTACK_JUMPEND
		, ATTACK_DOUBLE, ATTACK_SINGLE
		,HIT1, HIT2, HIT3, HIT4,DEATH
		, GROGGY_START, GROGGY_LOOP, GROGGY_END
		,BACKWARD_START,BACKWARD_LOOP, BACKWARD_END
		,FORWARD_START, FORWARD_LOOP, FORWARD_END
		,LEFTWALK_START, LEFTWALK_LOOP, LEFTWALK_END
		,RIGHTWALK_START, RIGHTWALK_LOOP, RIGHTWALK_END
		,PARING,TURN
		,TYPE_END};
private:
	explicit CMonster_Bastion_Sword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_Sword(const CMonster_Bastion_Sword& _rhs);
	virtual ~CMonster_Bastion_Sword() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
	virtual HRESULT	Render_Shadow()  override;

	virtual void Hit(const ATTACKDESC& _tAttackDesc);
	virtual void Parry(const PARRYDESC& _tParryDesc);
public:
	virtual HRESULT Set_SpawnPosition(_fvector vPos);
public:
	virtual void Set_Remove(_bool bCheck) override;

public:
	void Set_IsAttack(const _bool _isAttack);

private:
	virtual void OnTriggerEnter(CCollision& collision) override;
	virtual void OnTriggerExit(CCollision& collision) override;

private:
	virtual HRESULT SetUp_Components();
	HRESULT Set_Animation_FSM();
	HRESULT Set_State_FSM();
	HRESULT Set_Weapon();
	HRESULT Ready_UI();
	_int Dead_Check();
	void Hit();

private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CStateController* m_pStateController = nullptr;
	CCharacterController* m_pCharacterController = nullptr;

	class CUI_Monster_Panel* m_pPanel = nullptr;

private:
	CStargazer* m_pWeapon = nullptr;
private:
	ANIM_TYPE m_eHitType = ANIM_TYPE::TYPE_END;//Hit만 판정

	_bool m_bFirstHit = false; //맨처음들어와서 맞았을때 판넬크기바꿔줘야돼서
	_bool m_bFirst = false;
	_uint m_iCurScene = 0;

public:
	static CMonster_Bastion_Sword* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};
END
