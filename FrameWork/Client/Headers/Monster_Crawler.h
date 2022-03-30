#pragma once

#include "Actor.h"

BEGIN(Engine)
class CModel;
class CCapsuleCollider;
class CAnimator;
END

BEGIN(Client)
class CMonster_Crawler final : public CActor
{
public:
	enum MON_STATE {
		HEAD ,
		ATTACK_R1,
		DEATH,
		FLINCH_LEFT, FLINCH_RIGHT,
		IDLE,
		KNOCKBACK_END, KNOCKBACK_START,
		RICOCHET,
		WALK_BWD, WALK_FWD,
		WALK_LEFT, WALK_RIGHT,
		MON_STATE_END
	};
private:
	explicit CMonster_Crawler(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CMonster_Crawler(const CMonster_Crawler& _rhs);
	virtual ~CMonster_Crawler() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual _int Tick(_double _dDeltaTime) override;
	virtual _int LateTick(_double _dDeltaTime) override;
	virtual HRESULT Render() override;

private: 
	void	OnTriggerEnter(CCollision& collision) override;
	void	OnTriggerExit(CCollision& collision) override;

public:
	void Set_IsAttack(const _bool _isAttack);


private:
	virtual HRESULT SetUp_Components();
	HRESULT	Ready_Weapone();
	HRESULT Set_Animation_FSM();
	HRESULT Set_State_FSM();

private:
	CModel*						m_pModelCom = nullptr;
	CAnimator*					m_pAnimatorCom = nullptr;
	CStateController*			m_pStateController = nullptr;
	CCharacterController*		m_pCharacterController = nullptr;
	class CUI_Monster_Panel*	m_pPanel = nullptr;
	CSphereCollider*			m_pCollider = nullptr;
	
public:
	//GameObject�� Set_Remove�Լ��� ������ �ִµ� �� �� ���⼭ ����ĸ�
	//GameObject�� ������ �ִ� Set_Remove�� �׳� Remove�� true�� �ٲ��ִµ�
	//�����Լ��� �̿��ϸ� �� ��ü �ڽ��� ������ �ִ� �ٸ� ��ü ������ ���� �� �� �ֱ� ������
	virtual void Set_Remove(_bool bCheck) override;
private:
	_bool	m_bIsFall = false;
	_bool m_bFirstHit = false; //��ó�����ͼ� �¾����� �ǳ�ũ��ٲ���ߵż�
public:
	static CMonster_Crawler* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	virtual CGameObject* Clone(const _uint _iSceneID, void* _pArg = nullptr) override;
	virtual void Free() override;
};
END
