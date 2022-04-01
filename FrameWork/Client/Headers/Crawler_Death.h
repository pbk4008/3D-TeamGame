#ifndef Crawler_Death_h__
#define Crawler_Death_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CCrawler_Death final : public CMonster_FSM
{

protected:
	explicit CCrawler_Death(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCrawler_Death(const CCrawler_Death& rhs);
	virtual ~CCrawler_Death() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

private:
	CCameraShake::SHAKEEVENT m_tShakeEvent;

public:
	virtual void Look_Player(void) override;
	virtual void Look_Monster(void) override;

public:
	static CCrawler_Death* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Crawler_Death_h__
