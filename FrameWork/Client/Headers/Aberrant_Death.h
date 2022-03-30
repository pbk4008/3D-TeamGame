#ifndef Aberrant_Death_h__
#define Aberrant_Death_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CAberrant_Death final : public CMonster_FSM
{

protected:
	explicit CAberrant_Death(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAberrant_Death(const CAberrant_Death& rhs);
	virtual ~CAberrant_Death() = default;

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
	CCameraShake::SHAKEEVENT m_tShakeEvent;

public:
	static CAberrant_Death* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Aberrant_Death_h__
