#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
class CAnimator;
class CCharacterController;
END
BEGIN(Client)

class CMonster_Bastion_Shooter : public CActor
{
public:
	enum class ANIM_TYPE {
		HEAD, IDLE, RUN_START, RUN_END, RUN_LOOP
		, ATTACK
		, HIT1, HIT2, HIT3, HIT4, DEATH
		, GROGGY_START, GROGGY_LOOP, GROGGY_END
		, BACKWARD_START, BACKWARD_LOOP, BACKWARD_END
		, FORWARD_START, FORWARD_LOOP, FORWARD_END
		, LEFTWALK_START, LEFTWALK_LOOP, LEFTWALK_END
		, RIGHTWALK_START, RIGHTWALK_LOOP, RIGHTWALK_END
		, TYPE_END
	};
private:
	explicit CMonster_Bastion_Shooter(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Bastion_Shooter(const CMonster_Bastion_Shooter& _rhs);
	virtual ~CMonster_Bastion_Shooter() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;
private:
	virtual void OnTriggerEnter(CCollision& collision) override;
private:
	void Chase();
	void Hit();
private:
	HRESULT Ready_Components();
	HRESULT Ready_AnimationFSM();
	HRESULT Ready_StateFSM();
	_int Change_State();
private:
	CModel* m_pModelCom = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CStateController* m_pStateController = nullptr;
	CCharacterController* m_pCharacterController = nullptr;
private:
	wstring m_wstrCurState = L"";
	ANIM_TYPE m_eHitType = ANIM_TYPE::TYPE_END;
public:
	static CMonster_Bastion_Shooter* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};

END