#ifndef Boss_Attack_S3_h__
#define Boss_Attack_S3_h__

#include "Monster_FSM.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CBoss_Attack_S3 final : public CMonster_FSM
{

protected:
	explicit CBoss_Attack_S3(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Attack_S3(const CBoss_Attack_S3& rhs);
	virtual ~CBoss_Attack_S3() = default;

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
	_bool m_bEffectCheck = false;
	_bool m_bShakeCheck = false;

public:
	static CBoss_Attack_S3* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual void Free() override;
};

END

#endif // Boss_Attack_S3_h__
