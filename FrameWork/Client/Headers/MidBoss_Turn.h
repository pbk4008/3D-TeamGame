#ifndef MidBoss_Turn_h__
#define MidBoss_Turn_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CMidBoss_Turn final : public CMonster_FSM
{

protected:
	explicit CMidBoss_Turn(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMidBoss_Turn(const CMidBoss_Turn& rhs);
	virtual ~CMidBoss_Turn() = default;

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

	_float m_TurnTime = 0.0f;
	_float m_fRadian = 0.f;
	_float m_fRadAcc = 0.f;

	_bool m_bLeft = false;
	_float m_fFWD = 0.f;

public:
	static CMidBoss_Turn* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // MidBoss_Turn_h__
