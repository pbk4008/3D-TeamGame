#ifndef MidBoss_Stun_h__
#define MidBoss_Stun_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CMidBoss_Stun final : public CMonster_FSM
{

protected:
	explicit CMidBoss_Stun(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMidBoss_Stun(const CMidBoss_Stun& rhs);
	virtual ~CMidBoss_Stun() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;

public:
	static CMidBoss_Stun* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // MidBoss_Stun_h__
