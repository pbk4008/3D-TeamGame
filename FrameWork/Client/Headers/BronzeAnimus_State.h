#ifndef BronzeAnimus_State_h__
#define BronzeAnimus_State_h__

#include "Monster_FSM.h"

BEGIN(Client)
class CBronzeAnimus_State  : public CMonster_FSM
{
protected:
	explicit CBronzeAnimus_State(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);
	explicit CBronzeAnimus_State(const CBronzeAnimus_State& _rhs);
	virtual ~CBronzeAnimus_State() = default;

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
	void	Check_Attack(const _double& _dDeltaTime);

public:
	virtual	void OnTriggerEnter(CCollision& collision);

protected:
	_bool	m_bTargetOn = false;
	_bool   m_bAttackOn = false;
	_bool   m_bBattleOn = false;
	_bool	m_bPlayerAttack = false;
	_float	m_fChaserDelay;

private:
	_float m_CheckFWD = 0.0f;
	_float m_fRadian = 0.0f;
	_float m_fDistance = 0.0f;
	_float m_fAttackTime = 0.0f;

public:
	static CBronzeAnimus_State* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pDeviceContext, void* _pArg = nullptr);
	virtual void Free() override;
};

END

#endif // BronzeAnimus_State_h__