#ifndef Boss_Turn_h__
#define Boss_Turn_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CBoss_Turn final : public CMonster_FSM
{

protected:
	explicit CBoss_Turn(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Turn(const CBoss_Turn& rhs);
	virtual ~CBoss_Turn() = default;

public:
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int Tick(const _double & TimeDelta);
	virtual _int LateTick(const _double & TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT EnterState();
	virtual HRESULT ExitState();

public:
	virtual void Look_Player(void) override;

	_float m_fRadian = 0.f;

	_bool m_bLeft = false;

public:
	static CBoss_Turn* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Boss_Turn_h__
