#ifndef Aberrant_Idle_h__
#define Aberrant_Idle_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CAberrant_Idle final : public CMonster_FSM
{

protected:
	explicit CAberrant_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAberrant_Idle(const CAberrant_Idle& rhs);
	virtual ~CAberrant_Idle() = default;

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
	static CAberrant_Idle* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
private:
	_uint m_iRandDist;
};

END

#endif // Aberrant_Idle_h__
