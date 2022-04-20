#ifndef MidBoss_BattleCry_h__
#define MidBoss_BattleCry_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CMidBoss_BattleCry final : public CMonster_FSM
{

protected:
	explicit CMidBoss_BattleCry(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMidBoss_BattleCry(const CMidBoss_BattleCry& rhs);
	virtual ~CMidBoss_BattleCry() = default;

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

private:
	_uint m_iAttackFrame = 12;
	_bool m_bEffectCheck = false;

public:
	static CMidBoss_BattleCry* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // MidBoss_BattleCry_h__
