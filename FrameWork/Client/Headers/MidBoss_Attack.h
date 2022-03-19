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

	//중복없게해줄려고 선언한 변수
	_int m_iPreState_1;
	_int m_iPreState_2;

public:
	static CMidBoss_Attack* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // MidBoss_Attack_h__
