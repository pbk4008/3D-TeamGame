#ifndef MidBoss_Rage_h__
#define MidBoss_Rage_h__

#include "Monster_FSM.h"

BEGIN(Client)
class CMidBoss_Rage final : public CMonster_FSM
{

protected:
	explicit CMidBoss_Rage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMidBoss_Rage(const CMidBoss_Rage& rhs);
	virtual ~CMidBoss_Rage() = default;

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
	static CMidBoss_Rage* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // MidBoss_Rage_h__
