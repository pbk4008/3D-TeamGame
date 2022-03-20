#ifndef Bastion_2HSword_State_h__
#define Bastion_2HSword_State_h__

#include "Monster_FSM.h"

BEGIN(Client)
class CBastion_2HSword_State  : public CMonster_FSM
{
protected:
	explicit CBastion_2HSword_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBastion_2HSword_State(const CBastion_2HSword_State& _rhs);
	virtual ~CBastion_2HSword_State() = default;

public:
	virtual HRESULT NativeConstruct(void* _pArg = nullptr);
	virtual _int Tick(const _double & _dDeltaTime);
	virtual _int LateTick(const _double & _dDeltaTime);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;

public:
	HRESULT	Render_Debug();

protected:
	_bool	m_bTargetOn = false;
	_bool   m_bAttackOn = false;
	_bool	m_bPlayerAttack = false;
	_bool	m_bRageOn = false;
	_bool	m_bRageAttack = false;

public:
	static CBastion_2HSword_State* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Bastion_2HSword_State_h__