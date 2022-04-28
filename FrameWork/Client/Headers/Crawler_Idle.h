#ifndef Crawler_Idle_h__
#define Crawler_Idle_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CCrawler_Idle final : public CMonster_FSM
{

protected:
	explicit CCrawler_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCrawler_Idle(const CCrawler_Idle& rhs);
	virtual ~CCrawler_Idle() = default;

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
	static CCrawler_Idle* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;

private: _float m_timer = 0.f;
};

END

#endif // Crawler_Idle_h__
