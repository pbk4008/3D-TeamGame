#ifndef Aberrant_Dash_Bwd_h__
#define Aberrant_Dash_Bwd_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CAberrant_Dash_Bwd final : public CMonster_FSM
{

protected:
	explicit CAberrant_Dash_Bwd(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAberrant_Dash_Bwd(const CAberrant_Dash_Bwd& rhs);
	virtual ~CAberrant_Dash_Bwd() = default;

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
	static CAberrant_Dash_Bwd* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Aberrant_Dash_Bwd_h__
