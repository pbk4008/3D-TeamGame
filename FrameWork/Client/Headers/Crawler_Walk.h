#ifndef Crawler_Walk_h__
#define Crawler_Walk_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CCrawler_Walk final : public CMonster_FSM
{

protected:
	explicit CCrawler_Walk(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCrawler_Walk(const CCrawler_Walk& rhs);
	virtual ~CCrawler_Walk() = default;

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
	static CCrawler_Walk* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Crawler_Walk_h__
