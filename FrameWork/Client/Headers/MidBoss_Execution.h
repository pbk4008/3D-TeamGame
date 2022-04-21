#ifndef MidBoss_Execution_h__
#define MidBoss_Execution_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CMidBoss_Execution final : public CMonster_FSM
{

protected:
	explicit CMidBoss_Execution(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMidBoss_Execution(const CMidBoss_Execution& rhs);
	virtual ~CMidBoss_Execution() = default;

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
	static CMidBoss_Execution* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // MidBoss_Execution_h__
