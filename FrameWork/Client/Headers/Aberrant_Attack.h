#ifndef Aberrant_Attack_h__
#define Aberrant_Attack_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CAberrant_Attack final : public CMonster_FSM
{

protected:
	explicit CAberrant_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAberrant_Attack(const CAberrant_Attack& rhs);
	virtual ~CAberrant_Attack() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT EnterState(void* pArg);

public:
	virtual void Look_Player(void) override;

private:
	void Play_Sound(void);

private:
	_uint m_iAttackCount = 0;
	_uint m_iAttack1Frame = 45;
	_uint m_iAttack2Frame = 15;
	_uint m_iAttackType = 0;

public:
	static CAberrant_Attack* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Aberrant_Attack_h__
