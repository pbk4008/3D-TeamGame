#ifndef MidBoss_Attack_h__
#define MidBoss_Attack_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CMidBoss_Attack final : public CMonster_FSM
{

protected:
	explicit CMidBoss_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMidBoss_Attack(const CMidBoss_Attack& rhs);
	virtual ~CMidBoss_Attack() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;

private:
	void Play_Sound(void);

public:
	//중복없게해줄려고 선언한 변수
	_int m_iPreState_1;
	_int m_iPreState_2;

private:
	_uint m_iAttackR1_H = 25;
	_uint m_iAttackR1_1 = 54;
	_uint m_iAttackR1_2 = 105;
	_uint m_iAttackS1_1 = 70;
	_uint m_iAttackS1_2 = 120;
	_uint m_iAttackS2_1 = 10;
	_uint m_iAttackS2_2 = 40;
	_uint m_iAttackJog  = 20;
	_uint m_iAttackSprint = 14;
	_uint m_iAttackLegacy_1 = 5;
	_uint m_iAttackLegacy_2 = 14;
	_uint m_iAttackLegacy_3 = 30;

	_bool m_bAttackTypeFar = false;
	_bool m_bAttackR1_H = false;
	_bool m_bAttackR1_1 = false;
	_bool m_bAttackS1_1 = false;
	_bool m_bAttackS2_1 = false;
	_bool m_bAttackFar = false;
	_bool m_bAttackLegacy_1 = false;
	_bool m_bAttackLegacy_2 = false;

public:
	static CMidBoss_Attack* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // MidBoss_Attack_h__
