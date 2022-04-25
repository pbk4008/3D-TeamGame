#ifndef Boss_Idle_h__
#define Boss_Idle_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CBoss_Idle final : public CMonster_FSM
{

protected:
	explicit CBoss_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Idle(const CBoss_Idle& rhs);
	virtual ~CBoss_Idle() = default;

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
	static CBoss_Idle* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Boss_Idle_h__
