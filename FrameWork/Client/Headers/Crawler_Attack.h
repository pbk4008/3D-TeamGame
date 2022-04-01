#ifndef Crawler_Attack_h__
#define Crawler_Attack_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CCrawler_Attack final : public CMonster_FSM
{

protected:
	explicit CCrawler_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCrawler_Attack(const CCrawler_Attack& rhs);
	virtual ~CCrawler_Attack() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;

private:
	_uint m_iAttackCount = 0;
	_uint m_iAttackFrame = 44;
	_bool m_bPlay = false;
public:
	static CCrawler_Attack* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Crawler_Attack_h__
