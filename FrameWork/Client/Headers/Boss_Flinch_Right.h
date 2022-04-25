#ifndef Boss_Flinch_Right_h__
#define Boss_Flinch_Right_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CBoss_Flinch_Right final : public CMonster_FSM
{

protected:
	explicit CBoss_Flinch_Right(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Flinch_Right(const CBoss_Flinch_Right& rhs);
	virtual ~CBoss_Flinch_Right() = default;

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
	static CBoss_Flinch_Right* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Boss_Flinch_Right_h__
