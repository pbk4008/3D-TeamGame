#ifndef Aberrant_Execution_h__
#define Aberrant_Execution_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CAberrant_Execution final : public CMonster_FSM
{

protected:
	explicit CAberrant_Execution(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAberrant_Execution(const CAberrant_Execution& rhs);
	virtual ~CAberrant_Execution() = default;

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
	static CAberrant_Execution* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Aberrant_Execution_h__
