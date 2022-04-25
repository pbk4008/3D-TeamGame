#ifndef Boss_Dash_Back_h__
#define Boss_Dash_Back_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CBoss_Dash_Back final : public CMonster_FSM
{

protected:
	explicit CBoss_Dash_Back(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Dash_Back(const CBoss_Dash_Back& rhs);
	virtual ~CBoss_Dash_Back() = default;

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
	static CBoss_Dash_Back* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Boss_Dash_Back_h__
