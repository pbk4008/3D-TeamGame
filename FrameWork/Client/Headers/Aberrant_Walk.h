#ifndef Aberrant_Walk_h__
#define Aberrant_Walk_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CAberrant_Walk final : public CMonster_FSM
{

protected:
	explicit CAberrant_Walk(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAberrant_Walk(const CAberrant_Walk& rhs);
	virtual ~CAberrant_Walk() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();
	virtual HRESULT ExitState(void* pArg);

public:
	virtual void Look_Player(void) override;

public:
	static CAberrant_Walk* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
private:
	_uint m_iRand;
};

END

#endif // Aberrant_Walk_h__
