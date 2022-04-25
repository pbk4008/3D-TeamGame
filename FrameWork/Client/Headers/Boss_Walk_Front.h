#ifndef Boss_Walk_Front_h__
#define Boss_Walk_Front_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CBoss_Walk_Front final : public CMonster_FSM
{

protected:
	explicit CBoss_Walk_Front(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Walk_Front(const CBoss_Walk_Front& rhs);
	virtual ~CBoss_Walk_Front() = default;

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
	_float4 m_vLook;
	_float4 m_vRight;

public:
	static CBoss_Walk_Front* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Boss_Walk_Front_h__
