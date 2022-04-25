#ifndef Boss_Attack_R1_h__
#define Boss_Attack_R1_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CBoss_Attack_R1 final : public CMonster_FSM
{

protected:
	explicit CBoss_Attack_R1(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Attack_R1(const CBoss_Attack_R1& rhs);
	virtual ~CBoss_Attack_R1() = default;

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
	void Play_Sound(void);

private:
	_bool m_bShakeCheckLeft = false;
	_bool m_bShakeCheckRight = false;

public:
	static CBoss_Attack_R1* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Boss_Attack_R1_h__
