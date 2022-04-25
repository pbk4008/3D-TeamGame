#ifndef Boss_Taunt_Fist_h__
#define Boss_Taunt_Fist_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CBoss_Taunt_Fist final : public CMonster_FSM
{

protected:
	explicit CBoss_Taunt_Fist(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Taunt_Fist(const CBoss_Taunt_Fist& rhs);
	virtual ~CBoss_Taunt_Fist() = default;

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
	static CBoss_Taunt_Fist* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Boss_Taunt_Fist_h__
