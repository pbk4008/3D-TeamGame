#ifndef Crawler_Ricochet_h__
#define Crawler_Ricochet_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CCrawler_Ricochet final : public CMonster_FSM
{

protected:
	explicit CCrawler_Ricochet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCrawler_Ricochet(const CCrawler_Ricochet& rhs);
	virtual ~CCrawler_Ricochet() = default;

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
	static CCrawler_Ricochet* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Crawler_Ricochet_h__
